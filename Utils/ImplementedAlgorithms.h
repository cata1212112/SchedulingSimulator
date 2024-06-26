//
// Created by Cata on 2/27/2024.
//

#ifndef LICENTA_IMPLEMENTEDALGORITHMS_H
#define LICENTA_IMPLEMENTEDALGORITHMS_H

#endif //LICENTA_IMPLEMENTEDALGORITHMS_H

#include <vector>
#include <string>
#include "../Scheduler/FIFO/FIFO.h"
#include "../Scheduler/SJF/SJF.h"
#include "../Scheduler/RR/RR.h"
#include "../Scheduler/SRTF/SRTF.h"
#include "../Scheduler/EDRR/EDRR.h"
#include "../Scheduler/TestMultiCore1/TestMultiCore1.h"
#include "../Scheduler/TestMultiCore2/TestMultiCore2.h"
#include "../Scheduler/RealTime/GEDF/GEDF.h"
#include "../Scheduler/RealTime/LSTR/LSTR.h"
#include "../Scheduler/RealTime/DARTS/DARTS.h"
#include "../Scheduler/RealTime/LTS/LTS.h"
#include "../Scheduler/RealTime/DARTS/DARTSCore/DARTSCore.h"
#include "../Scheduler/RealTime/GEDF/GEDFCore/GEDFCore.h"
#include "../Scheduler/RealTime/LSTR/LSTRCore/LSTRCore.h"
#include "../Scheduler/RealTime/LTS/LTSCore/LTSCore.h"
#include "../Scheduler/MTSJ/MTSJ.h"
#include "../Scheduler/RRKMEANS/MinMaxRR.h"
#include "../Scheduler/CompletlyFairScheduler/CFS.h"
#include "../Scheduler/FairShareScheduling/FSS.h"
#include "../Scheduler/FairShareScheduling/FSSCore.h"
#include "../Scheduler/ETRR/MMRR.h"

class ImplementedAlgorithms {
public:
    static std::vector<std::string> getSingleCoreAlgorithms() {
        return {"Efficient Dynamic Round Robin", "First In First Out", "Shortest Job First", "Shortest Remaining Time First", "Round Robin", "Mean Threshold Shortest Job Round Robin", "Mean Median Time Quantum Round Robin", "Min Max Round Robin"};
    }

    static SchedulingAlgorithm& getAlgorithm(const std::string &name, int quant=-1) {
        if (name == "First In First Out") {
            FIFO *fifo = new FIFO();
            return *fifo;
        } else if (name == "Shortest Job First") {
            SJF *sjf = new SJF();
            return *sjf;
        } else if (name == "Shortest Remaining Time First") {
            SRTF *srtf = new SRTF();
            return *srtf;
        } else if (name == "Round Robin") {
            RR *rr = new RR(quant);
            return *rr;
        } else if (name == "Efficient Dynamic Round Robin") {
            EDRR *edrr = new EDRR();
            return *edrr;
        } else if (name == "Global Earliest Deadline First") {
            GEDF *gedf = new GEDF();
            return *gedf;
        } else if (name == "GEDFCore") {
            GEDFCore *gedfcore = new GEDFCore();
            return *gedfcore;
        } else if (name == "Least Slack Time Rate First") {
            LSTR *lstr = new LSTR();
            return *lstr;
        } else if (name == "LSTRCore") {
            LSTRCore *lstrCore = new LSTRCore();
            return *lstrCore;
        } else if (name == "Mean Threshold Shortest Job Round Robin") {
            MTSJ *mtsj = new MTSJ(quant);
            return *mtsj;
        } else if (name == "Mean Median Time Quantum Round Robin") {
            MMRR *ptqrr = new MMRR();
            return *ptqrr;
        } else if (name == "Min Max Round Robin") {
            MinMaxRR *minMaxRr = new MinMaxRR();
            return *minMaxRr;
        } else if (name == "Completely Fair Scheduler") {
            CFS *cfs = new CFS();
            return *cfs;
        } else if (name == "SingleCoreCFS") {
            SingleCoreCFS *sccfs = new SingleCoreCFS();
            return *sccfs;
        } else if (name == "Fair-Share Scheduling") {
            FSS *fss = new FSS();
            return *fss;
        } else if (name == "FSSCore") {
            FSSCore *fsscore = new FSSCore();
            return *fsscore;
        } else if (name == "DynAmic Real-time Task Scheduling") {
            DARTS *darts = new DARTS();
            return *darts;
        } else if (name == "DARTSCore") {
            DARTSCore *dartsCore = new DARTSCore();
            return *dartsCore;
        } else if (name == "LTSCore") {
            LTSCore *lstcore = new LTSCore();
            return *lstcore;
        } else if (name == "Linear Task Scheduling") {
            LTS *lts = new LTS();
            return *lts;
        }
    }

    static std::vector<std::string> getMultiCoreAlgortihms() {
        return {"Completely Fair Scheduler", "Fair-Share Scheduling"};
    }

    static std::vector<std::string> getRealTimeAlgortihms() {
        return {"DynAmic Real-time Task Scheduling", "Least Slack Time Rate First", "Linear Task Scheduling"};
    }
};