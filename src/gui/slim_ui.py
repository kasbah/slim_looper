from __future__ import print_function
import sys
from PyQt4.QtGui import QGroupBox, QWidget, QVBoxLayout, QMenuBar, QHBoxLayout, QDial
from PyQt4.QtGui import QStatusBar, QApplication, QMainWindow, QPushButton, QStyleFactory
from PyQt4.QtGui import QLabel 
from PyQt4.QtCore import QString, QRect, QMetaObject, SIGNAL, Qt

from ui_settings import slimUISettings
from slim_pb2 import SlimMessage
import slim_socket

import ui_palette

from carla_widgets.paramspinbox import ParamSpinBox

try:
    _fromUtf8 = QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

commands = []
cmd_dict = SlimMessage.Looper.DESCRIPTOR.enum_types_by_name["State"].values_by_number
for number, value in cmd_dict.iteritems():
    commands.append((number, value.name))

class LooperWidget(QGroupBox):
    def __init__(self, parent, looper_number, looper_settings):
        global commands
        super(LooperWidget, self).__init__(parent)
        self.verticalLayout = QVBoxLayout(self)
        self.hwidget = QWidget()
        self.verticalLayout.addWidget(self.hwidget)
        self.horizontalLayout = QHBoxLayout(self.hwidget)
        self.buttons = []
        self.sliders = []
        self.number = looper_number
        for number,name in commands:
            button = QPushButton(self,
                    text=QApplication.translate("MainWindow",
                        name.title(),
                        None, QApplication.UnicodeUTF8))
            self.buttons.append(button)
            self.horizontalLayout.addWidget(self.buttons[-1])
            button.command = number
            button.clicked.connect(self.onButtonClicked)
        for d in looper_settings:
            widget = QWidget()
            widget.setMaximumHeight(90)
            widget.setMaximumWidth (90)
            widget.setMinimumHeight(90)
            widget.setMinimumWidth (90)
            layout = QVBoxLayout(widget)
            name = d["lv2:name"][0].replace("Looper ","", 1)
            widget.label = QLabel(name)
            widget.label.setAlignment(Qt.AlignCenter)
            widget.dial = QDial()
            layout.addWidget(widget.dial)
            layout.addWidget(widget.label)
            self.sliders.append(widget)
            widget.dial.setMaximum(int(d["lv2:maximum"][0] * 100.0))
            widget.dial.setMinimum(int(d["lv2:minimum"][0] * 100.0))
            widget.dial.setValue  (int(d["lv2:default"][0] * 100.0))
            widget.dial.name = SlimMessage.Looper.Setting.__dict__[name.upper()]
            widget.dial.valueChanged.connect(self.onSliderChanged)
            self.verticalLayout.addWidget(widget)


    def retranslateUi(self):
        pass
    def onButtonClicked(self):
        msg = SlimMessage()
        msg.type = SlimMessage.LOOPER
        msg.looper.number = self.number 
        msg.looper.state = self.sender().command 
        slim_socket.send(msg)
    def onSliderChanged(self, value):
        msg = SlimMessage()
        msg.type = SlimMessage.LOOPER
        msg.looper.number = self.number 
        setting = msg.looper.settings.add()
        setting.name = self.sender().name 
        setting.value = value / 100.0
        slim_socket.send(msg)

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

        ui_settings = slimUISettings()

        QMetaObject.connectSlotsByName(SLim)

        self.loopers = []
        self.loopers.append(LooperWidget(self.centralwidget, 0, ui_settings.looper))
        self.loopers.append(LooperWidget(self.centralwidget, 1, ui_settings.looper))
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
    QApplication.setStyle(QStyleFactory.create("Cleanlooks"))
    QApplication.setPalette(QApplication.style().standardPalette())
    #QApplication.setStyle(QStyleFactory.create("QtCurve"))
    #QApplication.setPalette(ui_palette.fPalBlue)
    #QApplication.setPalette(ui_palette.fPalBlack)
    app = QApplication(sys.argv)
    mySW = ControlMainWindow()
    mySW.show() 
    sys.exit(app.exec_())
