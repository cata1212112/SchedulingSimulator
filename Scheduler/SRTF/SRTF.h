//
// Created by cata_ on 3/2/2024.
//

#ifndef LICENTA_SRTF_H
#define LICENTA_SRTF_H

#include <functional>
#include "../SchedulingAlgorithm.h"

class SRTF : public SchedulingAlgorithm{
private:
    static std::function<bool(const Process&, const Process&)> SJFQueue;

    WrapperPriorityQueue<decltype(SJFQueue)>* readyQueue;
public:
    SRTF() {
        readyQueue = new WrapperPriorityQueue<decltype(SJFQueue)>(SJFQueue);
    }

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    string getCoreAlgortihm(int coreID) override;

    std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    Event assignToCPU(int time, Metrics &stats) {
        currentProcess = new Process(readyQueue->top());
        readyQueue->pop();
        assignProcessToCPU(*currentProcess, stats, time);
        return Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), *currentProcess);
    }

    void preemtCPU(Metrics &stats, int time) override;
};


#endif //LICENTA_SRTF_H
