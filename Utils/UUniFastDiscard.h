//
// Created by Cata on 3/23/2024.
//

#ifndef LICENTA_UUNIFASTDISCARD_H
#define LICENTA_UUNIFASTDISCARD_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>

class RealTimeGenerator {
public:
    static std::vector<double> UUniFastDiscard(int n, int u) {
        static std::uniform_real_distribution<> uniform(0, 1);
        static std::mt19937 gen;
        std::vector<double> utilization;
        double sn = u;
        for (int i=1; i<=n; i++) {
            double snext = sn * pow(uniform(gen), 1.0 / (n-i));
            utilization.push_back(sn - snext);
            sn = snext;
        }

        if (std::all_of(utilization.begin(), utilization.end(), [](double d) {return d <= 1 && d >= 0.1;})) {
            return utilization;
        }
        return UUniFastDiscard(n, u);
    }

    static std::vector<double> generatePeriodsLogUniform(int n, double min, double max) {
        static std::uniform_real_distribution<> uniform(log(min), log(max));
        static std::mt19937 gen;

        std::vector<double> periods;
        for (int i=0; i<n; i++) {
            periods.push_back(exp(uniform(gen)));
        }
        return periods;
    }

    static std::vector<std::pair<int,int>> generateTasks(int n, int numCores) {

        std::string source_dir = __FILE__;
        source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

        std::string python_executable = "../Python/plotting/Scripts/python.exe";
        std::string python_script = "../Python/plotting/stafford.py";

        std::string command = source_dir + python_executable + " " + source_dir + python_script + " " + std::to_string(n) + " " + std::to_string(numCores) + " 0.1 1";

        FILE* pipe = popen(command.c_str(), "r");
        char buffer[128];
        std::string result;
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                result += buffer;
            }
        }

        pclose(pipe);
        result.erase(0, 1);
        result.pop_back();
        result.pop_back();
        std::vector<double> utilizations;

        std::stringstream ss(result);
        std::string token;
        while (ss >> token) {
            utilizations.push_back(stod(token));
        }



        std::vector<double> periods = generatePeriodsLogUniform(n, 10, 20);

        std::vector<std::pair<int,int>> tasks;
        for (int i=0; i<n; i++) {
            tasks.push_back({1e6 * utilizations[i] * periods[i] / 1e6, periods[i] * 1e6 / 1e6});
        }
        return tasks;
    }

    static std::string toString(std::vector<std::pair<int,int>> tasks) {
        std::string output = "";
        for (auto t:tasks) {
            output += std::to_string(t.first) + " " + std::to_string(t.second) + "\n";
        }
        return output;
    }
};

#endif //LICENTA_UUNIFASTDISCARD_H
