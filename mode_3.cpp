#include "mode_3.h"
#include "ui_mode_3.h"
#include <QGridLayout>
#include <QMouseEvent>
#include <QDebug>
#include <QRandomGenerator>
#include <QDir>
#include <QMessageBox>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QDialog>
#include <QtSql>

Mode_3::Mode_3(GameBoard *board, QString username, QWidget *parent)
    : QWidget(parent), ui(new Ui::mode_3), m_board(board), m_username(username)
{
    ui->setupUi(this);

    // 设置主题颜色为绿色系
    ui->topBar->setStyleSheet("background:#111; border-bottom:2px solid #7cffcb;");
    ui->boardWidget->setStyleSheet("background:#162640; border:2px solid #7cffcb; border-radius:12px;");
    ui->labelCountdown->setStyleSheet("color:#7cffcb; font:700 24pt 'Microsoft YaHei';");
    ui->btnBack->setStyleSheet(
        "QPushButton{"
        "  color:#fff; font:14pt 'Microsoft YaHei'; border:none; border-radius:8px;"
        "  background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #7cffcb, stop:1 #4dc6a0);"
        "}"
        "QPushButton:hover{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #4dc6a0, stop:1 #2da888);}"
        "QPushButton:pressed{background:qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #2da888, stop:1 #1f8a70);}"
        );

    m_score = 0;
    m_totalTime = 180;

    // UI 初始化
    ui->labelScore->setText("暂停");
    ui->labelScore->setStyleSheet("color:#fff; font:bold 15pt 'Microsoft YaHei';");
    ui->labelScore->setAlignment(Qt::AlignCenter);
    ui->labelScore->installEventFilter(this);
    ui->labelScore->setCursor(Qt::PointingHandCursor);

    ui->labelScore_2->setText("分数 0");
    ui->labelCountdown->setText("03:00");
    ui->btnHint->setText(QString("提示 (%1)").arg(m_hintCount));

    m_gridLayout = new QGridLayout(ui->boardWidget);
    m_gridLayout->setSpacing(2);
    m_gridLayout->setContentsMargins(4, 4, 4, 4);

    // 【核心】开启鼠标追踪
    ui->boardWidget->setMouseTracking(true);
    ui->boardWidget->installEventFilter(this);

    // 创建选中指示器（单个格子）
    m_selectionIndicator = new QLabel(ui->boardWidget);
    m_selectionIndicator->setStyleSheet("border: 3px solid #7cffcb; border-radius: 10px; background: rgba(124, 255, 203, 30);");
    m_selectionIndicator->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_selectionIndicator->hide();

    // 动画组初始化
    m_dropGroup = new QSequentialAnimationGroup(this);

    m_gameTimer = new QTimer(this);
    m_gameTimer->setInterval(1000);
    connect(m_gameTimer, &QTimer::timeout, this, &Mode_3::onTimerTick);

    connect(m_board, &GameBoard::gridUpdated, this, &Mode_3::rebuildGrid);
    connect(ui->btnBack, &QPushButton::clicked, this, &Mode_3::onBackButtonClicked);
    connect(ui->btnUndo, &QPushButton::clicked, this, &Mode_3::on_btnUndo_clicked);
    connect(ui->btnHint, &QPushButton::clicked, this, &Mode_3::on_btnHint_clicked);
    connect(ui->btnSkill, &QPushButton::clicked, this, &Mode_3::on_btnSkill_clicked);

    // 生成初始随机小动物
    generateRandomAnimal();
    rebuildGrid();
}

Mode_3::~Mode_3() { delete ui; }

/* =========================================================
 * 1. 随机小动物生成与显示
 * ========================================================= */
void Mode_3::generateRandomAnimal()
{
    auto &rng = *QRandomGenerator::global();
    m_currentAnimal = rng.bounded(6); // 0-5
    updateAnimalDisplay();
}

