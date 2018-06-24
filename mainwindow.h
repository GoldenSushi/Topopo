#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;

private slots:
    void on_newpoint_clicked();

    void on_removepoint_clicked();

    void on_azimuthEntry_textChanged(const QString &arg1);

    void on_table_cellChanged(int row, int column);

    void on_savebutton_clicked();

    void on_table_itemChanged(QTableWidgetItem *item);

    void on_loadbutton_clicked();

    void on_table_error_cellChanged(int row, int column);

private:
    QVector<QString> m_tableData;

};

#endif // MAINWINDOW_H
