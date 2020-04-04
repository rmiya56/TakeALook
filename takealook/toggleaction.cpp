#include "toggleaction.h"
#include <QDebug>


ToggleAction::ToggleAction(const QIcon &icon, const QIcon &icon_toggled, const QString &text, QObject *parent)
    : QAction(icon, text, parent)
{
    iconNormal = icon;
    iconChecked = icon_toggled;
    setCheckable(true);
    setChecked(false);

    connect(this, &ToggleAction::toggled, this, &ToggleAction::toggle_switch);
}

void ToggleAction::toggle_switch(bool checked)
{
    if (checked)
    {
        setIcon(iconChecked);
    }
    else
    {
        setIcon(iconNormal);
    }
}
