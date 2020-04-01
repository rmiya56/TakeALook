#ifndef MODESWITCHACTION_H
#define MODESWITCHACTION_H

#include <QAction>



class ModeSwitchAction : public QAction
{
public:
    ModeSwitchAction(const QIcon &icon, const QIcon &icon_toggled, const QString &text, QObject *parent);
private:
    QIcon iconNormal;
    QIcon iconChecked;


private slots:
    void switch_state(bool checked);
    //void switch_state2();

};

#endif // MODESWITCHACTION_H
