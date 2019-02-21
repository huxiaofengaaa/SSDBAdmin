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

    m_currentCommand = SSDB_COMMAND_NONE;

    m_taskThreadExit = false;
    auto l_threadTask = std::bind(&MainWindow::runSSDBActionTaskThread, this);
    m_taskThread = std::move(std::thread(l_threadTask));
}

MainWindow::~MainWindow()
{
    delete ui;
    QObject::disconnect(ui->treeWidget_keyValue, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                        this, SLOT(KeyValueTreeWidgetOneClicked(QTreeWidgetItem*, int)));
    m_taskThreadExit = true;
    m_taskThread.join();
}

SSDBCommandType MainWindow::getCurrentSSDBCommandType()
{
    m_currentCommandMutex.lock();
    SSDBCommandType l_currentType = m_currentCommand;
    m_currentCommandMutex.unlock();
    return l_currentType;
}

void MainWindow::setCurrentSSDBCommandType(SSDBCommandType p_type)
{
    m_currentCommandMutex.lock();
    m_currentCommand = p_type;
    m_currentCommandMutex.unlock();
}

void MainWindow::KeyValueTreeWidgetOneClicked(QTreeWidgetItem *item, int)
{
    if(item)
    {
        std::string key = item->text(0).toStdString();
        std::string expire = item->text(1).toStdString();
        std::string value = item->text(2).toStdString();

        ui->lineEdit_keyValue_insert_key->setText(key.c_str());
        ui->lineEdit_keyValue_insert_expire->setText(expire.c_str());
        ui->lineEdit_keyValue_insert_value->setText(value.c_str());
    }
}

bool MainWindow::MainWindwActionConnectOrDisConnect()
{
    std::string hostString = ui->lineEdit_host->text().toStdString();
    std::string portString = ui->lineEdit_port->text().toStdString();
    std::string passwdString = ui->lineEdit_passwd->text().toStdString();

    if(true == m_ssdbHandler.isConnected())
    {
        if(true == m_ssdbHandler.disConnectFromSSDB())
        {
            ui->pushButton_connect->setText("Connect");
            return true;
        }
        else
        {
            qDebug() << "disconnect from ssdb server error";
        }
    }
    else
    {
        if(true == m_ssdbHandler.connectToSSDB(hostString, std::atoi(portString.c_str()), passwdString))
        {
            ui->pushButton_connect->setText("DisConect");
            return true;
        }
        else
        {
            qDebug() << "connect to ssdb server error";
        }
    }
    return false;
}

bool MainWindow::MainWindwActionKeyValueUpdate()
{
    std::string key_start = ui->lineEdit_keyValue_update_keyStart->text().toStdString();
    std::string key_end   = ui->lineEdit_keyValue_update_keyEnd->text().toStdString();
    std::string key_limit = ui->lineEdit_keyValue_update_keyLimit->text().toStdString();

    std::vector<std::pair<std::string, std::string>> l_keyValueLists;
    if(m_ssdbHandler.getKeyValueLists(key_start, key_end, key_limit, l_keyValueLists) == false)
    {
        qDebug() << "get key value list error";
        return false;
    }

    ui->treeWidget_keyValue->clear();
    for(auto iter = l_keyValueLists.begin(); iter != l_keyValueLists.end(); iter++)
    {
        QTreeWidgetItem *keyValueIterm = new QTreeWidgetItem(ui->treeWidget_keyValue);
        keyValueIterm->setText(0, (*iter).first.c_str());
        keyValueIterm->setText(2, (*iter).second.c_str());
    }

    return true;
}

bool MainWindow::MainWindwActionKeyValueInsert()
{
    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();
    std::string value = ui->lineEdit_keyValue_insert_value->text().toStdString();
    std::string expire = ui->lineEdit_keyValue_insert_expire->text().toStdString();

    if(key.empty() == true || value.empty() == true)
    {
        qDebug() << "when insert keyValue, key & value can not be empty";
        return false;
    }

    if(m_ssdbHandler.insertKeyValue(key, value, expire) == false)
    {
        qDebug() << "ssdb hanlder insert key value error";
        return false;
    }

    ui->lineEdit_keyValue_insert_key->clear();
    ui->lineEdit_keyValue_insert_value->clear();
    ui->lineEdit_keyValue_insert_expire->clear();
    return true;
}

bool MainWindow::MainWindwActionKeyValueDelete()
{
    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();
    if(key.empty()== true)
    {
        qDebug() << "when delete keyValue, key can not be empty";
        return false;
    }

    if(m_ssdbHandler.deleteKeyValue(key) == false)
    {
        qDebug() << "ssdb handler delete key error";
        return false;
    }

    ui->lineEdit_keyValue_insert_key->clear();
    ui->lineEdit_keyValue_insert_value->clear();
    ui->lineEdit_keyValue_insert_expire->clear();
    return true;
}

void MainWindow::runSSDBActionTaskThread()
{
    while(m_taskThreadExit == false)
    {
        SSDBCommandType l_currentType = getCurrentSSDBCommandType();
        if(l_currentType == SSDB_COMMAND_NONE)
        {
            Utility::Msleep(10);
            continue;
        }

        switch(l_currentType)
        {
        case SSDB_COMMAND_CONNECT:
        {
            MainWindwActionConnectOrDisConnect();
            break;
        }
        case SSDB_COMMAND_KV_SCAN:
        {
            MainWindwActionKeyValueUpdate();
            break;
        }
        case SSDB_COMMAND_KV_SET:
        {
            MainWindwActionKeyValueInsert();
            break;
        }
        case SSDB_COMMAND_KV_DEL:
        {
            MainWindwActionKeyValueDelete();
            break;
        }
        default:
        {
            qDebug() << "unknown ssdb command " << l_currentType;
            break;
        }
        }
        setCurrentSSDBCommandType(SSDB_COMMAND_NONE);
    }
}

void MainWindow::on_pushButton_connect_clicked()
{
    if(SSDB_COMMAND_NONE != getCurrentSSDBCommandType())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }
    setCurrentSSDBCommandType(SSDB_COMMAND_CONNECT);
}

void MainWindow::on_pushButton_keyValue_update_clicked()
{
    if(SSDB_COMMAND_NONE != getCurrentSSDBCommandType())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }
    setCurrentSSDBCommandType(SSDB_COMMAND_KV_SCAN);
}

void MainWindow::on_pushButton_keyValue_insert_clicked()
{
    if(SSDB_COMMAND_NONE != getCurrentSSDBCommandType())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }
    setCurrentSSDBCommandType(SSDB_COMMAND_KV_SET);
}

void MainWindow::on_pushButton_keyValue_delete_clicked()
{
    if(SSDB_COMMAND_NONE != getCurrentSSDBCommandType())
    {
        NoticeDialog notice{"SSDB Client is busy now"};
        notice.exec();
        return;
    }
    setCurrentSSDBCommandType(SSDB_COMMAND_KV_DEL);
}
