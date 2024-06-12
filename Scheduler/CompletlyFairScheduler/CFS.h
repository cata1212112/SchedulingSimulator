//
// Created by Cata on 3/10/2024.
//

#ifndef LICENTA_CFS_H
#define LICENTA_CFS_H


#include "../SchedulingAlgorithm.h"
#include "SingleCoreCFS.h"
#include "../../DiscreteEventSimulator/Core/Core.h"
#include <iostream>

class CFS : public SchedulingAlgorithm {
    int sched_latency;
    int sched_nr_latency;
    int sched_min_granularity;
    int load_balanicng_period = 50;
    priority_queue<Event> *mainEventQueue;

    vector<Core*> cores;

    constexpr static const int prio_to_weight[40] = {
/* -20 */ 88761, 71755, 56483, 46273, 36291,
/* -15 */ 29154, 23254, 18705, 14949, 11916,
/* -10 */ 9548, 7620, 6100, 4904, 3906,
/* -5 */ 3121, 2501, 1991, 1586, 1277,
/* 0 */ 1024, 820, 655, 526, 423,
/* 5 */ 335, 272, 215, 172, 137,
/* 10 */ 110, 87, 70, 56, 45,
/* 15 */ 36, 29, 23, 18, 15,
    };
public:
    int assignCPU(Process p) override;

    void addCore(void *core) override;

    bool isRunning() override;

    int getAbsoluteDeadline() override;

public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    int getSchedLatency() const;

    void setSchedLatency(int schedLatency);

    int getSchedNrLatency() const;

    void setSchedNrLatency(int schedNrLatency);

    int getSchedMinGranularity() const;

    void setSchedMinGranularity(int schedMinGranularity);

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    void getMaximumVtimeDiff(Metrics &stats) override;

    int loadBalance(int time) override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    vector<double> getVtimesCore(int coreID);

};


#endif //LICENTA_CFS_H
