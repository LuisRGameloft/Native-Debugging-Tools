#
# MIT License
# 
# Copyright (c) 2017 David Landeros [dh.landeros08@gmail.com]
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

import ConfigParser
import socket
import time
import subprocess
import os.path
import time
import sys
import os
from shutil import copyfile

#
# do not touch these variables
#
ndt_path = ""

#
# it defines utils functions
#
def enableProfiling():
    ensureAdbIsReady()
    command = "adb shell setprop security.perf_harden 0"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def ensureAdbIsReady():
    command = "adb start-server"
    subprocess.Popen(command, stdout=subprocess.PIPE).wait();

def adbPullFile(src, dst) :
    ensureAdbIsReady()
    command = "adb pull " + src + " " + dst
    proc = subprocess.Popen(command)
    proc.wait()

def adbPushFile(src, dst) :
    ensureAdbIsReady()
    command = "adb push " + src + " " + dst
    proc = subprocess.Popen(command)
    proc.wait()   

def adbIsDeviceConnected() :
    ensureAdbIsReady()
    command = "adb devices"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    str = proc.stdout.readline()
    if "device" in str:
        return True
    return False

def system(command):
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adbPidOf(packagename):
    command = "adb shell ps | grep " + packagename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if str == "":
        return None
    return filter(None, str.split(" "))[1]

def adbForward(host_port, device_port):
    ensureAdbIsReady()
    command = "adb forward tcp:" + str(host_port) + " tcp:" + str(device_port)
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adbDelete(file):
    ensureAdbIsReady()
    command = "adb shell rm " + file
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adbFileExists(filename):
    command = "adb shell ls " + filename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "No such file" in str or len(str) == 0:
        return False
    return True

def findNdtPath():
    ndt_path = "";
    if adbFileExists("/data/data/com.android.ndt/lib/gdbserver.so"):
        ndt_path = "/data/data/com.android.ndt/lib/"
    elif adbFileExists("/data/app/com.android.ndt-1/lib/gdbserver.so"):
        ndt_path = "/data/app/com.android.ndt-1/lib/"
    elif adbFileExists("/data/app/com.android.ndt-2/lib/gdbserver.so"):
        ndt_path = "/data/app/com.android.ndt-2/lib/"
    elif adbFileExists("/data/app/com.android.ndt-3/lib/gdbserver.so"):
        ndt_path = "/data/app/com.android.ndt-3/lib/"
    else:
        ndt_path = "";
    return ndt_path;

#
# The program starts here
#

config = ConfigParser.RawConfigParser()
config.read('galaxy_profiler.config')
print "\r\n [+] Current configuration:"
print "     Package Name  : " + config.get("application","package_name")
print "     Symbols File  : " + os.path.expandvars(config.get("application","shared_lib_with_symbols")) + "\r\n"

print " [+] Checking whether device is connected "
if adbIsDeviceConnected() == False:
    print " Please connect the device and run the program to be profiled"
    exit(1)

print " [+] Checking whether application is running "
pid = adbPidOf(config.get("application","package_name"))
if pid == None:
    print "     Application is not running"
    exit(1)

print " [+] Checking whether symbols file exists "
aux = os.path.expandvars(config.get("application","shared_lib_with_symbols")).split("/")
sharedobject = aux[len(aux)-1]
if not os.path.exists(os.path.expandvars(config.get("application","shared_lib_with_symbols"))):
    print "\r\n [ERROR] No such file: " + os.path.expandvars(config.get("application","shared_lib_with_symbols"))
    print "        Did you edited galaxy_profiler.config ?"
    exit(1)

adbForward(3435,3435)

enableProfiling();

if adbFileExists("/sdcard/perf.data"):
    print " [+] Deleting existing record"
    adbDelete("/sdcard/perf.data")
if adbFileExists("/sdcard/profiling_finished"):
    adbDelete("/sdcard/profiling_finished")


print " [+] Checking whether native debugging tools are installed"
ndt_path = findNdtPath()
if len(ndt_path) is 0:
    print "     Installing Native Debugging tools..."
    system("adb install -r -d ../../device/native-debugging-tools.apk")
    ndt_path = findNdtPath()
    if len(ndt_path) is 0:
        print "     Installation failed"
        sys.exit(0);
print "     Installation found : " + ndt_path

s = None
try :
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 3435))
    data = s.recv(1024)
    if "in-app-remote-shell" not in data:
        sys.exit(0)
    s.send( ndt_path + "libSimpleperf.so record -o /sdcard/perf.data " + config.get("application","record_options") + " -p {PID} --symfs . && echo ok > /sdcard/profiling_finished")
    time.sleep(1)
    s.close()
except:
    print "\r\n     Error: connection failed (127.0.0.1:3435)"
    print "  Does your application implement AndroidRemoteExec ?"
    print "     if it does, try again or try restarting the app/device"
    exit(1)
print " [+] Profiling in progress ... "


if adbFileExists("/sdcard/profiling_finished"):
    adbDelete("/sdcard/profiling_finished")
profilingEnded = False
while not profilingEnded:
	if adbFileExists("/sdcard/profiling_finished"):
	    profilingEnded = True
	if not profilingEnded:
	    time.sleep(1)

if adbFileExists("/sdcard/profiling_finished"):
    adbDelete("/sdcard/profiling_finished")

print " [+] Profiling finished, Collecting data"
adbPullFile("/sdcard/perf.data", "perf.data")


# Determine installation folder name
dso1_path = ""
if(adbFileExists("/data/app/" + config.get("application","package_name") + "-1/lib/arm/")):
    dso1_path = "/data/app/" + config.get("application","package_name") + "-1/lib/arm/"
if(adbFileExists("/data/app/" + config.get("application","package_name") + "-2/lib/arm/")):
    dso1_path = "/data/app/" + config.get("application","package_name") + "-2/lib/arm/"
if(adbFileExists("/data/app/" + config.get("application","package_name") + "-3/lib/arm/")):
    dso1_path = "/data/app/" + config.get("application","package_name") + "-3/lib/arm/"

# Create binary cache folder and copy the shared object
if not os.path.exists("./binary_cache" + dso1_path):
    os.makedirs("./binary_cache" + dso1_path)
copyfile(os.path.expandvars(config.get("application", "shared_lib_with_symbols")), "./binary_cache" + dso1_path + sharedobject)

print " [+] Generating report..."
commands_file = open("generate-report.bat", "w")
commands_file.write("report.py -g --symfs ./binary_cache/ --dsos " + dso1_path + sharedobject + "\r\n")
commands_file.close()
system("generate-report.bat")
os.remove("generate-report.bat")


# libSimpleperf.so record -p 11484 -g -e cpu-cycles:u -f 3000 --duration 60 --dump-symbols -m 1024 --symfs .
# libSimpleperf.so record -p 11484 -g -e cpu-cycles:u -f 800 --duration 60 --dump-symbols --symfs .