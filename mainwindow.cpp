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


    connect(menuPage, &Menu::startGameRequested,
            this, &MainWindow::onStartGame);
}

MainWindow::~MainWindow() = default;

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
    return true;


    return db.open();
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
    q.prepare("SELECT password FROM user WHERE username=?");
    q.addBindValue(user);
    if (!q.exec() || !q.next()) {
        QMessageBox::warning(this, "登录", "该用户不存在！");
        return;
    }
    QString dbPwd = q.value(0).toString();
    if (dbPwd == pwd){
        m_currentUser = user; // 【关键】记录当前用户
        stack->setCurrentIndex(1);
    }else
        QMessageBox::warning(this, "登录", "密码错误！");
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
    q.prepare("INSERT INTO user(username,password) VALUES(?,?)");
    q.addBindValue(user);
    q.addBindValue(pwd);

    if (q.exec()) {
        // 2. 【新增】创建以用户名为名字的独立表
        // 注意：表名不能通过 bindValue 绑定，必须拼接字符串。
        // 表结构：mode(varchar), score(int)
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
    if (m_mode3Page) { stack->removeWidget(m_mode3Page); delete m_mode3Page; m_mode3Page = nullptr; } // 新增清理

    m_gameBoard = new GameBoard(this);
    m_gameBoard->initNoThree(); // 初始化逻辑通用

    if (mode == "闪电"){
        this->setStyleSheet("background:qradialgradient(cx:0.5 cy:0.5 radius:1.2, fx:0.5 fy:0.5, stop:0 rgba(61, 44, 98, 180), stop:1 #0f1029);");
        m_mode1Page = new Mode_1(m_gameBoard, m_currentUser, this);
        connect(m_mode1Page, &Mode_1::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode1Page);
        stack->setCurrentWidget(m_mode1Page);
    }
    else if (mode == "旋风") { // 新增旋风模式入口
        // 可以换个背景色，比如偏蓝青色代表风
        this->setStyleSheet(
            "background: qradialgradient(cx:0.5 cy:0.5 radius:1.4,"
            "fx:0.5 fy:0.3,"
            "stop:0 #0d3742,"
            "stop:0.55 #051d24,"
            "stop:1 #000000);");

        m_mode2Page = new Mode_2(m_gameBoard, m_currentUser, this);
        connect(m_mode2Page, &Mode_2::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode2Page);
        stack->setCurrentWidget(m_mode2Page);
    }
    else if (mode == "变身") { // 新增变身模式入口
        // 绿色系背景
        this->setStyleSheet(
            "background: qradialgradient(cx:0.5 cy:0.5 radius:1.4,"
            "fx:0.5 fy:0.5,"
            "stop:0 #0a2e1f,"
            "stop:0.5 #052615,"
            "stop:1 #000000);");

        m_mode3Page = new Mode_3(m_gameBoard, m_currentUser, this);
        connect(m_mode3Page, &Mode_3::gameFinished, this, &MainWindow::onGameFinished);
        stack->addWidget(m_mode3Page);
        stack->setCurrentWidget(m_mode3Page);
    }
}

void MainWindow::onGameFinished()
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

    // 切回菜单
    stack->setCurrentIndex(1);
    // 销毁游戏页面释放资源（可选，也可以下次 start 时处理）
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
    if (m_mode3Page) { // 新增：清理第三种模式
        stack->removeWidget(m_mode3Page);
        m_mode3Page->deleteLater();
        m_mode3Page = nullptr;
    }
}
