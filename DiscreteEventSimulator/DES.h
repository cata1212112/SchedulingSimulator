//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_DES_H
#define CPUSCHED_DES_H

#include <string>
#include "../Utils/Random.h"
#include "../Utils/ImplementedAlgorithms.h"
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>

#define MAXIMUMNUMCPUBURSTS 20
#define MAXIMUMARRIVAL 300

using namespace std;

class DES {
private:
    priority_queue<Event> *events;
    string input;
    string algorithm;

    static int getMultiplier(bool flag) {
        if (flag) {
            return 10;
        } else {
            return 5;
        }
    }

public:
    DES(string algorithm) : algorithm(std::move(algorithm)) {
        events = new priority_queue<Event>();
    }

    string generateInputData(int numProcesses, int maximumTime);

    void readInputDataFromFile(const string& filename);

    Metrics startSimulation(int numCPUS);
};


#endif //CPUSCHED_DES_H
