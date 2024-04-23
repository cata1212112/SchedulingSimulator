//
// Created by Cata on 4/23/2024.
//

#include "DataPoint.h"

DataPoint::DataPoint(double bt, double pw, double ncs) : bt(bt), pw(pw), ncs(ncs) {
    cluster = -1;
    minDist = 1e9;
}

double DataPoint::distance(const DataPoint &other) {
    return (bt - other.bt) * (bt - other.bt) + (pw - other.pw) * (pw - other.pw) + (ncs - other.ncs) * (ncs - other.ncs);
}

int DataPoint::getCluster() const {
    return cluster;
}

void DataPoint::setCluster(int cluster) {
    DataPoint::cluster = cluster;
}

double DataPoint::getMinDist() const {
    return minDist;
}

void DataPoint::setMinDist(double minDist) {
    DataPoint::minDist = minDist;
}

double DataPoint::getBt() const {
    return bt;
}

double DataPoint::getPw() const {
    return pw;
}

double DataPoint::getNcs() const {
    return ncs;
}

void DataPoint::setBt(double bt) {
    DataPoint::bt = bt;
}

void DataPoint::setPw(double pw) {
    DataPoint::pw = pw;
}

void DataPoint::setNcs(double ncs) {
    DataPoint::ncs = ncs;
}
