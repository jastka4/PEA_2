//
// Created by jskalska on 08/12/2018.
//

#pragma once

#include "../include/GraphMatrix.h"
#include "../include/Timer.h"
#include "TSP.h"

struct TabuElement {
    int i;
    int j;
    int lifetime;
};

class TS : public TSP {
public:
    std::string tabuSearch();
    void menu() override;

    void setDiversification(bool diversification);
    void setStopCriterion(double stopCriterion);
    void setCurrentNeighbourhood(neighbourhood currentNeighbourhood);
private:
    bool diversification = false; //diversification is turned on, 0 - no, 1 - yes
    double stopCriterion = 10; // time in seconds
    neighbourhood currentNeighbourhood = SWAP;
    std::vector<TabuElement> tabuList;
    int start = 0;

    TabuElement newSolution(int *result_permutation);
    int countPath(int *permutation);
    void restart(int *current_permutation);
    int beginning(int *current_permutation);
    bool inTabuList(int i, int j);

    void swap(int *permutation, int left, int right);
    void insert(int *permutation, int left, int right);
    void invert(int *permutation, int left, int right);
};
