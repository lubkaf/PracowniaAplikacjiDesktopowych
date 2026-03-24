# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'form.ui'
##
## Created by: Qt User Interface Compiler version 6.11.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QLabel, QLineEdit, QPushButton,
    QSizePolicy, QWidget)

class Ui_Widget(object):
    def setupUi(self, Widget):
        if not Widget.objectName():
            Widget.setObjectName(u"Widget")
        Widget.resize(291, 241)
        self.txtFirstNum = QLineEdit(Widget)
        self.txtFirstNum.setObjectName(u"txtFirstNum")
        self.txtFirstNum.setGeometry(QRect(10, 30, 113, 26))
        self.txtSecNum = QLineEdit(Widget)
        self.txtSecNum.setObjectName(u"txtSecNum")
        self.txtSecNum.setGeometry(QRect(160, 30, 113, 26))
        self.txtResult = QLineEdit(Widget)
        self.txtResult.setObjectName(u"txtResult")
        self.txtResult.setGeometry(QRect(10, 190, 113, 26))
        self.txtResult.setReadOnly(True)
        self.btnAdd = QPushButton(Widget)
        self.btnAdd.setObjectName(u"btnAdd")
        self.btnAdd.setGeometry(QRect(10, 80, 31, 26))
        self.btnSubstract = QPushButton(Widget)
        self.btnSubstract.setObjectName(u"btnSubstract")
        self.btnSubstract.setGeometry(QRect(50, 80, 31, 26))
        self.btnDivide = QPushButton(Widget)
        self.btnDivide.setObjectName(u"btnDivide")
        self.btnDivide.setGeometry(QRect(90, 80, 31, 26))
        self.btnMultiply = QPushButton(Widget)
        self.btnMultiply.setObjectName(u"btnMultiply")
        self.btnMultiply.setGeometry(QRect(130, 80, 31, 26))
        self.label = QLabel(Widget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(10, 170, 66, 18))
        self.label_2 = QLabel(Widget)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(10, 10, 111, 18))
        self.label_3 = QLabel(Widget)
        self.label_3.setObjectName(u"label_3")
        self.label_3.setGeometry(QRect(160, 10, 101, 18))

        self.retranslateUi(Widget)

        QMetaObject.connectSlotsByName(Widget)
    # setupUi

    def retranslateUi(self, Widget):
        Widget.setWindowTitle(QCoreApplication.translate("Widget", u"Widget", None))
        self.btnAdd.setText(QCoreApplication.translate("Widget", u"+", None))
        self.btnSubstract.setText(QCoreApplication.translate("Widget", u"-", None))
        self.btnDivide.setText(QCoreApplication.translate("Widget", u"/", None))
        self.btnMultiply.setText(QCoreApplication.translate("Widget", u"*", None))
        self.label.setText(QCoreApplication.translate("Widget", u"Wynik", None))
        self.label_2.setText(QCoreApplication.translate("Widget", u"pierwsza liczba", None))
        self.label_3.setText(QCoreApplication.translate("Widget", u"druga liczba", None))
    # retranslateUi

