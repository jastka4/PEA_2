//
// Created by jskalska on 08/12/2018.
//

#include "TS.h"

std::string TS::tabuSearch() {
    tabuList.clear();
    Timer t;
    t.start();
    int *current_permutation = new int[numberOfCities]; // stores currently analyzed permutation

    //first permutation - greedy algorithm
    int length = beginning(current_permutation); // stores the best known length

    int *result_permutation = new int[numberOfCities]; // stores the best known permutation
    for (int i = 0; i < numberOfCities; i++){
        result_permutation[i] = current_permutation[i];
    }

    int numberOfIterationsWithoutChange = numberOfCities * 3; // maximum number of iterations without a change - diversification
    while (t.getTimeWithoutStopping() < stopCriterion){
        // new solution - the best from the neighbourhood
        TabuElement tabuElement = newSolution(current_permutation); // returns swapped i, j

        int tmp = countPath(current_permutation); // the length of the found route

        if (tmp < length){ // if the solution is better, change for the new one
            numberOfIterationsWithoutChange = numberOfCities * 3;
            length = tmp;
            for (int i = 0; i < numberOfCities; i++){
                result_permutation[i] = current_permutation[i];
            }
        }

        //tabu list actualisation: reducing the lifetime (cadence) and removing unnecessary items
        for(auto it = tabuList.begin(); it != tabuList.end(); ++it) {
            it->lifetime--;

            if (it->lifetime == 0){
                tabuList.erase(it);
                --it;
            }
        }

        // add new elements to the tabu list
        if (tabuList.size() < numberOfCities * 2 ) { // maximum size of the tabu list
            tabuElement.lifetime = numberOfCities * 2;
            tabuList.push_back(tabuElement);
        }


        // diversification: if better after the restart then swap
        if(diversification && (numberOfIterationsWithoutChange <= 0)){
            restart(current_permutation);
            tmp = countPath(current_permutation);

            numberOfIterationsWithoutChange = numberOfCities * 3;
            if (tmp <= length){
                length = tmp;
                for (int i = 0; i < numberOfCities; i++){
                    result_permutation[i] = current_permutation[i];
                }
            }
        }
        numberOfIterationsWithoutChange--;
    }

    std::stringstream ss;
//    ss << "Tabu search algorithm.\nResult: " << std::endl;
//    for (int i = 0; i < numberOfCities; i++){
//        ss << result_permutation[i] << " ";
//    }
    ss << length << std::endl;

    delete[] current_permutation;
    delete[] result_permutation;
    tabuList.clear();
    return ss.str();
}

// restart - greedy algorithm, where the second vertex is random
void TS::restart(int *current_permutation) {
    bool *visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++) {
        visited[i] = false;
    }

    int k = start;
    current_permutation[0] = start; // first vertex is the starting vertex
    visited[start] = true;
    current_permutation[1] = rand() % (numberOfCities - 1) + 1; // getting random second vertex
    visited[current_permutation[1]] = true;
    for(int i = 2; i < numberOfCities; i++){ // greedy algorithm for next vertexes
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++){ // looking for a minimum for an actual position
            if (!visited[j] && gm.getEdgeLength(k,j) != -1 && gm.getEdgeLength(k,j) < min){
                min = gm.getEdgeLength(k,j);
                position = j;
            }
        }
        k = position;
        current_permutation[i] = position;
    }
    delete[] visited;
}

// Finds the best solution in the neighbourhood
TabuElement TS::newSolution(int *result_permutation) {
    TabuElement tabuElement;
    int min = INT32_MAX;
    int *current_permutation = new int[numberOfCities]; // current permutation
    for (int i = 0; i < numberOfCities; i++) {
        current_permutation[i] = result_permutation[i];
    }

    for (int i = 1; i < numberOfCities - 1; i++) {
        for (int j = 1; j < numberOfCities - 1; j++) {
            if (i != j && !inTabuList(i, j)) {
                switch(currentNeighbourhood) {  // choose neighbourhood
                    case SWAP: swap(current_permutation, i, j); break;
                    case INSERT: insert(current_permutation, i, j); break;
                    case INVERT: invert(current_permutation, i, j); break;
                }
                int tmp = countPath(current_permutation);

                if (tmp <= min) {
                    tabuElement.i = j; tabuElement.j = i;
                    min = tmp;
                    for (int k = 0; k < numberOfCities; k++) {
                        result_permutation[k] = current_permutation[k]; // here is the best permutation in the neighbourhood
                    }
                }
                switch(currentNeighbourhood){
                        case SWAP: swap(current_permutation, i, j); break;
                        case INSERT: insert(current_permutation, j, i); break;
                        case INVERT: invert(current_permutation, i, j); break;
                }

            }
        }
    }
    return tabuElement;
}

