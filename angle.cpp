#include <cassert>
#include <QDebug>
#include "Angle.h"

QStringList Angle::clean_angle_txt(const QStringList &list)
{
    QString result = list.join(" ");
    result = result.replace(result.indexOf(angle_symbols[DEGREE]), 1, "");
    result = result.replace(result.indexOf(angle_symbols[MINUTE]), 1, "");
    result = result.replace(result.indexOf(angle_symbols[SECOND]), 1, "");
    return result.split(" ");
}

QString Angle::clean_angle_txt(const QString &string)
{
    QString result = string;
    result = result.replace(result.indexOf(angle_symbols[DEGREE]), 1, "");
    result = result.replace(result.indexOf(angle_symbols[MINUTE]), 1, "");
    result = result.replace(result.indexOf(angle_symbols[SECOND]), 1, "");
    return result;
}

bool Angle::validate(const QStringList &list) //checks if list is a valid angle value
{
    if (list.size() != 3) { return false; }
    for (int i = 0; i < 3; ++i)
    {
        if (!list[i].toInt() && list[i] != "0") { return false; } //if there is any value that cant be converted to int
    }
    return true;
}

QString Angle::angle_format(QStringList &list)
{
    QString str;

    if (validate(list))
    {
        list[0].push_back(angle_symbols[0]);
        list[1].push_back(angle_symbols[1]);
        list[2].push_back(angle_symbols[2]);
        for (int i = 0; i < 3; ++i)
        {
            str += list[i];
            (i != 2) ? str += " " : str;
        }
    }
    else { str = list.join(" "); }

    return str;
}

QStringList Angle::angle_calc(const QStringList &ang1, const QStringList &ang2, calc_params param)
{
    assert(ang1.size() == 3 && ang2.size() == 3);

    int angle1 = ang1[0].toInt(),
            minute1 = ang1[1].toInt(),
            second1 = ang1[2].toInt();
    int angle2 = ang2[0].toInt(),
            minute2 = ang2[1].toInt(),
            second2 = ang2[2].toInt();

    int second_result;
    int minute_result;
    int angle_result;

    if (param == SUM)
    {
        int remain;
        second_result = (second1+second2)%60;
        remain = (int)(second1+second2)/60;
        minute_result = (minute1+minute2+remain)%60;
        remain = (int)(minute1+minute2+remain)/60;
        angle_result = (angle1+angle2+remain);
        QStringList result{QString::number(angle_result), QString::number(minute_result), QString::number(second_result)};
        return result;
    }
    else
    {
        if ((angle1-angle2) < 0)
        {
            QStringList result{QString::number(0), QString::number(0), QString::number(0)};
            return result;
        }
        angle_result = (angle1-angle2);
        if ((minute1-minute2) < 0)
        {
            angle_result -= 1;
            minute_result = (minute1-minute2)+60;
        }
        else { minute_result = (minute1-minute2); }
        if ((second1-second2) < 0)
        {
            minute_result -= 1;
            second_result = (second1-second2)+60;
        }
        else { second_result = (second1-second2); }
        QStringList result{QString::number(angle_result), QString::number(minute_result), QString::number(second_result)};
        return result;
    }
}

QStringList Angle::correct_azimuth(const QStringList &azimuth, const QStringList &hor_ang)
{
    QStringList result = angle_calc(azimuth, hor_ang);
    if (result[DEGREE].toInt() < 180)
    {
        result.replace(DEGREE, QString::number(result[DEGREE].toInt() + 180));
    }
    else if (result[DEGREE].toInt() > 180 && result[DEGREE].toInt() < 540)
    {
        result.replace(DEGREE, QString::number(result[DEGREE].toInt() - 180));
    }
    else
    {
        result.replace(DEGREE, QString::number(result[DEGREE].toInt() - 540));
    }
    return result;
}

void Angle::azimuth_calc(QTableWidget *table, const QStringList &ref_azimuth)
{
    int rows = table->rowCount();
    if (rows == 0) { return; }

    QStringList results = ref_azimuth, current_reference = ref_azimuth;

    for (int i = 0; i < rows; ++i)
    {
        if (i != 0 && table->item(i-1, HOR_ANG) && table->item(i-1, POINT)->text() == "Vante")
        {
            if (validate(clean_angle_txt(table->item(i-1, HOR_ANG)->text()).split(" ")))
                current_reference = correct_azimuth(current_reference, clean_angle_txt(table->item(i-1, HOR_ANG)->text()).split(" "));
        }
        if (table->item(i, HOR_ANG) && validate(clean_angle_txt(table->item(i, HOR_ANG)->text()).split(" ")))
        {
            QStringList cell_angle = table->item(i, HOR_ANG)->text().split(" "); //angle extracted from this cell
            if (i != 0) { results = correct_azimuth(clean_angle_txt(cell_angle), clean_angle_txt(current_reference)); } //first row must not be corrected
            table->item(i, AZIMUTH)->setText(angle_format(results));
        }
    }
}


