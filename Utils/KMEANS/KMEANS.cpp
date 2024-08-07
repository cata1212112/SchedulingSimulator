//
// Created by Cata on 3/17/2024.
//

#include <iostream>
#include "KMEANS.h"

KMEANS::KMEANS(const vector<DataPoint> &points, int numbefOfClusters, int epochs) : points(points),
                                                                                    numbefOfClusters(numbefOfClusters),
                                                                                    epochs(epochs) {

    for (int i=0; i<numbefOfClusters; i++) {
        centroids.push_back(points[rand() % points.size()]);
    }
//    kmeansplusplusInitialization();
    numberOfPoints.reserve(numbefOfClusters);
    for (int i=0; i<numbefOfClusters; i++) {
        numberOfPoints[i] = 0;
        means[0] = means[1] = means[2] = 0.0;
    }
}

void KMEANS::cluster() {
    for (int e=0; e<epochs; e++) {
        for (int i=0; i<centroids.size(); i++) {
            for (int j=0; j<points.size(); j++) {
                double distance = points[j].distance(centroids[i]);
                if (distance < points[j].getMinDist()) {
                    points[j].setMinDist(distance);
                    points[j].setCluster(i);
                }
            }
        }

        for (int i=0; i<numbefOfClusters; i++) {
            numberOfPoints[i] = 0;
            means[0] = means[1] = means[2] = 0.0;
        }

        for (auto &x:points) {
            numberOfPoints[x.getCluster()] ++;
            means[0] += x.getBt();
            means[1] += x.getPw();
            means[2] += x.getNcs();

            x.setMinDist(1e9);
        }

        for (int i=0; i<centroids.size(); i++) {
            if (numberOfPoints[i]) {
                centroids[i].setBt(means[0] / numberOfPoints[i]);
                centroids[i].setPw(means[1] / numberOfPoints[i]);
                centroids[i].setNcs(means[2] / numberOfPoints[i]);
            }
        }
    }
}

double KMEANS::silhouetteScore() {
    double scores[points.size()];
    for (int i=0; i<points.size(); i++) {
        scores[i] = 0;
    }

    for (int i=0; i<points.size(); i++) {
        double a = 0;
        double b = 0;

        int sameCluster = 0;
        int differentCluster = 0;

        for (int j=0; j<points.size(); j++) {
            if (i != j) {
                if (points[i].getCluster() == points[j].getCluster()) {
                    a += points[i].distance(points[j]);
                    sameCluster += 1;
                } else {
                    b += points[i].distance(points[j]);
                    differentCluster += 1;
                }
            }
        }
        a /= sameCluster;
        b /= differentCluster;
        scores[i] = (b - a) / max(a , b);
    }
    double avg = 0;
    for (int i=0; i<points.size(); i++) {
        avg += scores[i];
    }

    return avg / points.size();
}

double KMEANS::averageBurstTime(int clusterId) {
    double avg = 0;
    int len = 0;
    for (int i=0; i<points.size(); i++) {
        if (points[i].getCluster() == clusterId) {
            avg += points[i].getBt();
            len += 1;
        }
    }
    return avg / len;
}

double KMEANS::weight(int clusterId) {
    double averages[numbefOfClusters];
    double sum = 0;
    for (int i=0; i<numbefOfClusters; i++) {
        averages[i] = averageBurstTime(i);
        sum += averages[i];
    }
    return averages[clusterId] / sum;
}

int KMEANS::timeSlice(int clusterId, int STS) {
    return (1 - weight(clusterId)) * STS;
}

void KMEANS::printClusters() {
    for (const auto x:points) {
        std::cout << x.getCluster() << " ";
    }
    std::cout << "\n";
}

void KMEANS::kmeansplusplusInitialization() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, points.size() - 1);

    centroids.push_back(points[dis(gen)]);

    std::vector<double> minDistances(points.size(), std::numeric_limits<double>::max());

    for (int i = 1; i < numbefOfClusters; ++i) {
        for (int j = 0; j < points.size(); ++j) {
            double dist = points[j].distance(centroids.back());
            if (dist < minDistances[j]) {
                minDistances[j] = dist;
            }
        }

        double sum = std::accumulate(minDistances.begin(), minDistances.end(), 0.0);
        std::uniform_real_distribution<> distr(0, sum);
        double threshold = distr(gen);
        double cumulative = 0;

        for (int j = 0; j < points.size(); ++j) {
            cumulative += minDistances[j];
            if (cumulative >= threshold) {
                centroids.push_back(points[j]);
                break;
            }
        }
    }
}

int KMEANS::getCluster(int i) {
    return points[i].getCluster();
}

KMEANS::KMEANS() {}

KMEANS::KMEANS(const vector<DataPoint> &points, const vector<DataPoint> &centroids, const vector<int> &numberOfPoints,
               double *means, int numbefOfClusters, int epochs) : points(points), centroids(centroids),
                                                                  numberOfPoints(numberOfPoints),
                                                                  numbefOfClusters(numbefOfClusters), epochs(epochs) {
    this->means[0] = means[0];
    this->means[1] = means[1];
    this->means[2] = means[2];
}

