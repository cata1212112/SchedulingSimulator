//
// Created by Cata on 3/22/2024.
//

#ifndef LICENTA_LSTR_H
#define LICENTA_LSTR_H

#include "../../../DiscreteEventSimulator/Core/Core.h"
#include <algorithm>


class LSTR : public SchedulingAlgorithm {
    vector<Process> *readyQueue;
    vector<Core*> cores;
    priority_queue<Event> *mainEventQueue;
    std::mutex *queueMutex;
    int MOT = 1e9;
    bool setMot = false;

public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Process> *getReadyQueue() override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    LSTR();

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int assignCPU(Process p) override;

    string getCoreAlgortihm(int coreID) override;

    void addCore(void *core) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    int removeMissedDeadlines(int time);
};


#endif //LICENTA_LSTR_H
