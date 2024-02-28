//
// Created by Cata on 2/12/2024.
//

#include "Process.h"

int Process::ID = 1;

Process::Process(const vector<int> &cpUburst, const vector<int> &iOburst, int priority)
        : CPUburst(cpUburst), IOburst(iOburst), priority(priority) {
    id = ID++;
    currentBurst = 0;
}

const vector<int> &Process::getCpUburst() const {
    return CPUburst;
}

const vector<int> &Process::getIOburst() const {
    return IOburst;
}

int Process::getPriority() const {
    return priority;
}

int Process::getId() const {
    return id;
}

Process::Process() : currentBurst(0) {
    ID++;
}

void Process::setCpUburst(const vector<int> &cpUburst) {
    CPUburst = cpUburst;
}

void Process::setIOburst(const vector<int> &iOburst) {
    IOburst = iOburst;
}

void Process::setPriority(int priority) {
    Process::priority = priority;
}

void Process::setId(int id) {
    Process::id = id;
}
