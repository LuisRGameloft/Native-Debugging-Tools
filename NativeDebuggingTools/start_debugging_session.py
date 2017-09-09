
import subprocess
import os.path
import time
import sys
import os

########################################
#      configuration variables         #
########################################

package_name = "com.example.openglapp2"
main_activity = "OpenGLApp2Activity"
android_ndk_gdb = "./desktop/gdb/bin/gdb.exe"
shared_library_dir = "/../../../Example/obj/local/armeabi-v7a/"

########################################
#     declaring required functions     #
########################################
def ensure_adb_is_ready():
    command = "adb start-server"
    subprocess.Popen(command, stdout=subprocess.PIPE).wait();

def adb_run_app(packagename, activity):
	command = "adb shell am start " + packagename + "/" + packagename + "." + activity
	subprocess.Popen(command, stdout=subprocess.PIPE).wait();
	return

def adb_pid_of(packagename):
    command = "adb shell ps | grep " + packagename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    return filter(None, str.split(" "))[1]

def adb_file_exists(filename):
    command = "adb shell ls " + filename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "No such file" in str:
        return False
    return True

def run_jdb(packagename):
    pid = pid_of(packagename)
    command = "adb forward tcp:12346 jdwp:" + pid
    subprocess.Popen(command, stdout=subprocess.PIPE).wait()
    command = "jdb -sourcepath .\ -connect com.sun.jdi.SocketAttach:hostname=127.0.0.1,port=12346"
    subprocess.Popen(command).wait();
    return

def adb_create_file(filename, content) :
    command = "adb shell echo " + content + " ^> " + filename
    proc = subprocess.Popen(command)
    proc.wait()

def adb_delete_file(filename) :
    command = "adb shell rm " + filename
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adb_pull_file(src, dst) :
    command = "adb pull " + src + " " + dst
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def adb_is_debuggable(packagename) :
    command = "adb shell run-as " + packagename + " echo yes"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "is not debuggable" in str:
        return False
    return True

def adb_launch_gdbserver(packagename):
    system("adb forward tcp:12345 tcp:12345")
    pid = adb_pid_of(packagename)
    command = "adb shell run-as " + packagename
    command += " /data/data/" + packagename + "/lib/gdbserver.so :12345 --attach " + pid
    proc = subprocess.Popen(command)
    time.sleep(1)

def system(command):
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()

def is_device_connected() :
    command = "adb devices"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    str = proc.stdout.readline()
    if "device" in str:
        return True
    return False

def is_device_x86():
    command = "adb shell getprop ro.product.cpu.abilist"
    proc = subprocess.Popen(command, stdout=subprocess.PIPE)
    proc.wait()
    str = proc.stdout.readline()
    if "x86" in str:
        return True
    return False

########################################
#  creating commands.txt file for gdb  #
########################################

ensure_adb_is_ready();

if is_device_x86() :
    os.chdir(shared_library_dir + "/x86/")
else:
    os.chdir(shared_library_dir + "/armeabi-v7a/")
if os.path.exists("commands.txt") :
    os.remove("commands.txt")
commands_file = open("commands.txt", "w")
commands_file.write("set osabi GNU/Linux \r\n");
if is_device_x86() :
    commands_file.write("set architecture i386 \r\n");
else:
    commands_file.write("set architecture arm \r\n");
commands_file.write("set solib-search-path ./ \r\n");
commands_file.write("set sysroot ./ \r\n");
commands_file.write("file app_process32 \r\n");
commands_file.write("target remote:12345 \r\n");
commands_file.write("handle SIG33 nostop \r\n");
commands_file.write("handle SIG33 noprint \r\n");
commands_file.close();

########################################
#  pulling required files from device  #
########################################

if is_device_connected() :
    if adb_file_exists("/system/bin/app_process32"):
        adb_pull_file("/system/bin/app_process32", "app_process32")
    elif adb_file_exists("/system/bin/app_process"):
        adb_pull_file("/system/bin/app_process", "app_process32")
else :
    print "device disconnected!, please reconnect it and try it again"
    sys.exit(0)

########################################
#        checking requirements         #
########################################
debuggable = adb_is_debuggable(package_name)
gdbserver_exist = adb_file_exists("/data/data/" + package_name + "/lib/gdbserver.so")
failed = False

if not debuggable :
    print "ERROR : " + package_name + " is not debuggable, please add android:debuggable=\"true\" to the manifest file"
    failed = True
if not gdbserver_exist :
    if is_device_x86():
        print "ERROR : gdbserver.so was not found on the device, please copy gdbserver.so into /libs/x86 and rebuild the APK"
    else :
        print "ERROR : gdbserver.so was not found on the device, please copy gdbserver.so into /libs/armeabi-v7a and rebuild the APK"
    failed = True
if failed:
    sys.exit(0);

########################################
#         launching the game           #
########################################

if adb_is_debuggable(package_name) :
    adb_create_file("sdcard/start_gdbserver", "sdcard/start_gdbserver")
    adb_run_app(package_name, main_activity);
    time.sleep(2)
    adb_delete_file("/sdcard/start_gdbserver")
else :
    print package_name + " is not debuggable, please add android:debuggable=\"true\" to the manifest file"
    sys.exit(0)

########################################
#        launching GDB Server          #
########################################
if adb_file_exists("/data/data/" + package_name + "/lib/gdbserver.so") :
    adb_launch_gdbserver(package_name)
else :
    print "gdbserver.so was not found"
    sys.exit(0)


########################################
#        launching GNU Debugger        #
########################################
subprocess.call(android_ndk_gdb + " -q --command=commands.txt");

