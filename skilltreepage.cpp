// skilltreepage.cpp
#include "skilltreepage.h"
#include "ui_skilltreepage.h"
#include <QPainter>
#include <QStyleOption>
#include <QMessageBox>

void SkillTreePage::SkillTreeContainer::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 设置连线样式
    QPen pen(QColor(255, 157, 224,180)); // 粉色
    pen.setWidth(4);
    painter.setPen(pen);

    // 使用硬编码的相对坐标绘制连线
    // 行消除 -> 时间延长
    painter.drawLine(410, 65, 250, 180);

    // 行消除 -> 彩虹炸弹
    painter.drawLine(410, 65, 570, 180);

    // 时间延长 -> 得分翻倍
    painter.drawLine(250, 180, 250, 300);

    // 时间延长 -> 时间冻结
    painter.drawLine(250, 180, 90, 300);

    // 彩虹炸弹 -> 十字消除
    painter.drawLine(570, 180, 410, 300);

    // 彩虹炸弹 -> 颜色统一
    painter.drawLine(570, 180, 570, 300);

    // 颜色统一 -> 终极爆发
    painter.drawLine(570, 300, 570, 430);
}

SkillTreePage::SkillTreePage(SkillTree* skillTree, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SkillTreePage),
    skillTree(skillTree),
    skillTreeContainer(nullptr)
{
    ui->setupUi(this);

    // 注意：样式设置已迁移到.ui文件中
    // 仅保留信号连接和UI初始化

    connect(ui->btnBack, &QPushButton::clicked, this, &SkillTreePage::onBackButtonClicked);

    setupSkillTreeUI();
    refreshUI();
}

SkillTreePage::~SkillTreePage() {
    delete ui;
}

