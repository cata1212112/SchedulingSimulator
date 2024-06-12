//
// Created by Cata on 2/12/2024.
//

#include "Process.h"

int Process::ID = 1;

Process::Process(const vector<int> &cpUburst, const vector<int> &iOburst, int priority)
        : CPUburst(cpUburst), IOburst(iOburst), priority(priority) {
    id = ID++;
    currentBurst = 0;
    remainingBurst = cpUburst[0];
}

Process::Process(int burst, int deadline) : absoluteDeadline(deadline), period(deadline){
    CPUburst.push_back(burst);
    id = ID++;
    currentBurst = 0;
    remainingBurst = CPUburst[0];
    priority = remainingBurst;
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

int Process::getRemainingBurst() const {
    return remainingBurst;
}

Process *Process::consumeBurst() {
    if (!flag) {
        if (hasRemainingIO()) {
            remainingBurst = IOburst[currentBurst];
        }
        currentBurst++;
        flag = !flag;
    } else {
        if (currentBurst < CPUburst.size()) {
            remainingBurst = CPUburst[currentBurst];
        }
        flag = !flag;
    }
    return this;
}

void Process::setRemainingBurst(int remainingBurst) {
    Process::remainingBurst = remainingBurst;
}

int Process::getEnteredReadyQueue() const {
    return enteredReadyQueue;
}

void Process::setEnteredReadyQueue(int enteredReadyQueue) {
    Process::enteredReadyQueue = enteredReadyQueue;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

void Process::setArrivalTime(int arrivalTime) {
    Process::arrivalTime = arrivalTime;
}

bool Process::getAssigned() const {
    return assigned;
}

void Process::setAssigned(bool assigned) {
    Process::assigned = assigned;
}

int Process::getLastStarted() const {
    return lastStarted;
}

void Process::setLastStarted(int lastStarted) {
    Process::lastStarted = lastStarted;
}

int Process::getWaitingTime() const {
    return waitingTime;
}

void Process::setWaitingTime(int waitingTime) {
    Process::waitingTime = waitingTime;
}

int Process::getAbsoluteDeadline() const {
    return absoluteDeadline;
}

void Process::setAbsoluteDeadline(int absoluteDeadline) {
    Process::absoluteDeadline = absoluteDeadline;
}

int Process::getPeriod() const {
    return period;
}

void Process::setPeriod(int period) {
    Process::period = period;
}

int Process::getNextDeadline() const {
    return nextDeadline;
}

void Process::setNextDeadline(int nextDeadline) {
    Process::nextDeadline = nextDeadline;
}

double Process::getVtime() const {
    return vtime;
}

void Process::setVtime(double vtime) {
    Process::vtime = vtime;
}

Process::~Process() {

}

int Process::getDistributionId() const {
    return distributionID;
}

void Process::setDistributionId(int distributionId) {
    distributionID = distributionId;
}

double Process::getPtime() const {
    return ptime;
}

void Process::setPtime(double ptime) {
    Process::ptime = ptime;
}

int Process::getTimeSlice() const {
    return timeSlice;
}

void Process::setTimeSlice(int timeSlice) {
    Process::timeSlice = timeSlice;
}
