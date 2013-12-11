from __future__ import print_function
import sys
from PySide import QtCore, QtGui
from mainwindow import Ui_MainWindow

import socket
from slim_pb2 import SlimMessage

class ControlMainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(ControlMainWindow, self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

def playPause():
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/slim_socket")
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.loop_number = 0 
    msg.looper.command = SlimMessage.Looper.PAUSE 
    s.send(msg.SerializeToString())
    s.close()

def record():
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/slim_socket")
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.loop_number = 0 
    msg.looper.command = SlimMessage.Looper.RECORD
    s.send(msg.SerializeToString())
    s.close()

def dryValueChanged():
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/slim_socket")
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.loop_number = 0 
    msg.looper.command = SlimMessage.Looper.SET
    setting = msg.looper.settings.add()
    setting.name = SlimMessage.Looper.Setting.DRY 
    setting.value = mySW.ui.drySlider.value()/100.0
    s.send(msg.SerializeToString())
    s.close()

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    mySW = ControlMainWindow()
    mySW.ui.drySlider.valueChanged.connect (dryValueChanged)
    mySW.ui.playPauseButton.clicked.connect(playPause)
    mySW.ui.recordButton.clicked.connect(record)
    mySW.ui.drySlider.valueChanged.connect(dryValueChanged)
    mySW.show() 
    sys.exit(app.exec_())

