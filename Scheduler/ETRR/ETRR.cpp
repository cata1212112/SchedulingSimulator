//
// Created by Cata on 3/17/2024.
//

#include "ETRR.h"

vector<Event> ETRR::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> ETRR::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> ETRR::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> ETRR::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> ETRR::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}

string ETRR::getCoreAlgortihm(int coreID) {
    return std::string();
}
