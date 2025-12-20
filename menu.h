// menu.h
#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QButtonGroup>

// 前向声明
class MainWindow;

namespace Ui {
class Menu;
}

class Menu : public QWidget
{
    Q_OBJECT

public:
    explicit Menu(QWidget *parent = nullptr);
    ~Menu();

private slots:
    void on_btnSingle_clicked();          // 左侧单机按钮
    void onModeSelected(QAbstractButton *btn); // 模式选中

    void on_btnOnline_clicked();
    void on_btnSkill_clicked();           // 左侧技能选择按钮
    void on_btnRank_clicked();
    void on_btnRule_clicked();
    void on_btnSettings_clicked();

signals:
    void startGameRequested(const QString &mode);   // 携带模式名

private:
    Ui::Menu *ui;
    QWidget *createModeSelectPage();      // 工厂函数：模式选择页
    QStackedWidget *rightStack;           // 快捷指针
    QButtonGroup *modeGroup;              // 三选一
};

#endif // MENU_H
