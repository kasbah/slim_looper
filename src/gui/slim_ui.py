from __future__ import print_function
import sys
from PyQt4.QtGui import QGroupBox, QWidget, QVBoxLayout, QMenuBar, QHBoxLayout
from PyQt4.QtGui import QStatusBar, QApplication, QMainWindow, QPushButton
from PyQt4.QtCore import QString, QRect, QMetaObject, SIGNAL

from slim_pb2 import SlimMessage
from google.protobuf.internal import encoder
import operator

try:
    _fromUtf8 = QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

commands = SlimMessage.Looper.DESCRIPTOR.enum_types_by_name["Command"].values_by_number.copy()
for key in commands:
    commands[key] = commands[key].name
commands = sorted(commands.iteritems(), key=operator.itemgetter(0))


class LooperWidget(QGroupBox):
    def __init__(self, parent):
        global commands
        super(LooperWidget, self).__init__(parent)
        self.horizontalLayout = QHBoxLayout(self)
        self.buttons = []
        for number,name in commands:
            if name != "SET":
                self.buttons.append(QPushButton(self, text=QApplication.translate("MainWindow", name.title(), None, QApplication.UnicodeUTF8)))
                self.horizontalLayout.addWidget(self.buttons[-1])
    def retranslateUi(self):
        pass

class Ui_SLim(object):
    def setupUi(self, SLim):
        SLim.setObjectName(_fromUtf8("SLim"))
        SLim.resize(800, 601)
        self.centralwidget = QWidget(SLim)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayout = QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        SLim.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(SLim)
        self.menubar.setGeometry(QRect(0, 0, 800, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        SLim.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(SLim)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        SLim.setStatusBar(self.statusbar)

        QMetaObject.connectSlotsByName(SLim)

        self.loopers = []
        self.loopers.append(LooperWidget(self.centralwidget))
        self.loopers.append(LooperWidget(self.centralwidget))
        for looper in self.loopers:
            self.verticalLayout.addWidget(looper)

        self.retranslateUi(SLim)

    def retranslateUi(self, SLim):
        for looper in SLim.ui.loopers:
            looper.retranslateUi()
        SLim.setWindowTitle(QApplication.translate("SLim", "MainWindow", None, QApplication.UnicodeUTF8))

class ControlMainWindow(QMainWindow):
    def __init__(self, parent=None):
        super(ControlMainWindow, self).__init__(parent)
        self.ui = Ui_SLim()
        self.ui.setupUi(self)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    mySW = ControlMainWindow()
    mySW.show() 
    sys.exit(app.exec_())
