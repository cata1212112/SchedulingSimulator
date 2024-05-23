//
// Created by cata_ on 5/6/2024.
//

#ifndef LICENTA_FSS_H
#define LICENTA_FSS_H


#include "../SchedulingAlgorithm.h"
#include "../../DiscreteEventSimulator/Core/Core.h"
#include <algorithm>
#include <math.h>

class FSS : public SchedulingAlgorithm{
    int sched_latency;
    int sched_nr_latency;
    int sched_min_granularity;
    int load_balanicng_period = 100;
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

    int loadBalance(int time) override;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

    vector<pair<vector<Process>, double>> progressBalancing(int numCores, vector<vector<Process>> taskGroups);

    vector<vector<Process>> partition(int numCores, vector<Process> S);

    pair<vector<Process>, vector<Process>> binaryPartition(vector<Process> S);

    vector<double> throttle(vector<vector<Process>> taskGroups);

    void getMaximumVtimeDiff(Metrics &stats) override;

    vector<double> getVtimesCore(int coreID);
};


#endif //LICENTA_FSS_H
