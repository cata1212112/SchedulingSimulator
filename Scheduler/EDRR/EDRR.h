//
// Created by Cata on 3/16/2024.
//

#ifndef LICENTA_EDRR_H
#define LICENTA_EDRR_H

#include "../SchedulingAlgorithm.h"
#include <functional>
#include <queue>

class EDRR : public SchedulingAlgorithm{
private:
    int quant;
    int lastArrived = 0;

    static std::function<bool(const Process&, const Process&)> RRQueue;

    std::queue<Process>* readyQueue;
public:
    EDRR();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;
};


#endif //LICENTA_EDRR_H
