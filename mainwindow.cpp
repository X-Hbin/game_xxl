#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QPalette>

#include "mode_1.h"
#include "mode_3.h"

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

    // 连接信号
    connect(menuPage, &Menu::startGameRequested,
            this, &MainWindow::onStartGame);
    connect(m_skillTreePage, &SkillTreePage::backRequested,
            this, &MainWindow::onSkillTreeBack);
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
    db.setHostName("127.0.0.1");   // 本地
    db.setPort(3306);
    db.setDatabaseName("game");    // 你的库名
    db.setUserName("bank");        // 改成你的账号
    db.setPassword("210507377Qq@");      // 改成你的密码

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
        m_currentUser = user; // 【关键】记录当前用户

        // 加载技能树数据（这会自动设置技能点）
        m_skillTree->loadFromDatabase(m_currentUser);

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
    m_gameBoard->initNoThree(); // 初始化逻辑通用

    if (mode == "闪电"){
        this->setStyleSheet("background:qradialgradient(cx:0.5 cy:0.5 radius:1.2, fx:0.5 fy:0.5, stop:0 rgba(61, 44, 98, 180), stop:1 #0f1029);");
        m_mode1Page = new Mode_1(m_gameBoard, m_currentUser, this);
        // 传递技能树给游戏模式
        m_mode1Page->setSkillTree(m_skillTree); // 需要在Mode_1中添加此方法
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
        // 传递技能树给游戏模式
        m_mode2Page->setSkillTree(m_skillTree); // 需要在Mode_2中添加此方法
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
        // 传递技能树给游戏模式
        m_mode3Page->setSkillTree(m_skillTree); // 需要在Mode_3中添加此方法
        connect(m_mode3Page, &Mode_3::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode3Page);
        stack->setCurrentWidget(m_mode3Page);
    }
}

void MainWindow::onGameFinished(bool isNormalEnd)
{
    /* 换回背景图 */
    QPixmap bg("background.jpg");
    if (!bg.isNull()) {
        QPalette palette;
        palette.setBrush(QPalette::Window, bg.scaled(size(), Qt::IgnoreAspectRatio));
        setAutoFillBackground(true);
        setPalette(palette);
        setStyleSheet("");          // 清空之前游戏里的渐变样式
    } else {
        setStyleSheet("background-color: #2b2b2b;");
    }

    if (isNormalEnd) {
        // 游戏结束后给玩家5点技能点（仅在正常结束时）
        m_skillTree->addSkillPoints(5);
    }
    // 保存技能树状态到数据库
    m_skillTree->saveToDatabase(m_currentUser);

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
}

void MainWindow::onSkillTreeBack()
{
    // 保存技能树状态
    m_skillTree->saveToDatabase(m_currentUser);

    // 返回菜单页面
    stack->setCurrentIndex(1);

    // 刷新技能树页面显示（如果需要）
    m_skillTreePage->refreshUI();
}

void MainWindow::switchToSkillTreePage()
{
    if (m_skillTreePage) {
        // 刷新技能树页面显示
        m_skillTreePage->refreshUI();
        // 切换到技能树页面
        stack->setCurrentWidget(m_skillTreePage);
    }
}
