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

Process::Process(const Process &other) {
    id = other.id;
    CPUburst = other.CPUburst;
    IOburst = other.IOburst;
    priority = other.priority;
    currentBurst = other.currentBurst;
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
