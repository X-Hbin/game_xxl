/********************************************************************************
** Form generated from reading UI file 'menu.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENU_H
#define UI_MENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Menu
{
public:
    QHBoxLayout *mainLay;
    QWidget *sideBar;
    QVBoxLayout *sideLay;
    QPushButton *btnSingle;
    QPushButton *btnOnline;
    QPushButton *btnSkill;
    QPushButton *btnRank;
    QPushButton *btnRule;
    QSpacerItem *sideSpacer;
    QPushButton *btnSettings;
    QStackedWidget *stackRight;
    QWidget *pageWelcome;
    QVBoxLayout *welcomeLay;
    QSpacerItem *welcomeTopSpacer;
    QLabel *labelWelcome;
    QLabel *labelSub;
    QSpacerItem *welcomeBottomSpacer;

    void setupUi(QWidget *Menu)
    {
        if (Menu->objectName().isEmpty())
            Menu->setObjectName("Menu");
        Menu->resize(659, 600);
        Menu->setStyleSheet(QString::fromUtf8("QWidget#Menu{\n"
"        background-color: rgba(0,0,0,170);\n"
"        border:1px solid #ff9de0;\n"
"}"));
        mainLay = new QHBoxLayout(Menu);
        mainLay->setSpacing(0);
        mainLay->setObjectName("mainLay");
        sideBar = new QWidget(Menu);
        sideBar->setObjectName("sideBar");
        sideBar->setMaximumSize(QSize(160, 16777215));
        sideBar->setStyleSheet(QString::fromUtf8("#sideBar{background:#111;border-right:2px solid #ff9de0;}"));
        sideLay = new QVBoxLayout(sideBar);
        sideLay->setSpacing(12);
        sideLay->setObjectName("sideLay");
        sideLay->setContentsMargins(16, 40, 16, 40);
        btnSingle = new QPushButton(sideBar);
        btnSingle->setObjectName("btnSingle");
        btnSingle->setMinimumSize(QSize(0, 48));
        btnSingle->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    color:#fff;\n"
"    font:15px;\n"
"    text-align:left;\n"
"    padding-left:12px;\n"
"    border:none;\n"
"    border-radius:8px;\n"
"    background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff9de0, stop:1 transparent);\n"
"}\n"
"QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff6bcb, stop:1 transparent);}\n"
"QPushButton:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff4fa5, stop:1 transparent);}\n"
"         "));
        btnSingle->setCheckable(true);
        btnSingle->setAutoExclusive(true);

        sideLay->addWidget(btnSingle);

        btnOnline = new QPushButton(sideBar);
        btnOnline->setObjectName("btnOnline");
        btnOnline->setMinimumSize(QSize(0, 48));
        btnOnline->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    color:#fff;\n"
"    font:15px;\n"
"    text-align:left;\n"
"    padding-left:12px;\n"
"    border:none;\n"
"    border-radius:8px;\n"
"    background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #9de0ff, stop:1 transparent);\n"
"}\n"
"QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #6bcbff, stop:1 transparent);}\n"
"QPushButton:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4fa5ff, stop:1 transparent);}\n"
"         "));
        btnOnline->setCheckable(true);
        btnOnline->setAutoExclusive(true);

        sideLay->addWidget(btnOnline);

        btnSkill = new QPushButton(sideBar);
        btnSkill->setObjectName("btnSkill");
        btnSkill->setMinimumSize(QSize(0, 48));
        btnSkill->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    color:#fff;\n"
"    font:15px;\n"
"    text-align:left;\n"
"    padding-left:12px;\n"
"    border:none;\n"
"        border-radius:8px;\n"
"    background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #fff59d, stop:1 transparent);\n"
"}\n"
"QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #fdd835, stop:1 transparent);}\n"
"QPushButton:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #fbc02d, stop:1 transparent);}\n"
"         "));
        btnSkill->setCheckable(true);
        btnSkill->setAutoExclusive(true);

        sideLay->addWidget(btnSkill);

        btnRank = new QPushButton(sideBar);
        btnRank->setObjectName("btnRank");
        btnRank->setMinimumSize(QSize(0, 48));
        btnRank->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    color:#fff;\n"
"    font:15px;\n"
"    text-align:left;\n"
"    padding-left:12px;\n"
"    border:none;\n"
"    border-radius:8px;\n"
"    background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #b39ddb, stop:1 transparent);\n"
"}\n"
"QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #9575cd, stop:1 transparent);}\n"
"QPushButton:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7e57c2, stop:1 transparent);}\n"
"         "));
        btnRank->setCheckable(true);
        btnRank->setAutoExclusive(true);

        sideLay->addWidget(btnRank);

        btnRule = new QPushButton(sideBar);
        btnRule->setObjectName("btnRule");
        btnRule->setMinimumSize(QSize(0, 48));
        btnRule->setStyleSheet(QString::fromUtf8("\n"
"QPushButton{\n"
"    color:#fff;\n"
"    font:15px;\n"
"    text-align:left;\n"
"    padding-left:12px;\n"
"    border:none;\n"
"    border-radius:8px;\n"
"    background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ffab91, stop:1 transparent);\n"
"}\n"
"QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff7043, stop:1 transparent);}\n"
"QPushButton:checked{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff5722, stop:1 transparent);}\n"
"         "));
        btnRule->setCheckable(true);
        btnRule->setAutoExclusive(true);

        sideLay->addWidget(btnRule);

        sideSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);

        sideLay->addItem(sideSpacer);

        btnSettings = new QPushButton(sideBar);
        btnSettings->setObjectName("btnSettings");
        btnSettings->setMinimumSize(QSize(0, 36));
        btnSettings->setStyleSheet(QString::fromUtf8("QPushButton{color:#aaa;font:13px;border:none;border-radius:6px;}\n"
"QPushButton:hover{color:#fff;background:rgba(255,255,255,30);}"));

        sideLay->addWidget(btnSettings);


        mainLay->addWidget(sideBar);

        stackRight = new QStackedWidget(Menu);
        stackRight->setObjectName("stackRight");
        stackRight->setStyleSheet(QString::fromUtf8("#stackRight{background:#222;border:1px solid #ff9de0;border-radius:12px;}"));
        pageWelcome = new QWidget();
        pageWelcome->setObjectName("pageWelcome");
        welcomeLay = new QVBoxLayout(pageWelcome);
        welcomeLay->setObjectName("welcomeLay");
        welcomeTopSpacer = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

        welcomeLay->addItem(welcomeTopSpacer);

        labelWelcome = new QLabel(pageWelcome);
        labelWelcome->setObjectName("labelWelcome");
        labelWelcome->setStyleSheet(QString::fromUtf8("color:#ff9de0;font:700 36pt \"Microsoft YaHei\";"));
        labelWelcome->setAlignment(Qt::AlignCenter);

        welcomeLay->addWidget(labelWelcome);

        labelSub = new QLabel(pageWelcome);
        labelSub->setObjectName("labelSub");
        labelSub->setStyleSheet(QString::fromUtf8("color:#aaa;font:16pt \"Microsoft YaHei\";"));
        labelSub->setAlignment(Qt::AlignCenter);

        welcomeLay->addWidget(labelSub);

        welcomeBottomSpacer = new QSpacerItem(20, 80, QSizePolicy::Minimum, QSizePolicy::Expanding);

        welcomeLay->addItem(welcomeBottomSpacer);

        stackRight->addWidget(pageWelcome);

        mainLay->addWidget(stackRight);


        retranslateUi(Menu);

        QMetaObject::connectSlotsByName(Menu);
    } // setupUi

    void retranslateUi(QWidget *Menu)
    {
        Menu->setWindowTitle(QCoreApplication::translate("Menu", "\344\270\273\350\217\234\345\215\225", nullptr));
        btnSingle->setText(QCoreApplication::translate("Menu", "  \345\215\225\346\234\272\346\270\270\346\210\217", nullptr));
        btnOnline->setText(QCoreApplication::translate("Menu", "  \350\201\224\346\234\272\345\257\271\346\210\230", nullptr));
        btnSkill->setText(QCoreApplication::translate("Menu", "  \346\212\200\350\203\275\351\200\211\346\213\251", nullptr));
        btnRank->setText(QCoreApplication::translate("Menu", "  \346\216\222\350\241\214\346\246\234", nullptr));
        btnRule->setText(QCoreApplication::translate("Menu", "  \350\247\204\345\210\231", nullptr));
        btnSettings->setText(QCoreApplication::translate("Menu", "\350\256\276\347\275\256", nullptr));
        labelWelcome->setText(QCoreApplication::translate("Menu", "<html><head/><body><p align=\"center\">\346\266\210\346\266\210\344\271\220</p></body></html>", nullptr));
        labelSub->setText(QCoreApplication::translate("Menu", "<html><head/><body><p align=\"center\">\351\200\211\346\213\251\345\267\246\344\276\247\345\212\237\350\203\275\357\274\214\345\274\200\345\247\213\346\270\270\346\210\217\345\220\247\357\274\201</p></body></html>", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Menu: public Ui_Menu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENU_H