void Mode_3::updateAnimalDisplay()
{
    if (m_currentAnimal >= 0 && m_currentAnimal < 6) {
        QString path = QString("%1%2.png").arg(QDir::currentPath() + "/").arg(m_currentAnimal);
        QPixmap pix(path);
        if (!pix.isNull()) {
            ui->labelCurrentAnimal->setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

/* =========================================================
 * 2. 交互逻辑 (鼠标追踪与点击)
 * ========================================================= */
bool Mode_3::eventFilter(QObject *watched, QEvent *event)
{
    // 暂停按钮
    if (watched == ui->labelScore && event->type() == QEvent::MouseButtonPress) {
        togglePause();
        return true;
    }

    // 棋盘交互
    if (watched == ui->boardWidget && !m_isLocked && !m_isPaused && m_hasGameStarted) {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent *me = static_cast<QMouseEvent*>(event);
            updateSelectionPos(me->pos());
            return true;
        }
        else if (event->type() == QEvent::MouseButtonPress) {
            // 点击时停止提示动画
            stopHint();
            tryTransformInteraction();
            return true;
        }
        else if (event->type() == QEvent::Leave) {
            m_selectionIndicator->hide();
            m_selectedR = -1; m_selectedC = -1;
        }
    }
    return QWidget::eventFilter(watched, event);
}

void Mode_3::updateSelectionPos(QPoint mousePos)
{
    int cellSize = 48; int gap = 2;
    QRect cr = ui->boardWidget->contentsRect();
    int totalW = COL * (cellSize + gap) - gap;
    int totalH = ROW * (cellSize + gap) - gap;
    int ox = cr.left() + (cr.width() - totalW) / 2;
    int oy = cr.top()  + (cr.height() - totalH) / 2;

    // 计算鼠标位于哪个格子
    int c = (mousePos.x() - ox) / (cellSize + gap);
    int r = (mousePos.y() - oy) / (cellSize + gap);

    if (r < 0 || r >= ROW || c < 0 || c >= COL) {
        m_selectionIndicator->hide();
        m_selectedR = -1; m_selectedC = -1;
        return;
    }

    m_selectedR = r; m_selectedC = c;

    int targetX = ox + c * (cellSize + gap) - 1;
    int targetY = oy + r * (cellSize + gap) - 1;

    m_selectionIndicator->resize(cellSize + 2, cellSize + 2);
    m_selectionIndicator->move(targetX, targetY);
    m_selectionIndicator->show();
    m_selectionIndicator->raise();
}

void Mode_3::tryTransformInteraction()
{
    if (m_selectedR == -1 || m_selectedC == -1) return;
    if (m_currentAnimal < 0) return;

    // 检查点击的格子是否已经是当前动物类型
    if (m_board->m_grid[m_selectedR][m_selectedC].pic == m_currentAnimal) {
        // 抖动提示
        QPropertyAnimation *shake = new QPropertyAnimation(m_selectionIndicator, "pos");
        shake->setDuration(200);
        QPoint p = m_selectionIndicator->pos();
        shake->setKeyValueAt(0, p);
        shake->setKeyValueAt(0.25, p + QPoint(5, 0));
        shake->setKeyValueAt(0.75, p - QPoint(5, 0));
        shake->setKeyValueAt(1, p);
        shake->start(QAbstractAnimation::DeleteWhenStopped);
        return;
    }

    // 保存状态用于悔棋
    saveState();

    // 执行变身
    processTransform(m_selectedR, m_selectedC);
}

void Mode_3::processTransform(int r, int c)
{
    m_isLocked = true;
    m_selectionIndicator->hide();

    // 创建变身动画
    QPushButton *btn = m_cells[r * COL + c];
    if (!btn) {
        m_isLocked = false;
        return;
    }

    // 1. 首先将按钮变小并变透明（消失效果）
    QParallelAnimationGroup *disappearGroup = new QParallelAnimationGroup(this);

    QPropertyAnimation *scaleOut = new QPropertyAnimation(btn, "geometry");
    scaleOut->setDuration(150);
    QRect rect = btn->geometry();
    scaleOut->setStartValue(rect);
    scaleOut->setEndValue(rect.adjusted(24, 24, -24, -24));

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(btn);
    btn->setGraphicsEffect(eff);
    QPropertyAnimation *fadeOut = new QPropertyAnimation(eff, "opacity");
    fadeOut->setDuration(150);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);

    disappearGroup->addAnimation(scaleOut);
    disappearGroup->addAnimation(fadeOut);

    connect(disappearGroup, &QAbstractAnimation::finished, this, [this, btn, r, c, eff, disappearGroup](){
        // 2. 消失动画完成后，改变格子类型
        m_board->m_grid[r][c].pic = m_currentAnimal;

        // 更新按钮图标
        QString path = QString("%1%2.png").arg(QDir::currentPath() + "/").arg(m_currentAnimal);
        btn->setIcon(QIcon(path));

        // 3. 生成新的随机小动物
        generateRandomAnimal();

        // 4. 重新显示按钮（出现效果）
        QPropertyAnimation *scaleIn = new QPropertyAnimation(btn, "geometry");
        scaleIn->setDuration(150);
        QRect rect = btn->geometry();
        scaleIn->setStartValue(rect.adjusted(24, 24, -24, -24));
        scaleIn->setEndValue(rect);

        QPropertyAnimation *fadeIn = new QPropertyAnimation(eff, "opacity");
        fadeIn->setDuration(150);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        QParallelAnimationGroup *appearGroup = new QParallelAnimationGroup(this);
        appearGroup->addAnimation(scaleIn);
        appearGroup->addAnimation(fadeIn);

        connect(appearGroup, &QAbstractAnimation::finished, this, [this, appearGroup, btn, eff](){
            btn->setGraphicsEffect(nullptr);
            appearGroup->deleteLater();

            // 5. 检查消除
            QSet<QPoint> allMatches;
            ElimResult res = getEliminations(m_selectedR, m_selectedC);

            if (!res.points.isEmpty()) {
                allMatches.unite(res.points);

                // 播放特效（如果有）
                if (res.type != Normal && res.type != None) {
                    playSpecialEffect(res.type, res.center, 0);
                }
            }

            if (!allMatches.isEmpty()) {
                playEliminateAnim(allMatches);
            } else {
                m_isLocked = false;
                // 恢复选中指示器显示
                if (ui->boardWidget->underMouse()) {
                    m_selectionIndicator->show();
                }
            }
        });

        appearGroup->start();
        disappearGroup->deleteLater();
    });

    disappearGroup->start();
}

/* =========================================================
 * 3. 核心与动画 (Rebuild, Fall, Check)
 * ========================================================= */
void Mode_3::rebuildGrid()
{
    clearGridLayout();
    m_cells.resize(ROW * COL);
    const Grid &gr = m_board->grid();

    QRect cr = ui->boardWidget->contentsRect();
    int cellSize = 48; int gap = 2;
    int totalW = COL * (cellSize + gap) - gap;
    int totalH = ROW * (cellSize + gap) - gap;
    int ox = cr.left() + (cr.width() - totalW) / 2;
    int oy = cr.top()  + (cr.height() - totalH) / 2;

    for(int r=0; r<ROW; ++r)
        for(int c=0; c<COL; ++c) {
            QPushButton *btn = new QPushButton(ui->boardWidget);
            btn->setFixedSize(cellSize, cellSize);
            btn->setStyleSheet("border:none;");
            btn->setAttribute(Qt::WA_TransparentForMouseEvents);

            QString path = QString("%1%2.png").arg(QDir::currentPath() + "/").arg(gr[r][c].pic);
            btn->setIcon(QIcon(path));
            btn->setIconSize(QSize(cellSize, cellSize));

            int targetX = ox + c * (cellSize + gap);
            int targetY = oy + r * (cellSize + gap);
            int startY = targetY - (ROW * cellSize + 100);
            btn->move(targetX, startY);

            QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(btn);
            eff->setOpacity(0.0);
            btn->setGraphicsEffect(eff);
            btn->show();
            m_cells[r * COL + c] = btn;
        }

    createDropAnimation(ox, oy);
}

void Mode_3::createDropAnimation(int left0, int top0)
{
    if (m_dropGroup->state() == QAbstractAnimation::Running) {
        m_dropGroup->stop();
    }
    m_dropGroup->clear();
    m_dropGroup->disconnect(this);

    int cellSize = 48; int gap = 2;

    for(int r=ROW-1; r>=0; --r) {
        auto *rowPar = new QParallelAnimationGroup(this);
        for(int c=0; c<COL; ++c) {
            QPushButton *btn = m_cells[r * COL + c];
            if(!btn) continue;

            auto *eff = qobject_cast<QGraphicsOpacityEffect*>(btn->graphicsEffect());
            if(!eff) {
                eff = new QGraphicsOpacityEffect(btn);
                btn->setGraphicsEffect(eff);
            }

            int targetX = left0 + c * (cellSize + gap);
            int targetY = top0  + r * (cellSize + gap);

            auto *drop = new QPropertyAnimation(btn, "pos");
            drop->setDuration(180 + (ROW-1-r)*15);
            drop->setStartValue(QPoint(targetX, targetY - (ROW * cellSize + 100)));
            drop->setEndValue(QPoint(targetX, targetY));
            drop->setEasingCurve(QEasingCurve::OutBounce);

            auto *fade = new QPropertyAnimation(eff, "opacity");
            fade->setDuration(300);
            fade->setStartValue(0.0);
            fade->setEndValue(1.0);

            auto *cellAnim = new QParallelAnimationGroup(this);
            cellAnim->addAnimation(drop);
            cellAnim->addAnimation(fade);
            rowPar->addAnimation(cellAnim);
        }
        m_dropGroup->addAnimation(rowPar);
    }

    connect(m_dropGroup, &QSequentialAnimationGroup::finished, this, [this](){
        for(int r=0; r<ROW; ++r)
            for(int c=0; c<COL; ++c)
                if(m_cells[r*COL+c]) m_gridLayout->addWidget(m_cells[r*COL+c], r, c);
        m_isLocked = false;
        if(!m_hasGameStarted) {
            m_hasGameStarted = true;
            startGameSequence();
        }
    });
    m_dropGroup->start();
}

void Mode_3::performFallAnimation()
{
    // 与 Mode_2 逻辑一致，但生成新按钮时要注意属性
    QRect cr = ui->boardWidget->contentsRect();
    int cellSize = 48; int gap = 2;
    int totalW = COL * (cellSize + gap) - gap;
    int totalH = ROW * (cellSize + gap) - gap;
    int ox = cr.left() + (cr.width() - totalW) / 2;
    int oy = cr.top()  + (cr.height() - totalH) / 2;

    QParallelAnimationGroup *fallGroup = new QParallelAnimationGroup(this);
    auto *rng = QRandomGenerator::global();

    struct BlockData { QPushButton* btn; int color; };
    for (QPushButton *b : m_cells) if (b) m_gridLayout->removeWidget(b);

    for (int c = 0; c < COL; ++c) {
        QList<BlockData> survivors;
        for (int r = ROW - 1; r >= 0; --r) {
            int idx = r * COL + c;
            if (m_cells[idx]) {
                survivors.append({m_cells[idx], m_board->m_grid[r][c].pic});
                m_cells[idx] = nullptr;
            }
        }

        int survivorIdx = 0;
        for (int r = ROW - 1; r >= 0; --r) {
            QPushButton *btn = nullptr;
            int finalColor = 0;
            int destX = ox + c * (cellSize + gap);
            int destY = oy + r * (cellSize + gap);

            if (survivorIdx < survivors.size()) {
                BlockData bd = survivors[survivorIdx++];
                btn = bd.btn;
                finalColor = bd.color;
            } else {
                finalColor = rng->bounded(6);
                btn = new QPushButton(ui->boardWidget);
                btn->setFixedSize(cellSize, cellSize);
                btn->setStyleSheet("border:none;");
                btn->setAttribute(Qt::WA_TransparentForMouseEvents);

                QString path = QString("%1%2.png").arg(QDir::currentPath() + "/").arg(finalColor);
                btn->setIcon(QIcon(path));
                btn->setIconSize(QSize(cellSize, cellSize));
                btn->show();
                btn->move(destX, destY - (ROW * cellSize + 100));
            }
            m_cells[r * COL + c] = btn;
            m_board->m_grid[r][c].pic = finalColor;

            QPropertyAnimation *anim = new QPropertyAnimation(btn, "pos");
            anim->setDuration(500);
            anim->setStartValue(btn->pos());
            anim->setEndValue(QPoint(destX, destY));
            anim->setEasingCurve(QEasingCurve::OutBounce);
            fallGroup->addAnimation(anim);
        }
    }

    connect(fallGroup, &QAbstractAnimation::finished, this, [this, fallGroup](){
        for (QPushButton *b : m_cells) if (b) m_gridLayout->addWidget(b);
        fallGroup->deleteLater();
        checkComboMatches();
    });
    fallGroup->start();
}

void Mode_3::checkComboMatches()
{
    QSet<QPoint> allMatches;
    QSet<QPoint> processedCenters;

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            ElimResult res = getEliminations(r, c);
            if (!res.points.isEmpty()) {
                allMatches.unite(res.points);
                if (res.type != Normal && res.type != None) {
                    if (!processedCenters.contains(res.center)) {
                        playSpecialEffect(res.type, res.center, 0);
                        processedCenters.insert(res.center);
                    }
                }
            }
        }
    }

    if (!allMatches.isEmpty()) {
        m_isLocked = true;
        playEliminateAnim(allMatches);
    } else {
        if (m_board->isDead(m_board->grid())) handleDeadlock();
        else {
            m_isLocked = false;
            // 恢复选中指示器显示
            if (ui->boardWidget->underMouse()) m_selectionIndicator->show();
        }
    }
}

