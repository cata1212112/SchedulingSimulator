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
    WrapperPriorityQueue<Process, decltype(FIFOQueue)>* waitingQueue;
public:
    FIFO() {
        readyQueue = new WrapperPriorityQueue<Process, decltype(FIFOQueue)>(FIFOQueue);
//        waitingQueue = new WrapperPriorityQueue<Process, decltype(FIFOQueue)>(FIFOQueue);
    }

    std::vector<Event> processArrived(Process p, int time, Metrics &stats) override;

    std::vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    std::vector<Event> processIOComplete(Process p, int time, Metrics &stats) override;

    std::vector<Event> processPreempt(Process p, int time, Metrics &stats) override;

private:
    vector<Event> schedule(int time, Metrics &stats) override;
};


#endif //LICENTA_FIFO_H
