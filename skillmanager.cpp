#include "skillmanager.h"
#include <QRandomGenerator>
#include <QDebug>

SkillManager::SkillManager(GameBoard* board, SkillTree* skillTree, QObject* parent)
    : QObject(parent), m_board(board), m_skillTree(skillTree)
{
    m_skillTimer = new QTimer(this);
    m_skillTimer->setSingleShot(true);
}

bool SkillManager::executeSkill(const QString& skillId, int& scoreToAdd, int& timeToAdd)
{
    // 检查技能是否可用
    SkillNode* skill = m_skillTree->getSkill(skillId);
    if (!skill || !skill->equipped || skill->used) {
        return false;
    }

    // 执行技能
    if (skillId == "row_clear") {
        rowClear(scoreToAdd);
    } else if (skillId == "time_extend") {
        timeExtend(timeToAdd);
    } else if (skillId == "rainbow_bomb") {
        rainbowBomb(scoreToAdd);
    } else if (skillId == "cross_clear") {
        crossClear(scoreToAdd);
    } else if (skillId == "score_double") {
        // 需要持续效果，这里先标记激活
        m_scoreDoubleActive = true;
        scoreDouble(timeToAdd);
    } else if (skillId == "color_unify") {
        m_colorUnifyActive = true;
        colorUnify(timeToAdd);
    } else if (skillId == "time_freeze") {
        timeFreeze(timeToAdd);
    } else if (skillId == "ultimate_burst") {
        ultimateBurst(scoreToAdd);
    } else {
        return false;
    }

    // 标记技能已使用
    skill->used = true;
    return true;
}

void SkillManager::resetSkillUsage()
{
    const auto& skills = m_skillTree->getAllSkills();
    for (SkillNode* skill : skills) {
        skill->used = false;
    }
    m_scoreDoubleActive = false;
    m_colorUnifyActive = false;
}

bool SkillManager::canUseSkill(const QString& skillId) const
{
    SkillNode* skill = m_skillTree->getSkill(skillId);
    return skill && skill->equipped && !skill->used;
}

// ================= 具体技能实现 =================

void SkillManager::rowClear(int& scoreToAdd)
{
    int row = getRandomRow();
    int count = 0;

    for (int c = 0; c < COL; ++c) {
        if (m_board->m_grid[row][c].pic != -1) {
            m_board->m_grid[row][c].pic = -1;
            count++;
        }
    }

    scoreToAdd = count * 50;
    qDebug() << "行消除：第" << row << "行，消除" << count << "个方块";
}

void SkillManager::timeExtend(int& timeToAdd)
{
    timeToAdd = 5;
    qDebug() << "时间延长：增加5秒";
}

void SkillManager::rainbowBomb(int& scoreToAdd)
{
    int targetColor = getRandomColor();
    int count = 0;

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            if (m_board->m_grid[r][c].pic == targetColor) {
                m_board->m_grid[r][c].pic = -1;
                count++;
            }
        }
    }

    scoreToAdd = count * 50;
    qDebug() << "彩虹炸弹：消除颜色" << targetColor << "，共" << count << "个方块";
}

void SkillManager::crossClear(int& scoreToAdd)
{
    int centerR = getRandomRow();
    int centerC = getRandomCol();
    int count = 0;

    // 消除十字
    for (int r = 0; r < ROW; ++r) {
        if (m_board->m_grid[r][centerC].pic != -1) {
            m_board->m_grid[r][centerC].pic = -1;
            count++;
        }
    }

    for (int c = 0; c < COL; ++c) {
        if (m_board->m_grid[centerR][c].pic != -1) {
            m_board->m_grid[centerR][c].pic = -1;
            count++;
        }
    }

    scoreToAdd = count * 50;
    qDebug() << "十字消除：中心点(" << centerR << "," << centerC << ")，消除" << count << "个方块";
}

void SkillManager::scoreDouble(int& timeToAdd)
{
    timeToAdd = 8; // 持续8秒
    qDebug() << "得分翻倍：持续8秒";
}

void SkillManager::colorUnify(int& timeToAdd)
{
    timeToAdd = 6; // 持续6秒
    qDebug() << "颜色统一：持续6秒";
}

void SkillManager::timeFreeze(int& timeToAdd)
{
    timeToAdd = 15;
    qDebug() << "时间冻结：增加15秒";
}

void SkillManager::ultimateBurst(int& scoreToAdd)
{
    int count = 0;

    for (int r = 0; r < ROW; ++r) {
        for (int c = 0; c < COL; ++c) {
            if (m_board->m_grid[r][c].pic != -1) {
                m_board->m_grid[r][c].pic = -1;
                count++;
            }
        }
    }

    scoreToAdd = 3200; // 固定加分
    qDebug() << "终极爆发：消除所有方块，获得" << scoreToAdd << "分";
}

// ================= 辅助函数 =================

int SkillManager::getRandomRow() const
{
    return QRandomGenerator::global()->bounded(ROW);
}

int SkillManager::getRandomCol() const
{
    return QRandomGenerator::global()->bounded(COL);
}

int SkillManager::getRandomColor() const
{
    return QRandomGenerator::global()->bounded(6); // 0-5
}
