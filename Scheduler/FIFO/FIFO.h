//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_FIFO_H
#define LICENTA_FIFO_H

#include <functional>
#include "../SchedulingAlgorithm.h"

class FIFO : public SchedulingAlgorithm{
private:
    static std::function<bool(const Process&, const Process&)> FIFOQueue;

    WrapperPriorityQueue<Process, decltype(FIFOQueue)>* readyQueue;
//    queue<Process> *readyQueue;
public:
    FIFO() {
        readyQueue = new WrapperPriorityQueue<Process, decltype(FIFOQueue)>(FIFOQueue);
//        waitingQueue = new WrapperPriorityQueue<Process, decltype(FIFOQueue)>(FIFOQueue);
//        readyQueue = new queue<Process>();
    }

    std::vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    std::vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    std::vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    std::vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

private:
    std::vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;
};


#endif //LICENTA_FIFO_H
