#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encode.h"
#include "decode.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    encode encode;
    encode.setModal(true);
    encode.exec();
}

void MainWindow::on_pushButton_2_clicked()
{
    decode decode;
    decode.setModal(true);
    decode.exec();
}
