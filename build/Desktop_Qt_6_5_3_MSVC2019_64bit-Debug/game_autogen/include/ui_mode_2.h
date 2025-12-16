/********************************************************************************
** Form generated from reading UI file 'mode_2.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODE_2_H
#define UI_MODE_2_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mode_2
{
public:
    QWidget *topBar;
    QHBoxLayout *topLay;
    QLabel *labelScore;
    QLabel *labelCountdown;
    QLabel *labelScore_2;
    QWidget *boardWidget;
    QPushButton *btnBack;
    QWidget *leftSideWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *btnUndo;
    QPushButton *btnHint;
    QPushButton *btnSkill;

    void setupUi(QWidget *mode_2)
    {
        if (mode_2->objectName().isEmpty())
            mode_2->setObjectName("mode_2");
        mode_2->resize(659, 600);
        mode_2->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        topBar = new QWidget(mode_2);
        topBar->setObjectName("topBar");
        topBar->setGeometry(QRect(0, 0, 659, 60));
        topBar->setStyleSheet(QString::fromUtf8("background:#111;border-bottom:2px solid #ff9de0;"));
        topLay = new QHBoxLayout(topBar);
        topLay->setObjectName("topLay");
        topLay->setContentsMargins(-1, -1, 20, -1);
        labelScore = new QLabel(topBar);
        labelScore->setObjectName("labelScore");
        labelScore->setStyleSheet(QString::fromUtf8("color:#fff;font:700 10pt 'Microsoft YaHei';"));
        labelScore->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        topLay->addWidget(labelScore);

        labelCountdown = new QLabel(topBar);
        labelCountdown->setObjectName("labelCountdown");
        labelCountdown->setStyleSheet(QString::fromUtf8("color:#ff9de0;font:700 24pt 'Microsoft YaHei';"));
        labelCountdown->setAlignment(Qt::AlignCenter);

        topLay->addWidget(labelCountdown);

        labelScore_2 = new QLabel(topBar);
        labelScore_2->setObjectName("labelScore_2");
        labelScore_2->setStyleSheet(QString::fromUtf8("color:#fff;font:700 20pt 'Microsoft YaHei';"));
        labelScore_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        topLay->addWidget(labelScore_2);

        boardWidget = new QWidget(mode_2);
        boardWidget->setObjectName("boardWidget");
        boardWidget->setGeometry(QRect(130, 90, 400, 400));
        boardWidget->setStyleSheet(QString::fromUtf8("background:#162640;border:2px solid #ff9de0;border-radius:12px;"));
        btnBack = new QPushButton(mode_2);
        btnBack->setObjectName("btnBack");
        btnBack->setGeometry(QRect(540, 540, 90, 40));
        btnBack->setStyleSheet(QString::fromUtf8("\n"
"     QPushButton{\n"
"      color:#fff;\n"
"      font:14pt 'Microsoft YaHei';\n"
"      border:none;\n"
"      border-radius:8px;\n"
"      background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff9de0, stop:1 #ff6bcb);\n"
"     }\n"
"     QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff6bcb, stop:1 #ff4fa5);}\n"
"     QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #ff4fa5, stop:1 #e0438c);}\n"
"    "));
        leftSideWidget = new QWidget(mode_2);
        leftSideWidget->setObjectName("leftSideWidget");
        leftSideWidget->setGeometry(QRect(20, 150, 90, 200));
        verticalLayout = new QVBoxLayout(leftSideWidget);
        verticalLayout->setSpacing(20);
        verticalLayout->setObjectName("verticalLayout");
        btnUndo = new QPushButton(leftSideWidget);
        btnUndo->setObjectName("btnUndo");
        btnUndo->setMinimumSize(QSize(0, 45));
        btnUndo->setStyleSheet(QString::fromUtf8("\n"
"        QPushButton{\n"
"         color:#fff;\n"
"         font:bold 12pt 'Microsoft YaHei';\n"
"         border:2px solid #00e5ff;\n"
"         border-radius:10px;\n"
"         background:rgba(0, 229, 255, 30);\n"
"        }\n"
"        QPushButton:hover{background:rgba(0, 229, 255, 60); border-color:#fff;}\n"
"        QPushButton:pressed{background:rgba(0, 229, 255, 100);}\n"
"       "));

        verticalLayout->addWidget(btnUndo);

        btnHint = new QPushButton(leftSideWidget);
        btnHint->setObjectName("btnHint");
        btnHint->setMinimumSize(QSize(0, 45));
        btnHint->setStyleSheet(QString::fromUtf8("\n"
"        QPushButton{\n"
"         color:#fff;\n"
"         font:bold 12pt 'Microsoft YaHei';\n"
"         border:2px solid #ffeb3b;\n"
"         border-radius:10px;\n"
"         background:rgba(255, 235, 59, 30);\n"
"        }\n"
"        QPushButton:hover{background:rgba(255, 235, 59, 60); border-color:#fff;}\n"
"        QPushButton:pressed{background:rgba(255, 235, 59, 100);}\n"
"       "));

        verticalLayout->addWidget(btnHint);

        btnSkill = new QPushButton(leftSideWidget);
        btnSkill->setObjectName("btnSkill");
        btnSkill->setMinimumSize(QSize(0, 45));
        btnSkill->setStyleSheet(QString::fromUtf8("\n"
"        QPushButton{\n"
"         color:#fff;\n"
"         font:bold 12pt 'Microsoft YaHei';\n"
"         border:2px solid #b388ff;\n"
"         border-radius:10px;\n"
"         background:rgba(179, 136, 255, 30);\n"
"        }\n"
"        QPushButton:hover{background:rgba(179, 136, 255, 60); border-color:#fff;}\n"
"        QPushButton:pressed{background:rgba(179, 136, 255, 100);}\n"
"       "));

        verticalLayout->addWidget(btnSkill);


        retranslateUi(mode_2);

        QMetaObject::connectSlotsByName(mode_2);
    } // setupUi

    void retranslateUi(QWidget *mode_2)
    {
        mode_2->setWindowTitle(QCoreApplication::translate("mode_2", "\346\227\213\351\243\216\346\250\241\345\274\217", nullptr));
        labelScore->setText(QString());
        labelCountdown->setText(QCoreApplication::translate("mode_2", "03:00", nullptr));
        labelScore_2->setText(QCoreApplication::translate("mode_2", "\345\210\206\346\225\260 0", nullptr));
        btnBack->setText(QCoreApplication::translate("mode_2", "\350\277\224\345\233\236", nullptr));
        btnUndo->setText(QCoreApplication::translate("mode_2", "\346\222\244\346\255\245", nullptr));
        btnHint->setText(QCoreApplication::translate("mode_2", "\346\217\220\347\244\272", nullptr));
        btnSkill->setText(QCoreApplication::translate("mode_2", "\346\212\200\350\203\275", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mode_2: public Ui_mode_2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODE_2_H
