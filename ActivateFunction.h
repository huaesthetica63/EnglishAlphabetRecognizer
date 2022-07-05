#ifndef ActivateFunction_h
#define ActivateFunction_h
#include <iostream>
#include <cmath>
enum activateFunc { sigmoid = 1, ReLU, thx };
class ActivateFunction
{
    activateFunc actFunc;
public:
    void set();
    void use(double* value, int n);
    void useDer(double* value, int n);
    double useDer(double value);
};

#endif
