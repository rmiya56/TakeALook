#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scene.h"
#include "../image/imagehandler.h"
#include "../utility/icons.h"
#include "../baloontip/pixbaloontip.h"
#include <QKeyEvent>
#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QLabel>
#include <QClipboard>
#include <QDebug>



MainWindow::MainWindow(QWidget *parent, const char* filepath)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      imageHandler(new ImageHandler(this)),
      scene(new Scene(imageHandler)),
      statusbarLeft(new QLabel()),
      statusbarRight(new QLabel())
{

    ui->setupUi(this);
    setAcceptDrops(true);
    view = ui->graphicsView;
    view->setScene(scene);
    view->setAcceptDrops(false);
    qApp->installEventFilter(this);

    setupStatusBar();
    setupToolBar();
    setupFileToolBar();

    if (filepath)
    {
        view->viewport()->resize(size()-QSize(46,23)); // ???
        QFileInfo info(filepath);
        imageHandler->loadFile(info);
        displayImage(imageHandler->currentImage(), imageHandler->currentFilePath());
    }

    //connect(scene, SIGNAL(done_selection(bool)), this, SLOT(on_action_pointer_mode_toggled(bool)));
    connect(scene, SIGNAL(zoom_in_area(QRect)), this, SLOT(fit_to_rect(QRect)));

    // pointer
    mode.append(new QState);
    enter_events.append(&MainWindow::enter_pointer_mode);
    exit_events.append(&MainWindow::exit_pointer_mode);
    enter_signals.append(&MainWindow::pointer_mode);

    // area select
    mode.append(new QState);
    enter_events.append(&MainWindow::enter_area_select_mode);
    exit_events.append(&MainWindow::exit_area_select_mode);
    enter_signals.append(&MainWindow::area_select_mode);
}

void MainWindow::initStateMachine()
{
    for (int i=0; i<mode.size(); i++)
    {
        for (int j=0; j<mode.size(); j++)
            if (j!=i) mode[j]->addTransition(this, enter_signals[i], mode[i]);

        mode[i]->addTransition(this, &MainWindow::double_clicked, mode[(i+1)%mode.size()]);
        connect(mode[i],  &QState::entered, this, enter_events[i]);
        connect(mode[i],  &QState::exited, this, exit_events[i]);
        machine.addState(mode[i]);
    }
    machine.setInitialState(mode[0]);
    machine.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupToolBar()
{

    actionPointerMode = new ToggleAction(	QIcon(Icons::pointer),
                                            QIcon(Icons::pointer_toggled),
                                            tr("Pointer"), this);
    connect(actionPointerMode, &QAction::triggered, this, &MainWindow::enter_pointer_mode);
    ui->toolBar->addAction(actionPointerMode);

    actionAreaSelectionMode = new ToggleAction( QIcon(Icons::area),
                                                QIcon(Icons::area_toggled),
                                                tr("Area Select"), this);
    connect(actionAreaSelectionMode, &QAction::triggered, this, &MainWindow::enter_area_select_mode);
    ui->toolBar->addAction(actionAreaSelectionMode);

    actionFitToWindow = new QAction(	QIcon(":/icons/white/arrow_all_fill [#383].png"),
                                        tr("Fit to Window"), this);
    connect(actionFitToWindow, SIGNAL(triggered()), this, SLOT(on_action_fit_to_window_triggered()));
    ui->toolBar->addAction(actionFitToWindow);

    actionBaloonTip = new ToggleAction(	QIcon(":/icons/white/message [#1576].png"),
                                        QIcon(":/icons/green/message [#1576].png"),
                                        tr("BaloonTip"), this);
    connect(actionBaloonTip , SIGNAL(toggled(bool)), this, SLOT(on_action_baloontip_toggled(bool)));
    ui->toolBar->addAction(actionBaloonTip);
    baloonTip = new PixBaloonTip();

}

void MainWindow::setupFileToolBar()
{
    actionNextImage = new QAction(QIcon(Icons::next), tr("Next"), this);
    connect(actionNextImage, &QAction::triggered, this, &MainWindow::on_action_next_image_triggered);
    ui->toolBar->addAction(actionNextImage);

    actionPrevImage = new QAction(QIcon(Icons::prev), tr("Prev"), this);
    connect(actionPrevImage, &QAction::triggered, this, &MainWindow::on_action_prev_image_triggered);
    ui->toolBar->addAction(actionPrevImage);

    actionOpenImage = new QAction(QIcon(Icons::folder), tr("Open Image"), this);
    connect(actionOpenImage, &QAction::triggered, this, &MainWindow::on_action_open_image_triggered);
    ui->toolBar->addAction(actionOpenImage);

    actionSaveImage = new QAction(QIcon(Icons::save), tr("Save Image"), this);
    connect(actionSaveImage, &QAction::triggered, this, &MainWindow::on_action_save_image_triggered);
    ui->toolBar->addAction(actionSaveImage);
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
    fit_to_rect(scene->pixmapRect());
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls() && event->mimeData()->urls().count()==1)
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QFileInfo file_info(event->mimeData()->urls().at(0).toLocalFile());
    imageHandler->loadFile(file_info);
    displayImage(imageHandler->currentImage(), imageHandler->currentFilePath());
}

