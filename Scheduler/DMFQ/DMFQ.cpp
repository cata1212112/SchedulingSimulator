//
// Created by Cata on 3/17/2024.
//

#include "DMFQ.h"

vector<Event> DMFQ::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        Q[process.getPriority() / 2]->push_back(process);
    }
    return {};
}

vector<Event> DMFQ::processCPUComplete(Process p, int time, Metrics &stats) {
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());

    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
        int remainingTime = p.getRemainingBurst();
        events.push_back(Event(IOBURSTCOMPLETE, time + remainingTime, Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> DMFQ::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
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
        Q[p.getPriority() / 2]->push_back(p);
    }
    return {};
}

vector<Event> DMFQ::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DMFQ::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess != nullptr && existsProcess()) {
        for (int i=0; i<4; i++) {
            for (auto p:*Q[i]) {
                if ((time - p.getArrivalTime()) / p.getRemainingBurst() >= 1 && p.getPriority() / 2 == i + 1) {
//                    Q[i].pus
                }
            }
        }
        for (int i=0; i<4; i++) {

        }
    }
    return {};
}

string DMFQ::getCoreAlgortihm(int coreID) {
    return "Developed Multilevel Feedback Queue Scheduling";
}

DMFQ::DMFQ() {
    for (int i=0; i<5; i++) {
        Q[i] = new vector<Process>();
    }
}

bool DMFQ::existsProcess() {
    bool exists = false;
    for (int i=0; i<5; i++) {
        exists = (exists || !Q[i]->empty());
    }
    return exists;
}
