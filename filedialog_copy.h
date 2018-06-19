#ifndef WARNINGDIAG_H
#define WARNINGDIAG_H

#include <QDialog>

namespace Ui {
class WarningDiag;
}

class WarningDiag : public QDialog
{
    Q_OBJECT

public:
    explicit WarningDiag(QWidget *parent = 0);
    ~WarningDiag();

private:
    Ui::WarningDiag *ui;
};

#endif // WARNINGDIAG_H
