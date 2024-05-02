//
// Created by Cata on 3/11/2024.
//

#include "SingleCoreCFS.h"

vector<Event> SingleCoreCFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
    }
    return {};
}

vector<Event> SingleCoreCFS::processCPUComplete(Process p, int time, Metrics &stats) {
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

vector<Event> SingleCoreCFS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
        process.setEnteredReadyQueue(time);
        addToReadyQueue.push_back(process);
    }
    for (auto p:addToReadyQueue) {
        readyQueue.push_back(p);
    }
    return {};
}

vector<Event> SingleCoreCFS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}
