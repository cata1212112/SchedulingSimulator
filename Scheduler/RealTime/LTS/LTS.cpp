//
// Created by Cata on 5/21/2024.
//

#include <iostream>
#include "LTS.h"

vector<Event> LTS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push_back(proc);

        if (res.find(proc.getId()) == res.end()) {
            res[proc.getId()] = 1 - (proc.getRemainingBurst() + 0.0) / (proc.getPeriod() + 0.0);
            totalRes[proc.getId()] = 0;
        }
    }
    return {};
}

vector<Event> LTS::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LTS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LTS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LTS::schedule(int time, Metrics &stats, bool timerExpired) {
    int missed = removeMissedDeadlines(time);
    if (missed > 0) {
        stats.incrementCS();
    }
    P.clear();
    for (const auto &proc:*readyQueue) {
        if (proc.getNextDeadline() - time - proc.getRemainingBurst() == 0) {
            P[proc.getId()] = 1e9;
        } else {
            P[proc.getId()] = (proc.getRemainingBurst() + 0.0) / (proc.getNextDeadline() - time + 0.0) + (1 - (proc.getRemainingBurst() + 0.0) / (proc.getNextDeadline() - time + 0.0)) / ((proc.getRemainingBurst() + 0.0) / (proc.getNextDeadline() - time + 0.0)) * (time%proc.getNextDeadline() - time) - totalRes[proc.getId()];
        }
    }
    sort(readyQueue->begin(), readyQueue->end(), [this](const Process &a, const Process &b) {
        return P[a.getId()] > P[b.getId()];
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

int LTS::assignCPU(Process p) {
    Core* freeCore = nullptr;
    for (auto core:cores) {
        if (!core->running()) {
            freeCore = core;
        }
    }
    return freeCore->getCoreId();
}

string LTS::getCoreAlgortihm(int coreID) {
    return "LTSCore";
}

void LTS::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

bool LTS::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int LTS::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

LTS::LTS() {
    readyQueue = new vector<Process>();
}

void LTS::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}

int LTS::removeMissedDeadlines(int time) {
    vector<int> toDelete;
    for (int i=0; i<readyQueue->size(); i++) {
        if ((*readyQueue)[i].getNextDeadline() < time) {
            toDelete.push_back(i);
        }
    }

    for (auto it=toDelete.rbegin(); it != toDelete.rend(); it ++) {
        readyQueue->erase(readyQueue->begin() + *it);
    }
    return toDelete.size();

}
