//
// Created by Cata on 5/21/2024.
//

#ifndef LICENTA_LTS_H
#define LICENTA_LTS_H


#include "../../SchedulingAlgorithm.h"
#include "../../../DiscreteEventSimulator/Core/Core.h"
#include <algorithm>
#include <map>

class LTS : public SchedulingAlgorithm{
    vector<Process> *readyQueue;
    vector<Core*> cores;
    priority_queue<Event> *mainEventQueue;
    std::mutex *queueMutex;
    std::map<int, long double> res;
    std::map<int, long double> totalRes;
    std::map<int, long double> P;

public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    LTS();

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int assignCPU(Process p) override;

    string getCoreAlgortihm(int coreID) override;

    void addCore(void *core) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    int removeMissedDeadlines(int time);
};


#endif //LICENTA_LTS_H