void Mode_3::clearGridLayout()
{
    if (m_dropGroup) { m_dropGroup->stop(); m_dropGroup->clear(); }
    for (QPushButton *b : m_cells) if (b) delete b;
    m_cells.clear();
    // 强制清理布局
    if (m_gridLayout) {
        QLayoutItem *item;
        while ((item = m_gridLayout->takeAt(0)) != nullptr) {
            if (item->widget()) delete item->widget();
            delete item;
        }
    }
}

/* =========================================================
 * 4. 提示系统 (修改为查找可消除的点击位置)
 * ========================================================= */
void Mode_3::on_btnHint_clicked()
{
    if (m_hintCount <= 0 || m_isLocked || m_isPaused) return;
    if (m_hintAnimGroup && m_hintAnimGroup->state() == QAbstractAnimation::Running) return;

    int r, c;
    if (findValidMove(r, c)) {
        m_hintCount--;
        ui->btnHint->setText(QString("提示 (%1)").arg(m_hintCount));
        if (m_hintCount <= 0) {
            ui->btnHint->setEnabled(false);
            ui->btnHint->setStyleSheet("QPushButton{ border:2px solid #555; background:#333; color:#888; border-radius:10px; }");
        }
        showHint(r, c);
    }
}

bool Mode_3::findValidMove(int &outR, int &outC)
{
    // 遍历所有格子，模拟将其变为当前动物类型，检查是否能消除
    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            // 如果已经是当前类型，跳过
            if (m_board->m_grid[r][c].pic == m_currentAnimal) continue;

            // 模拟变换
            int originalType = m_board->m_grid[r][c].pic;
            m_board->m_grid[r][c].pic = m_currentAnimal;

            // 检查是否有消除
            ElimResult res = getEliminations(r, c);

            // 恢复原状
            m_board->m_grid[r][c].pic = originalType;

            if (!res.points.isEmpty()) {
                outR = r; outC = c;
                return true;
            }
        }
    }
    return false;
}

