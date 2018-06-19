#ifndef RECTCOORD_H
#define RECTCOORD_H
#include <QtMath>
#include <QStringList>

namespace RectCoord {

enum Coordinate
{
    XCOORD = 0, YCOORD
};

double validate(const QString &); //validates horizontal distance

double distSummation(const QVector<QString> &);

double dmsToDecimal(const QStringList &);

double polarToRect(const double &, const double &, Coordinate param);

}

#endif // RECTCOORD_H
