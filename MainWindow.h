#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "TCPClient.h"
#include "SSDBCommand.h"
#include "NoticeDialog.h"

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
    void ssdbResponseReady(QByteArray);
    void KeyValueTreeWidgetOneClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_connect_clicked();
    void on_pushButton_keyValue_update_clicked();
    void on_pushButton_keyValue_insert_clicked();
    void on_pushButton_keyValue_delete_clicked();

private:
    bool                     sendSSDBCommandRequest(std::string cmd);
    Ui::MainWindow           *ui;
    TCPClient                tcpclient;
    SSDBCommandType          currentCommand;
};

#endif // MAINWINDOW_H
