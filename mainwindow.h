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

public: //public vars
    enum
    {
        STATION = 0, POINT,  HOR_ANG, AZIMUTH, HOR_DIST, OBS
    };


public: //constructors
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//protected:
//    void closeEvent(QCloseEvent*) override;

private slots:
    void on_newpoint_clicked();

    void on_removepoint_clicked();

    void on_azimuthEntry_textChanged(const QString &arg1);

    void on_table_cellChanged(int row, int column);

private: //priv vars
    Ui::MainWindow *ui;
    QVector<QString> m_tableData;

private: //priv methods
    void grab_table();
    void debug_table();
};

#endif // MAINWINDOW_H
