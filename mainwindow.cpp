#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPalette>

#include "mode_1.h"
#include "mode_3.h"
#include "rankmanager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(659, 600);
    setWindowTitle("Login");

    /* 背景图*/
    QPixmap bg("background.jpg");
    if (!bg.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, bg.scaled(size(), Qt::IgnoreAspectRatio));
        setAutoFillBackground(true);
        setPalette(palette);
    } else {
        setStyleSheet("background-color: #2b2b2b;");
    }

    /* 打开数据库 */
    if (!openDB())
        QMessageBox::critical(this, "数据库", "无法连接到 MySQL！");

    // 1. 取下原中央控件（不销毁）
    QWidget *oldCentral = takeCentralWidget();   // 脱离主窗口

    // 2. 创建堆栈
    stack = new QStackedWidget(this);
    stack->setAutoFillBackground(true);   // 让它不再用父调色板

    // 3. 第 0 页：登录注册
    stack->addWidget(oldCentral);                // 原样塞进去

    // 4. 第 1 页：菜单
    menuPage = new Menu(this);
    stack->addWidget(menuPage);

    // 5. 把堆栈设成新中央
    setCentralWidget(stack);

    // 6. 初始化技能树
    m_skillTree = new SkillTree(this);
    m_skillTreePage = new SkillTreePage(m_skillTree, this);
    stack->addWidget(m_skillTreePage);

    // 7. 初始化排行榜
    m_rankManager = new RankManager(this);
    m_rankPage = new RankPage("", this);
    stack->addWidget(m_rankPage);

    // 连接排行榜返回信号
    connect(m_rankPage, &RankPage::backToMenu, this, &MainWindow::onRankPageBack);

    // 连接信号
    connect(menuPage, &Menu::startGameRequested,
            this, &MainWindow::onStartGame);
    connect(m_skillTreePage, &SkillTreePage::backRequested,
            this, &MainWindow::onSkillTreeBack);

    // 程序启动时播放登录界面的背景音乐
    qDebug() << "程序启动，播放登录界面音乐...";
    MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Loading);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* 建立 MySQL 连接 */
bool MainWindow::openDB()
{
    qDebug() << "Available SQL drivers:" << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QMYSQL");

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("game");
    db.setUserName("root");
    db.setPassword("562134XhF");

    if (!db.open()) {
        qDebug() << "MySQL error:" << db.lastError().text();
        return false;
    }

    // 检查并创建必要的表（技能系统相关）
    QSqlQuery checkQuery(db);

    // 检查user表是否有skill_points字段
    if (!checkQuery.exec("SHOW COLUMNS FROM user LIKE 'skill_points'")) {
        qDebug() << "Failed to check user table columns:" << checkQuery.lastError().text();
    } else {
        if (!checkQuery.next()) {
            // skill_points字段不存在，添加它
            if (!checkQuery.exec("ALTER TABLE user ADD COLUMN skill_points INT DEFAULT 0")) {
                qDebug() << "Failed to add skill_points column:" << checkQuery.lastError().text();
            }
        }
    }

    // 检查skill_tree表是否存在
    if (!checkQuery.exec("SHOW TABLES LIKE 'skill_tree'")) {
        qDebug() << "Failed to check skill_tree table:" << checkQuery.lastError().text();
    } else {
        if (!checkQuery.next()) {
            // skill_tree表不存在，创建它
            QString createSkillTableSql =
                "CREATE TABLE skill_tree ("
                "  id INT AUTO_INCREMENT PRIMARY KEY,"
                "  username VARCHAR(50) NOT NULL,"
                "  skill_id VARCHAR(50) NOT NULL,"
                "  unlocked BOOLEAN DEFAULT FALSE,"
                "  equipped BOOLEAN DEFAULT FALSE,"
                "  FOREIGN KEY (username) REFERENCES user(username) ON DELETE CASCADE,"
                "  UNIQUE KEY unique_user_skill (username, skill_id)"
                ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4";

            if (!checkQuery.exec(createSkillTableSql)) {
                qDebug() << "Failed to create skill_tree table:" << checkQuery.lastError().text();
            }
        }
    }

    // 检查game_records表是否存在（排行榜）
    if (!checkQuery.exec("SHOW TABLES LIKE 'game_records'")) {
        qDebug() << "Failed to check game_records table:" << checkQuery.lastError().text();
    } else {
        if (!checkQuery.next()) {
            // game_records表不存在，创建它
            QString createRecordsTableSql =
                "CREATE TABLE game_records ("
                "  id INT AUTO_INCREMENT PRIMARY KEY,"
                "  username VARCHAR(50) NOT NULL,"
                "  mode VARCHAR(50) NOT NULL,"
                "  score INT NOT NULL,"
                "  time DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,"
                "  is_multiplayer TINYINT(1) NOT NULL DEFAULT 0,"
                "  opponent VARCHAR(50) DEFAULT NULL,"
                "  FOREIGN KEY (username) REFERENCES user(username),"
                "  FOREIGN KEY (opponent) REFERENCES user(username)"
                ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4";

            if (!checkQuery.exec(createRecordsTableSql)) {
                qDebug() << "Failed to create game_records table:" << checkQuery.lastError().text();
            }
        }
    }

    return true;
}

