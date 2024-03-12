//
// Created by Cata on 3/11/2024.
//

#include "SingleCoreCFS.h"

vector<Event> SingleCoreCFS::processArrived(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::processCPUComplete(Process p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::processIOComplete(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::processPreempt(std::vector<Process> p, int time, Metrics &stats) {
    return std::vector<Event>();
}

vector<Event> SingleCoreCFS::schedule(int time, Metrics &stats, bool timerExpired) {
    return std::vector<Event>();
}
