//
// Created by cata_ on 3/2/2024.
//

#include "SJF.h"

std::function<bool(const Process&, const Process&)> SJF::SJFQueue = [](const Process &a, const Process &b) {
    return a.getRemainingBurst() < b.getRemainingBurst();
};

std::vector<Event> SJF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

std::vector<Event> SJF::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> SJF::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->top());
        readyQueue->pop();

        assignProcessToCPU(*currentProcess, stats, time);
        int remainingBurst = currentProcess->getRemainingBurst();

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, *currentProcess)};
    }
    return {};
}


string SJF::getCoreAlgortihm(int coreID) {
    return "Shortest Job First";
}
