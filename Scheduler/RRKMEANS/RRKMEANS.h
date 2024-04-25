//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_RRKMEANS_H
#define LICENTA_RRKMEANS_H

#include <functional>
#include "../SchedulingAlgorithm.h"
#include "../../Utils/KMEANS/DataPoint.h"
#include "../../Utils/KMEANS/KMEANS.h"

class RRKMEANS : public SchedulingAlgorithm{
private:
    static std::function<bool(const Process&, const Process&)> RRQueue;

    std::vector<Process>* readyQueue;
    vector<vector<int>> processesToRun;
    KMEANS clustering;
    int STS = 10;
public:
    RRKMEANS();

    vector<Event> processArrived(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processCPUComplete(Process p, int time, Metrics &stats) override;

    vector<Event> processIOComplete(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> processPreempt(std::vector<Process> p, int time, Metrics &stats) override;

    vector<Event> schedule(int time, Metrics &stats, bool timerExpired) override;

    string getCoreAlgortihm(int coreID) override;

    map<int, double> computePW();

    map<int, int> computeNCS();

    void clusterData();
};


#endif //LICENTA_RRKMEANS_H
