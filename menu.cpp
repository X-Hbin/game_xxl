#include "menu.h"
#include "ui_menu.h"
#include <QDebug>
#include <QMessageBox>
#include "mainwindow.h"
<<<<<<< HEAD
#include "musicsetting.h"
#include "musicmanager.h"


#include <QTextEdit>
#include <QScrollBar>
#include <QFrame>
=======
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d

Menu::Menu(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Menu)
{
    ui->setupUi(this);
<<<<<<< HEAD
    rightStack = ui->stackRight;

    // 初始化时播放菜单背景音乐
    MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Menu);
=======

    // 拿到主 ui 里已存在的 stackRight
    rightStack = ui->stackRight;
    // 先保留欢迎页（index=0）
    // 后面动态插入模式页
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
}

Menu::~Menu() = default;

<<<<<<< HEAD
void Menu::on_btnSingle_clicked()
{
=======
/* 左侧【单机游戏】被点击 */
void Menu::on_btnSingle_clicked()
{
    // 首次进入才创建模式页
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
    static int modeIndex = -1;
    if (modeIndex == -1) {
        QWidget *modePage = createModeSelectPage();
        modeIndex = rightStack->addWidget(modePage);
    }
    rightStack->setCurrentIndex(modeIndex);
}

<<<<<<< HEAD
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
=======
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
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
        mainWin->switchToSkillTreePage();
    }
}

<<<<<<< HEAD
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
    // 使用静态变量缓存页面索引，避免重复创建
    static int rulePageIndex = -1;

    if (rulePageIndex == -1) {
        QWidget *rulePage = createRulePage();
        rulePageIndex = rightStack->addWidget(rulePage);
    }

    // 切换到规则页
    rightStack->setCurrentIndex(rulePageIndex);
}

QWidget *Menu::createRulePage()
{
    QWidget *page = new QWidget();
    page->setObjectName("rulePage");

    // ============================================================
    // 1. 整体容器样式：磨砂黑底 + 霓虹粉微光边框 + 圆角
    // ============================================================
    page->setStyleSheet(
        "#rulePage {"
        "   background-color: rgba(20, 20, 35, 230);" // 深蓝黑，高透明度背景
        "   border: 1px solid rgba(255, 157, 224, 0.3);" // 极细的粉色边框
        "   border-radius: 16px;"
        "}"
        );

    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30); // 增加内边距
    layout->setSpacing(20);

    // ============================================================
    // 2. 标题区域 (带渐变装饰线)
    // ============================================================
    QLabel *title = new QLabel("GAME RULES", page);
    title->setStyleSheet(
        "color: #ff9de0;" // 霓虹粉
        "font: 900 24pt 'Arial Black';" // 使用厚重的英文字体更有科技感
        "letter-spacing: 2px;"
        );
    title->setAlignment(Qt::AlignCenter);
    layout->addWidget(title);

    // 装饰线：中间青色，两边透明的渐变线条
    QFrame *line = new QFrame(page);
    line->setFrameShape(QFrame::HLine);
    line->setStyleSheet("background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 transparent, stop:0.5 #00e5ff, stop:1 transparent); min-height: 2px; border:none;");
    layout->addWidget(line);

    // ============================================================
    // 3. 规则文本区域 (富文本 + 滚动条美化)
    // ============================================================
    QTextEdit *ruleText = new QTextEdit(page);
    ruleText->setReadOnly(true);
    ruleText->setFrameShape(QFrame::NoFrame);

    // 【关键】美化滚动条和文本域背景
    ruleText->setStyleSheet(
        "QTextEdit {"
        "   background: transparent;" // 背景透明，透出底部的磨砂玻璃
        "   color: #e0e0e0;"          // 文字银灰色
        "   font: 13pt 'Microsoft YaHei';"
        "   selection-background-color: #00e5ff;" // 选中文字变青色
        "   selection-color: #000;"
        "}"
        // --- 垂直滚动条美化 ---
        "QScrollBar:vertical {"
        "   background: rgba(0,0,0,50);" // 轨道半透明
        "   width: 8px;"                 // 极细
        "   margin: 0px;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #555;"           // 滑块深灰
        "   min-height: 20px;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical:hover { background: #ff9de0; }" // 悬停变粉
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }" // 隐藏上下箭头
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
        );

    // 使用 HTML 设置多彩文字
    QString htmlContent =
        "<div style='line-height:160%;'>"

        "<p><span style='font-size:15pt; font-weight:bold; color:#ffffff;'>▌ 基础玩法</span><br>"
        "通过交换相邻的方块，使 <span style='color:#ffd700; font-weight:bold;'>3个或更多</span> 相同方块连成一线即可消除。</p>"
        "<br>"

        "<p><span style='font-size:15pt; font-weight:bold; color:#ff9de0;'>▌ 闪电模式 (Lightning)</span><br>"
        "限时 <span style='color:#ff9de0;'>3分钟</span>！这是手速与眼力的极致挑战，连击越高分数加成越高。</p>"
        "<br>"

        "<p><span style='font-size:15pt; font-weight:bold; color:#00e5ff;'>▌ 旋风模式 (Cyclone)</span><br>"
        "右键点击可以 <span style='color:#00e5ff;'>旋转 2x2 区域</span>。策略至上，寻找连锁消除的最佳机会。</p>"
        "<br>"

        "<p><span style='font-size:15pt; font-weight:bold; color:#7cffcb;'>▌ 霓虹模式 (Shapeshift)</span><br>"
        "特殊方块会随机变换颜色。抓住它们变色的瞬间，打出意想不到的 <span style='color:#7cffcb;'>Combo</span>！</p>"
        "<br>"

        "<p><span style='font-size:15pt; font-weight:bold; color:#ffeb3b;'>▌ 技能系统</span><br>"
        "积累消除数，释放强力技能！<br>"
        "• <span style='color:#00e5ff;'>时光倒流</span>：恢复步数或时间<br>"
        "• <span style='color:#ff9de0;'>全屏爆破</span>：消除屏幕上所有指定颜色的方块</p>"

        "</div>";

    ruleText->setHtml(htmlContent);
    layout->addWidget(ruleText);

    // ============================================================
    // 4. 底部按钮 (全息科技风格)
    // ============================================================
    QPushButton *aiDemoBtn = new QPushButton("启动 AI 演示 [BETA]", page);
    aiDemoBtn->setCursor(Qt::PointingHandCursor);
    aiDemoBtn->setFixedHeight(50); // 稍微矮一点，显得更精致

    aiDemoBtn->setStyleSheet(
        "QPushButton {"
        "   color: #00e5ff;" // 青色文字
        "   font: bold 14pt 'Microsoft YaHei';"
        "   background-color: rgba(0, 229, 255, 15);" // 极淡的青色背景
        "   border: 1px solid #00e5ff;" // 青色边框
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(0, 229, 255, 40);" // 悬停稍微变亮
        "   color: #ffffff;" // 文字变白
        "   border: 1px solid #ffffff;" // 边框变白
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(0, 229, 255, 70);"
        "   padding-top: 2px;"
        "}"
        );

    connect(aiDemoBtn, &QPushButton::clicked, this, [this](){
        // QMessageBox::information... // 删除原来的提示
        emit startAIDemoRequested();   // 发送信号
    });

    layout->addWidget(aiDemoBtn);

    return page;
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

