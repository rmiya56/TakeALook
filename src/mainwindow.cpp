#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "icons.h"
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QLabel>
#include <QClipboard>
#include <QFileDialog>
#include <QDebug>
#include <QSettings>
#include <QtMath>


MainWindow::MainWindow(QWidget *parent, const char* filepath)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      statusbarLeft(new QLabel()),
      statusbarRight(new QLabel())
{
    ui->setupUi(this);
    setAcceptDrops(true);
    view = ui->graphicsView;
    view->setScene(&scene);
    view->setAcceptDrops(false);
    qApp->installEventFilter(this);

    setupStatusBar();
    setupToolBar();
    setupOptionToolBar();


    if (filepath)
    {
        view->viewport()->resize(size()-QSize(46,23)); // ???
        QFileInfo info(filepath);
        imgHandler.loadImage(info);
        displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());
    }

    connect(&scene, SIGNAL(done_selection(bool)), this, SLOT(on_action_pointer_mode_toggled(bool)));
    connect(&scene, SIGNAL(zoom_in_area(QRect)), this, SLOT(fit_to_rect(QRect)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupOptionToolBar()
{
    actionCanvasMode = new ModeSwitchAction(QIcon(":/icons/white/pen [#1319].png"), QIcon(":/icons/green/pen [#1319].png"), tr("Canvas"), this);
    connect(actionCanvasMode, SIGNAL(toggled(bool)), this, SLOT(on_action_canvas_mode_toggled(bool)));
    ui->optionToolBar->addAction(actionCanvasMode);
}

void MainWindow::setupToolBar()
{

    actionPointerMode = new ModeSwitchAction(QIcon(":/icons/white/mouse_pointer [#6].png"), QIcon(":/icons/green/mouse_pointer [#6].png"), tr("Pointer"), this);
    connect(actionPointerMode, SIGNAL(toggled(bool)), this, SLOT(on_action_pointer_mode_toggled(bool)));
    ui->toolBar->addAction(actionPointerMode);

    actionAreaSelectionMode = new ModeSwitchAction(QIcon(":/icons/white/focus_plus_round [#895].png"), QIcon(":/icons/green/focus_plus_round [#895].png"),tr("Area Select"), this);
    connect(actionAreaSelectionMode, SIGNAL(toggled(bool)), this, SLOT(on_action_area_selection_mode_toggled(bool)));
    ui->toolBar->addAction(actionAreaSelectionMode);

    actionPointerMode->setChecked(true);
    actionAreaSelectionMode->setChecked(false);

    actionFitToWindow = new QAction(QIcon(":/icons/white/arrow_all_fill [#383].png"), tr("Fit to Window"), this);
    connect(actionFitToWindow, SIGNAL(triggered()), this, SLOT(on_action_fit_to_window_triggered()));
    ui->toolBar->addAction(actionFitToWindow);

    actionNextImage = new QAction(QIcon(":/icons/white/arrow_right [#336].png"), tr("Next"), this);
    connect(actionNextImage, SIGNAL(triggered()), this, SLOT(on_action_next_image_triggered()));
    ui->toolBar->addAction(actionNextImage);

    actionPrevImage = new QAction(QIcon(":/icons/white/arrow_left [#335].png"), tr("Prev"), this);
    connect(actionPrevImage, SIGNAL(triggered()), this, SLOT(on_action_fit_prev_image_triggered()));
    ui->toolBar->addAction(actionPrevImage);

    actionOpenImage = new QAction(QIcon(":/icons/white/folder [#1792].png"), tr("Open Image"), this);
    connect(actionOpenImage, SIGNAL(triggered()), this, SLOT(on_action_open_image_triggered()));
    ui->toolBar->addAction(actionOpenImage);

    actionSaveImage = new QAction(QIcon(":/icons/white/save_item [#1411].png"), tr("Save Image"), this);
    connect(actionSaveImage, SIGNAL(triggered()), this, SLOT(on_action_save_image_triggered()));
    ui->toolBar->addAction(actionSaveImage);

    actionBaloonTip = new QAction(QIcon(":/icons/white/message [#1576].png"), tr("BaloonTip"), this);
    actionBaloonTip->setCheckable(true);
    connect(actionBaloonTip , SIGNAL(toggled(bool)), this, SLOT(on_action_baloontip_toggled(bool)));
    ui->toolBar->addAction(actionBaloonTip);
    actionBaloonTip->setChecked(false);

}

void MainWindow::setupStatusBar()
{
    statusbarRight->setFont(QFont("Courier"));
    statusbarRight->setStyleSheet("color:white");
    statusbarLeft->setFont(QFont("Courier"));
    statusbarLeft->setStyleSheet("color:white");
    ui->statusbar->addWidget(statusbarLeft, 1);
    ui->statusbar->addWidget(statusbarRight, 0);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    fit_to_rect(scene.pixmapRect());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls() && event->mimeData()->urls().count()==1)
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QFileInfo file_info(event->mimeData()->urls().at(0).toLocalFile());
    imgHandler.loadImage(file_info);
    displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());
}

