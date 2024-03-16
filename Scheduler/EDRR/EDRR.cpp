//
// Created by Cata on 3/16/2024.
//

#include "EDRR.h"

std::function<bool(const Process&, const Process&)> EDRR::RRQueue = [](const Process &a, const Process &b) {
    return a.getArrivalTime() > b.getArrivalTime();
};

vector<Event> EDRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
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
}

string EDRR::getCoreAlgortihm(int coreID) {
    return "Efficient Dynamic Round Robin";
}

EDRR::EDRR() {
    readyQueue = new WrapperPriorityQueue<Process, decltype(RRQueue)>(RRQueue);
}
