#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QImage>
#include <QPixmap>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("QMainWindow{background-color:white}");
    ui->startButton->setStyleSheet("QPushButton{color:white; background-color:#000066;border:none}");
    connect(&thread, SIGNAL(frame(void*)), this, SLOT(showImage(void*)));
    if (!thread.isRunning())
        thread.start();
}

MainWindow::~MainWindow()
{
    thread.stopVideo();
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    this->close();
}

void MainWindow::showImage(void *data)
{
    QImage image((unsigned char*)data, m_nSizeX, m_nSizeY,QImage::Format_RGB16);
    ui->label->setPixmap(QPixmap::fromImage(image.scaledToHeight(350, Qt::FastTransformation)));
}

