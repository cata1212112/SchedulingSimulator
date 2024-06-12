//
// Created by Cata on 5/21/2024.
//

#ifndef LICENTA_DARTS_H
#define LICENTA_DARTS_H


#include "../../../Process/Process.h"
#include "../../../DiscreteEventSimulator/Core/Core.h"
#include "../../../DiscreteEventSimulator/Event/Event.h"
#include <algorithm>

class DARTS : public SchedulingAlgorithm{
    vector<Process> *readyQueue;
    vector<Core*> cores;
    priority_queue<Event> *mainEventQueue;
    std::mutex *queueMutex;
    int MOT = -1;

public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    DARTS();

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int assignCPU(Process p) override;

    string getCoreAlgortihm(int coreID) override;

    void addCore(void *core) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    int removeMissedDeadlines(int time);

    vector<Process> *getReadyQueue() override;
};


#endif //LICENTA_DARTS_H
