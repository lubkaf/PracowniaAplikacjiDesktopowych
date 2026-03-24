#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H
#include  <cmath>
#include <iostream>
#include <optional>
class mathOperations
{
public:
    mathOperations();
    double Add(double a, double b){ return a + b; }
    double Substract(double a, double b){return a-b;}
    std::optional<double> Divide(double a, double b){
        if(b == 0.0) return std::nullopt;
        else return a / b;
    }
    double Multiply(double a, double b){ return a * b; }
};

#endif // MATHOPERATIONS_H
