//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_DES_H
#define CPUSCHED_DES_H

#include <string>
#include "../Utils/Random.h"
#include "Event/Event.h"
#include "../Process/Process.h"
#include <queue>
#include <unordered_map>
#include "../Scheduler/SchedulingAlgorithm.h"
#include <fstream>
#include<sstream>

#define MAXIMUMNUMCPUBURSTS 20
#define MAXIMUMARRIVAL 300

using namespace std;

class DES {
private:
    static int getMultiplier(bool flag) {
        if (flag) {
            return 10;
        } else {
            return 5;
        }
    }

public:
    priority_queue<Event> *events;

    DES() {
        events = new priority_queue<Event>();
    }

    string generateInputData(int numProcesses, int maximumTime);

    void readInputDataFromFile(const string& filename);

    void startSimulation(SchedulingAlgorithm &schedAlgo, int numCPUS);
};


#endif //CPUSCHED_DES_H
