#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->scene->sceneWidth=ui->graphicsView->width();
    this->scene->sceneHeight=ui->graphicsView->height();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setBackgroundBrush(QBrush(Qt::white, Qt::SolidPattern));
    config = ReadDataNetWork("Config.txt");
    NW.Init(config);
    NW.ReadWeights();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    this->scene->clearscene();
    this->ui->label_2->setText("");
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->scene->brushSize = ui->spinBox->value();
}


void MainWindow::on_pushButton_clicked()
{

    QImage pixm = this->ui->graphicsView->grab().toImage();
    cv::Mat  mat1( pixm.height(), pixm.width(),
                  CV_8UC4,
                  const_cast<uchar*>(pixm.bits()),
                  static_cast<size_t>(pixm.bytesPerLine())
                  );
    cv::Mat resized(50,50,CV_8U);
    cv::resize(mat1, resized, cv::Size(50,50),cv::INTER_AREA);
    Image tempinfo;
    tempinfo.pixels = new double[2500];
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++){
            tempinfo.pixels[i*50+j]=(255-resized.at<uchar>(i,j))/255.0;
        }
    }

    NW.SetInput(tempinfo.pixels);
    int predict = NW.ForwardFeed();
    ui->label_2->setText(QString((char)('A'+predict)));
}
void MainWindow::on_pushButton_3_clicked()
{
    int correct = this->ui->comboBox->currentIndex();
    NW.BackPropogation(correct);
    int epoch = 1;
    NW.WeightsUpdater(0.15 * exp(-epoch / 20.));
    NW.SaveWeights();
}

