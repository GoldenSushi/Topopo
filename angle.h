#ifndef ANGLE_H
#define ANGLE_H

#include <QStringList>
#include <QTableWidget>
#include <cassert>
#include <QDebug>

namespace Angle {

//static methods and constants
//public:

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

    static QStringList clean_angle_txt(QStringList const &list)
    {
        QString result = list.join(" ");
        result = result.replace(result.indexOf(angle_symbols[DEGREE]), 1, "");
        result = result.replace(result.indexOf(angle_symbols[MINUTE]), 1, "");
        result = result.replace(result.indexOf(angle_symbols[SECOND]), 1, "");
        return result.split(" ");
    }

    static QString clean_angle_txt(QString const &string)
    {
        QString result = string;
        result = result.replace(result.indexOf(angle_symbols[DEGREE]), 1, "");
        result = result.replace(result.indexOf(angle_symbols[MINUTE]), 1, "");
        result = result.replace(result.indexOf(angle_symbols[SECOND]), 1, "");
        return result;
    }

    static bool validate(QStringList const &list) //checks if list is a valid angle value
    {

        if (list.size() != 3)
            return false;
        for (int i = 0; i < 3; ++i)
        {
            if (!list[i].toInt() && list[i] != "0") //if there is any value that cant be converted to int
                return false;
        }
        return true;
    }

    static QString angle_format(QStringList &list)
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
        else
        {
            str = list.join(" ");
        }

        return str;
    }

    static QStringList angle_calc(QStringList const &ang1, QStringList const &ang2, calc_params param = SUM)
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
            second_result = (second1+second2)%60;
            minute_result = ((minute1+minute2)%60) + (int)(second1+second2)/60;
            angle_result = angle1+angle2;
            QStringList result{QString::number(angle_result), QString::number(minute_result), QString::number(second_result)};
            qDebug()<< QString();
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
            else
            {
                minute_result = (minute1-minute2);
            }
            if ((second1-second2) < 0)
            {
                minute_result -= 1;
                second_result = (second1-second2)+60;
            }
            else
            {
                second_result = (second1-second2);
            }
            QStringList result{QString::number(angle_result), QString::number(minute_result), QString::number(second_result)};
            return result;
        }
    }

    static QStringList correct_azimuth(QStringList const &azimuth, QStringList const &hor_ang)
    {
        QStringList result = angle_calc(azimuth, hor_ang);
        qDebug() << result;
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

    static void azimuth_calc(QTableWidget *table, QStringList const &ref_azimuth)
    {
        int rows = table->rowCount();

        if (rows == 0)
            return;

        QStringList results, current_reference = ref_azimuth;

        for (int i = 0; i < rows; ++i)
        {
            if (table->item(i, POINT)->text() == "Vante")
            {
                qDebug() << "[CLEAN TEXT]: " << clean_angle_txt(table->item(i, HOR_ANG)->text()).split(" ");
                current_reference = correct_azimuth(current_reference, clean_angle_txt(table->item(i, HOR_ANG)->text()).split(" "));
            }

            if (table->item(i, HOR_ANG) && validate(clean_angle_txt(table->item(i, HOR_ANG)->text()).split(" ")))
            {
                QStringList cell_angle = table->item(i, HOR_ANG)->text().split(" "); //angle extracted from this cell

                results = correct_azimuth(clean_angle_txt(cell_angle), clean_angle_txt(current_reference));
                table->item(i, AZIMUTH)->setText(angle_format(results));
            }

        }
    }


};

#endif // ANGLE_H
