#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QLabel>
#include "imagehandler.h"
#include "toggleaction.h"
#include "scene.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, const char* filepath="");
    ~MainWindow();

protected:
    virtual void dragEnterEvent(QDragEnterEvent *event);
    virtual void dropEvent(QDropEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual bool eventFilter(QObject *object, QEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    void _mouseMoveEvent(QMouseEvent *event);
    bool _keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    QGraphicsView *view;
    Scene scene;
    QLabel *statusbarLeft;
    QLabel *statusbarRight;
    ImageHandler imgHandler;

    void displayImage(QImage qimage, QString file_path);
    void showNext();
    void showPrev();
    void openFile();
    void saveFile();
    void setupToolBar();
    void setupStatusBar();

private: // Toolbar Actions
    ToggleAction *actionPointerMode;
    ToggleAction *actionAreaSelectionMode;
    QAction *actionFitToWindow;
    QAction *actionNextImage;
    QAction *actionPrevImage;
    QAction *actionOpenImage;
    QAction *actionSaveImage;
    ToggleAction *actionBaloonTip;

private slots:
    void on_action_pointer_mode_toggled(bool toggled);
    void on_action_area_selection_mode_toggled(bool toggled);
    void on_action_fit_to_window_triggered();
    void on_action_next_image_triggered();
    void on_action_prev_image_triggered();
    void on_action_open_image_triggered();
    void on_action_save_image_triggered();
    void on_action_baloontip_toggled(bool toggled);

    void fit_to_rect(QRect rect);

};
#endif // MAINWINDOW_H
