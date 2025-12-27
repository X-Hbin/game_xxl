#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>

#include <QStackedWidget>
#include "menu.h"

#include "gameboard.h"
#include "skilltree.h"
#include "skilltreepage.h"
#include "rankpage.h"

#include "mode_2.h"
#include "musicmanager.h"

#include "mode_ai.h"

class Mode_1;
class Mode_3;

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
    void switchToRankPage();

private slots:
    void on_pushButtonLogin_clicked();
    void on_pushButtonRegister_clicked();
    void onStartGame(const QString &mode);
    void onGameFinished(bool isNormalEnd = false);
    void onSkillTreeBack();
    void onRankPageBack();
    // 删除了 on_pushButtonMusic_clicked()

    void onAIDemoStart();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    bool openDB();
    void saveGameRecord(const QString& username, int score, const QString& mode);

    QStackedWidget *stack;
    Menu* menuPage;
    GameBoard *m_gameBoard = nullptr;
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
};

#endif // MAINWINDOW_H
