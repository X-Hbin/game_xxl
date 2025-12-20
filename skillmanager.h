// skillmanager.h
#ifndef SKILLMANAGER_H
#define SKILLMANAGER_H

#include <QObject>
#include <QTimer>
#include "gameboard.h"
#include "skilltree.h"

class SkillManager : public QObject
{
    Q_OBJECT
public:
    explicit SkillManager(GameBoard* board, SkillTree* skillTree, QObject* parent = nullptr);

    // 执行技能
    bool executeSkill(const QString& skillId, int& scoreToAdd, int& timeToAdd);

    // 重置技能使用状态
    void resetSkillUsage();

    // 检查技能是否可用
    bool canUseSkill(const QString& skillId) const;

private:
    // 具体技能实现
    void rowClear(int& scoreToAdd);
    void timeExtend(int& timeToAdd);
    void rainbowBomb(int& scoreToAdd);
    void crossClear(int& scoreToAdd);
    void scoreDouble(int& timeToAdd); // 持续时间用timer实现
    void colorUnify(int& timeToAdd);  // 持续时间用timer实现
    void timeFreeze(int& timeToAdd);
    void ultimateBurst(int& scoreToAdd);

    // 辅助函数
    int getRandomRow() const;
    int getRandomCol() const;
    int getRandomColor() const;

private:
    GameBoard* m_board;
    SkillTree* m_skillTree;

    // 状态变量
    bool m_scoreDoubleActive = false;
    bool m_colorUnifyActive = false;
    QTimer* m_skillTimer;
};

#endif // SKILLMANAGER_H
