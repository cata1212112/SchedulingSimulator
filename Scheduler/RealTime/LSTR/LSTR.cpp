//
// Created by Cata on 3/22/2024.
//

#include "LSTR.h"

vector<Event> LSTR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (const auto&proc:p) {
        readyQueue->push_back(proc);
    }
    return {};
}

vector<Event> LSTR::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LSTR::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LSTR::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> LSTR::schedule(int time, Metrics &stats, bool timerExpired) {
    sort(readyQueue->begin(), readyQueue->end(), [time](const Process &a, const Process &b) {
        int aDiff = a.getNextDeadline() - time;
        int bDiff = b.getNextDeadline() - time;

        if (aDiff == 0) {
            return true;
        }

        if (bDiff == 0) {
            return false;
        }

        double aRank = ((a.getRemainingBurst() + 0.0) / (a.getNextDeadline() - time + 0.0));
        double bRank = ((b.getRemainingBurst() + 0.0) / (b.getNextDeadline() - time + 0.0));

        if (aRank == bRank) {
            return a.getId() < b.getId();
        }

        return aRank > bRank;
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

int LSTR::assignCPU(Process p) {
    Core* freeCore = nullptr;
    for (auto core:cores) {
        if (!core->running()) {
            freeCore = core;
        }
    }
    return freeCore->getCoreId();
}

string LSTR::getCoreAlgortihm(int coreID) {
    return "LSTRCore";
}

void LSTR::addCore(void *core) {
    cores.push_back(reinterpret_cast<Core*>(core));
}

bool LSTR::isRunning() {
    return SchedulingAlgorithm::isRunning();
}

int LSTR::getAbsoluteDeadline() {
    return SchedulingAlgorithm::getAbsoluteDeadline();
}

LSTR::LSTR() {
    readyQueue = new vector<Process>();
}

void LSTR::addMainEventQueue(priority_queue<Event> *eventQueue, mutex *m) {
    mainEventQueue = eventQueue;
}
