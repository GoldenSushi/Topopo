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

 public:

    QString getEntry1() const;
    QString getEntry2() const;
    QString getEntry3() const;
    QString getEntry4() const;
    QString getEntry5() const;
    QString getEntry6() const;

    void setEntry1(QString) const;

private:
    Ui::InsertDiag *ui;
};

#endif // INSERTDIAG_H
