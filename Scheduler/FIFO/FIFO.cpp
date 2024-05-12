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
    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

std::vector<Event> FIFO::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        process.setEnteredReadyQueue(time);
        addToReadyQueue.push_back(process);
    }
    for (auto p:addToReadyQueue) {
        readyQueue->push(p);
    }
    return {};
}

std::vector<Event> FIFO::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return {};
}

std::vector<Event> FIFO::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        stats.incrementCS();

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

string FIFO::getCoreAlgortihm(int coreID) {
    return "First In First Out";
}
