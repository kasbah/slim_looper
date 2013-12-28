# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created: Fri Dec 27 18:04:40 2013
#      by: PyQt4 UI code generator 4.9.1
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    _fromUtf8 = lambda s: s

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName(_fromUtf8("MainWindow"))
        MainWindow.resize(651, 205)
        MainWindow.setStyleSheet(_fromUtf8(""))
        self.centralwidget = QtGui.QWidget(MainWindow)
        self.centralwidget.setObjectName(_fromUtf8("centralwidget"))
        self.verticalLayout_3 = QtGui.QVBoxLayout(self.centralwidget)
        self.verticalLayout_3.setObjectName(_fromUtf8("verticalLayout_3"))
        self.looperWidget_1 = QtGui.QGroupBox(self.centralwidget)
        self.looperWidget_1.setObjectName(_fromUtf8("looperWidget_1"))
        self.verticalLayout = QtGui.QVBoxLayout(self.looperWidget_1)
        self.verticalLayout.setObjectName(_fromUtf8("verticalLayout"))
        self.horizontalLayout_2 = QtGui.QHBoxLayout()
        self.horizontalLayout_2.setObjectName(_fromUtf8("horizontalLayout_2"))
        self.boolSettingWidget = ClickableWidget(self.looperWidget_1)
        self.boolSettingWidget.setMinimumSize(QtCore.QSize(60, 24))
        self.boolSettingWidget.setMaximumSize(QtCore.QSize(60, 24))
        self.boolSettingWidget.setMouseTracking(True)
        self.boolSettingWidget.setObjectName(_fromUtf8("boolSettingWidget"))
        self.horizontalLayout_3 = QtGui.QHBoxLayout(self.boolSettingWidget)
        self.horizontalLayout_3.setObjectName(_fromUtf8("horizontalLayout_3"))
        self.radioButton = LEDButton(self.boolSettingWidget)
        self.radioButton.setMinimumSize(QtCore.QSize(14, 14))
        self.radioButton.setMaximumSize(QtCore.QSize(14, 14))
        self.radioButton.setText(_fromUtf8(""))
        self.radioButton.setCheckable(True)
        self.radioButton.setAutoRepeat(False)
        self.radioButton.setObjectName(_fromUtf8("radioButton"))
        self.horizontalLayout_3.addWidget(self.radioButton)
        self.label = QtGui.QLabel(self.boolSettingWidget)
        self.label.setMinimumSize(QtCore.QSize(50, 14))
        self.label.setObjectName(_fromUtf8("label"))
        self.horizontalLayout_3.addWidget(self.label)
        self.horizontalLayout_2.addWidget(self.boolSettingWidget)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName(_fromUtf8("horizontalLayout"))
        self.drySlider = QtGui.QSlider(self.looperWidget_1)
        self.drySlider.setMaximum(100)
        self.drySlider.setOrientation(QtCore.Qt.Vertical)
        self.drySlider.setObjectName(_fromUtf8("drySlider"))
        self.horizontalLayout.addWidget(self.drySlider)
        self.recordButton = QtGui.QPushButton(self.looperWidget_1)
        self.recordButton.setMaximumSize(QtCore.QSize(16777215, 24))
        self.recordButton.setStyleSheet(_fromUtf8(""))
        self.recordButton.setCheckable(True)
        self.recordButton.setObjectName(_fromUtf8("recordButton"))
        self.horizontalLayout.addWidget(self.recordButton)
        self.overdubButton = QtGui.QPushButton(self.looperWidget_1)
        self.overdubButton.setMaximumSize(QtCore.QSize(16777215, 24))
        self.overdubButton.setObjectName(_fromUtf8("overdubButton"))
        self.horizontalLayout.addWidget(self.overdubButton)
        self.insertButton = QtGui.QPushButton(self.looperWidget_1)
        self.insertButton.setMinimumSize(QtCore.QSize(0, 24))
        self.insertButton.setMaximumSize(QtCore.QSize(16777215, 24))
        self.insertButton.setObjectName(_fromUtf8("insertButton"))
        self.horizontalLayout.addWidget(self.insertButton)
        self.replaceButton = QtGui.QPushButton(self.looperWidget_1)
        self.replaceButton.setMaximumSize(QtCore.QSize(16777215, 24))
        self.replaceButton.setObjectName(_fromUtf8("replaceButton"))
        self.horizontalLayout.addWidget(self.replaceButton)
        self.playPauseButton = QtGui.QPushButton(self.looperWidget_1)
        self.playPauseButton.setMaximumSize(QtCore.QSize(16777215, 24))
        self.playPauseButton.setCheckable(False)
        self.playPauseButton.setDefault(False)
        self.playPauseButton.setFlat(False)
        self.playPauseButton.setObjectName(_fromUtf8("playPauseButton"))
        self.horizontalLayout.addWidget(self.playPauseButton)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.verticalLayout_3.addWidget(self.looperWidget_1)
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtGui.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 651, 20))
        self.menubar.setObjectName(_fromUtf8("menubar"))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtGui.QStatusBar(MainWindow)
        self.statusbar.setObjectName(_fromUtf8("statusbar"))
        MainWindow.setStatusBar(self.statusbar)
        self.toolBar = QtGui.QToolBar(MainWindow)
        self.toolBar.setObjectName(_fromUtf8("toolBar"))
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.toolBar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.looperWidget_1.setTitle(QtGui.QApplication.translate("MainWindow", "Looper 1", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("MainWindow", "sync", None, QtGui.QApplication.UnicodeUTF8))
        self.recordButton.setText(QtGui.QApplication.translate("MainWindow", "Record", None, QtGui.QApplication.UnicodeUTF8))
        self.overdubButton.setText(QtGui.QApplication.translate("MainWindow", "Overdub", None, QtGui.QApplication.UnicodeUTF8))
        self.insertButton.setText(QtGui.QApplication.translate("MainWindow", "Insert", None, QtGui.QApplication.UnicodeUTF8))
        self.replaceButton.setText(QtGui.QApplication.translate("MainWindow", "Replace", None, QtGui.QApplication.UnicodeUTF8))
        self.playPauseButton.setText(QtGui.QApplication.translate("MainWindow", "Pause", None, QtGui.QApplication.UnicodeUTF8))
        self.toolBar.setWindowTitle(QtGui.QApplication.translate("MainWindow", "toolBar", None, QtGui.QApplication.UnicodeUTF8))

from carla_widgets.ledbutton import LEDButton
from carla_widgets.clickablelabel import ClickableWidget
