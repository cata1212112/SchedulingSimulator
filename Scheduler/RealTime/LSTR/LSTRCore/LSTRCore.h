//
// Created by Cata on 3/24/2024.
//

#ifndef LICENTA_LSTRCORE_H
#define LICENTA_LSTRCORE_H

#include "../../../SchedulingAlgorithm.h"
#include <functional>
#include <queue>


class LSTRCore : public SchedulingAlgorithm{
    queue<Process> *readyQueue;
    priority_queue<Event> *mainQueue;
    mutex *queueMutex;
public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    int assignCPU(Process p) override;

    LSTRCore();

    void addCore(void *core) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

};


#endif //LICENTA_LSTRCORE_H
