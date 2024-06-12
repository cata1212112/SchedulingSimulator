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

    int sched_latency = 6;
    int sched_nr_latency = 8;
    int sched_min_granularity = 1;
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

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    int getTimeSlice();

    long long int getLoad(int time, bool preemt = false) override;

    string getCoreAlgortihm(int coreID) override;

    vector<Process> *getReadyQueue() override;

    bool isRunning() override;

    void preemtCPU(Metrics &stats, int time) override;

    vector<double> getVtimes() override;

    Event assignToCPU(int time, Metrics &stats) {
        int minIndex = 0;
        int sumOfWeights = 0;
        for (int i=0; i<readyQueue.size(); i++) {
            if (readyQueue[i].getPtime() * prio_to_weight[20] / prio_to_weight[readyQueue[i].getPriority()] < readyQueue[minIndex].getPtime() * prio_to_weight[20] / prio_to_weight[readyQueue[minIndex].getPriority()]) {
                minIndex = i;
            }
            sumOfWeights += prio_to_weight[readyQueue[i].getPriority()];
        }

        int P = 0;
        if (readyQueue.size() < sched_nr_latency) {
            P = sched_latency;
        } else {
            P = sched_min_granularity * readyQueue.size();
        }

        currentProcess = new Process(readyQueue[minIndex]);

        readyQueue.erase(readyQueue.begin() + minIndex);

        int timeSclice = max(P * prio_to_weight[currentProcess->getPriority()] / sumOfWeights, 1);

        currentProcess->setTimeSlice(timeSclice);
        if (currentProcess->getRemainingBurst() <= timeSclice) {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), *currentProcess);

        } else {
            assignProcessToCPU(*currentProcess, stats, time);
            return Event{TIMEREXPIRED, time + timeSclice, *currentProcess};
        }
    }
};


#endif //LICENTA_SINGLECORECFS_H
