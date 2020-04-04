#ifndef TAKEALOOKMAINWINDOW_H
#define TAKEALOOKMAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QLabel>
#include "imagehandler.h"
#include "toggleaction.h"
#include "scene.h"


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
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    void _mouseMoveEvent(QMouseEvent *event);
    bool _keyPressEvent(QKeyEvent *event);

protected:
    Ui::TakeALookMainWindow *ui;
    QGraphicsView *view;
    Scene *scene;
    QLabel *statusbarLeft;
    QLabel *statusbarRight;
    ImageHandler *imageHandler;

    void displayImage(QImage qimage, QString file_path);
    void showNext();
    void showPrev();
    void setupToolBar();
    void setupFileToolBar();
    void setupStatusBar();

signals:
    void image_changed();


    // Toolbar
protected:
    ToggleAction *actionPointerMode;
    ToggleAction *actionAreaSelectionMode;
    QAction *actionFitToWindow;
    ToggleAction *actionBaloonTip;

protected slots:
    void on_action_pointer_mode_toggled(bool toggled);
    void on_action_area_selection_mode_toggled(bool toggled);
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
