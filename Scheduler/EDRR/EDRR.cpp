//
// Created by Cata on 3/16/2024.
//

#include <iostream>
#include "EDRR.h"

std::function<bool(const Process&, const Process&)> EDRR::RRQueue = [](const Process &a, const Process &b) {
    return a.getArrivalTime() > b.getArrivalTime();
};

vector<Event> EDRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    if (p.size() > 0) {
        lastArrived = time;
    }
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push(process);
    }
    return {};
}

vector<Event> EDRR::processCPUComplete(Process p, int time, Metrics &stats) {
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

vector<Event> EDRR::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
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

vector<Event> EDRR::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> EDRR::schedule(int time, Metrics &stats, bool timerExpired) {
    int maximumBurstTime = -1;

    std::queue<Process> readyQueueCopy;
    while (!readyQueue->empty()) {
        maximumBurstTime = max(maximumBurstTime, readyQueue->front().getRemainingBurst());
        readyQueueCopy.push(readyQueue->front());
        readyQueue->pop();
    }

    while (!readyQueueCopy.empty()) {
        readyQueue->push(readyQueueCopy.front());
        readyQueueCopy.pop();
    }

    if (lastArrived == time) {
        quant = int(0.6 * (maximumBurstTime + 0.0));
    }
    std::cout << quant << "\n";

    if (currentProcess == nullptr && !readyQueue->empty()) {
        bool assigned = false;
        int numProcs = readyQueue->size();
        int i = 0;

        while (i < numProcs) {
            Process p = readyQueue->front();
            readyQueue->pop();
            if (currentProcess != nullptr) {
                readyQueue->push(p);
            } else {
                if (p.getRemainingBurst() <= quant) {
                    currentProcess = new Process(p);
                    currentProcess->setLastStarted(time);

                    if (!currentProcess->getAssigned()) {
                        stats.addToRT(time - currentProcess->getArrivalTime());
                        currentProcess->setAssigned(true);
                    }

                    stats.addToWT(time - currentProcess->getEnteredReadyQueue());
                } else {
                    readyQueue->push(p);
                }
            }
            i++;
        }
        if (currentProcess != nullptr) {
            int remainingBurst = currentProcess->getRemainingBurst();
            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
        }
        quant = maximumBurstTime;
        currentProcess = new Process(readyQueue->front());
        readyQueue->pop();
        currentProcess->setLastStarted(time);
        if (!currentProcess->getAssigned()) {
            stats.addToRT(time - currentProcess->getArrivalTime());
            currentProcess->setAssigned(true);
        }
        int remainingBurst = currentProcess->getRemainingBurst();
        stats.addToWT(time - currentProcess->getEnteredReadyQueue());
        return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
    }
    return  {};
}

string EDRR::getCoreAlgortihm(int coreID) {
    return "Efficient Dynamic Round Robin";
}

EDRR::EDRR() {
    readyQueue = new std::queue<Process>();
}
