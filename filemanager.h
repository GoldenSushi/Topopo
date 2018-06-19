#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <QString>
#include <QTableWidget>
#include <QFile>
#include <QDebug>
#include <string>

namespace FileManager {

static QString createCsv(QTableWidget *table, QTableWidget *table_error)
{
    QString csv = "";
    if (table && table_error)
    {

        for (int column = 0; column < table->columnCount(); column++) //add header to string
        {
            csv += table->horizontalHeaderItem(column)->text();
            csv += ";";
        }
        for (int column = 2; column < table_error->columnCount(); ++column)
        {
           csv += table_error->horizontalHeaderItem(column)->text();
           csv += ";";
        }

        csv += '\n';

        for (int i = 0; i < table->rowCount(); i++)
        {
            for (int j = 0; j < table->columnCount(); j++)
            {
                if (table->item(i , j)->text().isEmpty()) { csv += ";"; }
                else
                {
                    csv += table->item(i, j)->text();
                    csv += ";";
                }
            }
            for (int j = 2; j < table_error->columnCount(); ++j)
            {
                if (table_error->item(i, j)->text().isEmpty()) { csv += ";"; }
                else
                {
                    csv += table_error->item(i, j)->text();
                    csv += ";";
                }
            }
            csv += '\n';
        }
    }
    return csv;
}

void clearTable(QTableWidget *table, QTableWidget *table_error)
{
    for (int i = 0; i < table->rowCount(); ++i)
    {
        table->removeRow(i);
        table_error->removeRow(i);
    }
}

}

static void loadCsv(QTableWidget * table, QTableWidget *table_error, QString &filename)
{
    QStringList fields;
    int totalColumnCount = table->columnCount() + table_error->columnCount() - 2; //minus two to compensate the repeated columns
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream input(&file);

        FileManager::clearTable(table, table_error);
        int count = 0;
        int table_errorCount = 0;
        input.readLine(); //discards header
        while (!input.atEnd())
        {
            fields = input.readLine().split(";");
            table->insertRow(count);
            table_error->insertRow(count);
            for (int i = 0; i < totalColumnCount; ++i)
            {
                if (i <= 1 )
                {
                    table->setItem(count, i, new QTableWidgetItem(fields[i]));
                    table_error->setItem(count, i, new QTableWidgetItem(fields[i]));
                    ++table_errorCount;
                }
                else if (i < table->columnCount())
                {
                    table->setItem(count, i, new QTableWidgetItem(fields[i]));
                }
                else if (i >= table->columnCount())
                {
                    table_error->setItem(count, table_errorCount, new QTableWidgetItem(fields[i]));
                    ++table_errorCount;
                }
            }
            ++count;
        }
    }
}

static void writeFile(QString &filename, QString &text)
{
    QFile csvfile(filename);
    if (csvfile.open(QIODevice::WriteOnly))
    {
        QTextStream output(&csvfile);
        output << text;
    }
}

#endif // FILEMANAGER_H
