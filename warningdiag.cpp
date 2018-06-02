#include "warningdiag.h"
#include "ui_warningdiag.h"

WarningDiag::WarningDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WarningDiag)
{
    ui->setupUi(this);
    setWindowTitle("Aviso");
    setFixedHeight(100);
    setFixedWidth(400);
}

WarningDiag::~WarningDiag()
{
    delete ui;
}
