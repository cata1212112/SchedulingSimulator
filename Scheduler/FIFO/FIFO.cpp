//
// Created by Cata on 2/29/2024.
//

#include "FIFO.h"

std::function<bool(const Process&, const Process&)> FIFO::FIFOQueue = [](const Process &a, const Process &b) {
    return false;
};

std::vector<Event> FIFO::processArrived(Process p, int time, Metrics &stats) {
    p.setEnteredReadyQueue(time);
    readyQueue->push(p);
    return schedule(time, stats);
}

std::vector<Event> FIFO::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    std::vector<Event> events = schedule(time, stats);
    if (p.hasRemainingIO()) {
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

std::vector<Event> FIFO::processIOComplete(Process p, int time, Metrics &stats) {
    if (p.finished()) {
        stats.addToTT(time - p.getArrivalTime());
        return {};
    } else {
        p.setEnteredReadyQueue(time);
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

        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }

        stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());

        int remainingBurst = currentProcess->getRemainingBurst();

        stats.addToCPUUtilization(remainingBurst);
        stats.addToWT(time - currentProcess->getEnteredReadyQueue());

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
    }
    return {};
}
