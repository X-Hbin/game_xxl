#include "menu.h"
#include "ui_menu.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"

Menu::Menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);

    // 拿到主 ui 里已存在的 stackRight
    rightStack = ui->stackRight;
    // 先保留欢迎页（index=0）
    // 后面动态插入模式页
}

Menu::~Menu() = default;

/* 左侧【单机游戏】被点击 */
void Menu::on_btnSingle_clicked()
{
    // 首次进入才创建模式页
    static int modeIndex = -1;
    if (modeIndex == -1) {
        QWidget *modePage = createModeSelectPage();
        modeIndex = rightStack->addWidget(modePage);
    }
    rightStack->setCurrentIndex(modeIndex);
}

/* 左侧【联机对战】被点击 */
void Menu::on_btnOnline_clicked()
{


}

/* 左侧【技能选择】被点击 */
void Menu::on_btnSkill_clicked()
{
    // 获取主窗口指针
    QWidget *mainWindow = this->window();
    MainWindow *mainWin = qobject_cast<MainWindow*>(mainWindow);
    if (mainWin) {
        // 调用主窗口的切换到技能树页面方法
        // 需要在MainWindow中添加一个公共方法
        mainWin->switchToSkillTreePage();
    }
}

/* 左侧【排行榜】被点击 */
void Menu::on_btnRank_clicked()
{

}

/* 左侧【规则】被点击 */
void Menu::on_btnRule_clicked()
{

}

/* 左侧【设置】被点击 */
void Menu::on_btnSettings_clicked()
{

}



/* 工厂：生成模式选择页 */
QWidget *Menu::createModeSelectPage()
{
    QWidget *page = new QWidget();
    page->setObjectName("modePage");
    page->setStyleSheet("#modePage{background:transparent;}");

    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(20);
    lay->setContentsMargins(60, 80, 60, 80);

    // 标题
    QLabel *title = new QLabel("选择模式", page);
    title->setStyleSheet("color:#ff9de0;font:700 28pt 'Microsoft YaHei';");
    title->setAlignment(Qt::AlignCenter);
    lay->addWidget(title);

    // 三按钮  宽度固定 220 px
    modeGroup = new QButtonGroup(page);
    const QStringList modes = {"闪电", "旋风", "变身"};  // ← 这里改为"变身"
    for (int i = 0; i < modes.size(); ++i) {
        QPushButton *btn = new QPushButton(modes[i], page);
        btn->setFixedSize(220, 54);              // 关键：变窄
        btn->setCheckable(true);
        btn->setStyleSheet(
            "QPushButton{"
            "color:#fff;font:18pt 'Microsoft YaHei';"
            "border:2px solid #ff9de0;border-radius:12px;"
            "background:rgba(255,157,224,40);"
            "}"
            "QPushButton:hover{"
            "border-color:#ff6bcb;background:rgba(255,107,203,80);"
            "}"
            "QPushButton:checked{"
            "border-color:#ff4fa5;background:rgba(255,79,165,120);"
            "}");

        // 单独套一层 HLayout 让按钮居中
        QHBoxLayout *hLay = new QHBoxLayout();
        hLay->addStretch();
        hLay->addWidget(btn);
        hLay->addStretch();
        lay->addLayout(hLay);

        modeGroup->addButton(btn, i);
    }

    connect(modeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &Menu::onModeSelected);

    // 开始游戏按钮  260 px 宽，居中
    QPushButton *startBtn = new QPushButton("开始游戏", page);
    startBtn->setObjectName("startBtn");
    startBtn->setEnabled(false);
    startBtn->setFixedSize(260, 50);              // 关键：比模式按钮宽一点
    startBtn->setStyleSheet(
        "QPushButton{"
        "color:#fff;font:700 16pt 'Microsoft YaHei';"
        "border:none;border-radius:12px;"
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4caf50, stop:1 #66bb6a);"
        "}"
        "QPushButton:disabled{background:#555;}"
        "QPushButton:hover:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #66bb6a, stop:1 #81c784);}"
        "QPushButton:pressed:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #388e3c, stop:1 #4caf50);}");

    // 单独居中
    QHBoxLayout *startLay = new QHBoxLayout();
    startLay->addStretch();
    startLay->addWidget(startBtn);
    startLay->addStretch();
    lay->addStretch(2);
    lay->addLayout(startLay);

    // 选中模式后才允许点开始
    connect(modeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [startBtn](QAbstractButton *){ startBtn->setEnabled(true); });

    // 正式开始游戏（暂时弹窗示意）
    connect(startBtn, &QPushButton::clicked, [this](){
        int id = modeGroup->checkedId();
        QString mode = modeGroup->checkedButton()->text();
        emit startGameRequested(mode);      // 只发信号，不越权跳页
    });

    return page;
}

/* 模式选中时额外光晕（可扩展） */
void Menu::onModeSelected(QAbstractButton *btn)
{
    // 简单打印调试
    qDebug() << "当前选中模式：" << btn->text();
}
