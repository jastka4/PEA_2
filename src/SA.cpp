//
// Created by jskalska on 09/12/2018.
//

#include "SA.h"

std::string SA::simulatedAnnealing() {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    std::default_random_engine generator;
    temperature = 10000;

    for (int i = 0; i < numberOfCities; i++) // initialization of the first path
    {
        bestPath.push_back(i);
    }

    tmpPath = bestPath;
    shortestPathWeight = pathLength(tmpPath);   // get length of the first path
    tmpShortestPathWeight = shortestPathWeight;

    while (temperature > minTemperature)
    {
        switch(neighbourhood) {  // choose neighbourhood
            case SWAP: swapVertex(); break;   // swap random vertexes
            case INSERT: insertVertex(); break; //
            case INVERT: invertVertex(); break;
        }

        tmpShortestPathWeight = pathLength(tmpPath); // get length of the current path
        if (tmpShortestPathWeight < shortestPathWeight)
        {
            shortestPathWeight = tmpShortestPathWeight;
            bestPath = tmpPath;
        }
        // set worse path as the best one if the condition is met
        else if (pow(EULER_CONSTANT,(shortestPathWeight - tmpShortestPathWeight)/temperature) > distribution(generator))
        {
            shortestPathWeight = tmpShortestPathWeight;
            bestPath = tmpPath;
        }
        temperature *= coolingRate; // exponential function
    }

    std::stringstream ss;
//    ss << "Simulated annealing algorithm.\nResult: " << std::endl;
//    for (int i = 0; i < numberOfCities; i++){
//        ss << bestPath[i] << " ";
//    }
    ss  << shortestPathWeight << std::endl;

    return ss.str();
}

int SA::pathLength(std::vector<int> vector)
{
    int result = 0;
    int tmp = 0;
    for (int i = 1; i < numberOfCities; i++) { //-1 bo dla ostatniego dadawane jest recznie
        result += gm.getEdgeLength(tmp, vector[i]);
        tmp = vector[i];
    }
    result += gm.getEdgeLength(tmp, 0);
    return result;
}

void SA::swapVertex()
{
    int vertex1 = rand() % (numberOfCities - 1) + 1;
    int vertex2 = rand() % (numberOfCities - 1) + 1;
    while (vertex1 == vertex2) {
        vertex2 = rand() % (numberOfCities - 1) + 1;
    }
    int tmp = bestPath[vertex1];
    tmpPath = bestPath;
    tmpPath[vertex1] = tmpPath[vertex2];
    tmpPath[vertex2] = tmp;
}

void SA::insertVertex()
{
    int vertex1 = rand() % (numberOfCities - 1) + 1;
    int vertex2 = rand() % (numberOfCities - 1) + 1;
    while (vertex1 == vertex2) {
        vertex2 = rand() % (numberOfCities - 1) + 1;
    }

    if (vertex2 < vertex1){
        int tmp = bestPath[vertex1];
        tmpPath = bestPath;
        for (int i = vertex1; i > vertex2; i--){
            tmpPath[i] = tmpPath[i-1];
        }
        tmpPath[vertex2] = tmp;
    }
    else {
        int tmp = bestPath[vertex1];
        tmpPath = bestPath;
        for (int i = vertex1; i < vertex2; i++){
            tmpPath[i] = tmpPath[i+1];
        }
        tmpPath[vertex2] = tmp;
    }
}

void SA::invertVertex()
{
    int vertex1 = rand() % (numberOfCities - 1) + 1;
    int vertex2 = rand() % (numberOfCities - 1) + 1;
    while (vertex1 == vertex2) {
        vertex2 = rand() % (numberOfCities - 1) + 1;
    }

    if (vertex2 == vertex1) return;
    if (vertex2 < vertex1) {
        int tmp = vertex1;
        vertex1 = vertex2;
        vertex2 = tmp;
    }
    tmpPath = bestPath;

    for (int i = 0; i < (vertex2 - vertex1 + 1) / 2; i++) {
        iter_swap(tmpPath.begin() + vertex1 + i, tmpPath.begin() + vertex2 - i);
    }
}

void SA::menu() {
    std::cout << "1. Change the temperature. Actual: " << temperature << ".\n"
                 "2. Choose the cooling rate. Actual: " << coolingRate << ".\n"
                 "3. Choose neighbourhood. Actual: " << neighbourhood << ". (0 - swap, 1 - insert, 2 - invert).\n"
                 "4. Perform search.\n"
                 "5. Exit.\n"
                 "Please enter the appropriate number: ";
    int chosen;
    std::string file_name;
    std::cin >> chosen;
    switch(chosen){
        case 1:
            std::cout << "Please specify the temperature: \n";
            std::cin >> temperature;
            break;
        case 2:
            std::cout << "Please specify the cooling rate (less than 1): \n";
            std::cin >> coolingRate;
            break;
        case 3:
            std::cout << "Please choose a neighborhood:\n"
                         "0 - swap\n"
                         "1 - insert\n"
                         "2 - invert\n";
            int neighbours;
            std::cin >> neighbours;
            switch(neighbours){
                case 0:
                    neighbourhood = SWAP;
                    break;
                case 1:
                    neighbourhood = INSERT;
                    break;
                case 2:
                    neighbourhood = INVERT;
                    break;
            }
            break;
        case 4:
            std::cout << "\n########################################\n" << simulatedAnnealing() << std::endl;
            break;
        case 5:
            return;
        default:
            std::cout  << "Please enter the correct number!\n";
            std::cin.clear();
            std::cin.sync();
            break;
    }
    menu();
}

void SA::setNeighbourhood(TSP::neighbourhood neighbourhood) {
    SA::neighbourhood = neighbourhood;
}

void SA::setTemperature(double temperature) {
    SA::temperature = temperature;
}

void SA::setCoolingRate(double coolingRate) {
    SA::coolingRate = coolingRate;
}

