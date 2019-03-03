//
// Created by jskalska on 08/12/2018.
//

#include <iostream>
#include <ctime>
#include <thread>
#include "TS.h"
#include "SA.h"

void testMenu();


int main() {
    srand(time(NULL));

    TS * ts = new TS();
    SA * sa = new SA();

    int chosen;
    std::string file_name;

    while (chosen != 4) {
        std::cout << "1. Load from file\n"
                     "2. Tabu search\n"
                     "3. Simulated Annealing\n"
                     "4. Test\n"
                     "5. Exit.\n"
                     "Please enter the appropriate number: ";

        std::cin >> chosen;

        switch (chosen) {
            case 1:
                std::cout << "Please enter the file name: ";
                std::cin >> file_name;
                if (ts->loadFromFile(file_name) && sa->loadFromFile(file_name)){
                    std::cout << "Loaded correctly!\n";
                }
                else std::cout << "The file could not be loaded!\n";
                break;
            case 2:
                ts->menu();
                break;
            case 3:
                sa->menu();
                break;
            case 4:
                testMenu();
            break;
            default:
                std::cin.clear();
                std::cin.sync();
                break;
        }
    }

    delete ts;
    delete sa;

    return 0;
}

void testTS(){
    TS *tsp = new TS();
    std::ofstream fout;
    std::string filenames[] = {"10.txt", "17.txt", "gr21.tsp", "gr48.tsp", "gr120.tsp", "ftv47.atsp", "ftv170.atsp", "rgb403.atsp"};
    std::string filename, result;
    unsigned long long int result_position;
    double times[] = {0.5, 1, 2, 5, 10, 20,};
    for (const auto &i : filenames) { // files
        tsp->loadFromFile(i);
        filename = "../data/output/ts/res_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 3; j++) { // neighbourhood
            tsp->setCurrentNeighbourhood(static_cast<TSP::neighbourhood>(j));
            for (int k = 0; k < 2; k++) { // diversification
                tsp->setDiversification(static_cast<bool>(k));
                for (double time : times) { // time of algorithm execution
                    tsp->setStopCriterion(time);
                    result = tsp->tabuSearch();
                    result_position = result.find(':', 31);
                    result = result.substr(result_position, result.size() - result_position-1);
                    fout << j << " " << k << " " << time << " " << result << std::endl;
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void testSA(){
    SA *tsp = new SA();
    std::ofstream fout;
    std::string filenames[] = {"gr120.tsp"};
//    std::string filenames[] = {"17.txt"};
    std::string filename, result;
    Timer *timer = new Timer();
    unsigned long long int result_position;
    double coolingRates[] = { 0.5, 0.6, 0.75, 0.90, 0.99, 0.9999 };
    double temperatures [] = {1000, 10000, 100000, 1000000};
    for (const auto &i : filenames) { // files
        tsp->loadFromFile(i);
        filename = "../data/output/sa/res_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 3; j++) { // neighbourhood
            tsp->setNeighbourhood(static_cast<TSP::neighbourhood>(j));
//            for (double temperature : temperatures) {
//                tsp->setTemperature(temperature);
                for (double coolingRate : coolingRates) { // cooling rate
                    tsp->setCoolingRate(coolingRate);
                    delete timer;
                    timer = new Timer();
                    result = tsp->simulatedAnnealing();
                    int test = atoi(result.c_str())-6942;
                    double res = test / 6942.0;
                    double res2 = res * 100.0;
//                    result_position = result.find(':', 38);
//                    result = result.substr(result_position, result.size() - result_position-1);
                    fout << j << " " << 10000 << " " << coolingRate << " " << " " << result << " - " << res2 << std::endl;
                }
//            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void testMenu() {
    std::cout << "1. Tabu search\n"
                 "2. Simulated Annealing\n"
                 "3. Exit\n";

    int chosen;
    std::string file_name;
    std::cin >> chosen;
    switch(chosen) {
        case 1:
            testTS();
            break;
        case 2:
            testSA();
            break;
        case 3:
            return;
        default:
            std::cout << "Please enter the correct number!\n";
            std::cin.clear();
            std::cin.sync();
            break;
    }

    testMenu();
}