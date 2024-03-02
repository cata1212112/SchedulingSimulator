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
    int flag = 0;
    bool assigned = 0;

public:

    void setCpUburst(const vector<int> &cpUburst);

    void setIOburst(const vector<int> &iOburst);

    void setPriority(int priority);

    void setId(int id);

    int getRemainingBurst() const;

    Process();

    Process(const vector<int> &cpUburst, const vector<int> &iOburst, int priority);

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
            assigned(other.assigned)
            {}

    const vector<int> &getCpUburst() const;

    const vector<int> &getIOburst() const;

    int getPriority() const;

    int getId() const;

    Process* consumeBurst();

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
