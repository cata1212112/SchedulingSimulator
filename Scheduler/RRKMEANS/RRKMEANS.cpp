//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "RRKMEANS.h"


std::function<bool(const Process&, const Process&)> RRKMEANS::RRQueue = [](const Process &a, const Process &b) {
    return a.getArrivalTime() < b.getArrivalTime();
};

RRKMEANS::RRKMEANS() {
    readyQueue = new std::vector<Process>;
}

vector<Event> RRKMEANS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    for (auto &process : p) {
        process.setEnteredReadyQueue(time);
        readyQueue->push_back(process);
    }
    clusterData();
    return {};
}

vector<Event> RRKMEANS::processCPUComplete(Process p, int time, Metrics &stats) {
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

vector<Event> RRKMEANS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    vector<Process> addToReadyQueue;
    for (auto &process:p) {
//        ioQueue -= 1;
        process.setEnteredReadyQueue(time);
        addToReadyQueue.push_back(process);
    }
    for (auto p:addToReadyQueue) {
        readyQueue->push_back(p);
    }
    return {};
}

vector<Event> RRKMEANS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> RRKMEANS::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}

string RRKMEANS::getCoreAlgortihm(int coreID) {
    return "Round Robin K-Means";
}

map<int, double> RRKMEANS::computePW() {
    map<int, double> mp;
    int sumOfBursts = 0;
    for (const auto &x:*readyQueue) {
        sumOfBursts += x.getRemainingBurst();
    }
    for (const auto &x:*readyQueue) {
        mp[x.getId()] = (x.getRemainingBurst() + 0.0) / (sumOfBursts + 0.0);
    }
    return mp;
}

map<int, int> RRKMEANS::computeNCS() {
    map<int, int> mp;
    for (const auto &x:*readyQueue) {
        if (x.getRemainingBurst() / STS * STS == x.getRemainingBurst()) {
            mp[x.getId()] = x.getRemainingBurst() / STS - 1;
        } else {
            mp[x.getId()] = x.getRemainingBurst() / STS;
        }
    }
    return mp;
}

void RRKMEANS::clusterData() {
    map<int, double> pw = computePW();
    map<int, int> ncs = computeNCS();

    vector<DataPoint> points;
    for (const auto &p:*readyQueue) {
        points.push_back(DataPoint(p.getRemainingBurst(), pw[p.getId()], ncs[p.getId()]));
        std::cout << p.getId() << " " << p.getRemainingBurst() << " " << pw[p.getId()] << " " << ncs[p.getId()] << "\n";
    }

    int clusterization[readyQueue->size()];
    int maximulSil = -1;
    for (int k=2; k<readyQueue->size(); k++) {
        KMEANS kmeans = KMEANS(points, k, 10);
        kmeans.cluster();
        kmeans.printClusters();
        double score = kmeans.silhouetteScore();
        cout << score << "\n";
        if (score > maximulSil) {
            maximulSil = score;
            for (int i=0; i<readyQueue->size(); i++) {
                clusterization[i] = kmeans.getCluster(i);
            }
            clustering = kmeans;
        }
    }
}
