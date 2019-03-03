//
// Created by jskalska on 09/12/2018.
//

#pragma once

#include "../include/GraphMatrix.h"
#include "TSP.h"
#include <random>

#define EULER_CONSTANT std::exp(1.0)

class SA : public TSP {
public:
    std::string simulatedAnnealing();
    void menu() override;

    void setNeighbourhood(neighbourhood neighbourhood);
    void setTemperature(double temperature);
    void setCoolingRate(double coolingRate);

private:
    double temperature = 10000;
    double minTemperature = 0.00001;
    double coolingRate = 0.9999;
    int shortestPathWeight;
    int tmpShortestPathWeight;
    std::vector <int> bestPath;
    std::vector <int> tmpPath;
    neighbourhood neighbourhood = SWAP;

    void swapVertex();
    void insertVertex();
    void invertVertex();
    int pathLength(std::vector<int> vector);
};
