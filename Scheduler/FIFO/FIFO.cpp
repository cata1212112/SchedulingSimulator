//
// Created by Cata on 2/29/2024.
//

#include "FIFO.h"

std::function<bool(const Process&, const Process&)> FIFO::FIFOQueue = [](const Process &a, const Process &b) {
    return false;
};

std::vector<Event> FIFO::processArrived(Process p, int time, Metrics &stats) {
    readyQueue->push(p);
    return schedule(time, stats);
}

std::vector<Event> FIFO::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    std::vector<Event> events = schedule(time, stats);
//    waitingQueue->push(p);
    events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    return events;
}

std::vector<Event> FIFO::processIOComplete(Process p, int time, Metrics &stats) {
    if (p.finished()) {
        return {};
    } else {
        readyQueue->push(p);
        return schedule(time, stats);
    }
}

std::vector<Event> FIFO::processPreempt(Process p, int time, Metrics &stats) {
    return {};
}

vector<Event> FIFO::schedule(int time, Metrics &stats) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->top());
        readyQueue->pop();

        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());

        return {Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), Process(*currentProcess->consumeBurst()))};
    }
    return {};
}