bool TS::inTabuList(int i, int j) {
    for (auto &it : tabuList) {
        if ((it.i == i && it.j == j)||(it.i == j && it.j == i)) // cases in which we do not swap
            return true;
    }
    return false;
}

// a simple greedy algorithm is looking for an initial solution
int TS::beginning(int *current_permutation) {
    bool *visited = new bool[numberOfCities];
    int length = 0;

    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    int k = start;

    current_permutation[0] = start;
    for(int i = 1; i < numberOfCities; i++) { // greedy algorithm looks for the initial setting
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++) { // looking for the minimum from the current position
            if (!visited[j] && (gm.getEdgeLength(k,j) != -1 && gm.getEdgeLength(k,j) != 0) && gm.getEdgeLength(k,j) < min) { // TODO: check 17 for 0 instead of -1
                min = gm.getEdgeLength(k,j);
                position = j;
            }
        }
        length += min;
        k = position;
        current_permutation[i] = position;
    }
    length += gm.getEdgeLength(k,start);

    delete[] visited;
    return length;
}

int TS::countPath(int *permutation) {
    int length = 0;
    int end;

    for (int i = 1; i < numberOfCities; i++){
        length += gm.getEdgeLength(permutation[i-1],permutation[i]);
        end = i;
    }
    length += gm.getEdgeLength(permutation[end],permutation[0]);

    return length;
}

void TS::swap(int *permutation, int left, int right) {
    if (right == left) return;
    int tmp = permutation[left];
    permutation[left] = permutation[right];
    permutation[right] = tmp;
}

void TS::insert(int *permutation, int left, int right) {
    if (right == left) return;
    if (right < left){
        int tmp = permutation[left];
        for (int i = left; i > right; i--){
            permutation[i] = permutation[i-1];
        }
        permutation[right] = tmp;
    }
    else {
        int tmp = permutation[left];
        for (int i = left; i < right; i++){
            permutation[i] = permutation[i+1];
        }
        permutation[right] = tmp;
    }

}

void TS::invert(int *permutation, int left, int right) {
    if (right == left) return;
    if (right < left) {
        int tmp = left;
        left = right;
        right = tmp;
    }
    for (int i = 0; i < (right - left +1)/2; i++){
        swap(permutation,left + i,right - i);
    }
}

void TS::menu() {
    std::cout << "1. Change the stop criterion. Actual: " << stopCriterion << " s.\n"
                 "2. Switch the diversification. Actual: " << diversification << ". (0 - off, 1 - on).\n"
                 "3. Choose neighbourhood. Actual: " << currentNeighbourhood << ". (0 - swap, 1 - insert, 2 - invert).\n"
                 "4. Perform search.\n"
                 "5. Exit.\n"
                 "Please enter the appropriate number: ";
    int chosen;
    std::cin >> chosen;
    switch(chosen){
        case 1:
            std::cout << "Please specify the stop criterion (in seconds): \n";
            std::cin >> stopCriterion;
            break;
        case 2:
            std::cout << "The diversification settings have been changed!\n";
            diversification = !diversification;
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
                    currentNeighbourhood = SWAP;
                    break;
                case 1:
                    currentNeighbourhood = INSERT;
                    break;
                case 2:
                    currentNeighbourhood = INVERT;
                    break;
            }
            break;
        case 4:
            std::cout << "\n########################################\n" << tabuSearch();
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

void TS::setDiversification(bool diversification) {
    TS::diversification = diversification;
}

void TS::setStopCriterion(double stopCriterion) {
    TS::stopCriterion = stopCriterion;
}

void TS::setCurrentNeighbourhood(TS::neighbourhood currentNeighbourhood) {
    TS::currentNeighbourhood = currentNeighbourhood;
}
