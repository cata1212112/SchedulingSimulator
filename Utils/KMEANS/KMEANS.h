//
// Created by Cata on 3/17/2024.
//

#ifndef LICENTA_KMEANS_H
#define LICENTA_KMEANS_H

#include <vector>
#include "DataPoint.h"
#include <random>

using namespace std;


class KMEANS {
private:
    vector<DataPoint> points;
    vector<DataPoint> centroids;
    vector<int> numberOfPoints;
    double means[3];
    int numbefOfClusters;
    int epochs;
public:
    KMEANS(const vector<DataPoint> &points, int numbefOfClusters, int epochs);

    void cluster();

    double silhouetteScore();

    double averageBurstTime(int clusterId);

    double weight(int clusterId);

    int timeSlice(int clusterId, int STS);

    void printClusters();

    void kmeansplusplusInitialization();

    int getCluster(int i);

    KMEANS(const vector<DataPoint> &points, const vector<DataPoint> &centroids, const vector<int> &numberOfPoints,
           double *means, int numbefOfClusters, int epochs);

    KMEANS();
};


#endif //LICENTA_KMEANS_H
