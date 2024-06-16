//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_METRICS_H
#define LICENTA_METRICS_H

#include <vector>
#include <map>

using namespace std;

class Metrics {
private:
    int core;
    double cpuUtilization = 0;
    double averageWaitingTime = 0;
    double averageTurnaroundTime = 0;
    double averageResponseTime = 0;
    int contextSwitches = 0;
    string algorithm;
    vector<std::tuple<int, int, int>> gantt;
    vector<double> maximumLoadDifference;
    map<int, double> avgWaitTimeByDistributionId;
    map<int, double> avgTurnaroundTimeByDistributionId;
    map<int, double> avgResponseTimeByDistributionId;

public:
    explicit Metrics(const std::string &algorithm, int roundRobinQuant=0) : algorithm(algorithm) {
        if (algorithm == "Round Robin") {
            Metrics::algorithm = algorithm + " TQ = " + std::to_string(roundRobinQuant);
        } else if (algorithm == "Mean Threshold Shortest Job Round Robin") {
            Metrics::algorithm = algorithm + " T = " + std::to_string(roundRobinQuant);
        }
    }

    void incrementCS() {
        contextSwitches += 1;
    }

    int getContextSwitches() const {
        return contextSwitches;
    }

    void addToGanttChart(int pid, int left, int right) {
        gantt.push_back({pid, left, right});
    }

    std::string getGanttData() {
        if (gantt.empty()) {
            return "[]";
        }
        std::string data = "[";
        for (auto x : gantt) {
            data += "[" + std::to_string(std::get<0>(x)) + "," + std::to_string(std::get<1>(x)) + "," + std::to_string(std::get<2>(x)) + "],";
        }
        data.pop_back();
        data += "]";
        return data;
    }

    void addToCPUUtilization(int val) {
        cpuUtilization += val;
    }

    void addToWT(int val, int dId) {
        avgWaitTimeByDistributionId[dId] += val;
        averageWaitingTime += val;
    }

    void addToTT(int val, int dId) {
        avgTurnaroundTimeByDistributionId[dId] += val;
        averageTurnaroundTime += val;
    }

    void addToRT(int val, int dId) {
        avgResponseTimeByDistributionId[dId] += val;
        averageResponseTime += val;
    }

    std::string getMetrics() {
        string perDistributionsStats;
        for (auto x:avgWaitTimeByDistributionId) {
            perDistributionsStats += to_string(x.first) + " " + to_string(avgWaitTimeByDistributionId[x.first]) + " " +
                    to_string(avgResponseTimeByDistributionId[x.first]) + " " + to_string(avgTurnaroundTimeByDistributionId[x.first]) + " ";
        }
        return std::to_string(averageWaitingTime) + " " + std::to_string(averageTurnaroundTime) + " " +
               std::to_string(averageResponseTime) + " " + std::to_string(contextSwitches) + " " + perDistributionsStats + " " + algorithm;
    }

    void divide(double timespan, double numOfProcs) {
        cpuUtilization = cpuUtilization / timespan * 100;
        averageWaitingTime /= numOfProcs;
        averageResponseTime /= numOfProcs;
        averageTurnaroundTime /= numOfProcs;

        for (auto x:avgResponseTimeByDistributionId) {
            avgResponseTimeByDistributionId[x.first] = x.second / numOfProcs;
        }

        for (auto x:avgTurnaroundTimeByDistributionId) {
            avgTurnaroundTimeByDistributionId[x.first] = x.second / numOfProcs;
        }

        for (auto x:avgWaitTimeByDistributionId) {
            avgWaitTimeByDistributionId[x.first] = x.second / numOfProcs;
        }
    }

    int getCore() const {
        return core;
    }

    void setCore(int core) {
        Metrics::core = core;
    }

    void addDifference(double value) {
        maximumLoadDifference.push_back(value);
    }

    const std::vector<double> &getMaximumLoadDifference() const {
        return maximumLoadDifference;
    }
};


#endif //LICENTA_METRICS_H
