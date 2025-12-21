// skilltreepage.h
#ifndef SKILLTREEPAGE_H
#define SKILLTREEPAGE_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScrollArea>
#include "skilltree.h"

namespace Ui {
class SkillTreePage;
}

class SkillTreePage : public QWidget {
    Q_OBJECT

public:
    explicit SkillTreePage(SkillTree* skillTree, QWidget *parent = nullptr);
    ~SkillTreePage();

    void refreshUI();

signals:
    void backRequested();

private slots:
    void onSkillButtonClicked();
    void onEquipButtonClicked();
    void onUnequipButtonClicked();
    void onBackButtonClicked();

private:
    class SkillTreeContainer : public QWidget {
    public:
        SkillTreeContainer(QWidget* parent = nullptr) : QWidget(parent) {}
    protected:
        void paintEvent(QPaintEvent* event) override;
    };

    Ui::SkillTreePage *ui;
    SkillTree* skillTree;
    QMap<QString, QPushButton*> skillButtons;
    QMap<QString, QLabel*> costLabels;
    QMap<QString, QPushButton*> equipButtons;
    QMap<QString, QPushButton*> unequipButtons;
    QMap<QString, QPoint> skillPositions; // 记录技能位置用于连线
    SkillTreeContainer* skillTreeContainer; // 技能树容器

    void setupSkillTreeUI();
    void updateSkillButtonState(const QString& skillId);

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SKILLTREEPAGE_H
