#ifndef STATS_H
#define STATS_H

#include <QVector>

class Stats
{
public:
    Stats(QVector<double> d);
    double getMean();
    double getVariance();
    double getStdDev();

private:
    QVector<double> _data;
    int _size;
};

#endif // STATS_H
