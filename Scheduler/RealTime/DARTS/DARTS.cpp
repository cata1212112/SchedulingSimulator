//
// Created by Cata on 5/21/2024.
//

#include <iostream>
#include "DARTS.h"

vector<Event> DARTS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push_back(proc);
    }
    return {};
}

vector<Event> DARTS::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DARTS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DARTS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> DARTS::schedule(int time, Metrics &stats, bool timerExpired) {
    int missed = removeMissedDeadlines(time);
    if (missed > 0) {
        stats.incrementCS();
    }
    sort(readyQueue->begin(), readyQueue->end(), [time](const Process &a, const Process &b) {

        double aDynamicScore = (a.getRemainingBurst() + 0.0) / ((a.getNextDeadline() - time + 0.0) * ((a.getNextDeadline() - a.getRemainingBurst() - time + 0.0)));
        double bDynamicScore = (b.getRemainingBurst() + 0.0) / ((b.getNextDeadline() - time + 0.0) * ((b.getNextDeadline() - b.getRemainingBurst() - time + 0.0)));

       return aDynamicScore > bDynamicScore;
    });
    for (int i=0; i<min(cores.size(), readyQueue->size()); i++) {
        Process p((*readyQueue)[i]);

        Process p2(p);
        p2.setRemainingBurst(1);
        cores[i]->addEvent(Event(ARRIVAL, time, p2));
//        std::cout << time << " " << "Core " << i << " Task " << p2.getId() << " " << 1 << "\n";

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

int DARTS::assignCPU(Process p) {
    Core* freeCore = nullptr;
    for (auto core:cores) {
        if (!core->running()) {
            freeCore = core;
        }
    }
    return freeCore->getCoreId();
}

string DARTS::getCoreAlgortihm(int coreID) {
    return "DARTSCore";
}

void DARTS::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

bool DARTS::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int DARTS::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

DARTS::DARTS() {
    readyQueue = new vector<Process>();
}

void DARTS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}

int DARTS::removeMissedDeadlines(int time) {
    vector<int> toDelete;
    for (int i=0; i<readyQueue->size(); i++) {
        if ((*readyQueue)[i].getNextDeadline() <= time) {
            toDelete.push_back(i);
        }
    }

    for (auto it=toDelete.rbegin(); it != toDelete.rend(); it ++) {
        readyQueue->erase(readyQueue->begin() + *it);
    }
    return toDelete.size();
}
