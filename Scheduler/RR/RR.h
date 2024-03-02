//
// Created by cata_ on 3/2/2024.
//

#ifndef LICENTA_RR_H
#define LICENTA_RR_H

#include "../SchedulingAlgorithm.h"

class RR : public SchedulingAlgorithm{
private:
    int quant;

    static std::function<bool(const Process&, const Process&)> RRQueue;

    WrapperPriorityQueue<Process, decltype(RRQueue)>* readyQueue;
public:
    int getQuant() const {
        return quant;
    }

    void setQuant(int quant) {
        RR::quant = quant;
    }

    RR(int quant) :quant(quant){
        readyQueue = new WrapperPriorityQueue<Process, decltype(RRQueue)>(RRQueue);
    }

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;
};


#endif //LICENTA_RR_H
