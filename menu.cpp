#include "menu.h"
#include "ui_menu.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
#include "musicsetting.h"
#include "musicmanager.h"

Menu::Menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);
    rightStack = ui->stackRight;

    // 初始化时播放菜单背景音乐
    MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Menu);
}

Menu::~Menu() = default;

void Menu::on_btnSingle_clicked()
{
    static int modeIndex = -1;
    if (modeIndex == -1) {
        QWidget *modePage = createModeSelectPage();
        modeIndex = rightStack->addWidget(modePage);
    }
    rightStack->setCurrentIndex(modeIndex);
}

void Menu::on_btnOnline_clicked()
{
    QMessageBox::information(this, "功能提示", "联机对战功能开发中...");
    rightStack->setCurrentIndex(0);
}

void Menu::on_btnSkill_clicked()
{
    QWidget *mainWindow = this->window();
    MainWindow *mainWin = qobject_cast<MainWindow*>(mainWindow);
    if (mainWin) {
        mainWin->switchToSkillTreePage();
    }
}

void Menu::on_btnRank_clicked()
{
    QWidget *mainWindow = this->window();
    MainWindow *mainWin = qobject_cast<MainWindow*>(mainWindow);
    if (mainWin) {
        mainWin->switchToRankPage();
    }
}

void Menu::on_btnRule_clicked()
{
    QMessageBox::information(this, "游戏规则",
                             "游戏规则：\n"
                             "1. 通过交换相邻的方块，使三个或更多相同方块连成一线即可消除\n"
                             "2. 闪电模式：限时挑战，在规定时间内获得尽可能高的分数\n"
                             "3. 旋风模式：步数限制，用有限的步数完成目标分数\n"
                             "4. 变身模式：特殊方块会随机变换，增加游戏难度\n"
                             "5. 连续消除可以获得额外分数奖励");
    rightStack->setCurrentIndex(0);
}

void Menu::on_btnSettings_clicked()
{
    // 创建并切换到音乐设置界面
    static int musicSettingIndex = -1;
    if (musicSettingIndex == -1) {
        MusicSetting *musicSettingPage = new MusicSetting(this);
        musicSettingPage->setObjectName("musicSettingPage");
        musicSettingPage->setStyleSheet("#musicSettingPage{background:transparent;}");
        musicSettingIndex = rightStack->addWidget(musicSettingPage);
    }
    rightStack->setCurrentIndex(musicSettingIndex);
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
    const QStringList modes = {"闪电", "旋风", "变身"};
    for (int i = 0; i < modes.size(); ++i) {
        QPushButton *btn = new QPushButton(modes[i], page);
        btn->setFixedSize(220, 54);
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
    startBtn->setFixedSize(260, 50);
    startBtn->setStyleSheet(
        "QPushButton{"
        "color:#fff;font:700 16pt 'Microsoft YaHei';"
        "border:none;border-radius:12px;"
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4caf50, stop:1 #66bb6a);"
        "}"
        "QPushButton:disabled{background:#555;}"
        "QPushButton:hover:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #66bb6a, stop:1 #81c784);}"
        "QPushButton:pressed:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #388e3c, stop:1 #4caf50);}");

    QHBoxLayout *startLay = new QHBoxLayout();
    startLay->addStretch();
    startLay->addWidget(startBtn);
    startLay->addStretch();
    lay->addStretch(2);
    lay->addLayout(startLay);

    connect(modeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [startBtn](QAbstractButton *){ startBtn->setEnabled(true); });

    connect(startBtn, &QPushButton::clicked, [this](){
        QString mode = modeGroup->checkedButton()->text();
        emit startGameRequested(mode);

        // 切换到游戏场景音乐
        MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Playing);
    });

    return page;
}

void Menu::onModeSelected(QAbstractButton *btn)
{
    qDebug() << "当前选中模式：" << btn->text();
}
