# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'mainwindow.ui'
#
# Created: Wed Dec 11 15:47:02 2013
#      by: pyside-uic 0.2.13 running on PySide 1.1.0
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(700, 420)
        self.centralWidget = QtGui.QWidget(MainWindow)
        self.centralWidget.setObjectName("centralWidget")
        self.horizontalLayoutWidget = QtGui.QWidget(self.centralWidget)
        self.horizontalLayoutWidget.setGeometry(QtCore.QRect(0, 0, 701, 60))
        self.horizontalLayoutWidget.setObjectName("horizontalLayoutWidget")
        self.horizontalLayout_4 = QtGui.QHBoxLayout(self.horizontalLayoutWidget)
        self.horizontalLayout_4.setContentsMargins(0, 0, 0, 0)
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.drySlider = QtGui.QSlider(self.horizontalLayoutWidget)
        self.drySlider.setMaximum(100)
        self.drySlider.setOrientation(QtCore.Qt.Vertical)
        self.drySlider.setObjectName("drySlider")
        self.horizontalLayout_4.addWidget(self.drySlider)
        self.verticalLayout_5 = QtGui.QVBoxLayout()
        self.verticalLayout_5.setObjectName("verticalLayout_5")
        self.recordButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.recordButton.setObjectName("recordButton")
        self.verticalLayout_5.addWidget(self.recordButton)
        self.overdubButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.overdubButton.setObjectName("overdubButton")
        self.verticalLayout_5.addWidget(self.overdubButton)
        self.horizontalLayout_4.addLayout(self.verticalLayout_5)
        self.verticalLayout_6 = QtGui.QVBoxLayout()
        self.verticalLayout_6.setObjectName("verticalLayout_6")
        self.replaceButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.replaceButton.setObjectName("replaceButton")
        self.verticalLayout_6.addWidget(self.replaceButton)
        self.insertButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.insertButton.setObjectName("insertButton")
        self.verticalLayout_6.addWidget(self.insertButton)
        self.horizontalLayout_4.addLayout(self.verticalLayout_6)
        self.verticalLayout_4 = QtGui.QVBoxLayout()
        self.verticalLayout_4.setObjectName("verticalLayout_4")
        self.playPauseButton = QtGui.QPushButton(self.horizontalLayoutWidget)
        self.playPauseButton.setCheckable(False)
        self.playPauseButton.setDefault(False)
        self.playPauseButton.setFlat(False)
        self.playPauseButton.setObjectName("playPauseButton")
        self.verticalLayout_4.addWidget(self.playPauseButton)
        self.horizontalLayout_4.addLayout(self.verticalLayout_4)
        MainWindow.setCentralWidget(self.centralWidget)
        self.menuBar = QtGui.QMenuBar(MainWindow)
        self.menuBar.setGeometry(QtCore.QRect(0, 0, 700, 24))
        self.menuBar.setObjectName("menuBar")
        self.menuHey = QtGui.QMenu(self.menuBar)
        self.menuHey.setObjectName("menuHey")
        MainWindow.setMenuBar(self.menuBar)
        self.mainToolBar = QtGui.QToolBar(MainWindow)
        self.mainToolBar.setObjectName("mainToolBar")
        MainWindow.addToolBar(QtCore.Qt.TopToolBarArea, self.mainToolBar)
        self.statusBar = QtGui.QStatusBar(MainWindow)
        self.statusBar.setObjectName("statusBar")
        MainWindow.setStatusBar(self.statusBar)
        self.actionExit = QtGui.QAction(MainWindow)
        self.actionExit.setObjectName("actionExit")
        self.menuHey.addAction(self.actionExit)
        self.menuBar.addAction(self.menuHey.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QtGui.QApplication.translate("MainWindow", "MainWindow", None, QtGui.QApplication.UnicodeUTF8))
        self.recordButton.setText(QtGui.QApplication.translate("MainWindow", "Record", None, QtGui.QApplication.UnicodeUTF8))
        self.overdubButton.setText(QtGui.QApplication.translate("MainWindow", "Overdub", None, QtGui.QApplication.UnicodeUTF8))
        self.replaceButton.setText(QtGui.QApplication.translate("MainWindow", "Replace", None, QtGui.QApplication.UnicodeUTF8))
        self.insertButton.setText(QtGui.QApplication.translate("MainWindow", "Insert", None, QtGui.QApplication.UnicodeUTF8))
        self.playPauseButton.setText(QtGui.QApplication.translate("MainWindow", "Pause", None, QtGui.QApplication.UnicodeUTF8))
        self.menuHey.setTitle(QtGui.QApplication.translate("MainWindow", "File", None, QtGui.QApplication.UnicodeUTF8))
        self.actionExit.setText(QtGui.QApplication.translate("MainWindow", "Exit", None, QtGui.QApplication.UnicodeUTF8))

