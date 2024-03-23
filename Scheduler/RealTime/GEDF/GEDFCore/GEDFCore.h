//
// Created by Cata on 3/22/2024.
//

#ifndef LICENTA_GEDFCORE_H
#define LICENTA_GEDFCORE_H

#include "../../../SchedulingAlgorithm.h"
#include <functional>
#include <queue>

class GEDFCore : public SchedulingAlgorithm{
    queue<Process> *readyQueue;
public:
    GEDFCore();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int assignCPU(Process p) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_GEDFCORE_H
