#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "insertdiag.h"
#include "warningdiag.h"
#include "angle.h"
#include <QDebug>
#include <QSettings>
#include <QCloseEvent>

#define TABLE ui->table
#define last_row ui->table->rowCount()-1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Topopo");
    TABLE->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //columns size fill the window
    TABLE->horizontalHeader()->setMinimumHeight(40);
    TABLE->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::debug_table()
{
    for (int i = 0; i < TABLE->columnCount(); ++i)
        {
            qDebug() << m_tableData[i];
        }
}

void MainWindow::grab_table() //used to store table data into a vector
{
    int rows = ui->table->rowCount();
    int columns = ui->table->columnCount();

    for (int i = 0; i < rows; ++i)
        for(int j = 0; j < columns; ++j)
        {
            if (TABLE->item(i,j))
                m_tableData.push_back(TABLE->item(i,j)->text());
            else
                m_tableData.push_back("null");
        }
}

//void MainWindow::closeEvent(QCloseEvent *event)
//{
//    grab_table();
//}

void MainWindow::on_newpoint_clicked()
{
    InsertDiag diag;
    int return_signal;
    QString autofill = "";


    //checks if item isnt null
    if (TABLE->item(last_row, 0))
        autofill = TABLE->item(last_row, 0)->text();

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

        TABLE->insertRow(last_row+1);
        for (int column = 0; column < TABLE->columnCount(); ++column)
        {
            TABLE->setItem(last_row, column, (new QTableWidgetItem("")));
        }
        TABLE->setItem(last_row, STATION, new QTableWidgetItem(diag.getEntry1()));
        TABLE->setItem(last_row, POINT, new QTableWidgetItem(diag.getEntry2()));
        TABLE->setItem(last_row, HOR_ANG, new QTableWidgetItem(diag.getEntry3()));
        TABLE->setItem(last_row, AZIMUTH, new QTableWidgetItem(diag.getEntry4()));
        TABLE->setItem(last_row, HOR_DIST, new QTableWidgetItem(diag.getEntry5()));
        TABLE->setItem(last_row, OBS, new QTableWidgetItem(diag.getEntry6()));
    }
}

void MainWindow::on_removepoint_clicked()
{
    TABLE->removeRow(ui->table->currentRow());
}

void MainWindow::on_azimuthEntry_textChanged(const QString &arg1)
{
    int cursor_pos = ui->azimuthEntry->cursorPosition();
    QString entry = arg1;

    entry = Angle::clean_angle_txt(entry);

    QStringList list = entry.split(" ");

    qDebug() << "[VALIDATE]: " << Angle::validate(list);
    if (Angle::validate(list))
    {
        ui->azimuthEntry->setText(Angle::angle_format(list));
        ui->azimuthEntry->setCursorPosition(cursor_pos);
        Angle::azimuth_calc(TABLE, list);
    }
    else
    {
        ui->azimuthEntry->setText(list.join(" "));
        ui->azimuthEntry->setCursorPosition(cursor_pos);
    }


}

void MainWindow::on_table_cellChanged(int row, int column)
{
    QStringList list;
    bool cell_content = TABLE->item(row, column); //returns 0 if pointer is null

    if (cell_content && column == HOR_ANG) //checks if its a non-null column related to angles
    {
        list = Angle::clean_angle_txt(TABLE->item(row, column)->text()).split(" ");
        if (Angle::validate(list))
        {
            if (Angle::validate(Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" "))) //updates azimuth if reference is valid
                Angle::azimuth_calc(TABLE, Angle::clean_angle_txt(ui->azimuthEntry->text()).split(" "));

            TABLE->item(row, column)->setText(Angle::angle_format(list));
        }
        return;
    }

    if (cell_content && column == AZIMUTH) //checks if its a non-null column related to angles
    {
        list = Angle::clean_angle_txt(TABLE->item(row, column)->text()).split(" ");
        if (Angle::validate(list))
        {
            TABLE->item(row, column)->setText(Angle::angle_format(list));
        }
    }

}
