#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertdiag.h"
#include "warningdiag.h"
#include "angle.h"
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>

#define last_row ui->table->rowCount()-1

enum columns
{
    STATION = 0, POINT,  HOR_ANG, AZIMUTH, HOR_DIST, OBS
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
    return_signal = diag.exec();

    if (return_signal == InsertDiag::Accepted)
    {
        if(diag.getEntry1().isEmpty())
        {
            WarningDiag w_diag;
            w_diag.exec();
            return;
        }
        ui->table->insertRow(last_row+1);
        for (int column = 0; column < ui->table->columnCount(); ++column)
        {
            ui->table->setItem(last_row, column, (new QTableWidgetItem("")));
        }
        ui->table->setItem(last_row, STATION, new QTableWidgetItem(diag.getEntry1()));
        ui->table->setItem(last_row, POINT, new QTableWidgetItem(diag.getEntry2()));
        ui->table->setItem(last_row, HOR_ANG, new QTableWidgetItem(diag.getEntry3()));
        ui->table->setItem(last_row, AZIMUTH, new QTableWidgetItem(diag.getEntry4()));
        ui->table->setItem(last_row, HOR_DIST, new QTableWidgetItem(diag.getEntry5()));
        ui->table->setItem(last_row, OBS, new QTableWidgetItem(diag.getEntry6()));
    }
}

void MainWindow::on_removepoint_clicked()
{
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
        ui->azimuthEntry->setCursorPosition(cursor_pos);
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
    QStringList list = Angle::clean_angle_txt(ui->table->item(row, column)->text()).split(" ");

    if (column == HOR_ANG) //checks if its a non-null column related to angles
    {
        if (Angle::validate(list))
        {
            if (Angle::validate(Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" "))) //updates azimuth if reference is valid
            { Angle::azimuth_calc(ui->table, Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" ")); }

            ui->table->item(row, column)->setText(Angle::angle_format(list));
        }
        return;
    }

    if (column == AZIMUTH) //checks if its a non-null column related to angles
    {
        if (Angle::validate(list))
        { ui->table->item(row, column)->setText(Angle::angle_format(list)); }
        return;
    }

    if (column == POINT)
    {
        if (ui->table->item(row, HOR_ANG) && Angle::validate(Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" "))) //updates azimuth if reference is valid
        { Angle::azimuth_calc(ui->table, Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" ")); }
        return;
    }
}

//******************************************************************************************************************************************************

//****************************** Unimplemented stuff / To be continued... **********************************************************************************
void MainWindow::debug_table() //used only for debug purposes
{
    for (int i = 0; i < ui->table->columnCount(); ++i)
        { qDebug() << m_tableData[i]; }
}

void MainWindow::grab_table() //used to store table data into a vector. will be user in the future for persistent data
{
    int rows = ui->table->rowCount();
    int columns = ui->table->columnCount();

    for (int i = 0; i < rows; ++i)
        for(int j = 0; j < columns; ++j)
        {
            if (ui->table->item(i,j))
                m_tableData.push_back(ui->table->item(i,j)->text());
            else
                m_tableData.push_back("null");
        }
}
//******************************************************************************************************************************************************
