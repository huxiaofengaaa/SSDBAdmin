#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <thread>
#include "TCPClient.h"
#include "SSDBCommand.h"
#include "SSDBHandler.h"
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
    void KeyValueTreeWidgetOneClicked(QTreeWidgetItem *item, int column);
    void on_pushButton_connect_clicked();
    void on_pushButton_keyValue_update_clicked();
    void on_pushButton_keyValue_insert_clicked();
    void on_pushButton_keyValue_delete_clicked();

private:
    bool                     getControlIfApplicationInIdleStatus();
    void                     releaseApplicationControl();

    Ui::MainWindow           *ui;
    SSDBHandler              m_ssdbHandler;
    bool                     m_applicationInIdle;
    std::mutex               m_applicationInIdleMutex;
};

#endif // MAINWINDOW_H
