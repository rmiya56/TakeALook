#ifndef TOGGLEACTION_H
#define TOGGLEACTION_H

#include <QAction>



class ToggleAction : public QAction
{
public:
    ToggleAction(const QIcon &normal, const QIcon &active, const QString &text, QObject *parent);
    void activate();
    void deactivate();

private:
    QIcon iconNormal;
    QIcon iconActive;

};

#endif // TOGGLEACTION_H
