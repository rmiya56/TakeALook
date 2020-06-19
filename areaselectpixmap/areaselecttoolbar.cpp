#include "areaselecttoolbar.h"

AreaSelectToolbar::AreaSelectToolbar()
{
//    actionAreaSelectionMode = new ToggleAction( QIcon(Icons::area),
//                                                QIcon(Icons::area_toggled),
//                                                tr("Area Select"), this);
//    connect(actionAreaSelectionMode, &QAction::triggered, this, &MainWindow::enter_area_select_mode);
//    ui->toolBar->addAction(actionAreaSelectionMode);



    pixmapItem = new AreaSelectPixmapItem();
}
