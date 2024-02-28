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

public:

    void setCpUburst(const vector<int> &cpUburst);

    void setIOburst(const vector<int> &iOburst);

    void setPriority(int priority);

    void setId(int id);

    Process();

    Process(const vector<int> &cpUburst, const vector<int> &iOburst, int priority);

    Process(const Process& other) :
            CPUburst(other.CPUburst),
            IOburst(other.IOburst),
            currentBurst(other.currentBurst),
            priority(other.priority),
            id(ID++) {}

    const vector<int> &getCpUburst() const;

    const vector<int> &getIOburst() const;

    int getPriority() const;

    int getId() const;
};


#endif //CPUSCHED_PROCESS_H
