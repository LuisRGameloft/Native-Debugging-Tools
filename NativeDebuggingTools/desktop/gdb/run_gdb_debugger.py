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

import subprocess
import os.path
import socket
import time
import sys
import os
from shutil import copyfile

#
# Configuration variables
#
package_name       = os.environ["PACKAGE_NAME"]
main_activity      = os.environ["ACTIVITY_NAME"]
shared_library_dir = os.environ["SHARED_LIB_PATH"]
android_ndk_gdb    = os.environ["NDK_PATH"] + "/prebuilt/windows-x86_64/bin/gdb.exe"
adb_tool           = os.environ["ADB_PATH"] + "/adb.exe"
os.environ["NDT_FOLDER"] = os.getcwd()

#
# do not touch these variables
#
start_app = False
ndt_path  = ""

#
# it defines utils functions
#
def ensureAdbIsReady():
    command = adb_tool + " start-server "
    subprocess.Popen(command, stdout=subprocess.PIPE).wait();

def adbRunApp(packagename, activity):
    command = adb_tool + " shell am start " + packagename + "/" + activity
    subprocess.Popen(command, stdout=subprocess.PIPE).wait();
    return

def adbFileExists(filename):
    command = adb_tool + " shell ls " + filename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "No such file" in str:
        return False
    if len(str) is 0:
        return False
    return True

def adbCreateFile(filename, content) :
    f = open("commands.txt","w")
    f.write(content)
    f.close()
    command = adb_tool + " push commands.txt " + filename
    proc = subprocess.Popen(command)
    proc.wait()
    os.remove("commands.txt")

def adbDeleteFile(filename) :
    command = adb_tool + " shell rm " + filename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adbPullFile(src, dst) :
    command = adb_tool + " pull " + src + " " + dst
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adbIsDebuggable(packagename) :
    command = adb_tool + " shell run-as " + packagename + " echo yes"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "is not debuggable" in str:
        return False
    return True

def system(command):
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def isDeviceConnected() :
    command = adb_tool + " devices"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    str = proc.stdout.readline()
    if "device" in str:
        return True
    return False

def isDeviceX86():
    command = adb_tool + " shell getprop ro.product.cpu.abilist"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "x86" in str:
        return True
    return False

def adbPidOf(packagename):
    command = adb_tool + " shell ps | grep " + packagename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if len(str) is 0:
        return None;
    return filter(None, str.split(" "))[1]

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

print "\r\n[+] Current config:"
print "    Package name          : " + package_name
print "    Main Activity         : " + main_activity
print "    Shared library folder : " + shared_library_dir
print "    Gdb executable        : " + android_ndk_gdb
print "\r\n"

ensureAdbIsReady();

print " [+] Checking whether device is connected"
if not isDeviceConnected() :
    print "Error: device disconnected!"
    sys.exit(0);
system("adb forward tcp:12345 tcp:12345");

print " [+] Checking whether application is debuggable"
if not adbIsDebuggable(package_name):
    print "\r\n   Error: application is not debuggable"
    sys.exit(0);

print " [+] Checking whether native debugging tools are installed"
ndt_path = findNdtPath()
if len(ndt_path) is 0:
    print "     Installing Native Debugging tools..."
    system("adb install -r -d ./../../device/native-debugging-tools.apk")
    ndt_path = findNdtPath()
    if len(ndt_path) is 0:
        print "Installation failed"
        sys.exit(0);
print "     Installation found : " + ndt_path

print " [+] Checking whether application is running "
pid = adbPidOf(package_name)
if pid == None:
    print "     Application is not running (debug from start)"
    start_app = True
else:
    print "     Pid: " + pid

if start_app :
    print "     Creating commands.txt into the device"
    print "\r\n     IMPORTANT NOTE: It only works if the applications implements AndroidRemoteExec"
    adbCreateFile("/sdcard/commands.txt", ndt_path + "gdbserver.so :12345 --attach {PID}")
    adbRunApp(package_name, main_activity);
    time.sleep(2);
    adbDeleteFile("/sdcard/commands.txt");
else :
    print " [+] Connecting to remote process"
    system("adb forward tcp:3435 tcp:3435");
    s = None
    try :
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 3435))
        data = s.recv(1024)
        if "in-app-remote-shell" in data:
            print "     Connection succeed"
            print "     Attaching debugger"
            s.send( ndt_path + "gdbserver.so :12345 --attach {PID}")
            time.sleep(1);
            s.close();
        else:
            # print "\r\n   Error: Application doesn't implement AndroidRemoteExec"
            # print "   Read ./AndroidRemoteExec/README.md for more info"
            exit(1)    
    except:
        print "\r\n   Error: connection failed (127.0.0.1:3435)"
        print "   Does your application implement AndroidRemoteExec ?"
        print "     if it does, try again or try restarting the app/device"
        exit(1)

#  creating commands.txt file for gdb client 
print " [+] Creating configuration for Gdb client"
if isDeviceX86() :
    copyfile(".gdbinit", shared_library_dir + "/x86/.gdbinit")
    os.chdir(shared_library_dir + "/x86/")
else:
    copyfile(".gdbinit", shared_library_dir + "/armeabi-v7a/.gdbinit")
    os.chdir(shared_library_dir + "/armeabi-v7a/")
if os.path.exists("commands.txt") :
    os.remove("commands.txt")
commands_file = open("commands.txt", "w")
commands_file.write("set osabi GNU/Linux \r\n");
if isDeviceX86() :
    commands_file.write("set architecture i386 \r\n");
else:
    commands_file.write("set architecture arm \r\n");
commands_file.write("set solib-search-path ./ \r\n");
commands_file.write("set sysroot ./ \r\n");
commands_file.write("file app_process32 \r\n");
commands_file.write("target remote:12345 \r\n");
commands_file.write("handle SIG33 nostop \r\n");
commands_file.write("handle SIG33 noprint \r\n");
commands_file.write("shell cls\r\n");
commands_file.write("shell echo.\r\n");
commands_file.write("shell echo         Welcome to the GNU Project Debugger\r\n");
commands_file.write("shell echo         you are debugging: " + package_name + "\r\n");
commands_file.write("shell echo         type 'help' for a list of gdb commands\r\n");
commands_file.write("shell echo.\r\n");
commands_file.close();

print " [+] Pulling required files from device"
if isDeviceConnected() :
    if adbFileExists("/system/bin/app_process32"):
        adbPullFile("/system/bin/app_process32", "app_process32")
    elif adbFileExists("/system/bin/app_process"):
        adbPullFile("/system/bin/app_process", "app_process32")
else :
    print "   [+] Error: device disconnected!, please reconnect it and try it again"
    sys.exit(0)

try:
    subprocess.call(android_ndk_gdb + " -q --command=commands.txt -iex \\\"add-auto-load-safe-path /\\\" ");
except:
    sys.exit(0);