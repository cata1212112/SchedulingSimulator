//
// Created by Cata on 4/23/2024.
//

#ifndef LICENTA_DATAPOINT_H
#define LICENTA_DATAPOINT_H


class DataPoint {
private:
    double bt, pw, ncs;
    int cluster;
    double minDist;

public:
    DataPoint(double bt, double pw, double ncs);

    double distance(const DataPoint &other);

    int getCluster() const;

    void setCluster(int cluster);

    double getMinDist() const;

    void setMinDist(double minDist);

    void setBt(double bt);

    void setPw(double pw);

    void setNcs(double ncs);

    double getBt() const;

    double getPw() const;

    double getNcs() const;
};


#endif //LICENTA_DATAPOINT_H
