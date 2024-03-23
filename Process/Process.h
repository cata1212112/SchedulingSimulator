//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_PROCESS_H
#define CPUSCHED_PROCESS_H

#include <vector>

using namespace std;


class Process {
private:
    static int ID;

    vector<int> CPUburst;
    vector<int> IOburst;
    int currentBurst;
    int priority;
    int id;
    int remainingBurst = 0;
    int enteredReadyQueue = 0;
    int arrivalTime = 0;
    bool flag = false;
    bool assigned = 0;
    int waitingTime = 0;
    int lastStarted;
    double vtime = 0;
    int absoluteDeadline;
    int period;

public:

    void setCpUburst(const vector<int> &cpUburst);

    int getPeriod() const;

    void setPeriod(int period);

    void setIOburst(const vector<int> &iOburst);

    void setPriority(int priority);

    int getAbsoluteDeadline() const;

    void setAbsoluteDeadline(int absoluteDeadline);

    void setId(int id);

    int getRemainingBurst() const;

    Process();

    Process(const vector<int> &cpUburst, const vector<int> &iOburst, int priority);

    Process(int burst, int deadline);

    Process(const Process& other) :
            CPUburst(other.CPUburst),
            IOburst(other.IOburst),
            currentBurst(other.currentBurst),
            priority(other.priority),
            id(other.id),
            remainingBurst(other.remainingBurst),
            flag(other.flag),
            enteredReadyQueue(other.enteredReadyQueue),
            arrivalTime(other.arrivalTime),
            assigned(other.assigned),
            lastStarted(other.lastStarted),
            vtime(other.vtime),
            waitingTime(other.waitingTime),
            absoluteDeadline(other.absoluteDeadline),
            period(other.period)
            {}

    const vector<int> &getCpUburst() const;

    const vector<int> &getIOburst() const;

    int getWaitingTime() const;

    void setWaitingTime(int waitingTime);

    int getPriority() const;

    int getId() const;

    Process* consumeBurst();

    int getLastStarted() const;

    void setLastStarted(int lastStarted);

    bool finished() {
        return currentBurst >= CPUburst.size();
    }

    bool hasRemainingIO() {
        return currentBurst < IOburst.size();
    }

    void setRemainingBurst(int remainingBurst);

    int getEnteredReadyQueue() const;

    void setEnteredReadyQueue(int enteredReadyQueue);

    int getArrivalTime() const;

    void setArrivalTime(int arrivalTime);

    bool getAssigned() const;

    void setAssigned(bool assigned);

};


#endif //CPUSCHED_PROCESS_H
