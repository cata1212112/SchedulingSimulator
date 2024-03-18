//
// Created by Cata on 3/17/2024.
//

#include "TestMultiCore2.h"

vector<Event> TestMultiCore2::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore2::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore2::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore2::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> TestMultiCore2::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}

string TestMultiCore2::getCoreAlgortihm(int coreID) {
        if (coreID == 0) {
            return "Round Robin";
        } else if (coreID == 1) {
            return "First In First Out";
        } else if (coreID == 2) {
            return "Shortest Job First";
        } else {
            return "Shortest Remaining Time First";
        }
}

int TestMultiCore2::assignCPU(Process p) {
    return p.getId()%4;
}

TestMultiCore2::TestMultiCore2() {}