void Mode_3::showHint(int r, int c)
{
    m_selectedR = r; m_selectedC = c;

    int cellSize = 48; int gap = 2;
    QRect cr = ui->boardWidget->contentsRect();
    int totalW = COL * (cellSize + gap) - gap;
    int totalH = ROW * (cellSize + gap) - gap;
    int ox = cr.left() + (cr.width() - totalW) / 2;
    int oy = cr.top()  + (cr.height() - totalH) / 2;
    int targetX = ox + c * (cellSize + gap) - 1;
    int targetY = oy + r * (cellSize + gap) - 1;

    m_selectionIndicator->resize(cellSize + 2, cellSize + 2);
    m_selectionIndicator->move(targetX, targetY);

    // 设置闪烁样式
    m_selectionIndicator->setStyleSheet("border: 3px solid #ffeb3b; border-radius: 10px; background: rgba(255, 235, 59, 60);");
    m_selectionIndicator->show();
    m_selectionIndicator->raise();

    // 闪烁动画
    if (m_hintAnimGroup) m_hintAnimGroup->deleteLater();
    m_hintAnimGroup = new QSequentialAnimationGroup(this);
    m_hintAnimGroup->setLoopCount(-1);

    QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(m_selectionIndicator);
    m_selectionIndicator->setGraphicsEffect(eff);

    QPropertyAnimation *anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(800);
    anim->setStartValue(1.0);
    anim->setKeyValueAt(0.5, 0.3);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::InOutSine);

    m_hintAnimGroup->addAnimation(anim);
    m_hintAnimGroup->start();
}

