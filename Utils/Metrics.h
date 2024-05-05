//
// Created by Cata on 2/29/2024.
//

#ifndef LICENTA_METRICS_H
#define LICENTA_METRICS_H

#include <vector>

class Metrics {
private:
    int core;
    double cpuUtilization = 0;
    double averageWaitingTime = 0;
    double averageTurnaroundTime = 0;
    double averageResponseTime = 0;
    std::string algorithm;
    std::vector<std::tuple<int, int, int>> gantt;
    std::vector<double> maximumLoadDifference;
public:
    explicit Metrics(const std::string &algorithm, int roundRobinQuant=0) : algorithm(algorithm) {
        if (algorithm == "Round Robin") {
            Metrics::algorithm = algorithm + " TQ = " + std::to_string(roundRobinQuant);
        }
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

    void addToWT(int val) {
        averageWaitingTime += val;
    }

    void addToTT(int val) {
        averageTurnaroundTime += val;
    }

    void addToRT(int val) {
        averageResponseTime += val;
    }

    std::string getMetrics() {
        return "\"" + algorithm + "\" " + std::to_string(cpuUtilization) + " " + std::to_string(averageWaitingTime) + " " + std::to_string(averageTurnaroundTime) + " " +
                std::to_string(averageResponseTime);
    }

    void divide(double timespan, double numOfProcs) {
        cpuUtilization = cpuUtilization / timespan * 100;
        averageWaitingTime /= numOfProcs;
        averageResponseTime /= numOfProcs;
        averageTurnaroundTime /= numOfProcs;
    }

    int getCore() const {
        return core;
    }

    void setCore(int core) {
        Metrics::core = core;
    }

    void addDifference(int value) {
        maximumLoadDifference.push_back(value);
    }
};


#endif //LICENTA_METRICS_H
