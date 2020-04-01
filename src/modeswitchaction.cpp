#include "modeswitchaction.h"
#include <QDebug>


ModeSwitchAction::ModeSwitchAction(const QIcon &icon, const QIcon &icon_toggled, const QString &text, QObject *parent)
    : QAction(icon, text, parent)
{
    iconNormal = icon;
    iconChecked = icon_toggled;
    setCheckable(true);
    setChecked(false);

    connect(this, &ModeSwitchAction::toggled, this, &ModeSwitchAction::switch_state);
    //connect(this, &ModeSwitchAction::changed, this, &ModeSwitchAction::switch_state2);
}

void ModeSwitchAction::switch_state(bool checked)
{
    if (checked)
    {
        setIcon(iconChecked);
        //setChecked(true);
    }
    else
    {
        setIcon(iconNormal);
        //setChecked(false);
    }

}

//void ModeSwitchAction::switch_state2()
//{
//    if (iconChecked == nullptr) return;

//    if(isChecked())
//    {
//        setIcon(*iconChecked);
//    }
//    else
//    {
//        setIcon(*iconNormal);

//    }

//}
