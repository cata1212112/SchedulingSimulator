//
// Created by cata_ on 5/6/2024.
//

#ifndef LICENTA_FSSCORE_H
#define LICENTA_FSSCORE_H

#include "../SchedulingAlgorithm.h"


class FSSCore : public SchedulingAlgorithm{
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

    int sched_latency = 10;
    int sched_nr_latency = 10;
    int sched_min_granularity = 1;
    vector<Process> readyQueue;
    int numProcs = 0;
    int isIdle = false;
    std::priority_queue<Event> *eventQueue = nullptr;
    Metrics *workaroundStats = nullptr;

    bool isThrottled = false;
    int throttleMaixmum = 1e9;
public:
    void setThrottleMaixmum(int throttleMaixmum);

    void setIsThrottled(bool isThrottled);

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


#endif //LICENTA_FSSCORE_H
