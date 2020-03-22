#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
//#include <QGraphicsPixmapItem>
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
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    view = ui->graphicsView;
    view->setScene(&scene);
    view->setAcceptDrops(false);
    qApp->installEventFilter(this);

    setupStatusBar();
    setupToolBar();

    if (filepath)
    {
        view->viewport()->resize(size()-QSize(46,23)); // ???
        QFileInfo info(filepath);
        imgHandler.loadImage(info);
        displayImage(imgHandler.currentImage(), imgHandler.currentFilePath());
    }

    connect(&scene, SIGNAL(done_selection()), this, SLOT(on_action_switch_to_normal_mode_triggered()));
    connect(&scene, SIGNAL(zoom_in_area(QRect)), this, SLOT(fit_to_rect(QRect)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupToolBar()
{
    actionPointerMode = new QAction(QIcon(":/icons/mouse_pointer [#6].png"), tr("Pointer"), this);
    actionPointerMode->setChecked(true);
    connect(actionPointerMode, SIGNAL(triggered()), this, SLOT(on_action_pointer_mode_triggered()));
    ui->toolBar->addAction(actionPointerMode);

    actionAreaSelectionMode = new QAction(QIcon(":/icons/focus_plus_round [#895].png"), tr("Area Select"), this);
    connect(actionAreaSelectionMode, SIGNAL(triggered()), this, SLOT(on_action_area_selection_mode_triggered()));
    ui->toolBar->addAction(actionAreaSelectionMode);

    actionFitToWindow = new QAction(QIcon(":/icons/arrow_all_fill [#383].png"), tr("Fit to Window"), this);
    connect(actionFitToWindow, SIGNAL(triggered()), this, SLOT(on_action_fit_to_window_triggered()));
    ui->toolBar->addAction(actionFitToWindow);

    actionNextImage = new QAction(QIcon(":/icons/arrow_right [#336].png"), tr("Next"), this);
    connect(actionNextImage, SIGNAL(triggered()), this, SLOT(on_action_next_image_triggered()));
    ui->toolBar->addAction(actionNextImage);

    actionPrevImage = new QAction(QIcon(":/icons/arrow_left [#335].png"), tr("Prev"), this);
    connect(actionPrevImage, SIGNAL(triggered()), this, SLOT(on_action_fit_prev_image_triggered()));
    ui->toolBar->addAction(actionPrevImage);

    actionOpenImage = new QAction(QIcon(":/icons/folder [#1792].png"), tr("Open Image"), this);
    connect(actionOpenImage, SIGNAL(triggered()), this, SLOT(on_action_open_image_triggered()));
    ui->toolBar->addAction(actionOpenImage);

    actionSaveImage = new QAction(QIcon(":/icons/save_item [#1411].png"), tr("Save Image"), this);
    connect(actionSaveImage, SIGNAL(triggered()), this, SLOT(on_action_save_image_triggered()));
    ui->toolBar->addAction(actionSaveImage);

}

void MainWindow::setupStatusBar()
{
    statusbar_left = new QLabel;
    statusbar_right = new QLabel;
    statusbar_right->setFont(QFont("Courier"));
    statusbar_right->setStyleSheet("color:white");
    statusbar_left->setFont(QFont("Courier"));
    statusbar_left->setStyleSheet("color:white");
    ui->statusbar->addWidget(statusbar_left, 1);
    ui->statusbar->addWidget(statusbar_right, 0);
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
    original_pixmap.convertFromImage(qimage);
    scene.pixmap_item = new QGraphicsPixmapItem(original_pixmap);
    scene.clear();
    scene.addItem(scene.pixmap_item);
    fit_to_rect(scene.pixmapRect());

    QString image_property = QString("[%1x%2]").arg(QString::number(imgHandler.currentImage().width()), QString::number(imgHandler.currentImage().height()));
    statusbar_left->setText(file_path + " " + image_property);
}

void MainWindow::_mouseMoveEvent(QMouseEvent *event)
{
    QString pix_location, pix_color;
    ImageProperties prop = imgHandler.currentProperties(); // reading almost static all the time..

    QPointF pos = view->mapToScene(event->pos());
    QPoint pix(qFloor(pos.x()), qFloor(pos.y()));
    QString x = QString::number(pix.x()).rightJustified(prop.digits_x, ' ');
    QString y = QString::number(pix.y()).rightJustified(prop.digits_y, ' ');
    pix_location = QString("[%1, %2]").arg(x, y);

    QImage qImg;
    if (!scene.pixmap().isNull()) qImg = scene.pixmap().toImage();

    QPoint offset = scene.areaRect().topLeft();
    pix = pix - offset;
    if (qImg.valid(pix))
    {
        QColor c = qImg.pixel(pix);
        QString r = QString::number(c.red()).rightJustified(3, ' ');
        QString g = QString::number(c.green()).rightJustified(3, ' ');
        QString b = QString::number(c.blue()).rightJustified(3, ' ');
        if (prop.channels == 3)         pix_color = QString("(%1,%2,%3)").arg(r,g,b);
        else if (prop.channels == 1)    pix_color = QString("(%1)").arg(r);
    }

    statusbar_right->setText(pix_color + " " + pix_location);
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
    read.beginGroup("OpenFile");
    if(read.contains("dir")) dir = read.value("dir", "").toString();
    if(read.contains("filter"))  filter = read.value("filter", tr("All(*.*)")).toString();

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
    settings.beginGroup("OpenFile");
    settings.setValue("dir", file_info.absoluteDir().absolutePath());
    settings.setValue("filter", filter);
    settings.endGroup();

}

void MainWindow::saveFile()
{
   imgHandler.writeToFile(scene.areaRect());
}

void MainWindow::fit_to_rect(QRect rect)
{
    if (rect.isNull()) return;
    view->setSceneRect(scene.itemsBoundingRect()); // shrink viewport
    view->fitInView(rect, Qt::KeepAspectRatio);
    qDebug() << view->viewport()->size();
}


void MainWindow::on_action_pointer_mode_triggered()
{
    ui->graphicsView->mouse_control = true;
    scene.area_selection_is_active = false;
    setCursor(Qt::ArrowCursor);
    is_normal_mode = true;
    //actionPointerMode->setIcon(QIcon(":/icons/focus_plus_round [#895].png"));
    //actionAreaSelectionMode->setIcon();
}

void MainWindow::on_action_area_selection_mode_triggered()
{
    ui->graphicsView->mouse_control = false;
    scene.area_selection_is_active = true;
    setCursor(Qt::CrossCursor);
    is_normal_mode = false;

    actionPointerMode->setIcon(QIcon(":/icons/mouse_pointer [#6].png"));
    //actionAreaSelectionMode->setIcon();
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

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)

    if(is_normal_mode)
    {
        on_action_area_selection_mode_triggered();
        qDebug() << "AREA_SELECT_MODE";
    }
    else
    {
        on_action_pointer_mode_triggered();
        qDebug() << "NORMAL_MODE";
    }
}


