/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *first_number;
    QLabel *second_number;
    QLineEdit *txtFirstNum;
    QLineEdit *txtSecNum;
    QPushButton *btnAdd;
    QPushButton *btnSubstract;
    QPushButton *btnDivide;
    QPushButton *btnMultiply;
    QLabel *result;
    QLineEdit *txtResult;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1038, 575);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        first_number = new QLabel(centralwidget);
        first_number->setObjectName("first_number");
        first_number->setGeometry(QRect(310, 40, 101, 18));
        second_number = new QLabel(centralwidget);
        second_number->setObjectName("second_number");
        second_number->setGeometry(QRect(440, 40, 111, 18));
        txtFirstNum = new QLineEdit(centralwidget);
        txtFirstNum->setObjectName("txtFirstNum");
        txtFirstNum->setGeometry(QRect(310, 60, 113, 26));
        txtSecNum = new QLineEdit(centralwidget);
        txtSecNum->setObjectName("txtSecNum");
        txtSecNum->setGeometry(QRect(440, 60, 113, 26));
        btnAdd = new QPushButton(centralwidget);
        btnAdd->setObjectName("btnAdd");
        btnAdd->setGeometry(QRect(310, 100, 41, 41));
        btnSubstract = new QPushButton(centralwidget);
        btnSubstract->setObjectName("btnSubstract");
        btnSubstract->setGeometry(QRect(360, 100, 41, 41));
        btnDivide = new QPushButton(centralwidget);
        btnDivide->setObjectName("btnDivide");
        btnDivide->setGeometry(QRect(410, 100, 41, 41));
        btnMultiply = new QPushButton(centralwidget);
        btnMultiply->setObjectName("btnMultiply");
        btnMultiply->setGeometry(QRect(460, 100, 41, 41));
        result = new QLabel(centralwidget);
        result->setObjectName("result");
        result->setGeometry(QRect(310, 180, 111, 18));
        txtResult = new QLineEdit(centralwidget);
        txtResult->setObjectName("txtResult");
        txtResult->setGeometry(QRect(400, 170, 113, 26));
        txtResult->setReadOnly(true);
        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        first_number->setText(QCoreApplication::translate("MainWindow", "First number:", nullptr));
        second_number->setText(QCoreApplication::translate("MainWindow", "Second number:", nullptr));
        btnAdd->setText(QCoreApplication::translate("MainWindow", "+", nullptr));
        btnSubstract->setText(QCoreApplication::translate("MainWindow", "-", nullptr));
        btnDivide->setText(QCoreApplication::translate("MainWindow", "/", nullptr));
        btnMultiply->setText(QCoreApplication::translate("MainWindow", "*", nullptr));
        result->setText(QCoreApplication::translate("MainWindow", "The result is: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
