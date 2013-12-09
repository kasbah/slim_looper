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
    msg = slim_pb2.LooperMessage()
    msg.type = slim_pb2.LooperMessage.COMMAND
    msg.command.command = slim_pb2.PLAY_OR_PAUSE
    s.send(msg.SerializeToString())

def record():
    msg = slim_pb2.LooperMessage()
    msg.type = slim_pb2.LooperMessage.COMMAND
    msg.command.command = slim_pb2.RECORD
    s.send(msg.SerializeToString())

def dryValueChanged():
    msg = slim_pb2.LooperMessage()
    msg.type = slim_pb2.LooperMessage.SETTING
    msg.setting.dry = mySW.ui.drySlider.value()/100.0
    s.send(msg.SerializeToString())

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    mySW = ControlMainWindow()
    mySW.ui.drySlider.valueChanged.connect (dryValueChanged)
    mySW.ui.playPauseButton.clicked.connect(playPause)
    mySW.ui.recordButton.clicked.connect(playPause)
    mySW.ui.drySlider.valueChanged.connect(dryValueChanged)
    mySW.show() 
    sys.exit(app.exec_())

s.close()

