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
    void                     runSSDBActionTaskThread();
    SSDBCommandType          getCurrentSSDBCommandType();
    void                     setCurrentSSDBCommandType(SSDBCommandType p_type);

    bool                     MainWindwActionConnectOrDisConnect();
    bool                     MainWindwActionKeyValueUpdate();
    bool                     MainWindwActionKeyValueInsert();
    bool                     MainWindwActionKeyValueDelete();

    Ui::MainWindow           *ui;
    SSDBHandler              m_ssdbHandler;
    std::thread              m_taskThread;
    bool                     m_taskThreadExit;
    SSDBCommandType          m_currentCommand;
    std::mutex               m_currentCommandMutex;
};

#endif // MAINWINDOW_H
