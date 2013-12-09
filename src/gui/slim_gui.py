from __future__ import print_function
import sys
from PySide import QtCore, QtGui
from mainwindow import Ui_MainWindow

import socket
import slim_pb2

s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
s.connect("/tmp/slim_socket")

class ControlMainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(ControlMainWindow, self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

def playPause():
    msg = slim_pb2.LooperCommandMessage()
    msg.command = slim_pb2.PLAY_OR_PAUSE
    print ("msg: " +  msg.SerializeToString())
    s.send(msg.SerializeToString())

def record():
    msg = slim_pb2.LooperCommandMessage()
    msg.command = slim_pb2.RECORD
    print ("msg: " +  msg.SerializeToString())
    s.send(msg.SerializeToString())

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    mySW = ControlMainWindow()
    buttons = {}
    for key in mySW.ui.__dict__:
        if "pushButton" in key:
            button = mySW.ui.__dict__[key]
            buttons[button.text()] = button
    buttons["Play/Pause"].clicked.connect(playPause)
    buttons["Record"].clicked.connect(record)
    mySW.show() 
    sys.exit(app.exec_())

s.close()

