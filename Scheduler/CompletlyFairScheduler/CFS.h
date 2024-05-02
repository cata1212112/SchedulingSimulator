//
// Created by Cata on 3/10/2024.
//

#ifndef LICENTA_CFS_H
#define LICENTA_CFS_H


#include "../SchedulingAlgorithm.h"

class CFS : public SchedulingAlgorithm{
    int time_interval;
    int load_balance_period;
public:
    int assignCPU(Process p) override;

    void addCore(void *core) override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_CFS_H
