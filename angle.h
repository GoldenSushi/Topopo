#ifndef ANGLE_H
#define ANGLE_H

#include <QStringList>
#include <QTableWidget>

namespace Angle {

    const QStringList angle_symbols{"°", "\'", "\""};

    enum columns
    {
        STATION = 0, POINT,  HOR_ANG, AZIMUTH, HOR_DIST, OBS
    };

    enum symbol_characters
    {
        DEGREE = 0, MINUTE, SECOND
    };

    enum calc_params
    {
        SUM = 0, SUBTRACTION
    };

    QStringList clean_angle_txt(const QStringList &);

    QString clean_angle_txt(const QString &);

    bool validate(const QStringList &); //checks if list is a valid angle value

    QString angle_format(QStringList &);

    QStringList angle_calc(const QStringList &, const QStringList &, calc_params param = SUM);

    QStringList correct_azimuth(const QStringList &, const QStringList &);

    void azimuth_calc(QTableWidget *, const QStringList &);
}

#endif // ANGLE_H
