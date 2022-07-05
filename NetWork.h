#ifndef NetWork_h
#define NetWork_h
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
#include <vector>

using namespace std;
struct data_NetWork {
    int L;
    int* size;
};
data_NetWork ReadDataNetWork(string path);
struct Image{//image format for neural network
    int letter;//correct letter
    double*pixels;//array of pixels
};
vector<Image> readFromDirectory(string directory);
class NetWork
{
    int L;
    int* size;
    ActivateFunction actFunc;
    Matrix* weights;
    double** bios;
    double** neurons_val, ** neurons_err;
    double* neurons_bios_val;
public:
    void Init(data_NetWork data);
    void PrintConfig();
    void SetInput(double* values);

    double ForwardFeed();
    int SearchMaxIndex(double* value);
    void PrintValues(int L);

    void BackPropogation(double expect);
    void WeightsUpdater(double lr);

    void SaveWeights();
    void ReadWeights();
};


#endif
