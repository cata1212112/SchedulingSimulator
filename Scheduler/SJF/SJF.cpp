//
// Created by cata_ on 3/2/2024.
//

#include "SJF.h"

std::function<bool(const Process&, const Process&)> SJF::SJFQueue = [](const Process &a, const Process &b) {
    return a.getRemainingBurst() < b.getRemainingBurst();
};

vector<Event> SJF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

vector<Event> SJF::processCPUComplete(Process p, int time, Metrics &stats) {
    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> SJF::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        if (process.finished()) {
            stats.addToTT(time - process.getArrivalTime());
        } else {
            process.setEnteredReadyQueue(time);
            addToReadyQueue.push_back(process);
        }
    }
    for (auto p:addToReadyQueue) {
        readyQueue->push(p);
    }
    return {};
}

vector<Event> SJF::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

std::vector<Event> SJF::schedule(int time, Metrics &stats, bool timerExpired) {
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

string SJF::getCoreAlgortihm(int coreID) {
    return "Shortest Job First";
}
