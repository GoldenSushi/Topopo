#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertdiag.h"
#include "filedialog.h"
#include "angle.h"
#include "FileManager.h"
#include "rectcoord.h"
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>
#include <QTabWidget>

#define last_row ui->table->rowCount()-1

enum ErrorColumns
{
    XCOORDINATE = 2, XFACTOR, CORRECTEDX,
    YCOORDINATE, YFACTOR, CORRECTEDY
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Topopo");
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //columns size fill the window
    ui->table->horizontalHeader()->setMinimumHeight(40);
    ui->table->setAlternatingRowColors(true);
    ui->table->setMaximumSize(maximumSize().width(), maximumSize().height());

    ui->table_error->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //columns size fill the window
    ui->table_error->horizontalHeader()->setMinimumHeight(40);
    ui->table_error->setAlternatingRowColors(true);
    ui->table_error->setMaximumSize(maximumSize().width(), maximumSize().height());

    ui->data->layout()->setContentsMargins(0, 0, 0, 0);
    ui->errorCalculation->layout()->setContentsMargins(0, 0, 0, 0);

    ui->tabWidget->tabBar()->setExpanding(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_newpoint_clicked()
{
    InsertDiag diag;
    int return_signal;
    QString autofill = "";

    if (ui->table->item(last_row, 0)) { autofill = ui->table->item(last_row, 0)->text(); } //checks if item isnt null

    diag.setEntry1(autofill);
    if (autofill != "") { diag.getEntry2_ptr()->setFocus(); } //jumps to entry2 if first entry is filled
    return_signal = diag.exec();

    if (return_signal == InsertDiag::Accepted)
    {
        ui->table->insertRow(last_row+1);
        for (int column = 0; column < ui->table->columnCount(); ++column)
        {
            ui->table->setItem(last_row, column, new QTableWidgetItem(""));
        }
        using namespace Angle;
        ui->table->item(last_row, STATION)->setText(diag.getEntry1());
        ui->table->item(last_row, POINT)->setText(diag.getEntry2());
        ui->table->item(last_row, HOR_ANG)->setText(diag.getEntry3());
        ui->table->item(last_row, AZIMUTH)->setText(diag.getEntry4());
        ui->table->item(last_row, HOR_DIST)->setText(diag.getEntry5());
        ui->table->item(last_row, OBS)->setText(diag.getEntry6());
        //********* Error Table ************************************************************************
        ui->table_error->insertRow(last_row);
        for (int column = 0; column < ui->table_error->columnCount(); ++column)
        {
            if ((column == STATION || column == POINT) && ui->table->item(last_row, column))
            {
                ui->table_error->setItem(last_row, column, new QTableWidgetItem(ui->table->item(last_row, column)->text()));
            }
            else
            {
                ui->table_error->setItem(last_row, column, new QTableWidgetItem(""));
            }
        }
    }

    if (ui->azimuthEntry) //updates azimuthEntry and thus calls on_table_cellChanged, refreshing the table
        on_azimuthEntry_textChanged(ui->azimuthEntry->text());


}

void MainWindow::on_removepoint_clicked()
{
    //table must be the last removed since the other will use its row index as reference
    ui->table_error->removeRow(ui->table->currentRow());
    ui->table->removeRow(ui->table->currentRow());
}

//******************* Events triggered on change / used to refresh data ************************************************************************************
void MainWindow::on_azimuthEntry_textChanged(const QString &arg1)
{
    int cursor_pos = ui->azimuthEntry->cursorPosition();
    QString entry = Angle::clean_angle_txt(arg1);
    QStringList list = entry.split(" ");

    if (Angle::validate(list))
    {
        ui->azimuthEntry->setText(Angle::angle_format(list));
           {
                /*increments cursor position if text wasn't formatted before*/
                if (arg1.indexOf(Angle::angle_symbols[Angle::DEGREE]) != -1)
                    ui->azimuthEntry->setCursorPosition(cursor_pos);
                else
                    ui->azimuthEntry->setCursorPosition(cursor_pos + 2);
           }
        Angle::azimuth_calc(ui->table, list);
    }
    else
    {
        ui->azimuthEntry->setText(list.join(" "));
        ui->azimuthEntry->setCursorPosition(cursor_pos);
    }
}

void MainWindow::on_table_cellChanged(int row, int column)
{
    bool cell_content = ui->table->item(row, column); //returns 0 if pointer is null

    if (!cell_content) { return; }

    QStringList list;

    /*which column is being altered?*/
    using namespace Angle;
    switch (column)
    {

            case POINT :
      {
                if (column == POINT)
                {
                    using namespace Angle;
                    if (ui->table->item(row, HOR_ANG) && validate(clean_angle_txt(ui->azimuthEntry->text()).split(" "))) //updates azimuth if reference is valid
                    {
                        azimuth_calc(ui->table, clean_angle_txt(ui->azimuthEntry->text()).split(" "));
                    }
                    return;
                }
     }

            case HOR_ANG :
    {
                list = clean_angle_txt(ui->table->item(row, column)->text()).split(" ");

                if (validate(list))
                {
                    if (validate(clean_angle_txt(ui->azimuthEntry->text()).split(" "))) //updates azimuth if reference is valid
                    {
                        azimuth_calc(ui->table, clean_angle_txt(ui->azimuthEntry->text()).split(" "));
                    }
                    ui->table->item(row, column)->setText(angle_format(list));
                }

                /*formats angle if a single zero is provided as input*/
                if (ui->table->item(row, column)->text() == "0")
                {
                    QStringList zero{"0", "0", "0"};
                    ui->table->item(row, column)->setText(angle_format(zero));
                }
                return;
    }

             case AZIMUTH :
    {
                list = Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ");

                if (Angle::validate(Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ")))
                {
                    list = Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ");
                    double distance;
                    double decimalAzimuth;
                    //RectCoord namespace
                    {
                        using namespace RectCoord;
                        if      (ui->table->item(row, HOR_DIST) &&
                                 ui->table_error->item(row, XCOORDINATE) &&
                                 ui->table_error->item(row, YCOORDINATE) &&
                                (distance = validate(ui->table->item(row, HOR_DIST)->text())) &&
                                (decimalAzimuth = dmsToDecimal(list)))
                        {

                            ui->table_error->item(row, XCOORDINATE)
                                    ->setText(QString::number(polarToRect(decimalAzimuth, distance, XCOORD)));
                            ui->table_error->item(row, YCOORDINATE)
                                    ->setText(QString::number(polarToRect(decimalAzimuth, distance, YCOORD)));
                        }
                        ui->table->item(row, AZIMUTH)->setText(angle_format(list));
                    }
                    //RectCoord namespace end
                }

                if (ui->table->item(row, AZIMUTH)->text() == "0") //formats angle if a single zero is provided as input
                {
                    QStringList zero{"0", "0", "0"};
                    ui->table->item(row, AZIMUTH)->setText(angle_format(zero));
                }

                return;
    }
            //basically the same code of the above case
            case HOR_DIST :
    {
                list = Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ");

                if (Angle::validate(Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ")))
                {
                    list = Angle::clean_angle_txt(ui->table->item(row, AZIMUTH)->text()).split(" ");
                    double distance;
                    double decimalAzimuth;
                    //RectCoord namespace
                    {
                        using namespace RectCoord;
                        if      (ui->table->item(row, HOR_DIST) &&
                                (ui->table_error->item(row, XCOORDINATE)) &&
                                (ui->table_error->item(row, YCOORDINATE)) &&
                                (distance = validate(ui->table->item(row, HOR_DIST)->text())) &&
                                (decimalAzimuth = dmsToDecimal(list)))
                        {
                            ui->table_error->item(row, XCOORDINATE)
                                    ->setText(QString::number(polarToRect(decimalAzimuth, distance, XCOORD)));
                            ui->table_error->item(row, YCOORDINATE)
                                    ->setText(QString::number(polarToRect(decimalAzimuth, distance, YCOORD)));
                        }
                    }
                    //RectCoord namespace end
                }
                return;
    }

    }
}

void MainWindow::on_table_itemChanged(QTableWidgetItem *item) //responsible for the angles summation
{
    QVector<QStringList> angleVec;
    QString sumText = "Soma dos Ângulos de Vante: ";

    for (int i = 0; i < ui->table->rowCount(); i++)
    {
        if (ui->table->item(i, Angle::HOR_ANG) && ui->table->item(i, Angle::POINT) && ui->table->item(i, Angle::POINT)->text() == "Vante")
        {
            angleVec.push_back(Angle::clean_angle_txt(ui->table->item(i, Angle::HOR_ANG)->text().split(" ")));
        }
    }
    if (!angleVec.isEmpty())
    {
        QStringList sumResult = Angle::angle_summation(angleVec);
        sumText += Angle::angle_format(sumResult);
        ui->sumText->setText(sumText);
    }
}

void MainWindow::on_table_error_cellChanged(int row, int column)
{
    switch (column)
    {
            case XCOORDINATE :
    {
                QString sumText = "Soma de X : ";
                QVector<QString> distSum;
                for (int i = 0; i < ui->table_error->rowCount(); ++i)
                {
                    if (ui->table_error->item(i, XCOORDINATE) &&
                       (ui->table->item(i, Angle::POINT)) &&
                       (ui->table->item(i, Angle::POINT)->text() == "Vante"))
                    {
                        distSum.push_back(ui->table_error->item(i, XCOORDINATE)->text());
                    }
                }
                if (!distSum.isEmpty())
                {
                    double sum = RectCoord::distSummation(distSum);
                    sumText += QString::number(sum);
                    ui->errorText1->setText(sumText);

                    for (int i = 0; i < ui->table_error->rowCount(); ++i)
                    {
                        if((ui->table->item(i, Angle::POINT)) &&
                           (ui->table->item(i, Angle::POINT)->text() == "Vante"))
                        {
                            double factorX;
                            double correctedX;
                            if ((ui->table_error->item(i, XCOORDINATE)) &&
                                (ui->table_error->item(i, CORRECTEDX)) &&
                                (ui->table_error->item(i, XFACTOR)))
                            {
                                factorX = sum/ui->table_error->rowCount();
                                correctedX = (ui->table_error->item(i, XCOORDINATE)->text()).toDouble() + factorX;

                                ui->table_error->item(i, XFACTOR)->setText(QString::number(factorX));
                                ui->table_error->item(i, CORRECTEDX)->setText(QString::number(correctedX));
                            }
                        }
                    }
                }
    }

            case YCOORDINATE :
    {
                QString sumText = "Soma de Y : ";
                QVector<QString> distSum;
                for (int i = 0; i < ui->table_error->rowCount(); ++i)
                {
                    if (ui->table_error->item(i, YCOORDINATE) &&
                       (ui->table->item(i, Angle::POINT)) &&
                       (ui->table->item(i, Angle::POINT)->text() == "Vante"))
                    {
                        distSum.push_back(ui->table_error->item(i, YCOORDINATE)->text());
                    }
                }
                if (!distSum.isEmpty())
                {
                    double sum = RectCoord::distSummation(distSum);
                    sumText += QString::number(sum);
                    ui->errorText2->setText(sumText);

                    for (int i = 0; i < ui->table_error->rowCount(); ++i)
                    {
                        if((ui->table->item(i, Angle::POINT)) &&
                           (ui->table->item(i, Angle::POINT)->text() == "Vante"))
                        {
                            double factorY;
                            double correctedY;
                            if ((ui->table_error->item(i, YCOORDINATE)) &&
                                (ui->table_error->item(i, CORRECTEDY)) &&
                                (ui->table_error->item(i, YFACTOR)))
                            {
                                factorY = sum/ui->table_error->rowCount();
                                correctedY = (ui->table_error->item(i, YCOORDINATE)->text()).toDouble() + factorY;

                                ui->table_error->item(i, YFACTOR)->setText(QString::number(factorY));
                                ui->table_error->item(i, CORRECTEDY)->setText(QString::number(correctedY));
                            }
                        }
                    }
                }
    }
    }

}


//******************************************************************************************************************************************************

//********************************************** File Management ***************************************************************************************

void MainWindow::on_savebutton_clicked()
{
    FileDialog dialog;
    int return_signal;

    return_signal = dialog.exec();

    if(return_signal)
    {
        QString tableString;
        QString filename;
        if (!dialog.getEntry1().isEmpty())
        {
            using namespace FileManager;
            filename = dialog.getEntry1();
            filename.contains(".csv") ? filename : filename += ".csv";
            tableString = createCsv(ui->table, ui->table_error);
            writeFile(filename, tableString);
        }

    }
}

void MainWindow::on_loadbutton_clicked()
{
    FileDialog dialog;
    int return_signal;

    return_signal = dialog.exec();

    if (return_signal)
    {
        QString filename;
        if(!dialog.getEntry1().isEmpty())
        {
            using namespace FileManager;
            filename = dialog.getEntry1();
            loadCsv(ui->table, ui->table_error, filename);
        }
    }
}
//******************************************************************************************************************************************************