void Mode_3::stopHint()
{
    if (m_hintAnimGroup) {
        m_hintAnimGroup->stop();
        m_hintAnimGroup->deleteLater();
        m_hintAnimGroup = nullptr;
    }
    // 恢复默认样式
    m_selectionIndicator->setGraphicsEffect(nullptr);
    m_selectionIndicator->setStyleSheet("border: 3px solid #7cffcb; border-radius: 10px; background: rgba(124, 255, 203, 30);");
}

/* =========================================================
 * 5. 辅助函数 (特效、悔棋、结算等)
 * ========================================================= */
void Mode_3::saveState() {
    GameStateSnapshot snapshot;
    snapshot.grid = m_board->grid();
    snapshot.score = m_score;
    snapshot.currentAnimal = m_currentAnimal; // 保存当前动物
    m_undoStack.push(snapshot);
}

void Mode_3::on_btnUndo_clicked() {
    if (m_isLocked || m_isPaused || m_undoStack.isEmpty()) return;
    GameStateSnapshot last = m_undoStack.pop();
    m_board->m_grid = last.grid;
    m_score = last.score;
    m_currentAnimal = last.currentAnimal; // 恢复动物
    updateAnimalDisplay();
    ui->labelScore_2->setText(QString("分数 %1").arg(m_score));
    rebuildGrid();
}

