#include "NetWork.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <random>
#include <algorithm>
#define SQUARE_SIZE 50
data_NetWork ReadDataNetWork(string path) {
    data_NetWork data{};
    ifstream fin;
    fin.open(path);
    if (!fin.is_open()) {
        cout << "Error reading the file " << path << endl;
        system("pause");
    }
    else
        cout << path << " loading...\n";
    string tmp;
    int L;
    while (!fin.eof()) {
        fin >> tmp;
        if (tmp == "NetWork") {
            fin >> L;
            data.L = L;
            data.size = new int[L];
            for (int i = 0; i < L; i++) {
                fin >> data.size[i];
            }
        }
    }
    fin.close();
    return data;
}
vector<Image> readFromDirectory(string directory){
    vector<Image> res;
    for(char curLetter='A'; curLetter<='Z'; curLetter++){
        string curFile = directory +"files"+curLetter+".txt";
        ifstream files(curFile);
        string imagefile;
        while(files>>imagefile){
            string fullImagePath = directory+curLetter+"//"+imagefile;
            cv::Mat im = cv::imread(fullImagePath, cv::IMREAD_UNCHANGED);
            cv::Mat resized;
            cv::resize(im, resized, cv::Size(SQUARE_SIZE,SQUARE_SIZE));
            cv::Mat newimage = cv::Mat::zeros(SQUARE_SIZE, SQUARE_SIZE, CV_8U);
            for(int i=0;i<SQUARE_SIZE;i++){
                for(int j=0;j<SQUARE_SIZE;j++){
                    auto& pixel = resized.at<cv::Vec4b>(i,j);
                    newimage.at<uchar>(i,j)=pixel[3];
                }
            }
            Image imageFormat;
            imageFormat.letter=curLetter-'A';
            imageFormat.pixels = new double[SQUARE_SIZE*SQUARE_SIZE];
            for(int i=0;i<SQUARE_SIZE;i++){
                for(int j=0;j<SQUARE_SIZE;j++){
                    imageFormat.pixels[i*50+j]=newimage.at<uchar>(i,j)/255.0;
                }
            }
            res.push_back(imageFormat);
        }
    }
    mt19937 gen( chrono::system_clock::now().time_since_epoch().count() );
    std::shuffle(res.begin(), res.end(), gen);
    return res;
}
void NetWork::Init(data_NetWork data) {
    actFunc.set();
    srand(time(NULL));
    L = data.L;
    size = new int[L];
    for (int i = 0; i < L; i++)
        size[i] = data.size[i];

    weights = new Matrix[L - 1];
    bios = new double* [L - 1];
    for (int i = 0; i < L - 1; i++) {
        weights[i].Init(size[i + 1], size[i]);
        bios[i] = new double[size[i + 1]];
        weights[i].Rand();
        for (int j = 0; j < size[i + 1]; j++) {
            bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
        }
    }
    neurons_val = new double* [L]; neurons_err = new double* [L];
    for (int i = 0; i < L; i++) {
        neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]];
    }
    neurons_bios_val = new double[L - 1];
    for (int i = 0; i < L - 1; i++)
        neurons_bios_val[i] = 1;
}
void NetWork::PrintConfig() {
    cout << "***********************************************************\n";
    cout << "NetWork has " << L << " layers\nSIZE[]: ";
    for (int i = 0; i < L; i++) {
        cout << size[i] << " ";
    }
    cout << "\n***********************************************************\n\n";
}
void NetWork::SetInput(double* values) {
    for (int i = 0; i < size[0]; i++) {
        neurons_val[0][i] = values[i];
    }
}

double NetWork::ForwardFeed() {
    for (int k = 1; k < L; ++k) {
        Matrix::Multi(weights[k - 1], neurons_val[k - 1], size[k - 1], neurons_val[k]);
        Matrix::SumVector(neurons_val[k], bios[k - 1], size[k]);
        actFunc.use(neurons_val[k], size[k]);
    }
    int pred = SearchMaxIndex(neurons_val[L - 1]);
    return pred;
}
int NetWork::SearchMaxIndex(double* value) {
    double max = value[0];
    int prediction = 0;
    double tmp;
    for (int j = 1; j < size[L - 1]; j++) {
        tmp = value[j];
        if (tmp > max) {
            prediction = j;
            max = tmp;
        }
    }
    return prediction;
}
void NetWork::PrintValues(int L) {
    for (int j = 0; j < size[L]; j++) {
        cout << j << " " << neurons_val[L][j] << endl;
    }
}

void NetWork::BackPropogation(double expect) {
    for (int i = 0; i < size[L - 1]; i++) {
        if (i != int(expect))
            neurons_err[L - 1][i] = -neurons_val[L - 1][i] * actFunc.useDer(neurons_val[L - 1][i]);
        else
            neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]) * actFunc.useDer(neurons_val[L - 1][i]);
    }
    for (int k = L - 2; k > 0; k--) {
        Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
        for (int j = 0; j < size[k]; j++)
            neurons_err[k][j] *= actFunc.useDer(neurons_val[k][j]);
    }
}
void NetWork::WeightsUpdater(double lr) {
    for (int i = 0; i < L - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            for (int k = 0; k < size[i]; ++k) {
                weights[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * lr;
            }
        }
    }
    for (int i = 0; i < L - 1; i++) {
        for (int k = 0; k < size[i + 1]; k++) {
            bios[i][k] += neurons_err[i + 1][k] * lr;
        }
    }
}
void NetWork::SaveWeights() {
    ofstream fout;
    fout.open("Weights.txt");
    if (!fout.is_open()) {
        cout << "Error reading the file";
        system("pause");
    }
    for (int i = 0; i < L - 1; ++i)
        fout << weights[i] << " ";

    for (int i = 0; i < L - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            fout << bios[i][j] << " ";
        }
    }
    cout << "Weights saved \n";
    fout.close();
}
void NetWork::ReadWeights() {
    ifstream fin;
    fin.open("Weights.txt");
    if (!fin.is_open()) {
        cout << "Error reading the file";
        system("pause");
    }
    for (int i = 0; i < L - 1; ++i) {
        fin >> weights[i];
    }
    for (int i = 0; i < L - 1; ++i) {
        for (int j = 0; j < size[i + 1]; ++j) {
            fin >> bios[i][j];
        }
    }
    cout << "Weights readed \n";
    fin.close();
}
