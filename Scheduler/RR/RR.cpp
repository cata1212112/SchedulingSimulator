//
// Created by cata_ on 3/2/2024.
//

#include <functional>
#include <iostream>
#include "RR.h"

std::function<bool(const Process&, const Process&)> RR::RRQueue = [](const Process &a, const Process &b) {
    return a.getArrivalTime() < b.getArrivalTime();
};

vector<Event> RR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push_back(process);
    }
    lastArrived = time;
    return {};
}

vector<Event> RR::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> RR::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr) {
        if (readyQueue->empty()) {
            return {};
        }
        return {assignToCPU(time, stats)};
    } else if (timerExpired){
        RR::preemtCPU(stats, time);

        return {assignToCPU(time, stats)};
    }
    return {};
}

string RR::getCoreAlgortihm(int coreID) {
    return "Round Robin";
}

RR::RR() {
    quant = 10;
}

void RR::preemtCPU(Metrics &stats, int time) {
    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - quant);
    currentProcess->setEnteredReadyQueue(time);
    readyQueue->push_back(*currentProcess);

    SchedulingAlgorithm::preemtCPU(stats, time);
}
