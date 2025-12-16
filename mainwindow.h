#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>   // 新增
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>


#include <QStackedWidget>
#include "menu.h"


#include "gameboard.h"

#include "mode_2.h"
class Mode_1;   // 前向声明

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonRegister_clicked();
    void onStartGame(const QString &mode);

    // 新增：处理游戏结束返回菜单
    void onGameFinished();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    bool openDB();

    QStackedWidget *stack;
    Menu* menuPage;
    GameBoard *m_gameBoard = nullptr;
    Mode_1 *m_mode1Page = nullptr; // 修改：改为成员变量方便管理
    Mode_2 *m_mode2Page = nullptr;

    QString m_currentUser; // 【新增】记录当前登录用户名
};
#endif // MAINWINDOW_H
