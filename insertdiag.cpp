#include "insertdiag.h"
#include "ui_insertdiag.h"
#include "warningdiag.h"

InsertDiag::InsertDiag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertDiag)
{
    ui->setupUi(this);
    setWindowTitle("Novo Ponto");
}

//******** Getters and Setters ******************************************
QString InsertDiag::getEntry1() const
{
    return ui->entry1->text();
}

QString InsertDiag::getEntry2() const
{
    return ui->entry2->text();
}

QString InsertDiag::getEntry3() const
{
    return ui->entry3->text();
}

QString InsertDiag::getEntry4() const
{
    return ui->entry4->text();
}

QString InsertDiag::getEntry5() const
{
    return ui->entry5->text();
}

QString InsertDiag::getEntry6() const
{
    return ui->entry6->text();
}

void InsertDiag::setEntry1(QString str) const {
    ui->entry1->setText(str);
}
//*******************************************************************

InsertDiag::~InsertDiag()
{
    delete ui;
}
