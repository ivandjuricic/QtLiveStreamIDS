#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "videothread.h"

extern int m_nSizeX;
extern int m_nSizeY;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showImage(void* data);
    void on_startButton_clicked();

private:
    Ui::MainWindow *ui;
    VideoThread thread;
};

#endif // MAINWINDOW_H
