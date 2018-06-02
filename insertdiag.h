#ifndef INSERTDIAG_H
#define INSERTDIAG_H

#include <QDialog>

namespace Ui {
class InsertDiag;
}

class InsertDiag : public QDialog
{
    Q_OBJECT

public:

    explicit InsertDiag(QWidget *parent = 0);
    ~InsertDiag();

//setters and getters
 public:

    QString getEntry1() const;
    QString getEntry2() const;
    QString getEntry3() const;
    QString getEntry4() const;
    QString getEntry5() const;
    QString getEntry6() const;

    void setEntry1(QString str) const;
    void setEntry2(QString str) const;
    void setEntry3(QString str) const;
    void setEntry4(QString str) const;
    void setEntry5(QString str) const;
    void setEntry6(QString str) const;


private:
    Ui::InsertDiag *ui;
};

#endif // INSERTDIAG_H
