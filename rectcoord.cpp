#include "rectcoord.h"
#include "Angle.h"
#include <QtMath>

double RectCoord::validate(const QString &horDist)
{
    return horDist.toDouble(); // returns 0.0 if conversion fails. Works as a bool test
}

double RectCoord::distSummation(const QVector<QString> &distSum)
{
    double number = 0.0f;
    for (int i = 0; i < distSum.size(); ++i)
    {
        if (validate(distSum[i]))
        {
            number += abs(distSum[i].toDouble());
        }
    }
    return number;
}

double RectCoord::dmsToDecimal(const QStringList &angle)
{
    if (Angle::validate(angle))
    {
        return angle[0].toDouble() + (angle[1].toDouble()/60) + (angle[2].toDouble()/3600);
    }
    return 0.0; // Can work as a bool test
}

double RectCoord::polarToRect(const double &angle, const double &horDist, Coordinate param)
{
    double degrees = qDegreesToRadians(angle);
    if (param == XCOORD)
    {
        return qSin(degrees) * horDist;
    }
    if (param == YCOORD)
    {
        return qCos(degrees) * horDist;
    }
    return 0.0; //Can work as a bool test
}
