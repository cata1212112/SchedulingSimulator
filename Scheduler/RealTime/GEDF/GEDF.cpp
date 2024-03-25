//
// Created by Cata on 3/22/2024.
//

#include "GEDF.h"
#include <iostream>

vector<Event> GEDF::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push_back(proc);
    }
    return {};
}

vector<Event> GEDF::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> GEDF::schedule(int time, Metrics &stats, bool timerExpired) {
    sort(readyQueue->begin(), readyQueue->end(), [](const Process &a, const Process &b) {
        if (a.getAbsoluteDeadline() == b.getAbsoluteDeadline()) {
            return a.getId() < b.getId();
        }
        return a.getAbsoluteDeadline() <= b.getAbsoluteDeadline();
    });
    for (int i=0; i<min(cores.size(), readyQueue->size()); i++) {
        Process p((*readyQueue)[i]);

        Process p2(p);
        p2.setRemainingBurst(1);
        cores[i]->addEvent(Event(ARRIVAL, time, p2));

        Process p3(p);
        p3.setRemainingBurst(p3.getRemainingBurst() - 1);
        p3.setAbsoluteDeadline(p3.getAbsoluteDeadline() - 1);

        if (p3.getRemainingBurst() > 0) {
            mainEventQueue->push(Event(REALTIME, time+1, p3));
        }
    }
    readyQueue->erase(readyQueue->begin(), readyQueue->begin() + min(cores.size(), readyQueue->size()));
    for (auto &p:*readyQueue) {
        p.setAbsoluteDeadline(p.getAbsoluteDeadline()-1);
    }
    mainEventQueue->push(Event(TICK, time+1, Process()));

    return std::vector<Event>();
}

string GEDF::getCoreAlgortihm(int coreID) {
    return "GEDFCore";
}

GEDF::GEDF() {
    readyQueue = new vector<Process>();
}

int GEDF::assignCPU(Process p) {

}

void GEDF::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

void GEDF::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}
