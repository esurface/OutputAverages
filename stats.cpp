#include "stats.h"
#include <cmath>

Stats::Stats(QVector<double> d)
{
    _data = d;
    _size = d.size();
}

double Stats::getMean()
{
    double tmp = 0;
    for (double d : _data)
    {
        tmp += d;
    }

    return (tmp/(double)_size);
}

double Stats::getVariance()
{
    double mean = getMean();
    double tmp = 0;
    for(double d : _data)
        tmp += (d - mean) * (d - mean);
    return tmp/_size;

}

double Stats::getStdDev()
{
    return (sqrt(getVariance()));
}
