//
// Created by cata_ on 3/2/2024.
//

#include <functional>
#include "RR.h"

std::function<bool(const Process&, const Process&)> RR::RRQueue = [](const Process &a, const Process &b) {
    return a.getArrivalTime() > b.getArrivalTime();
};

vector<Event> RR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

vector<Event> RR::processCPUComplete(Process p, int time, Metrics &stats) {
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

vector<Event> RR::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
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

vector<Event> RR::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

std::vector<Event> RR::schedule(int time, Metrics &stats, bool timerExpired) {
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

        if (remainingBurst > quant) {

            return {Event{TIMEREXPIRED, time + quant, Process(*currentProcess)}};
        }

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
    } else if (timerExpired) {
        if (readyQueue->empty()) {
            return {};
        }
        currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
        currentProcess->setEnteredReadyQueue(time);

        Process currentCopy(*currentProcess);
        stats.addToGanttChart(currentProcess->getId(), currentProcess->getLastStarted(), time);

        stats.addToCPUUtilization(time - currentProcess->getLastStarted());
        Event e(PREEMT, -1000, currentCopy);
        currentProcess = new Process(readyQueue->top());
        currentProcess->setLastStarted(time);
        readyQueue->pop();
        readyQueue->push(currentCopy);

        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }

        stats.addToWT(time - currentProcess->getEnteredReadyQueue());
        int remainingBurst = currentProcess->getRemainingBurst();
        if (remainingBurst > quant) {

            return {Event{TIMEREXPIRED, time + quant, Process(*currentProcess)}};
        }

        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};

    }
    return {};
}

string RR::getCoreAlgortihm(int coreID) {
    return "Round Robin";
}
