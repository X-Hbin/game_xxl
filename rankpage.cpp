#include "rankpage.h"
#include "ui_rankpage.h"
#include <QHeaderView>
#include <QPushButton>
#include <algorithm>
#include <QDebug>
#include <QStyle>
#include <QFont>

RankPage::RankPage(const QString& username, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankPage),
    m_rankManager(new RankManager(this)),
    m_username(username)
{
    ui->setupUi(this);

    //设置黑色背景
    QPalette palette;
    palette.setColor(QPalette::Window, Qt::black);
    palette.setColor(QPalette::Base, Qt::black);
    palette.setColor(QPalette::AlternateBase, QColor(30, 30, 30));
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Button, QColor(50, 50, 50));
    palette.setColor(QPalette::ButtonText, Qt::white);
    this->setPalette(palette);

    ui->titleLabel->setAutoFillBackground(true);
    QPalette titlePalette = ui->titleLabel->palette();
    titlePalette.setColor(QPalette::Window, Qt::black);
    titlePalette.setColor(QPalette::WindowText, QColor("#ff9de0"));
    ui->titleLabel->setPalette(titlePalette);

    //初始化表格
    initTable(ui->mode1Table, 4);
    initTable(ui->mode2Table, 4);
    initTable(ui->mode3Table, 4);
    initTable(ui->personalTable, 4);

    //表格标题
    setTableHeaders();

    //刷新排行榜数据
    refreshRankings();

    //连接返回按钮信号
    connect(ui->backButton, &QPushButton::clicked, this, &RankPage::on_backButton_clicked);

    qDebug() << "RankPage初始化完成，用户名:" << m_username;
}

RankPage::~RankPage()
{
    delete ui;
}

void RankPage::on_backButton_clicked()
{
    emit backToMenu();
}

void RankPage::refreshRankings()
{
    qDebug() << "=== 开始刷新排行榜 ===";

    //刷新各个模式的排行榜
    qDebug() << "获取闪电模式排行榜...";
    QVector<RankInfo> mode1Records = m_rankManager->getTopRecords(10, "闪电模式");
    qDebug() << "闪电模式记录数:" << mode1Records.size();
    fillModeTable(ui->mode1Table, mode1Records);

    qDebug() << "获取旋风模式排行榜...";
    QVector<RankInfo> mode2Records = m_rankManager->getTopRecords(10, "旋风模式");
    qDebug() << "旋风模式记录数:" << mode2Records.size();
    fillModeTable(ui->mode2Table, mode2Records);

    qDebug() << "获取变身模式排行榜...";
    QVector<RankInfo> mode3Records = m_rankManager->getTopRecords(10, "变身模式");
    qDebug() << "变身模式记录数:" << mode3Records.size();
    fillModeTable(ui->mode3Table, mode3Records);

    //刷新个人记录
    qDebug() << "获取个人记录...";
    QVector<RankInfo> personalRecords = m_rankManager->getUserRecords(m_username, 20);
    qDebug() << "个人记录数:" << personalRecords.size();

    //按时间降序排序
    std::sort(personalRecords.begin(), personalRecords.end(),
              [](const RankInfo& a, const RankInfo& b) {
                  return a.time > b.time;
              });

    fillPersonalTable(personalRecords);

    qDebug() << "=== 排行榜刷新完成 ===";
}

void RankPage::setTableHeaders()
{
    //表格标题
    QStringList mode1Headers;
    mode1Headers << "排名" << "用户名" << "分数" << "时间";
    ui->mode1Table->setHorizontalHeaderLabels(mode1Headers);

    QStringList mode2Headers;
    mode2Headers << "排名" << "用户名" << "分数" << "时间";
    ui->mode2Table->setHorizontalHeaderLabels(mode2Headers);

    QStringList mode3Headers;
    mode3Headers << "排名" << "用户名" << "分数" << "时间";
    ui->mode3Table->setHorizontalHeaderLabels(mode3Headers);

    QStringList personalHeaders;
    personalHeaders << "用户名" << "分数" << "模式" << "时间";
    ui->personalTable->setHorizontalHeaderLabels(personalHeaders);
}

void RankPage::initTable(QTableWidget* table, int columnCount)
{
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionMode(QAbstractItemView::NoSelection);
    table->setRowCount(0);
    table->setColumnCount(columnCount);

    //黑色背景
    table->setAutoFillBackground(true);
    QPalette tablePalette = table->palette();
    tablePalette.setColor(QPalette::Base, Qt::black);
    tablePalette.setColor(QPalette::AlternateBase, QColor(30, 30, 30));
    tablePalette.setColor(QPalette::Text, Qt::white);
    tablePalette.setColor(QPalette::Window, Qt::black);
    table->setPalette(tablePalette);

    table->horizontalHeader()->setAutoFillBackground(true);
    QPalette headerPalette = table->horizontalHeader()->palette();
    headerPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    headerPalette.setColor(QPalette::WindowText, Qt::white);
    table->horizontalHeader()->setPalette(headerPalette);
    table->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "   background-color: #1e1e1e;"
        "   color: white;"
        "   padding: 8px;"
        "   border: 1px solid #333333;"
        "   font-weight: bold;"
        "   font-size: 12pt;"
        "}"
        );

    //交替行颜色
    table->setAlternatingRowColors(true);

    //表格样式
    table->setStyleSheet(
        "QTableWidget {"
        "   background-color: black;"
        "   color: white;"
        "   gridline-color: #333333;"
        "   border: 1px solid #444444;"
        "   font-size: 11pt;"
        "}"
        "QTableWidget::item {"
        "   background-color: black;"
        "   color: white;"
        "   padding: 6px;"
        "   border: none;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #222222;"
        "}"
        "QTableWidget QTableCornerButton::section {"
        "   background-color: #1e1e1e;"
        "   border: 1px solid #333333;"
        "}"
        );
}