void Mode_3::playEliminateAnim(const QSet<QPoint>& points) {
    if (!points.isEmpty()) addScore(points.size());

    QParallelAnimationGroup *elimGroup = new QParallelAnimationGroup(this);
    for (const QPoint &p : points) {
        int idx = p.x() * COL + p.y();
        QPushButton *btn = m_cells[idx];
        if (!btn) continue;

        QPropertyAnimation *scale = new QPropertyAnimation(btn, "geometry");
        scale->setDuration(250);
        QRect rect = btn->geometry();
        scale->setStartValue(rect);
        scale->setEndValue(rect.adjusted(24, 24, -24, -24));

        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(btn);
        btn->setGraphicsEffect(eff);
        QPropertyAnimation *fade = new QPropertyAnimation(eff, "opacity");
        fade->setDuration(250);
        fade->setStartValue(1.0);
        fade->setEndValue(0.0);

        elimGroup->addAnimation(scale);
        elimGroup->addAnimation(fade);
        m_board->m_grid[p.x()][p.y()].pic = -1;
    }

    connect(elimGroup, &QAbstractAnimation::finished, this, [this, elimGroup, points](){
        for (const QPoint &p : points) {
            int idx = p.x() * COL + p.y();
            if (m_cells[idx]) { delete m_cells[idx]; m_cells[idx] = nullptr; }
        }
        elimGroup->deleteLater();
        performFallAnimation();
    });
    elimGroup->start();
}

Mode_3::ElimResult Mode_3::getEliminations(int r, int c) {
    ElimResult res; res.center = QPoint(r, c);
    int color = m_board->m_grid[r][c].pic;
    if (color == -1) return res;
    int up = countDirection(r, c, -1, 0); int down = countDirection(r, c, 1, 0);
    int left = countDirection(r, c, 0, -1); int right = countDirection(r, c, 0, 1);

    if ((up + down >= 4) || (left + right >= 4)) {
        res.type = ColorClear;
        for (int i=0; i<ROW; ++i) for (int j=0; j<COL; ++j) if(m_board->m_grid[i][j].pic==color) res.points.insert(QPoint(i,j));
        return res;
    }
    if (up + down == 3) { res.type = ColBomb; for(int i=0; i<ROW; ++i) res.points.insert(QPoint(i, c)); return res; }
    if (left + right == 3) { res.type = RowBomb; for(int j=0; j<COL; ++j) res.points.insert(QPoint(r, j)); return res; }
    if (up + down >= 2 && left + right >= 2) {
        res.type = AreaBomb;
        for(int i=r-2; i<=r+2; ++i) for(int j=c-2; j<=c+2; ++j) if(i>=0 && i<ROW && j>=0 && j<COL) res.points.insert(QPoint(i, j));
        return res;
    }
    if (up + down >= 2) { res.type = Normal; for(int i=r-up; i<=r+down; ++i) res.points.insert(QPoint(i,c)); }
    if (left + right >= 2) { res.type = Normal; for(int j=c-left; j<=c+right; ++j) res.points.insert(QPoint(r,j)); }
    return res;
}

int Mode_3::countDirection(int r, int c, int dR, int dC) {
    int color = m_board->m_grid[r][c].pic;
    int cnt = 0; int nr = r+dR; int nc = c+dC;
    while(nr>=0 && nr<ROW && nc>=0 && nc<COL && m_board->m_grid[nr][nc].pic == color) { cnt++; nr+=dR; nc+=dC; }
    return cnt;
}

void Mode_3::handleDeadlock() {
    if (m_isLocked && !ui->btnBack->hasFocus()) return;
    m_isLocked = true;
    QLabel *lbl = new QLabel(ui->boardWidget);
    lbl->setText("死局！重新洗牌");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setStyleSheet("background-color:rgba(0,0,0,180); color:#7cffcb; font:bold 26pt 'Microsoft YaHei'; border-radius:15px; border:2px solid #7cffcb;");
    lbl->adjustSize(); lbl->resize(lbl->width()+60, lbl->height()+40);
    QRect cr = ui->boardWidget->rect();
    lbl->move((cr.width()-lbl->width())/2, (cr.height()-lbl->height())/2);
    lbl->show();
    lbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    QTimer::singleShot(2000, [this, lbl](){
        lbl->deleteLater();
        m_board->initNoThree();
    });
}

void Mode_3::startGameSequence() {
    m_isLocked = true;
    QLabel *lbl = new QLabel(ui->boardWidget);
    lbl->setText("Ready Go!"); lbl->setStyleSheet("color:#7cffcb; font:bold 40pt 'Microsoft YaHei';");
    lbl->adjustSize();
    QRect cr = ui->boardWidget->rect();
    lbl->move((cr.width()-lbl->width())/2, (cr.height()-lbl->height())/2);
    lbl->show();
    QTimer::singleShot(1000, [this, lbl](){ lbl->deleteLater(); m_isLocked=false; m_gameTimer->start(); });
}

