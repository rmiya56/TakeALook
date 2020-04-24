#ifndef TAKEALOOKMAINWINDOW_H
#define TAKEALOOKMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QLabel>
#include <QStateMachine>
#include "imagehandler.h"
#include "toggleaction.h"
#include "scene.h"
#include "../baloontip/pixbaloontip.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TakeALookMainWindow; }
QT_END_NAMESPACE

class TakeALookMainWindow : public QMainWindow

{
    Q_OBJECT

public:
    TakeALookMainWindow(QWidget *parent = nullptr, const char* filepath="");
    ~TakeALookMainWindow();

protected:
    void displayImage(QImage qimage, QString file_path);
    void showNext();
    void showPrev();
    void setupToolBar();
    void setupFileToolBar();
    void setupStatusBar();

    Ui::TakeALookMainWindow *ui;
    QGraphicsView *view;
    ImageHandler *imageHandler;
    Scene *scene;
    QLabel *statusbarLeft;
    QLabel *statusbarRight;
    PixBaloonTip *baloonTip;


// mouse & key event
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    void _mouseMoveEvent(QMouseEvent *event);
    bool _keyPressEvent(QKeyEvent *event);
signals:
    void image_changed();
    void double_clicked();


// State Machine
protected:
    QStateMachine machine;

protected slots:
    void enter_pointer_mode();
    void exit_pointer_mode();
    void enter_area_select_mode();
    void exit_area_select_mode();

// Toolbar
protected:
    ToggleAction *actionPointerMode;
    ToggleAction *actionAreaSelectionMode;
    QAction *actionFitToWindow;
    ToggleAction *actionBaloonTip;

protected slots:
    void on_action_fit_to_window_triggered();
    void on_action_baloontip_toggled(bool toggled);
    void fit_to_rect(QRect rect);


// Toolbar (File)
protected:
    QAction *actionNextImage;
    QAction *actionPrevImage;
    QAction *actionOpenImage;
    QAction *actionSaveImage;

protected slots:
    void on_action_next_image_triggered();
    void on_action_prev_image_triggered();
    void on_action_open_image_triggered();
    void on_action_save_image_triggered();

};
#endif // TAKEALOOKMAINWINDOW_H