void RankPage::fillModeTable(QTableWidget* table, const QVector<RankInfo>& records)
{
    table->setRowCount(records.size());

    qDebug() << "填充表格，记录数:" << records.size();

    for (int i = 0; i < records.size(); ++i) {
        const RankInfo& info = records[i];

        qDebug() << "填充第" << i << "条记录:"
                 << "用户=" << info.username
                 << "分数=" << info.score
                 << "排名=" << info.rank;

        QTableWidgetItem* rankItem = new QTableWidgetItem(QString::number(info.rank));
        QTableWidgetItem* userItem = new QTableWidgetItem(info.username);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(info.score));
        QTableWidgetItem* timeItem = new QTableWidgetItem(info.time.toString("yyyy-MM-dd hh:mm:ss"));

        // 设置居中显示
        rankItem->setTextAlignment(Qt::AlignCenter);
        userItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);
        timeItem->setTextAlignment(Qt::AlignCenter);

        // 设置文字颜色为白色
        rankItem->setForeground(Qt::white);
        userItem->setForeground(Qt::white);
        scoreItem->setForeground(Qt::white);
        timeItem->setForeground(Qt::white);

        if (info.rank == 1) {
            rankItem->setForeground(Qt::yellow);
            rankItem->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
            userItem->setForeground(Qt::yellow);
            userItem->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
            scoreItem->setForeground(Qt::yellow);
            scoreItem->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
            timeItem->setForeground(Qt::yellow);
            timeItem->setFont(QFont("Microsoft YaHei", 10, QFont::Bold));
        } else if (info.rank == 2) {
            rankItem->setForeground(QColor(192, 192, 192));
            userItem->setForeground(QColor(192, 192, 192));
            scoreItem->setForeground(QColor(192, 192, 192));
            timeItem->setForeground(QColor(192, 192, 192));
        } else if (info.rank == 3) {
            rankItem->setForeground(QColor(205, 127, 50));
            userItem->setForeground(QColor(205, 127, 50));
            scoreItem->setForeground(QColor(205, 127, 50));
            timeItem->setForeground(QColor(205, 127, 50));
        }

        table->setItem(i, 0, rankItem);
        table->setItem(i, 1, userItem);
        table->setItem(i, 2, scoreItem);
        table->setItem(i, 3, timeItem);
    }
}

void RankPage::fillPersonalTable(const QVector<RankInfo>& records)
{
    ui->personalTable->setRowCount(records.size());

    qDebug() << "填充个人记录表，记录数:" << records.size();

    for (int i = 0; i < records.size(); ++i) {
        const RankInfo& info = records[i];

        qDebug() << "个人记录" << i << ":"
                 << "用户=" << info.username
                 << "分数=" << info.score
                 << "模式=" << info.mode;

        QTableWidgetItem* userItem = new QTableWidgetItem(info.username);
        QTableWidgetItem* scoreItem = new QTableWidgetItem(QString::number(info.score));
        QTableWidgetItem* modeItem = new QTableWidgetItem(info.mode);
        QTableWidgetItem* timeItem = new QTableWidgetItem(info.time.toString("yyyy-MM-dd hh:mm:ss"));

        // 设置居中显示
        userItem->setTextAlignment(Qt::AlignCenter);
        scoreItem->setTextAlignment(Qt::AlignCenter);
        modeItem->setTextAlignment(Qt::AlignCenter);
        timeItem->setTextAlignment(Qt::AlignCenter);

        // 设置文字颜色为白色
        userItem->setForeground(Qt::white);
        scoreItem->setForeground(Qt::white);
        modeItem->setForeground(Qt::white);
        timeItem->setForeground(Qt::white);

        // 如果是当前用户，可以高亮显示
        if (info.username == m_username) {
            userItem->setBackground(QColor(40, 40, 60));  // 深蓝色背景
            scoreItem->setBackground(QColor(40, 40, 60));
            modeItem->setBackground(QColor(40, 40, 60));
            timeItem->setBackground(QColor(40, 40, 60));
        }

        ui->personalTable->setItem(i, 0, userItem);
        ui->personalTable->setItem(i, 1, scoreItem);
        ui->personalTable->setItem(i, 2, modeItem);
        ui->personalTable->setItem(i, 3, timeItem);
    }
}
