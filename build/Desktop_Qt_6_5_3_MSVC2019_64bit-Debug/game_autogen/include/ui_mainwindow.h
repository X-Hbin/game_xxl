/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *lineEditUser;
    QLineEdit *lineEditPass;
    QPushButton *pushButtonLogin;
    QPushButton *pushButtonRegister;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(659, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(200, 90, 261, 91));
        label->setStyleSheet(QString::fromUtf8("color: #ff9de0; font-weight: 700;"));
        lineEditUser = new QLineEdit(centralwidget);
        lineEditUser->setObjectName("lineEditUser");
        lineEditUser->setGeometry(QRect(210, 220, 241, 40));
        lineEditUser->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit{\n"
"    border: 2px solid #ffb6e5;\n"
"    border-radius: 12px;\n"
"    padding: 0 10px;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ffeef8, stop:1 #ffffff);\n"
"    font: 15px \"Microsoft YaHei\";\n"
"    color: #444;\n"
"}\n"
"QLineEdit:focus{\n"
"    border: 2px solid #ff6bcb;\n"
"}\n"
"     "));
        lineEditPass = new QLineEdit(centralwidget);
        lineEditPass->setObjectName("lineEditPass");
        lineEditPass->setGeometry(QRect(210, 280, 241, 40));
        lineEditPass->setStyleSheet(QString::fromUtf8("\n"
"QLineEdit{\n"
"    border: 2px solid #ffb6e5;\n"
"    border-radius: 12px;\n"
"    padding: 0 10px;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ffeef8, stop:1 #ffffff);\n"
"    font: 15px \"Microsoft YaHei\";\n"
"    color: #444;\n"
"}\n"
"QLineEdit:focus{\n"
"    border: 2px solid #ff6bcb;\n"
"}\n"
"     "));
        lineEditPass->setEchoMode(QLineEdit::Password);
        pushButtonLogin = new QPushButton(centralwidget);
        pushButtonLogin->setObjectName("pushButtonLogin");
        pushButtonLogin->setGeometry(QRect(210, 360, 101, 41));
        pushButtonLogin->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff9de0, stop:1 #ff6bcb);\n"
"    color: #fff;\n"
"    font: 16pt \"Microsoft YaHei\";\n"
"}\n"
"QPushButton:hover{\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff6bcb, stop:1 #ff4fa5);\n"
"}\n"
"QPushButton:pressed{\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff4fa5, stop:1 #e0438c);\n"
"}\n"
"     "));
        pushButtonRegister = new QPushButton(centralwidget);
        pushButtonRegister->setObjectName("pushButtonRegister");
        pushButtonRegister->setGeometry(QRect(350, 360, 101, 41));
        pushButtonRegister->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    border: none;\n"
"    border-radius: 16px;\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #9de0ff, stop:1 #6bcbff);\n"
"    color: #fff;\n"
"    font: 16pt \"Microsoft YaHei\";\n"
"}\n"
"QPushButton:hover{\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #6bcbff, stop:1 #4fa5ff);\n"
"}\n"
"QPushButton:pressed{\n"
"    background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4fa5ff, stop:1 #438ce0);\n"
"}\n"
"     "));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 659, 17));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Login", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p align=\"center\"><span style=\" font-size:48pt;\">\346\266\210\346\266\210\344\271\220</span></p></body></html>", nullptr));
        lineEditUser->setPlaceholderText(QCoreApplication::translate("MainWindow", "Username", nullptr));
        lineEditPass->setPlaceholderText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        pushButtonLogin->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225", nullptr));
        pushButtonRegister->setText(QCoreApplication::translate("MainWindow", "\346\263\250\345\206\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
