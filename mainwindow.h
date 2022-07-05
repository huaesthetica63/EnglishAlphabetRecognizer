#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "NetWork.h"
#include <QMainWindow>
#include "paintscene.h"
#include <opencv2/opencv.hpp>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    paintscene *scene=new paintscene();
    NetWork NW;
    data_NetWork config;
private slots:
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
