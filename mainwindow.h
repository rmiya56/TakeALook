#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QLabel>
#include "imagehandler.h"
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

private slots:
    //void fit_to_view();
    void fit_to_rect(QRect rect);
    void on_action_switch_to_normal_mode_triggered();
    void on_action_switch_to_area_selection_mode_triggered();
    void on_action_fit_to_window_triggered();
    void on_action_next_image_triggered();
    void on_action_prev_image_triggered();
    void on_action_open_file_triggered();
    void on_action_save_triggered();

private:
    Ui::MainWindow *ui;
    QLabel *statusbar_left;
    QLabel *statusbar_right;
    ImageHandler imgHandler;
    Scene scene;
    QGraphicsView *view;
    QPixmap original_pixmap;
    bool is_normal_mode = true;

    void displayImage(QImage qimage, QString file_path);
    void showNext();
    void showPrev();
    void openFile();
    void saveFile();

};
#endif // MAINWINDOW_H
