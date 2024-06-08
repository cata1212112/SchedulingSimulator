//
// Created by cata_ on 3/2/2024.
//

#include <iostream>
#include "SRTF.h"

std::function<bool(const Process&, const Process&)> SRTF::SJFQueue = [](const Process &a, const Process &b) {
    return a.getRemainingBurst() < b.getRemainingBurst();
};

std::vector<Event> SRTF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

std::vector<Event> SRTF::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> SRTF::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr) {
        if (readyQueue->empty()) {
            return {};
        }
        return {assignToCPU(time, stats)};
    } else {
        if (readyQueue->top().getRemainingBurst() <= currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted())) {
            Event preemtEvent(PREEMT, -1000, *currentProcess);

            preemtCPU(stats, time);

            return {preemtEvent, assignToCPU(time, stats)};
        }
    }
    return {};
}

string SRTF::getCoreAlgortihm(int coreID) {
    return "Shortest Remaining Time First";
}

void SRTF::preemtCPU(Metrics &stats, int time) {

    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
    currentProcess->setEnteredReadyQueue(time);
    readyQueue->push(*currentProcess);

    SchedulingAlgorithm::preemtCPU(stats, time);
}
