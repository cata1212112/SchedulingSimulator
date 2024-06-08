//
// Created by cata_ on 3/2/2024.
//

#ifndef LICENTA_SJF_H
#define LICENTA_SJF_H

#include <functional>
#include "../SchedulingAlgorithm.h"

class SJF : public SchedulingAlgorithm{
private:
    static std::function<bool(const Process&, const Process&)> SJFQueue;

    WrapperPriorityQueue<decltype(SJFQueue)>* readyQueue;

public:
    SJF() {
        readyQueue = new WrapperPriorityQueue<decltype(SJFQueue)>(SJFQueue);
    }

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_SJF_H
