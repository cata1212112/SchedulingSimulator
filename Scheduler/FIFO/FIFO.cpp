//
// Created by Cata on 2/29/2024.
//

#include "FIFO.h"

std::function<bool(const Process&, const Process&)> FIFO::FIFOQueue = [](const Process &a, const Process &b) {
    if (a.getArrivalTime() == b.getArrivalTime()) {
        return a.getId() < b.getId();
    }
    return a.getArrivalTime() < b.getArrivalTime();
};

std::vector<Event> FIFO::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

std::vector<Event> FIFO::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> FIFO::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->top());
        readyQueue->pop();

        assignProcessToCPU(*currentProcess, stats, time);
        int remainingBurst = currentProcess->getRemainingBurst();

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, *currentProcess)};
    }
    return {};
}

string FIFO::getCoreAlgortihm(int coreID) {
    return "First In First Out";
}