void SkillTreePage::setupSkillTreeUI() {
    // 清空现有UI元素
    QLayoutItem* item;
    while ((item = ui->skillTreeLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    skillButtons.clear();
    costLabels.clear();
    equipButtons.clear();
    unequipButtons.clear();
    skillPositions.clear();

    // 创建技能树容器
    skillTreeContainer = new SkillTreeContainer();
    skillTreeContainer->setObjectName("skillTreeContainer");
    skillTreeContainer->setStyleSheet("background:transparent;");

    QGridLayout* grid = new QGridLayout(skillTreeContainer);
    grid->setSpacing(15);
    grid->setContentsMargins(20, 20, 20, 20);

    // 设置容器的最小大小以确保滚动区域正常工作
    skillTreeContainer->setMinimumSize(450, 400);

    // 第0层: 行消除 (根节点)
    SkillNode* rowClear = skillTree->getSkill("row_clear");
    if (rowClear) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(rowClear->name);
        btn->setObjectName("row_clear");
        btn->setFixedSize(80, 30);
        btn->setToolTip(rowClear->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["row_clear"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(rowClear->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["row_clear"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("row_clear_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["row_clear"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("row_clear_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["row_clear"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 根节点放在中间位置 (第3列，第0行)
        grid->addLayout(vLayout, 0, 3, Qt::AlignCenter);
        skillPositions["row_clear"] = QPoint(3, 0);
    }

    // 第1层
    SkillNode* timeExtend = skillTree->getSkill("time_extend");
    SkillNode* rainbowBomb = skillTree->getSkill("rainbow_bomb");

    // 左分支：时间延长
    if (timeExtend) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(timeExtend->name);
        btn->setObjectName("time_extend");
        btn->setFixedSize(80, 30);
        btn->setToolTip(timeExtend->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["time_extend"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(timeExtend->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["time_extend"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("time_extend_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["time_extend"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("time_extend_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["time_extend"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在根节点左下方 (第1列，第1行)
        grid->addLayout(vLayout, 1, 1, Qt::AlignCenter);
        skillPositions["time_extend"] = QPoint(1, 1);
    }

    // 右分支：彩虹炸弹
    if (rainbowBomb) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(rainbowBomb->name);
        btn->setObjectName("rainbow_bomb");
        btn->setFixedSize(80, 30);
        btn->setToolTip(rainbowBomb->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["rainbow_bomb"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(rainbowBomb->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["rainbow_bomb"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("rainbow_bomb_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["rainbow_bomb"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("rainbow_bomb_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["rainbow_bomb"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在根节点右下方 (第5列，第1行)
        grid->addLayout(vLayout, 1, 5, Qt::AlignCenter);
        skillPositions["rainbow_bomb"] = QPoint(5, 1);
    }

    // 第2层 - 左分支的子节点
    SkillNode* crossClear = skillTree->getSkill("cross_clear");
    SkillNode* scoreDouble = skillTree->getSkill("score_double");

    // 十字消除（彩虹炸弹的子节点）
    if (crossClear) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(crossClear->name);
        btn->setObjectName("cross_clear");
        btn->setFixedSize(80, 30);
        btn->setToolTip(crossClear->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["cross_clear"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(crossClear->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["cross_clear"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("cross_clear_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["cross_clear"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("cross_clear_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["cross_clear"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在彩虹炸弹的左下方（第3列，第2行）
        grid->addLayout(vLayout, 2, 3, Qt::AlignCenter);
        skillPositions["cross_clear"] = QPoint(3, 2);
    }

    // 得分翻倍（时间延长的子节点）
    if (scoreDouble) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(scoreDouble->name);
        btn->setObjectName("score_double");
        btn->setFixedSize(80, 30);
        btn->setToolTip(scoreDouble->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["score_double"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(scoreDouble->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["score_double"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("score_double_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["score_double"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("score_double_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["score_double"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在时间延长的右下方 (第1列，第2行)
        grid->addLayout(vLayout, 2, 1, Qt::AlignCenter);
        skillPositions["score_double"] = QPoint(1, 2);
    }

    // 第2层 - 右分支的子节点
    SkillNode* colorUnify = skillTree->getSkill("color_unify");
    SkillNode* timeFreeze = skillTree->getSkill("time_freeze");

    // 颜色统一（彩虹炸弹的子节点）
    if (colorUnify) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(colorUnify->name);
        btn->setObjectName("color_unify");
        btn->setFixedSize(80, 30);
        btn->setToolTip(colorUnify->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["color_unify"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(colorUnify->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["color_unify"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("color_unify_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["color_unify"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("color_unify_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["color_unify"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在彩虹炸弹的右下方（第5列，第2行）
        grid->addLayout(vLayout, 2, 5, Qt::AlignCenter);
        skillPositions["color_unify"] = QPoint(5, 2);
    }

    // 时间冻结（时间延长的子节点）
    if (timeFreeze) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(timeFreeze->name);
        btn->setObjectName("time_freeze");
        btn->setFixedSize(80, 30);
        btn->setToolTip(timeFreeze->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["time_freeze"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(timeFreeze->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["time_freeze"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("time_freeze_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["time_freeze"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("time_freeze_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["time_freeze"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在时间延长的左下方 (第0列，第2行)
        grid->addLayout(vLayout, 2, 0, Qt::AlignCenter);
        skillPositions["time_freeze"] = QPoint(0, 2);
    }

    // 第3层：终极爆发（颜色统一的子节点）
    SkillNode* ultimateBurst = skillTree->getSkill("ultimate_burst");

    if (ultimateBurst) {
        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setSpacing(2);

        QPushButton* btn = new QPushButton(ultimateBurst->name);
        btn->setObjectName("ultimate_burst");
        btn->setFixedSize(80, 30);
        btn->setToolTip(ultimateBurst->description);
        connect(btn, &QPushButton::clicked, this, &SkillTreePage::onSkillButtonClicked);
        skillButtons["ultimate_burst"] = btn;
        vLayout->addWidget(btn, 0, Qt::AlignCenter);

        QLabel* costLabel = new QLabel(QString("消耗: %1").arg(ultimateBurst->cost));
        costLabel->setAlignment(Qt::AlignCenter);
        costLabel->setStyleSheet("color:#ffeb3b; font:8pt 'Microsoft YaHei';");
        costLabels["ultimate_burst"] = costLabel;
        vLayout->addWidget(costLabel);

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->setSpacing(5);

        QPushButton* equipBtn = new QPushButton("装");
        equipBtn->setObjectName("ultimate_burst_equip");
        equipBtn->setFixedSize(35, 20);
        equipBtn->setStyleSheet("background:#4caf50; color:white; font:8pt 'Microsoft YaHei';");
        connect(equipBtn, &QPushButton::clicked, this, &SkillTreePage::onEquipButtonClicked);
        equipButtons["ultimate_burst"] = equipBtn;
        buttonLayout->addWidget(equipBtn);

        QPushButton* unequipBtn = new QPushButton("卸");
        unequipBtn->setObjectName("ultimate_burst_unequip");
        unequipBtn->setFixedSize(35, 20);
        unequipBtn->setStyleSheet("background:#f44336; color:white; font:8pt 'Microsoft YaHei';");
        connect(unequipBtn, &QPushButton::clicked, this, &SkillTreePage::onUnequipButtonClicked);
        unequipButtons["ultimate_burst"] = unequipBtn;
        buttonLayout->addWidget(unequipBtn);

        vLayout->addLayout(buttonLayout);

        // 放在颜色统一的正下方 (第5列，第3行)
        grid->addLayout(vLayout, 3, 5, Qt::AlignCenter);
        skillPositions["ultimate_burst"] = QPoint(5, 3);
    }

    ui->skillTreeLayout->addWidget(skillTreeContainer);
}

void SkillTreePage::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
}

void SkillTreePage::refreshUI() {
    // 更新技能点显示
    ui->labelSkillPoints->setText(QString("技能点: %1").arg(skillTree->getSkillPoints()));

    // 更新所有技能按钮状态
    for (const QString& skillId : skillButtons.keys()) {
        updateSkillButtonState(skillId);
    }

    // 更新容器绘制
    if (skillTreeContainer) {
        skillTreeContainer->update();
    }
}

void SkillTreePage::updateSkillButtonState(const QString& skillId) {
    if (!skillButtons.contains(skillId)) return;

    SkillNode* skill = skillTree->getSkill(skillId);
    if (!skill) return;

    QPushButton* btn = skillButtons[skillId];

    // 设置按钮样式
    QString style;
    if (skill->equipped) {
        style = "background:#4caf50; color:white;";  // 已装备 - 绿色
    } else if (skill->unlocked) {
        style = "background:#ff9de0; color:white;";  // 已解锁 - 粉色
    } else if (skillTree->getSkillPoints() >= skill->cost &&
               skillTree->checkPrerequisites(skillId)) {
        style = "background:#2196f3; color:white;";  // 可解锁 - 蓝色
    } else {
        style = "background:#757575; color:#ccc;";    // 不可解锁 - 灰色
    }

    btn->setStyleSheet(style + "border-radius:8px; font:bold 10pt 'Microsoft YaHei';");

    // 更新装备/卸下按钮可见性
    if (equipButtons.contains(skillId)) {
        equipButtons[skillId]->setVisible(skill->unlocked && !skill->equipped);
    }
    if (unequipButtons.contains(skillId)) {
        unequipButtons[skillId]->setVisible(skill->equipped);
    }
}

void SkillTreePage::onSkillButtonClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString skillId = btn->objectName();

    // 尝试解锁技能
    if (skillTree->unlockSkill(skillId)) {
        QMessageBox::information(this, "成功", QString("已解锁技能: %1").arg(skillTree->getSkill(skillId)->name));
        refreshUI();
    } else {
        SkillNode* skill = skillTree->getSkill(skillId);
        if (skill && !skill->unlocked) {
            QString message = QString("无法解锁技能:\n");
            if (skillTree->getSkillPoints() < skill->cost) {
                message += QString("技能点不足 (需要: %1, 当前: %2)\n").arg(skill->cost).arg(skillTree->getSkillPoints());
            }
            if (!skillTree->checkPrerequisites(skillId)) {
                message += "前置技能未解锁\n";
            }
            QMessageBox::warning(this, "解锁失败", message);
        }
    }
}

void SkillTreePage::onEquipButtonClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString btnName = btn->objectName();
    QString skillId = btnName.replace("_equip", "");

    if (skillTree->equipSkill(skillId)) {
        refreshUI();
    } else {
        // 检查是否已装备满3个技能
        QList<SkillNode*> equipped = skillTree->getEquippedSkills();
        if (equipped.size() >= 3) {
            QMessageBox::warning(this, "装备失败", "最多只能装备3个技能！");
        } else {
            QMessageBox::warning(this, "装备失败", "技能未解锁！");
        }
    }
}

void SkillTreePage::onUnequipButtonClicked() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString btnName = btn->objectName();
    QString skillId = btnName.replace("_unequip", "");

    if (skillTree->unequipSkill(skillId)) {
        refreshUI();
    }
}

void SkillTreePage::onBackButtonClicked() {
    // 保存技能状态到数据库
    emit backRequested();
}
