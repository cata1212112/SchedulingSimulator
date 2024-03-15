//
// Created by Cata on 3/15/2024.
//

#include "TestMultiCore1.h"

vector<Event> TestMultiCore1::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore1::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore1::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore1::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore1::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}

int TestMultiCore1::assignCPU(Process p) {
    if (p.getId()%2 == 0) {
        return 0;
    } else {
        return 1;
    }
}

string TestMultiCore1::getCoreAlgortihm(int coreID) {
    if (coreID == 0) {
        return "First In First Out";
    } else {
        return "Shortest Remaining Time First";
    }
}
