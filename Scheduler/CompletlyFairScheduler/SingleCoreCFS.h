//
// Created by Cata on 3/11/2024.
//

#ifndef LICENTA_SINGLECORECFS_H
#define LICENTA_SINGLECORECFS_H

#include "../SchedulingAlgorithm.h"
#include <iostream>


class SingleCoreCFS : public SchedulingAlgorithm{
private:
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

    constexpr static const int sched_prio_to_wmult[40] = {
            /* -20 */     48388,     59856,     76040,     92818,    118348,
            /* -15 */    147320,    184698,    229616,    287308,    360437,
            /* -10 */    449829,    563644,    704093,    875809,   1099582,
            /*  -5 */   1376151,   1717300,   2157191,   2708050,   3363326,
            /*   0 */   4194304,   5237765,   6557202,   8165337,  10153587,
            /*   5 */  12820798,  15790321,  19976592,  24970740,  31350126,
            /*  10 */  39045157,  49367440,  61356676,  76695844,  95443717,
            /*  15 */ 119304647, 148102320, 186737708, 238609294, 286331153,
    };

    int sched_latency = 100;
    int sched_nr_latency = 10;
    int sched_min_granularity = 10;
    vector<Process> readyQueue;
    int numProcs = 0;
    int isIdle = false;
    std::priority_queue<Event> *eventQueue = nullptr;
    Metrics *workaroundStats = nullptr;
public:
    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    int getIsIdle() const;

    void addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) override;

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

    int getTimeSlice();

    long long int getLoad(int time, bool preemt = false) override;

    string getCoreAlgortihm(int coreID) override;

    vector<Process> *getReadyQueue() override;

    bool isRunning() override;
};


#endif //LICENTA_SINGLECORECFS_H
