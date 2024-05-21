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
 110, 87, 70, 56, 45,
  36, 29, 23, 18, 15,
    };

    int sched_latency = 30;
    int sched_nr_latency = 10;
    int sched_min_granularity = 3;
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

    vector<double> getVtimes() override;
};


#endif //LICENTA_SINGLECORECFS_H