void MainWindow::displayImage(QImage qimage, QString file_path)
{
    scene.setImage(qimage);
    fit_to_rect(scene.pixmapRect());
    QString image_property = QString("[%1x%2]").arg(QString::number(imgHandler.currentImage().width()), QString::number(imgHandler.currentImage().height()));
    statusbarLeft->setText(file_path + " " + image_property);
}

void MainWindow::_mouseMoveEvent(QMouseEvent *event)
{
    QString pix_location, pix_color;
    ImageProperties prop = imgHandler.currentProperties(); // reading almost static all the time..

    QPointF pos = view->mapToScene(event->pos());
    QPoint pix(qFloor(pos.x()), qFloor(pos.y()));
    QString x = QString::number(pix.x()).rightJustified(prop.digitsX, ' ');
    QString y = QString::number(pix.y()).rightJustified(prop.digitsY, ' ');
    pix_location = QString("[%1, %2]").arg(x, y);

    QImage qImg;
    if (!scene.pixmap().isNull()) qImg = scene.pixmap().toImage();

    QPoint offset = scene.areaRect().topLeft();
    pix = pix - offset;
    QString r, g, b;
    if (qImg.valid(pix))
    {
        QColor c = qImg.pixel(pix);

        r = QString::number((c.red()-prop.beta)/prop.alpha).rightJustified(prop.digitsD, ' ');
        g = QString::number((c.green()-prop.beta)/prop.alpha).rightJustified(prop.digitsD, ' ');
        b = QString::number((c.blue()-prop.beta)/prop.alpha).rightJustified(prop.digitsD, ' ');
        if (prop.channels == 3)         pix_color = QString("(%1,%2,%3)").arg(r,g,b);
        else if (prop.channels == 1)    pix_color = QString("(%1)").arg(r);
    }

    statusbarRight->setText(pix_color + " " + pix_location);

    if (scene.baloonTip) scene.baloonTip->setPixProperties(pos, r, g, b);
}

bool MainWindow::_keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Left:
            showPrev();
            break;

        case Qt::Key_Right:
            showNext();
            break;

        case Qt::Key_C:
            if (event->modifiers() & (Qt::ControlModifier))
            {
                QClipboard *clipboard = QApplication::clipboard();
                clipboard->setImage(imgHandler.currentImage());
            }
            break;

        default:
            qDebug("keypress %x", event->key());
            break;
     }
     return true;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)

    if(event->type() == QEvent::KeyPress)
    {
        return _keyPressEvent(static_cast<QKeyEvent*>(event));
    }
    else if(event->type() == QEvent::MouseMove)
    {
        _mouseMoveEvent(static_cast<QMouseEvent*>(event));
    }
    return false;
}

void MainWindow::showNext()
{
    imgHandler.loadNext();
    displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());
}

void MainWindow::showPrev()
{
    imgHandler.loadPrev();
    displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());
}

