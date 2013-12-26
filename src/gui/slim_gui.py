from __future__ import print_function
import sys
from PySide import QtCore, QtGui
from mainwindow import Ui_MainWindow

import socket
from slim_pb2 import SlimMessage

from google.protobuf.internal import encoder


#from os import unlink
#unlink("/tmp/slim_socket")
#s.bind("/tmp/slim_socket")
#s.listen(5);

def send(msg):
    string = msg.SerializeToString()
    string = encoder._VarintBytes(len(string)) + string
    msg.looper.number = 1 
    string2 = msg.SerializeToString()
    string += encoder._VarintBytes(len(string2)) + string2
    s = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    s.connect("/tmp/slim_socket") 
    s.send(string)
    s.close()

class ControlMainWindow(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(ControlMainWindow, self).__init__(parent)
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

def playPause():
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.number = 0 
    msg.looper.command = SlimMessage.Looper.PAUSE 
    send(msg)

def record():
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.number = 0 
    msg.looper.command = SlimMessage.Looper.RECORD
    send(msg)

def dryValueChanged():
    msg = SlimMessage()
    msg.type = SlimMessage.LOOPER
    msg.looper.number = 0 
    msg.looper.command = SlimMessage.Looper.SET
    setting = msg.looper.settings.add()
    setting.name = SlimMessage.Looper.Setting.DRY 
    setting.value = mySW.ui.drySlider.value()/100.0
    send(msg)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    mySW = ControlMainWindow()
    mySW.ui.drySlider.valueChanged.connect (dryValueChanged)
    mySW.ui.playPauseButton.clicked.connect(playPause)
    mySW.ui.recordButton.clicked.connect(record)
    mySW.ui.drySlider.valueChanged.connect(dryValueChanged)
    mySW.show() 
    sys.exit(app.exec_())