=======
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



>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
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
<<<<<<< HEAD
    const QStringList modes = {"闪电", "旋风", "霓虹"};
    for (int i = 0; i < modes.size(); ++i) {
        QPushButton *btn = new QPushButton(modes[i], page);
        btn->setFixedSize(220, 54);
=======
    const QStringList modes = {"闪电", "旋风", "霓虹"};  // ← 这里改为"变身"
    for (int i = 0; i < modes.size(); ++i) {
        QPushButton *btn = new QPushButton(modes[i], page);
        btn->setFixedSize(220, 54);              // 关键：变窄
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
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

<<<<<<< HEAD
=======
        // 单独套一层 HLayout 让按钮居中
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
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
<<<<<<< HEAD
    startBtn->setFixedSize(260, 50);
=======
    startBtn->setFixedSize(260, 50);              // 关键：比模式按钮宽一点
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
    startBtn->setStyleSheet(
        "QPushButton{"
        "color:#fff;font:700 16pt 'Microsoft YaHei';"
        "border:none;border-radius:12px;"
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4caf50, stop:1 #66bb6a);"
        "}"
        "QPushButton:disabled{background:#555;}"
        "QPushButton:hover:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #66bb6a, stop:1 #81c784);}"
        "QPushButton:pressed:!disabled{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #388e3c, stop:1 #4caf50);}");

<<<<<<< HEAD
=======
    // 单独居中
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
    QHBoxLayout *startLay = new QHBoxLayout();
    startLay->addStretch();
    startLay->addWidget(startBtn);
    startLay->addStretch();
    lay->addStretch(2);
    lay->addLayout(startLay);

<<<<<<< HEAD
    connect(modeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [startBtn](QAbstractButton *){ startBtn->setEnabled(true); });

    connect(startBtn, &QPushButton::clicked, [this](){
        QString mode = modeGroup->checkedButton()->text();
        emit startGameRequested(mode);

        // 切换到游戏场景音乐
        MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Playing);
=======
    // 选中模式后才允许点开始
    connect(modeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [startBtn](QAbstractButton *){ startBtn->setEnabled(true); });

    // 正式开始游戏（暂时弹窗示意）
    connect(startBtn, &QPushButton::clicked, [this](){
        int id = modeGroup->checkedId();
        QString mode = modeGroup->checkedButton()->text();
        emit startGameRequested(mode);      // 只发信号，不越权跳页
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
    });

    return page;
}

<<<<<<< HEAD
void Menu::onModeSelected(QAbstractButton *btn)
{
=======
/* 模式选中时额外光晕（可扩展） */
void Menu::onModeSelected(QAbstractButton *btn)
{
    // 简单打印调试
>>>>>>> 4e3f68ed03493357fbfd61249ab16fec8e965e2d
    qDebug() << "当前选中模式：" << btn->text();
}
