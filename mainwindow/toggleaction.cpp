#include "toggleaction.h"
#include <QDebug>



ToggleAction::ToggleAction(const QIcon &normal, const QIcon &active, const QString &text, QObject *parent)
    : QAction(normal, text, parent)
{
    iconNormal = normal;
    iconActive = active;
    deactivate();
}

void ToggleAction::activate()
{
    setIcon(iconActive);
}

void ToggleAction::deactivate()
{
    setIcon(iconNormal);
}