void MainWindow::openFile()
{
    QString dir, filter;

    QSettings read("settings.ini", QSettings::IniFormat, this);
    read.beginGroup("ImageFiles");
    if(read.contains("dir")) dir = read.value("dir", "").toString();
    if(read.contains("open_filter"))  filter = read.value("filter", tr("All(*.*)")).toString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                   dir,
                                                   tr("All(*.*);;PNG(*.png);;JPG(*.jpg);;BMP(*.bmp);;TIFF(*.tif*)"),
                                                   &filter,
                                                   QFileDialog::DontUseCustomDirectoryIcons);
    if (fileName.isEmpty()) return;

    QFileInfo file_info(fileName);
    imgHandler.loadImage(file_info);
    displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());

    QSettings settings("settings.ini", QSettings::IniFormat, this);
    settings.beginGroup("ImageFiles");
    settings.setValue("dir", file_info.absoluteDir().absolutePath());
    settings.setValue("filter", filter);
    settings.endGroup();

}

void MainWindow::saveFile()
{
    QString dir, filter;

    QSettings read("settings.ini", QSettings::IniFormat, this);
    read.beginGroup("ImageFiles");
    if(read.contains("save_filter"))  filter = read.value("filter", tr("All(*.*)")).toString();

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    imgHandler.currentFilePath(),
                                                    tr("All(*.*);;PNG(*.png);;JPG(*.jpg);;BMP(*.bmp);;TIFF(*.tif*)"),
                                                    &filter,
                                                    QFileDialog::DontUseCustomDirectoryIcons);

    if (fileName.isEmpty()) return;

    QFileInfo file_info(fileName);
    imgHandler.writeToFile(file_info, scene.areaRect());

    QSettings settings("settings.ini", QSettings::IniFormat, this);
    settings.beginGroup("ImageFiles");
    settings.setValue("filter", filter);
    settings.endGroup();
}

void MainWindow::fit_to_rect(QRect rect)
{
    if (rect.isNull()) return;
    view->setSceneRect(scene.itemsBoundingRect()); // shrink viewport
    view->fitInView(rect, Qt::KeepAspectRatio);
    qDebug() << view->viewport()->size();
}


void MainWindow::on_action_pointer_mode_toggled(bool toggled)
{
    if (toggled)
    {
        actionPointerMode->setChecked(true);
        ui->graphicsView->mouse_control = true;
        scene.area_selection_is_active = false;
        on_action_area_selection_mode_toggled(false);
        setCursor(Qt::ArrowCursor);
    }
    else
    {
        actionPointerMode->setChecked(false);
    }
}

void MainWindow::on_action_area_selection_mode_toggled(bool toggled)
{
    if (toggled)
    {
        actionAreaSelectionMode->setChecked(true);
        ui->graphicsView->mouse_control = false;
        scene.area_selection_is_active = true;
        on_action_pointer_mode_toggled(false);
        setCursor(Qt::CrossCursor);
    }
    else
    {
        actionAreaSelectionMode->setChecked(false);
    }
}

void MainWindow::on_action_canvas_mode_toggled(bool toggled)
{
     if (toggled)
    {
        actionCanvasMode->setChecked(true);
        overlayItem = new OverlayPixmapItem(imgHandler.currentImage().size());
        scene.addItem(overlayItem);
    }
    else
    {
        actionCanvasMode->setChecked(false);
        scene.removeItem(overlayItem);
    }
}

void MainWindow::on_action_fit_to_window_triggered()
{
    fit_to_rect(scene.pixmapRect());
}

void MainWindow::on_action_next_image_triggered()
{
    showNext();
}

void MainWindow::on_action_prev_image_triggered()
{
    showPrev();
}

void MainWindow::on_action_open_image_triggered()
{
    openFile();
}

void MainWindow::on_action_save_image_triggered()
{
    saveFile();
}

void MainWindow::on_action_baloontip_toggled(bool toggled)
{
    if (toggled)
    {
        scene.addItem(scene.baloonTip);
        actionBaloonTip->setIcon(QIcon(":/icons/green/message [#1576].png"));
    }
    else
    {
        scene.removeItem(scene.baloonTip);
        actionBaloonTip->setIcon(QIcon(":/icons/white/message [#1576].png"));
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if (actionPointerMode->isChecked())
    {
        on_action_area_selection_mode_toggled(true);
        qDebug() << "AREA_SELECT_MODE";
    }
    else
    {
        on_action_pointer_mode_toggled(true);
        qDebug() << "NORMAL_MODE";
    }
}


