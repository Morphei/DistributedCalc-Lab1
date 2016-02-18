#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include <thread>
#include <mutex>
#include "random"

std::mutex threadMutex;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setValidator(new QIntValidator(0,65536, ui->lineEdit));
    ui->lineEdit_2->setValidator(new QIntValidator(0,65536, ui->lineEdit));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox msg;
    if(ui->lineEdit->text().isEmpty())
    {
        if(ui->lineEdit_2->text().isEmpty())
        {
            msg.setText("Enter range and size!");
            msg.exec();
        }
        else
        {
            msg.setText("Enter range!");
            msg.exec();
        }
    }
    else if(ui->lineEdit_2->text().isEmpty())
    {
        msg.setText("Enter size!");
        msg.exec();
    }

    int n, range;

    range = ui->lineEdit->text().toInt();
    n = ui->lineEdit_2->text().toInt();

    int* array = new int[n];

    srand(time(NULL));

    for(int i = 0; i < n; i++)
    {
        array[i] = rand() % range;
    }

    std::thread sortThread(&MainWindow::sortInThread, this, array, n);
    sortThread.join();

    QString text;
    for(int i = 0; i < n; i++)
    {
        text += QString::number(array[i]) += " ";
    }

    ui->textBrowser->append(text);
}

void MainWindow::sortInThread(int* array, int n)
{
    threadMutex.lock();

    int i = 0;
    while (i < n) {
        if( i == 0 || array[i - 1] <= array[i])
            i++;

        else
        {
            int temp = array[i];
            array[i] = array[i-1];
            array[i-1] = temp;
            i--;
        }
    }

    threadMutex.unlock();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->textBrowser->clear();
}
