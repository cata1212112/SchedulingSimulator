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
    priority_queue<Event> events;

    static int getMultiplier(bool flag) {return 10 ? flag : 5;}

public:
    char* generateInputData(int numProcesses, int maximumTime);

    unordered_map<string, vector<Event>> readInputDataFromFile(const string& filename);

    unordered_map<string, vector<Event>> readInputDataFromJSON(const string& json);

    void startSimulation(SchedulingAlgorithm &schedAlgo, int numCPUS);
};


#endif //CPUSCHED_DES_H