void MainWindow::displayImage(QImage qimage, QString file_path)
{
    scene->setImage(qimage);
    fit_to_rect(scene->pixmapRect());
    QString image_property = QString("[%1x%2]").arg(QString::number(imageHandler->currentImage().width()), QString::number(imageHandler->currentImage().height()));
    statusbarLeft->setText(file_path + " " + image_property);
}

bool MainWindow::_mouseMoveEvent(QMouseEvent *event)
{
    ImageProperties prop = imageHandler->currentProperties(); // reading almost static all the time..
    QPointF pos = view->mapToScene(event->pos());
    QString pix_location = prop.formatPixelLocation(pos);

    QImage qImg;
    if (!scene->pixmap().isNull()) qImg = scene->pixmap().toImage();
    QPoint offset = scene->areaRect().topLeft();
    QStringList color = prop.formatPixelColor(qImg, pos, offset);

    QString pix_color;
    if (color.isEmpty())
        pix_color = "";
    else if (color.size() == 1)
        pix_color = QString("(%1)").arg(color[0]);
    else if (color.size() == 3)
        pix_color = QString("(%1,%2,%3)").arg(color[0], color[1], color[2]);

    statusbarRight->setText(pix_color + " " + pix_location);
    baloonTip->setPixProperties(pos, color);

    return false;
}

bool MainWindow::_keyPressEvent(QKeyEvent *event)
{

    switch(event->key())
    {
        case Qt::Key_Left:
            showPrev();
            image_changed();
            break;

        case Qt::Key_Right:
            showNext();
            image_changed();
            break;

        case Qt::Key_C:
            if (event->modifiers() & (Qt::ControlModifier))
            {
                QClipboard *clipboard = QApplication::clipboard();
                clipboard->setImage(imageHandler->currentImage());
            }
            break;

        default:
            qDebug("keypress %x", event->key());
            break;
     }
    return false;
}


bool MainWindow::eventFilter(QObject *object, QEvent *event)
{

    if(event->type() == QEvent::KeyPress)
    {
        return _keyPressEvent(static_cast<QKeyEvent*>(event));
    }
    else if(event->type() == QEvent::MouseMove)
    {
        return _mouseMoveEvent(static_cast<QMouseEvent*>(event));
    }
    else if(event->type() == QEvent::MouseButtonDblClick)
    {
        double_clicked();
        return true;
    }
    return false;

    Q_UNUSED(object)
}

void MainWindow::showNext()
{
    imageHandler->loadNext();
    displayImage(imageHandler->currentImage(), imageHandler->currentFilePath());
}

void MainWindow::showPrev()
{
    imageHandler->loadPrev();
    displayImage(imageHandler->currentImage(), imageHandler->currentFilePath());
}

void MainWindow::fit_to_rect(QRect rect)
{
    if (rect.isNull()) return;
    view->setSceneRect(scene->itemsBoundingRect()); // shrink viewport
    view->fitInView(rect, Qt::KeepAspectRatio);
}

void MainWindow::enter_pointer_mode()
{
    actionPointerMode->activate();
    setCursor(Qt::ArrowCursor);
    ((View*)view)->setDragScroll(true);
    pointer_mode();
}

void MainWindow::exit_pointer_mode()
{
    actionPointerMode->deactivate();
    ((View*)view)->setDragScroll(false);
}

void MainWindow::enter_area_select_mode()
{
    actionAreaSelectionMode->activate();
    setCursor(Qt::CrossCursor);

    area_select_mode();
}

void MainWindow::exit_area_select_mode()
{
    actionAreaSelectionMode->deactivate();
}

void MainWindow::on_action_fit_to_window_triggered()
{
    fit_to_rect(scene->pixmapRect());
}

void MainWindow::on_action_baloontip_toggled(bool toggled)
{
    if (toggled)
        scene->addItem(baloonTip);
    else
        scene->removeItem(baloonTip);
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
    imageHandler->openFile();
    displayImage(imageHandler->currentImage(), imageHandler->currentFilePath());
}

void MainWindow::on_action_save_image_triggered()
{
    imageHandler->saveFile(scene->areaRect());
}

