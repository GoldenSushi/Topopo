#include "filedialog.h"
#include "ui_filedialog.h"

FileDialog::FileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
    setFixedWidth(250);
    setFixedHeight(100);
    setWindowFlag(Qt::FramelessWindowHint);
}

FileDialog::~FileDialog()
{
    delete ui;
}

QString FileDialog::getEntry1() const
{
    return ui->fileline->text();
}
