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

private slots:
    void on_newpoint_clicked();

    void on_removepoint_clicked();

    void on_azimuthEntry_textChanged(const QString &arg1);

    void on_table_cellChanged(int row, int column);

private:
    Ui::MainWindow *ui;
    QVector<QString> m_tableData;

private:
    void grab_table();
    void debug_table();
};

#endif // MAINWINDOW_H