/* 登录槽 */
void MainWindow::on_pushButtonLogin_clicked()
{
    QString user = ui->lineEditUser->text().trimmed();
    QString pwd  = ui->lineEditPass->text().trimmed();
    if (user.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空！");
        return;
    }

    QSqlQuery q(db);
    q.prepare("SELECT password, skill_points FROM user WHERE username=?");
    q.addBindValue(user);
    if (!q.exec() || !q.next()) {
        QMessageBox::warning(this, "登录", "该用户不存在！");
        return;
    }

    QString dbPwd = q.value(0).toString();

    if (dbPwd == pwd){
        m_currentUser = user;

        // 加载技能树数据
        m_skillTree->loadFromDatabase(m_currentUser);

        // 更新排行榜页面的用户名
        m_rankPage->setUsername(m_currentUser);

        // 切换到菜单页面时，切换背景音乐到菜单场景
        qDebug() << "登录成功，切换到菜单音乐...";
        MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Menu);

        stack->setCurrentIndex(1);
    } else {
        QMessageBox::warning(this, "登录", "密码错误！");
    }
}

/* 注册槽 */
void MainWindow::on_pushButtonRegister_clicked()
{
    QString user = ui->lineEditUser->text().trimmed();
    QString pwd  = ui->lineEditPass->text().trimmed();
    if (user.isEmpty() || pwd.isEmpty()) {
        QMessageBox::warning(this, "提示", "用户名或密码不能为空！");
        return;
    }

    QSqlQuery q(db);
    q.prepare("SELECT 1 FROM user WHERE username=?");
    q.addBindValue(user);
    if ((q.exec() && q.next()) || user =="user") {
        QMessageBox::warning(this, "注册", "该用户已存在！");
        return;
    }

    // 1. 插入到总 user 表
    q.prepare("INSERT INTO user(username,password,skill_points) VALUES(?,?,0)");
    q.addBindValue(user);
    q.addBindValue(pwd);

    if (q.exec()) {
        // 2. 创建以用户名为名字的独立表
        QString sql = QString(
                          "CREATE TABLE IF NOT EXISTS `%1` ("
                          "  `mode`  VARCHAR(50)  NOT NULL,"
                          "  `score` INT          NOT NULL,"
                          "  `time`  DATETIME     NOT NULL"
                          ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;"
                          ).arg(user);

        QSqlQuery createQ(db);
        if (createQ.exec(sql)) {
            QMessageBox::information(this, "注册", "注册成功！");
        } else {
            QMessageBox::warning(this, "注册", "用户创建成功但个人表创建失败：" + createQ.lastError().text());
        }
    }
    else
        QMessageBox::critical(this, "注册", "数据库错误："+q.lastError().text());
}

void MainWindow::onStartGame(const QString &mode)
{
    // 清理旧页面
    if (m_gameBoard) { delete m_gameBoard; m_gameBoard = nullptr; }
    if (m_mode1Page) { stack->removeWidget(m_mode1Page); delete m_mode1Page; m_mode1Page = nullptr; }
    if (m_mode2Page) { stack->removeWidget(m_mode2Page); delete m_mode2Page; m_mode2Page = nullptr; }
    if (m_mode3Page) { stack->removeWidget(m_mode3Page); delete m_mode3Page; m_mode3Page = nullptr; }

    m_gameBoard = new GameBoard(this);
    m_gameBoard->initNoThree();

    m_currentGameMode = mode;

    // 切换到游戏页面时，切换背景音乐到游戏场景
    qDebug() << "开始游戏，切换到游戏音乐...";
    MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Playing);

    if (mode == "闪电"){
        this->setStyleSheet("background:qradialgradient(cx:0.5 cy:0.5 radius:1.2, fx:0.5 fy:0.5, stop:0 rgba(61, 44, 98, 180), stop:1 #0f1029);");
        m_mode1Page = new Mode_1(m_gameBoard, m_currentUser, this);
        m_mode1Page->setSkillTree(m_skillTree);
        connect(m_mode1Page, &Mode_1::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode1Page);
        stack->setCurrentWidget(m_mode1Page);
    }
    else if (mode == "旋风") {
        this->setStyleSheet(
            "background: qradialgradient(cx:0.5 cy:0.5 radius:1.4,"
            "fx:0.5 fy:0.3,"
            "stop:0 #0d3742,"
            "stop:0.55 #051d24,"
            "stop:1 #000000);");

        m_mode2Page = new Mode_2(m_gameBoard, m_currentUser, this);
        m_mode2Page->setSkillTree(m_skillTree);
        connect(m_mode2Page, &Mode_2::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode2Page);
        stack->setCurrentWidget(m_mode2Page);
    }
    else if (mode == "变身") {
        this->setStyleSheet(
            "background: qradialgradient(cx:0.5 cy:0.5 radius:1.4,"
            "fx:0.5 fy:0.5,"
            "stop:0 #0a2e1f,"
            "stop:0.5 #052615,"
            "stop:1 #000000);");

        m_mode3Page = new Mode_3(m_gameBoard, m_currentUser, this);
        m_mode3Page->setSkillTree(m_skillTree);
        connect(m_mode3Page, &Mode_3::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode3Page);
        stack->setCurrentWidget(m_mode3Page);
    }
}

