#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
<<<<<<< HEAD
#include <QtSql/QSqlDatabase>
=======
#include <QtSql/QSqlDatabase>   // 新增
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>

#include <QStackedWidget>
#include "menu.h"

#include "gameboard.h"
<<<<<<< HEAD
#include "skilltree.h"
#include "skilltreepage.h"
#include "rankpage.h"

#include "mode_2.h"
#include "musicmanager.h"

#include "mode_ai.h"

class Mode_1;
class Mode_3;
=======
#include "skilltree.h"          // 新增
#include "skilltreepage.h"      // 新增

#include "mode_2.h"
class Mode_1;   // 前向声明
class Mode_3;   // 新增：第三种模式的前向声明
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void switchToSkillTreePage();
<<<<<<< HEAD
    void switchToRankPage();
=======
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonRegister_clicked();
    void onStartGame(const QString &mode);
<<<<<<< HEAD
    void onGameFinished(bool isNormalEnd = false);
    void onSkillTreeBack();
    void onRankPageBack();
    // 删除了 on_pushButtonMusic_clicked()
    void onAIDemoStart();
=======

    // 新增：处理游戏结束返回菜单
    void onGameFinished(bool isNormalEnd = false);

    // 新增：处理技能树页面返回
    void onSkillTreeBack();
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    bool openDB();
<<<<<<< HEAD
    void saveGameRecord(const QString& username, int score, const QString& mode);
=======
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d

    QStackedWidget *stack;
    Menu* menuPage;
    GameBoard *m_gameBoard = nullptr;
<<<<<<< HEAD
    Mode_1 *m_mode1Page = nullptr;
    Mode_2 *m_mode2Page = nullptr;
    Mode_3 *m_mode3Page = nullptr;

    SkillTree *m_skillTree = nullptr;
    SkillTreePage *m_skillTreePage = nullptr;

    RankPage *m_rankPage = nullptr;
    RankManager *m_rankManager = nullptr;

    QString m_currentUser;
    QString m_currentGameMode;

    Mode_AI *m_modeAIPage = nullptr;
=======
    Mode_1 *m_mode1Page = nullptr; // 修改：改为成员变量方便管理
    Mode_2 *m_mode2Page = nullptr;
    Mode_3 *m_mode3Page = nullptr; // 新增：第三种模式页面

    // 新增：技能树相关
    SkillTree *m_skillTree = nullptr;
    SkillTreePage *m_skillTreePage = nullptr;

    QString m_currentUser; // 【新增】记录当前登录用户名
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
};

#endif // MAINWINDOW_H
