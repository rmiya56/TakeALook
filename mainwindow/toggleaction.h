#ifndef TOGGLEACTION_H
#define TOGGLEACTION_H

#include <QAction>



class ToggleAction : public QAction
{
public:
    ToggleAction(const QIcon &icon, const QIcon &icon_toggled, const QString &text, QObject *parent);
private:
    QIcon iconNormal;
    QIcon iconChecked;


private slots:
    void toggle_switch(bool checked);

};

#endif // TOGGLEACTION_H