void MainWindow::onGameFinished(bool isNormalEnd)
{
    // 在销毁游戏页面之前，先保存游戏记录
    if (isNormalEnd) {
        int finalScore = 0;
        QString fullModeName;

        // 根据当前游戏模式获取分数和完整模式名
        if (m_mode1Page && m_currentGameMode == "闪电") {
            finalScore = 100; // 临时值，需要根据实际游戏逻辑修改
            fullModeName = "闪电模式";

            // 给玩家5点技能点
            m_skillTree->addSkillPoints(5);
        }
        else if (m_mode2Page && m_currentGameMode == "旋风") {
            finalScore = 100; // 临时值，需要根据实际游戏逻辑修改
            fullModeName = "旋风模式";

            // 给玩家5点技能点
            m_skillTree->addSkillPoints(5);
        }
        else if (m_mode3Page && m_currentGameMode == "变身") {
            finalScore = 100; // 临时值，需要根据实际游戏逻辑修改
            fullModeName = "变身模式";

            // 给玩家5点技能点
            m_skillTree->addSkillPoints(5);
        }

        // 保存游戏记录到数据库
        if (finalScore > 0 && !fullModeName.isEmpty()) {
            saveGameRecord(m_currentUser, finalScore, fullModeName);
        }

        // 保存技能树状态
        m_skillTree->saveToDatabase(m_currentUser);
    }

    /* 换回背景图 */
    QPixmap bg("background.jpg");
    if (!bg.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, bg.scaled(size(), Qt::IgnoreAspectRatio));
        setAutoFillBackground(true);
        setPalette(palette);
        setStyleSheet("");
    } else {
        setStyleSheet("background-color: #2b2b2b;");
    }

    // 切换回菜单页面时，切换背景音乐到菜单场景
    qDebug() << "游戏结束，切换回菜单音乐...";
    MusicManager::instance().playSceneMusic(MusicManager::MusicScene::Menu);

    // 切回菜单
    stack->setCurrentIndex(1);

    // 销毁游戏页面释放资源
    if (m_mode1Page) {
        stack->removeWidget(m_mode1Page);
        m_mode1Page->deleteLater();
        m_mode1Page = nullptr;
    }
    if (m_mode2Page) {
        stack->removeWidget(m_mode2Page);
        m_mode2Page->deleteLater();
        m_mode2Page = nullptr;
    }
    if (m_mode3Page) {
        stack->removeWidget(m_mode3Page);
        m_mode3Page->deleteLater();
        m_mode3Page = nullptr;
    }

    // 如果游戏正常结束，刷新排行榜（如果当前在排行榜页面）
    if (isNormalEnd) {
        // 更新排行榜数据
        m_rankPage->refreshRankings();
    }
}

void MainWindow::saveGameRecord(const QString& username, int score, const QString& mode)
{
    if (score <= 0) return;

    // 保存到game_records表
    QSqlQuery q(db);
    q.prepare("INSERT INTO game_records (username, mode, score, time, is_multiplayer) "
              "VALUES (?, ?, ?, NOW(), 0)");
    q.addBindValue(username);
    q.addBindValue(mode);
    q.addBindValue(score);

    if (!q.exec()) {
        qDebug() << "Failed to save game record:" << q.lastError().text();
    }

    // 同时保存到用户的个人记录表（可选）
    QString tableName = username;
    QSqlQuery q2(db);
    q2.prepare(QString("INSERT INTO `%1` (mode, score, time) VALUES (?, ?, NOW())").arg(tableName));

    // 转换模式名为简写
    QString shortMode = mode;
    if (mode == "闪电模式") shortMode = "闪电";
    else if (mode == "旋风模式") shortMode = "旋风";
    else if (mode == "变身模式") shortMode = "变身";

    q2.addBindValue(shortMode);
    q2.addBindValue(score);

    if (!q2.exec()) {
        qDebug() << "Failed to save personal record:" << q2.lastError().text();
    }
}

void MainWindow::onSkillTreeBack()
{
    // 保存技能树状态
    m_skillTree->saveToDatabase(m_currentUser);

    // 返回菜单页面
    stack->setCurrentIndex(1);

    // 刷新技能树页面显示
    m_skillTreePage->refreshUI();
}

void MainWindow::onRankPageBack()
{
    // 返回菜单页面
    stack->setCurrentIndex(1);
}

void MainWindow::switchToSkillTreePage()
{
    if (m_skillTreePage) {
        m_skillTreePage->refreshUI();
        stack->setCurrentWidget(m_skillTreePage);
    }
}

void MainWindow::switchToRankPage()
{
    if (m_rankPage) {
        m_rankPage->refreshRankings();
        stack->setCurrentWidget(m_rankPage);
    }
}
