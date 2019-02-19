#ifndef NOTICEDIALOG_H
#define NOTICEDIALOG_H

#include <QDialog>

namespace Ui {
class NoticeDialog;
}

class NoticeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NoticeDialog(QWidget *parent = 0, QString message = "");
    explicit NoticeDialog(QString message = "");
    ~NoticeDialog();

private slots:
    void on_pushButton_confirm_clicked();

private:
    Ui::NoticeDialog *ui;
};

#endif // NOTICEDIALOG_H
