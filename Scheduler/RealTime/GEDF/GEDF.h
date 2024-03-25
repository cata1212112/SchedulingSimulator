//
// Created by Cata on 3/22/2024.
//

#ifndef LICENTA_GEDF_H
#define LICENTA_GEDF_H

#include "../../../DiscreteEventSimulator/Core/Core.h"
#include <algorithm>

class GEDF : public SchedulingAlgorithm {
    vector<Process> *readyQueue;
    vector<Core*> cores;
    priority_queue<Event> *mainEventQueue;
public:
    GEDF();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    int assignCPU(Process p) override;

    void addCore(void *core) override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;
};


#endif //LICENTA_GEDF_H
