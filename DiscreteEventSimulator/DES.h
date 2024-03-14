//
// Created by Cata on 2/12/2024.
//

#ifndef CPUSCHED_DES_H
#define CPUSCHED_DES_H

#include <string>
#include "../Utils/Random.h"
#include "../Utils/ImplementedAlgorithms.h"
#include <queue>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <utility>
#include <thread>
#include <mutex>
#include <condition_variable>

#define MAXIMUMNUMCPUBURSTS 20
#define MAXIMUMARRIVAL 300

using namespace std;

class DES {
private:
    priority_queue<Event> *events;
    string input;
    string algorithm;
    bool usedFileAsInput = false;
    int numberOfProcesses = 0;
    string partialMetricsInput[32];
    vector<pair<string, string>> partialGantt[32];
    int roundRobinQuant;
    std::condition_variable cv;
    std::mutex cvMutex;
    bool osTimeUpdated = false;
    static vector<string> algortihms;

    static int getMultiplier(bool flag) {
        if (flag) {
            return 10;
        } else {
            return 5;
        }
    }

public:
    int getRoundRobinQuant() const;

    void setRoundRobinQuant(int roundRobinQuant);


    DES(string algorithm) : algorithm(algorithm) {
        algortihms.push_back(algorithm);
        events = new priority_queue<Event>();
    }

    string generateInputData(int numProcesses, int maximumTime);

    void readInputDataFromFile(const string& filename);

    void setInputFromString(const string& input);

    vector<Metrics> startSimulation(int numCPUS);

    bool isUsedFileAsInput() const;

    void setAlgorithm(const string &algorithm);

    const string &getAlgorithm() const;

    const string &getInput() const;

    void setInput(const string &input);

    const string &getPartialMetricsInput(int core) const;

    void setPartialMetricsInput(const string &partialMetricsInput, int core);

    void addToGantt(const pair<string, string> &gantt, int core) {
        partialGantt[core].push_back(gantt);
    }

    vector<pair<string, string>> getGantt(int core) {
        return partialGantt[core];
    }

    static vector<string> getAlgorithms() {
        return algortihms;
    }
};


#endif //CPUSCHED_DES_H
