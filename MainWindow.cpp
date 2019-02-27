#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget_valueType->setTabText(0, "Key Value");
    ui->tabWidget_valueType->setTabText(1, "HashMap");
    ui->tabWidget_valueType->setTabText(2, "Sorted Set");
    ui->tabWidget_valueType->setTabText(3, "List");

    QObject::connect(ui->treeWidget_keyValue, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                     this, SLOT(KeyValueTreeWidgetOneClicked(QTreeWidgetItem*, int)));

    m_applicationInIdle = true;
}

MainWindow::~MainWindow()
{
    delete ui;
    QObject::disconnect(ui->treeWidget_keyValue, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                        this, SLOT(KeyValueTreeWidgetOneClicked(QTreeWidgetItem*, int)));
}

bool MainWindow::getControlIfApplicationInIdleStatus()
{
    m_applicationInIdleMutex.lock();
    if(m_applicationInIdle == true)
    {
        m_applicationInIdle = false;
        m_applicationInIdleMutex.unlock();
        return true;
    }
    else
    {
        m_applicationInIdleMutex.unlock();
        return false;
    }
}

void MainWindow::releaseApplicationControl()
{
    m_applicationInIdleMutex.lock();
    m_applicationInIdle = true;
    m_applicationInIdleMutex.unlock();
}

void MainWindow::KeyValueTreeWidgetOneClicked(QTreeWidgetItem *item, int)
{
    if(item)
    {
        std::string key = item->text(1).toStdString();
        std::string expire = item->text(2).toStdString();
        std::string value = item->text(3).toStdString();

        ui->lineEdit_keyValue_insert_key->setText(key.c_str());
        ui->lineEdit_keyValue_insert_expire->setText(expire.c_str());
        ui->lineEdit_keyValue_insert_value->setText(value.c_str());
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(false == getControlIfApplicationInIdleStatus())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }

    std::string hostString = ui->lineEdit_host->text().toStdString();
    std::string portString = ui->lineEdit_port->text().toStdString();
    std::string passwdString = ui->lineEdit_passwd->text().toStdString();

    if(true == m_ssdbHandler.isConnected())
    {
        if(true == m_ssdbHandler.disConnectFromSSDB())
        {
            ui->pushButton_connect->setText("Connect");
        }
        else
        {
            qDebug() << "disconnect from ssdb server error";
        }
    }
    else
    {
        if(true == m_ssdbHandler.connectToSSDB(hostString,
                                               std::atoi(portString.c_str()),
                                               passwdString))
        {
            ui->pushButton_connect->setText("DisConect");
        }
        else
        {
            qDebug() << "connect to ssdb server error";
        }
    }

    releaseApplicationControl();
}

void MainWindow::on_pushButton_keyValue_update_clicked()
{
    if(false == getControlIfApplicationInIdleStatus())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }

    std::string key_start = ui->lineEdit_keyValue_update_keyStart->text().toStdString();
    std::string key_end   = ui->lineEdit_keyValue_update_keyEnd->text().toStdString();
    std::string key_limit = ui->lineEdit_keyValue_update_keyLimit->text().toStdString();

    std::vector<std::pair<std::string, std::string>> l_keyValueLists;
    if(true == m_ssdbHandler.getKeyValueLists(key_start, key_end, key_limit, l_keyValueLists))
    {
        ui->treeWidget_keyValue->clear();
        int l_itemIndex = 1;
        for(auto iter = l_keyValueLists.begin(); iter != l_keyValueLists.end(); iter++)
        {
            QTreeWidgetItem *keyValueIterm = new QTreeWidgetItem(ui->treeWidget_keyValue);
            keyValueIterm->setText(0, std::to_string(l_itemIndex).c_str());
            keyValueIterm->setText(1, (*iter).first.c_str());
            keyValueIterm->setText(3, (*iter).second.c_str());
            l_itemIndex++;
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " - getKeyValueLists error";
    }

    releaseApplicationControl();
}

void MainWindow::on_pushButton_keyValue_insert_clicked()
{
    if(false == getControlIfApplicationInIdleStatus())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }

    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();
    std::string value = ui->lineEdit_keyValue_insert_value->text().toStdString();
    std::string expire = ui->lineEdit_keyValue_insert_expire->text().toStdString();

    if(key.empty() == false && value.empty() == false)
    {
        if(true == m_ssdbHandler.insertKeyValue(key, value, expire))
        {
            ui->lineEdit_keyValue_insert_key->clear();
            ui->lineEdit_keyValue_insert_value->clear();
            ui->lineEdit_keyValue_insert_expire->clear();
        }
        else
        {
            qDebug() << __FUNCTION__ << " - insertKeyValue error";
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " - key or value is null";
    }

    releaseApplicationControl();
    on_pushButton_keyValue_update_clicked();
}

void MainWindow::on_pushButton_keyValue_delete_clicked()
{
    if(false == getControlIfApplicationInIdleStatus())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }

    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();

    if(key.empty() == false)
    {
        if(true == m_ssdbHandler.deleteKeyValue(key))
        {
            ui->lineEdit_keyValue_insert_key->clear();
            ui->lineEdit_keyValue_insert_value->clear();
            ui->lineEdit_keyValue_insert_expire->clear();
        }
        else
        {
            qDebug() << __FUNCTION__ << " - deleteKeyValue error";
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " - key is null";
    }

    releaseApplicationControl();
    on_pushButton_keyValue_update_clicked();
}
