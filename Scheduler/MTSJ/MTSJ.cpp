//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "MTSJ.h"

vector<Event> MTSJ::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue.push_back(process);
    }

    if (!p.empty()) {
        for (auto &process:sjfQueue) {
            readyQueue.push_back(process);
        }

        for (auto &process:rrQueue) {
            readyQueue.push_back(process);
        }

        sjfQueue.clear();
        rrQueue.clear();

        int sumOfBursts = 0;
        for (const auto &process:readyQueue) {
            sumOfBursts += process.getRemainingBurst();
        }
        if (!readyQueue.empty()) {
            sumOfBursts /= readyQueue.size();

            for (const auto &process:readyQueue) {
                if (process.getRemainingBurst() <= sumOfBursts) {
                    sjfQueue.push_back(process);
                } else {
                    rrQueue.push_back(process);
                }
            }
            sort(sjfQueue.begin(), sjfQueue.end(), [](const Process &a, const Process &b) {return a.getRemainingBurst() < b.getRemainingBurst();});
            readyQueue.clear();
        }
    }


    return {};
}

vector<Event> MTSJ::processCPUComplete(Process p, int time, Metrics &stats) {
    stats.addToGanttChart(p.getId(), p.getLastStarted(), time);
    stats.addToCPUUtilization(time - p.getLastStarted());

    currentProcess = nullptr;
    std::vector<Event> events;
    if (p.hasRemainingIO()) {
//        ioQueue += 1;
        events.push_back(Event(IOBURSTCOMPLETE, time + p.getRemainingBurst(), Process(*p.consumeBurst())));
    } else {
        stats.addToTT(time - p.getArrivalTime());
    }
    return events;
}

vector<Event> MTSJ::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
//        ioQueue -= 1;
        process.setEnteredReadyQueue(time);
        addToReadyQueue.push_back(process);
    }
    for (auto p:addToReadyQueue) {
        readyQueue.push_back(p);
    }
    return {};
}

vector<Event> MTSJ::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> MTSJ::schedule(int time, Metrics &stats, bool timerExpired) {
    if (currentProcess == nullptr || timerExpired) {
        if (!sjfQueue.empty()) {
            currentProcess = new Process(sjfQueue[0]);
            sjfQueue.erase(sjfQueue.begin());

            if (!currentProcess->getAssigned()) {
                stats.addToRT(time - currentProcess->getArrivalTime());
                currentProcess->setAssigned(true);
            }

            stats.addToGanttChart(currentProcess->getId(), time, time + currentProcess->getRemainingBurst());
            std::cout << currentProcess->getId() << " " << time << " " << time + currentProcess->getRemainingBurst() << "\n";

            int remainingBurst = currentProcess->getRemainingBurst();

            stats.addToCPUUtilization(remainingBurst);
            stats.addToWT(time - currentProcess->getEnteredReadyQueue());

            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
        } else if (!rrQueue.empty()){
            currentProcess = new Process(rrQueue[0]);
            currentProcess->setLastStarted(time);


            if (!currentProcess->getAssigned()) {
                stats.addToRT(time - currentProcess->getArrivalTime());
                currentProcess->setAssigned(true);
            }

            int remainingBurst = currentProcess->getRemainingBurst();

            stats.addToWT(time - currentProcess->getEnteredReadyQueue());

            if (remainingBurst > quant) {
                std::cout << currentProcess->getId() << " " << time << " " << time + quant << "\n";

                rrQueue[0].setRemainingBurst(rrQueue[0].getRemainingBurst() - quant);
                stats.addToGanttChart(currentProcess->getId(), time, time+quant);
                rrQueue[0].setEnteredReadyQueue(time+quant);

                rrQueue.push_back(rrQueue[0]);
                rrQueue.erase(rrQueue.begin());

                return {Event{TIMEREXPIRED, time + quant, Process(*currentProcess)}};
            }
            std::cout << currentProcess->getId() << " " << time << " " << time + currentProcess->getRemainingBurst() << "\n";

            rrQueue.erase(rrQueue.begin());
            return {Event(CPUBURSTCOMPLETE, time + remainingBurst, Process(*currentProcess->consumeBurst()))};
        }
    }
    return {};
}

string MTSJ::getCoreAlgortihm(int coreID) {
    return "Mean Threshold Shortest Job Round Robin";
}

MTSJ::MTSJ(int quant) : quant(quant) {}
