//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_MTSJ_H
#define LICENTA_MTSJ_H


#include "../SchedulingAlgorithm.h"
#include <algorithm>

class MTSJ : public SchedulingAlgorithm{
private:
    vector<Process> readyQueue;
    vector<Process> sjfQueue;
    vector<Process> rrQueue;
    int quant = 10;
    int lastArrived = -1;
public:
    explicit MTSJ(int quant);

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;


    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    void preemtCPU(Metrics &stats, int time) override;
};


#endif //LICENTA_MTSJ_H
