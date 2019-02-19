#include "NoticeDialog.h"
#include "ui_NoticeDialog.h"

NoticeDialog::NoticeDialog(QWidget *parent, QString message) :
    QDialog(parent),
    ui(new Ui::NoticeDialog)
{
    ui->setupUi(this);
    ui->label_notice->setText(message);
}

NoticeDialog::NoticeDialog(QString message):
    QDialog(NULL),
    ui(new Ui::NoticeDialog)
{
    ui->setupUi(this);
    ui->label_notice->setText(message);
}

NoticeDialog::~NoticeDialog()
{
    delete ui;
}

void NoticeDialog::on_pushButton_confirm_clicked()
{
    accept();
}
