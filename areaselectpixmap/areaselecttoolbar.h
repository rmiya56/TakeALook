#ifndef AREASELECTTOOLBAR_H
#define AREASELECTTOOLBAR_H

#include "areaselectpixmapitem.h"
#include "takealook/mainwindow/toggleaction.h"



class AreaSelectToolbar
{
public:
    AreaSelectToolbar();

private:
    AreaSelectPixmapItem *pixmapItem;
    ToggleAction *actionAreaSelectionMode;

};

#endif // AREASELECTTOOLBAR_H
