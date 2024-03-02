//
// Created by cata_ on 3/2/2024.
//

#include <iostream>
#include "SRTF.h"

std::function<bool(const Process&, const Process&)> SRTF::SJFQueue = [](const Process &a, const Process &b) {
    return a.getRemainingBurst() > b.getRemainingBurst();
};

vector<Event> SRTF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

vector<Event> SRTF::processCPUComplete(Process p, int time, Metrics &stats) {
//    stats.addToGanttChart(currentProcess->getId(), currentProcess->getLastStarted(), time);
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());

    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> SRTF::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        if (process.finished()) {
            stats.addToTT(time - process.getArrivalTime());
        } else {
            addToReadyQueue.push_back(process);
        }
    }
    for (auto p:addToReadyQueue) {
        readyQueue->push(p);
    }
    return {};
}

vector<Event> SRTF::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

std::vector<Event> SRTF::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr && !readyQueue->empty()) {
        currentProcess = new Process(readyQueue->top());
        currentProcess->setLastStarted(time);
        readyQueue->pop();

        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }

        int remainingBurst = currentProcess->getRemainingBurst();

        stats.addToWT(time - currentProcess->getEnteredReadyQueue());

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
    } else if (currentProcess != nullptr && !readyQueue->empty() && readyQueue->top().getRemainingBurst() < currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted())) {
        currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
        currentProcess->setEnteredReadyQueue(time);
        readyQueue->push(*currentProcess);
        stats.addToGanttChart(currentProcess->getId(), currentProcess->getLastStarted(), time);


        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }

        stats.addToCPUUtilization(time - currentProcess->getLastStarted());
        Event e(PREEMT, -1000, *currentProcess);
        currentProcess = new Process(readyQueue->top());
        currentProcess->setLastStarted(time);
        readyQueue->pop();


        stats.addToWT(time - currentProcess->getEnteredReadyQueue());
        int remainingBurst = currentProcess->getRemainingBurst();


        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst())), e};
    }
    return {};
}
