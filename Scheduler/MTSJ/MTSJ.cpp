//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "MTSJ.h"

std::vector<Event> MTSJ::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
    }
    if (!p.empty()) {
        lastArrived = time;
    }
    return {};
}

std::vector<Event> MTSJ::processCPUComplete(Process p, int time, Metrics &stats) {
    return SchedulingAlgorithm::processCPUComplete(p, time, stats);
}

std::vector<Event> MTSJ::schedule(int time, Metrics &stats, bool timerExpired) {
    vector<Event> events;
    int preemtedProcId = -1;
    if (time == lastArrived) {
        if (currentProcess != nullptr) {
            Event preemtEvent(PREEMT, -1000, *currentProcess);
            preemtedProcId = currentProcess->getId();
            preemtCPU(stats, time);
            events.push_back(preemtEvent);
        }


        for (auto p:sjfQueue) {
            readyQueue.push_back(p);
        }

        sjfQueue.clear();

        for (auto p:rrQueue) {
            readyQueue.push_back(p);
        }

        rrQueue.clear();

        int sumOfBursts = 0;
        for (const auto &p:readyQueue) {
            sumOfBursts += p.getRemainingBurst();
        }

        sumOfBursts /= readyQueue.size();

        for (auto p:readyQueue) {
            if (p.getRemainingBurst() <= sumOfBursts) {
                sjfQueue.push_back(p);
            } else {
                rrQueue.push_back(p);
            }
        }

        readyQueue.clear();
        sort(sjfQueue.begin(), sjfQueue.end(), [](const Process &a, const Process &b) {
            return a.getRemainingBurst() < b.getRemainingBurst();
        });

        cout << sumOfBursts << "\n";
        //        quant = sumOfBursts;
        lastArrived = -1;
    }
    if (!sjfQueue.empty()) {
        if (currentProcess == nullptr) {
            currentProcess = new Process(*sjfQueue.begin());
            sjfQueue.erase(sjfQueue.begin());

            assignProcessToCPU(*currentProcess, stats, time);
            int remainingBurst = currentProcess->getRemainingBurst();
            if (currentProcess->getId() == preemtedProcId) {
                events.clear();
            }
            events.push_back(Event(CPUBURSTCOMPLETE, time + remainingBurst, *currentProcess));
        }
    } else {
        if (currentProcess == nullptr && rrQueue.empty()) {
            return {};
        }
        if (currentProcess != nullptr) {
            if (!timerExpired) {
                return {};
            }
            preemtCPU(stats, time);
        }
        currentProcess = new Process(*rrQueue.begin());
        rrQueue.erase(rrQueue.begin());

        if (currentProcess->getId() == preemtedProcId) {
            events.clear();
        }

        if (currentProcess->getRemainingBurst() <= quant) {
            assignProcessToCPU(*currentProcess, stats, time);
            events.push_back(Event(CPUBURSTCOMPLETE, time + currentProcess->getRemainingBurst(), *currentProcess));

        } else {
            assignProcessToCPU(*currentProcess, stats, time);
            events.push_back(Event{TIMEREXPIRED, time + quant, *currentProcess});
        }
    }
    return events;
}

string MTSJ::getCoreAlgortihm(int coreID) {
    return "Mean Threshold Shortest Job Round Robin";
}

MTSJ::MTSJ(int quant) : quant(quant) {}

void MTSJ::preemtCPU(Metrics &stats, int time) {

    currentProcess->setRemainingBurst(currentProcess->getRemainingBurst() - (time - currentProcess->getLastStarted()));
    currentProcess->setEnteredReadyQueue(time);
    if (currentProcess->getRemainingBurst() > 0) {
        rrQueue.push_back(*currentProcess);
    }

    SchedulingAlgorithm::preemtCPU(stats, time);
}
