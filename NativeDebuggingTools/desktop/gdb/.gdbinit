python

import socket
import time
import subprocess
import inspect
import os

#
# called when the user type enable-ui
#
def openEditor():
    command = "nw " + os.environ["NDT_FOLDER"] + "/dashboard"
    subprocess.Popen(command, stdout=subprocess.PIPE);

#
# called when inferior process continues
#
def on_continue(arg):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(("127.0.0.1", 2222))
    s.send("continue");
    time.sleep(1);
    s.close();

#
# called when inferior process stops
#
def on_stop(arg):
    try:
        sal = gdb.selected_frame().find_sal()
        current_line = sal.line
        file_name = sal.symtab.fullname()
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 2222))
        s.send(file_name + "@" + str(current_line));
        time.sleep(1);
        s.close();
    except:
        return

#
# Command to replace the original "frame" command
#

class FrameOverride (gdb.Command):
  """Greet the whole world."""

  def __init__ (self):
    super (FrameOverride, self).__init__ ("frame", gdb.COMMAND_USER)

  def invoke (self, arg, from_tty):
    pos = int(arg)
    frame = gdb.newest_frame()
    while(pos):
        frame = frame.older();
        pos -= 1
    frame.select()
    on_stop(0)


#
# Command to enable UI mode
#

class EnableUICommand (gdb.Command):
  """Greet the whole world."""

  def __init__ (self):
    super (EnableUICommand, self).__init__ ("enable-ui", gdb.COMMAND_USER)

  def invoke (self, arg, from_tty):
      FrameOverride()
      openEditor()
      gdb.events.cont.connect(on_continue)
      gdb.events.stop.connect(on_stop)

EnableUICommand()
end