void Mode_3::onTimerTick() {
    if (m_totalTime > 0) {
        m_totalTime--;
        ui->labelCountdown->setText(QString("%1:%2").arg(m_totalTime/60, 2, 10, QChar('0')).arg(m_totalTime%60, 2, 10, QChar('0')));
    } else { m_gameTimer->stop(); gameOver(); }
}

void Mode_3::addScore(int count) {
    m_score += count * 50;
    ui->labelScore_2->setText(QString("分数 %1").arg(m_score));
}

void Mode_3::togglePause() {
    if (!m_hasGameStarted) return;
    m_isPaused = !m_isPaused;
    if (m_isPaused) { m_gameTimer->stop(); ui->labelScore->setText("继续"); }
    else { m_gameTimer->start(); ui->labelScore->setText("暂停"); }
}

void Mode_3::on_btnSkill_clicked() { qDebug() << "Skill"; }

void Mode_3::playSpecialEffect(EffectType type, QPoint center, int colorCode)
{
    if (type == None || type == Normal) return;

    // 重新计算坐标偏移 (确保特效位置准确)
    int cellSize = 48; int gap = 2;
    QRect cr = ui->boardWidget->contentsRect();
    int totalW = COL * (cellSize + gap) - gap;
    int totalH = ROW * (cellSize + gap) - gap;
    int ox = cr.left() + (cr.width() - totalW) / 2;
    int oy = cr.top()  + (cr.height() - totalH) / 2;

    int centerX = ox + center.y() * (cellSize + gap) + cellSize / 2;
    int centerY = oy + center.x() * (cellSize + gap) + cellSize / 2;

    // 1. 行/列 激光炮 (改为绿色系)
    if (type == RowBomb || type == ColBomb) {
        QLabel *beam = new QLabel(ui->boardWidget);
        beam->setAttribute(Qt::WA_TransparentForMouseEvents);
        beam->show();

        // 样式：中间青绿，两边透明
        QString qss = "background: qlineargradient(x1:0, y1:0, x2:%1, y2:%2, "
                      "stop:0 rgba(124,255,203,0), stop:0.5 rgba(200,255,235,230), stop:1 rgba(124,255,203,0));";

        QPropertyAnimation *anim = new QPropertyAnimation(beam, "geometry");
        anim->setDuration(400);
        anim->setEasingCurve(QEasingCurve::OutExpo);

        if (type == RowBomb) {
            beam->setStyleSheet(qss.arg("0").arg("1"));
            anim->setStartValue(QRect(ox, centerY - 2, totalW, 4));
            anim->setEndValue(QRect(ox, centerY - 25, totalW, 50));
        } else {
            beam->setStyleSheet(qss.arg("1").arg("0"));
            anim->setStartValue(QRect(centerX - 2, oy, 4, totalH));
            anim->setEndValue(QRect(centerX - 25, oy, 50, totalH));
        }

        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(beam);
        beam->setGraphicsEffect(eff);
        QPropertyAnimation *fade = new QPropertyAnimation(eff, "opacity");
        fade->setDuration(400);
        fade->setStartValue(1.0);
        fade->setEndValue(0.0);
        fade->start(QAbstractAnimation::DeleteWhenStopped);

        anim->start(QAbstractAnimation::DeleteWhenStopped);
        connect(anim, &QAbstractAnimation::finished, beam, &QLabel::deleteLater);
    }

    // 2. 区域炸弹 (冲击波 - 改为绿色)
    else if (type == AreaBomb) {
        QLabel *shockwave = new QLabel(ui->boardWidget);
        shockwave->setAttribute(Qt::WA_TransparentForMouseEvents);
        shockwave->setStyleSheet(
            "background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, "
            "stop:0 rgba(0,0,0,0), stop:0.6 rgba(124, 255, 203, 180), stop:1 rgba(0,0,0,0));"
            "border-radius: 100px;");
        shockwave->show();

        QPropertyAnimation *anim = new QPropertyAnimation(shockwave, "geometry");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::OutQuad);

        QRect startRect(centerX, centerY, 0, 0);
        QRect endRect(centerX - 150, centerY - 150, 300, 300);

        anim->setStartValue(startRect);
        anim->setEndValue(endRect);
        anim->start(QAbstractAnimation::DeleteWhenStopped);
        connect(anim, &QAbstractAnimation::finished, shockwave, &QLabel::deleteLater);
    }

    // 3. 全屏闪光 (改为绿色闪光)
    else if (type == ColorClear) {
        QLabel *flash = new QLabel(ui->boardWidget);
        flash->setAttribute(Qt::WA_TransparentForMouseEvents);
        flash->setStyleSheet("background-color: rgba(124, 255, 203, 120);");
        flash->setGeometry(ui->boardWidget->rect());
        flash->show();

        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(flash);
        flash->setGraphicsEffect(eff);

        QPropertyAnimation *fade = new QPropertyAnimation(eff, "opacity");
        fade->setDuration(600);
        fade->setStartValue(1.0);
        fade->setEndValue(0.0);

        connect(fade, &QAbstractAnimation::finished, flash, &QLabel::deleteLater);
        fade->start();
    }
}

