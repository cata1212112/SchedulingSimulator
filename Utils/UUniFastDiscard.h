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
//        static std::uniform_real_distribution<> uniform(log(min), log(max + 1));
        static std::uniform_real_distribution<> uniform(min, 100);
        static std::mt19937 gen;

        std::vector<double> periods;
        for (int i=0; i<n; i++) {
//            periods.push_back(exp(uniform(gen)));
            periods.push_back(uniform(gen));
        }
        return periods;
    }

    static std::vector<int> adjustPeriods(std::vector<double> &periods) {
        std::vector<int> refprd;
        int exp2 = 3;
        int exp3 = 2;
        int exp5 = 1;
        int exp7 = 0;

        for (int i = 0; i <= exp2; ++i) {
            for (int j = 0; j <= exp3; ++j) {
                for (int k = 0; k <= exp5; ++k) {
                    for (int l = 0; l <= exp7; ++l) {
                        refprd.push_back(pow(2, i) * pow(3, j) * pow(5, k) * pow(7, l));
                    }
                }
            }
        }
        std::sort(refprd.begin(), refprd.end());

        for (int i=0; i<periods.size(); i++) {
            for (int j = 0; j < refprd.size(); j++) {
                if (refprd[j] >= periods[i]) {
                    periods[i] =  refprd[j - 1];
                    break;
                }
            }
        }

        return {};
    }

    static std::vector<std::pair<int,int>> generateTasks(double n, double utilization) {

        std::string source_dir = __FILE__;
        source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

        std::string python_executable = "../Python/plotting/Scripts/python.exe";
        std::string python_script = "../Python/plotting/stafford.py";

        std::string command = source_dir + python_executable + " " + source_dir + python_script + " " + std::to_string(n) + " 1 " + std::to_string(utilization) + " 0.1 1";

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



        std::vector<double> periods = generatePeriodsLogUniform(n, 10, 1000);
        adjustPeriods(periods);

        std::vector<std::pair<int,int>> tasks;
        for (int i=0; i<n; i++) {
            tasks.push_back({1e6 * utilizations[i] * periods[i] / 1e6, periods[i]});
        }
        return tasks;
    }

    static vector<vector<pair<int,int>>> generateTaskSet(double n, double utilization, int howMany=10000) {

        std::string source_dir = __FILE__;
        source_dir = source_dir.substr(0, source_dir.find_last_of("\\/") + 1);

        std::string python_executable = "../Python/plotting/Scripts/python.exe";
        std::string python_script = "../Python/plotting/stafford.py";

        std::string command = source_dir + python_executable + " " + source_dir + python_script + " " + std::to_string(int(n)) + " " + std::to_string(howMany) + " " + std::to_string(utilization) + " 0.1 1";

        FILE* pipe = popen(command.c_str(), "r");
        char buffer[128];
        std::string result;
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr) {
                result += buffer;
            }
        }

        pclose(pipe);
//        result.erase(0, 1);
//        result.pop_back();
//        result.pop_back();
        std::vector<double> utilizations;
//        cout << result << "\n";
        if (result == "" || result == "\n") {
            return {};
        }
        std::stringstream ss(result);
        std::string token;
        while (ss >> token) {
            utilizations.push_back(stod(token));
        }

        vector<vector<pair<int,int>>> taskset;
        for (int i=0; i<int(n) * howMany; i += int(n)) {
            std::vector<double> periods = generatePeriodsLogUniform(n, 10, 1000);
            adjustPeriods(periods);

            std::vector<std::pair<int,int>> tasks;
            for (int i=0; i<n; i++) {
                tasks.push_back({1e6 * utilizations[i] * periods[i] / 1e6, periods[i]});
            }
            taskset.push_back(tasks);
        }

        return taskset;
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
