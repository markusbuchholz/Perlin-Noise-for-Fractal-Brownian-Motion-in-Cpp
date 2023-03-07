// Markus Buchholz, 2023

// g++ blend.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//---------------------------------------------------------------

void plot2D(std::vector<float> X, std::vector<float> Y)
{

    plt::title("Blending function ");
    plt::named_plot("blending function", X, Y);
    plt::xlabel("X");
    plt::ylabel("Y");
    plt::legend();
    plt::xlabel("X");
    plt::ylabel("Y");
    plt::show();
}

//---------------------------------------------------------------

std::tuple<std::vector<float>, std::vector<float>> simulation()
{
    std::vector<float> X;
    std::vector<float> Y;

    auto dt = 0.01;

    for (float t = 0.0f; t <= 1.0f; t = t + dt)
    {
        X.push_back(t);
        Y.push_back(6 * std::pow(t, 5) - 15 * std::pow(t, 4) + 10* std::pow(t, 3) );

    }
    return std::make_tuple(X, Y);
}

int main()
{

    auto sim = simulation();

    plot2D(std::get<0>(sim), std::get<1>(sim));
}