void Mode_3::onBackButtonClicked()
{
    bool wasRunning = m_gameTimer->isActive();
    m_gameTimer->stop();

    QDialog dlg(this);
    dlg.setWindowTitle("确认返回");
    dlg.setFixedSize(360, 220);
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    dlg.setStyleSheet(
        "QDialog {"
        "   background-color: #162640;"
        "   border: none;"
        "   border-radius: 10px;"
        "}"
        "QLabel {"
        "   color: #ffffff;"
        "   font: 14pt 'Microsoft YaHei';"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QPushButton {"
        "   border-radius: 15px;"
        "   color: black;"
        "   font: bold 12pt 'Microsoft YaHei';"
        "   height: 30px;"
        "   width: 80px;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(&dlg);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *lblMsg = new QLabel("若返回，当前对局信息丢失。\n是否确定返回？", &dlg);
    lblMsg->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(lblMsg);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(40);
    btnLayout->addStretch();

    QPushButton *btnYes = new QPushButton("确定", &dlg);
    btnYes->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff5555, stop:1 #ff7777); color: white; }"
        "QPushButton:hover { background: #ff4444; }"
        );
    connect(btnYes, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnLayout->addWidget(btnYes);

    QPushButton *btnNo = new QPushButton("取消", &dlg);
    btnNo->setStyleSheet(
        "QPushButton { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7cffcb, stop:1 #4dc6a0); }"
        "QPushButton:hover { background: #2da888; }"
        );
    connect(btnNo, &QPushButton::clicked, &dlg, &QDialog::reject);
    btnLayout->addWidget(btnNo);

    btnLayout->addStretch();
    mainLayout->addLayout(btnLayout);

    int ret = dlg.exec();

    if (ret == QDialog::Accepted) {
        emit gameFinished();
    } else {
        if (wasRunning) m_gameTimer->start();
    }
}

void Mode_3::gameOver()
{
    m_isLocked = true;

    // 1. 数据库写入
    {
        QSqlDatabase db = QSqlDatabase::database();
        if (db.isOpen()) {
            QSqlQuery q(db);
            QString insertSql = QString("INSERT INTO `%1` (mode, score, time) VALUES (?, ?, NOW())").arg(m_username);
            q.prepare(insertSql); q.addBindValue("变身"); q.addBindValue(m_score);
            q.exec();
            QSqlQuery qUp(db);
            qUp.prepare("UPDATE user SET points = points + ? WHERE username = ?");
            qUp.addBindValue(m_score); qUp.addBindValue(m_username);
            qUp.exec();
        }
    }

    // 2. 弹窗 UI (绿色风格)
    QDialog dlg(this);
    dlg.setWindowTitle("结算");
    dlg.setFixedSize(320, 240);
    dlg.setWindowFlags(dlg.windowFlags() & ~Qt::WindowContextHelpButtonHint);

    dlg.setStyleSheet(
        "QDialog {"
        "   background-color: #162640;"
        "   border: none;"
        "   border-radius: 10px;"
        "}"
        "QLabel {"
        "   color: #ffffff;"
        "   font-family: 'Microsoft YaHei';"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #7cffcb, stop:1 #4dc6a0);"
        "   border-radius: 15px;"
        "   color: black;"
        "   font: bold 14pt 'Microsoft YaHei';"
        "   height: 36px;"
        "}"
        "QPushButton:hover { background: #2da888; }"
        "QPushButton:pressed { background: #1f8a70; }"
        );

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setSpacing(20);
    layout->setContentsMargins(30, 30, 30, 30);

    QLabel *lblTitle = new QLabel("GAME OVER", &dlg);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setStyleSheet("font-size: 24pt; font-weight: bold; color: #7cffcb;");
    layout->addWidget(lblTitle);

    QLabel *lblScore = new QLabel(QString("本局得分: %1").arg(m_score), &dlg);
    lblScore->setAlignment(Qt::AlignCenter);
    lblScore->setStyleSheet("font-size: 18pt;");
    layout->addWidget(lblScore);

    QPushButton *btnOk = new QPushButton("返回菜单", &dlg);
    connect(btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);
    layout->addWidget(btnOk);

    dlg.exec();
    emit gameFinished();
}
