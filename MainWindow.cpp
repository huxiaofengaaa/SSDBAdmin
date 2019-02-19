#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentCommand(SSDB_COMMAND_NONE)
{
    ui->setupUi(this);
    ui->tab_keyValue->setWindowTitle("xxxx");
    ui->tabWidget_valueType->setTabText(0, "Key Value");
    ui->tabWidget_valueType->setTabText(1, "HashMap");
    ui->tabWidget_valueType->setTabText(2, "Sorted Set");
    ui->tabWidget_valueType->setTabText(3, "List");
    QObject::connect(&tcpclient,
                     SIGNAL(ResponseReady(QByteArray)),
                     this,
                     SLOT(ssdbResponseReady(QByteArray)));
    QObject::connect(ui->treeWidget_keyValue,
                     SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                     this,
                     SLOT(KeyValueTreeWidgetOneClicked(QTreeWidgetItem*, int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    QObject::disconnect(&tcpclient, SIGNAL(ResponseReady(QByteArray)), this, SLOT(ssdbResponseReady(QByteArray)));
    QObject::disconnect(ui->treeWidget_keyValue,
                        SIGNAL(itemClicked(QTreeWidgetItem*, int)),
                        this,
                        SLOT(KeyValueTreeWidgetOneClicked(QTreeWidgetItem*, int)));
}

bool MainWindow::sendSSDBCommandRequest(std::string cmd)
{
    qDebug() << cmd.c_str();
    if(false == tcpclient.writeCount(cmd.c_str(), cmd.size()))
    {
        return false;
    }
    return true;
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

void MainWindow::ssdbResponseReady(QByteArray response)
{
    qDebug() << response;
    bool resolveResult = false;
    switch(currentCommand)
    {
    case SSDB_COMMAND_KV_KEYS:
    {
        std::vector<std::string> keylist;
        resolveResult = SSDBCommand::KeysResolve(response.data(), keylist);
        if(true == resolveResult)
        {
            ui->treeWidget_keyValue->clear();
            for(auto iter = keylist.begin(); iter != keylist.end(); iter++)
            {
                QTreeWidgetItem *keyValueIterm = new QTreeWidgetItem(ui->treeWidget_keyValue);
                keyValueIterm->setText(0, (*iter).c_str());
            }
        }
        break;
    }
    case SSDB_COMMAND_KV_SCAN:
    {
        std::vector<std::pair<std::string, std::string>> keylist;
        resolveResult = SSDBCommand::ScanResolve(response.data(), keylist);
        if(true == resolveResult)
        {
            ui->treeWidget_keyValue->clear();
            for(auto iter = keylist.begin(); iter != keylist.end(); iter++)
            {
                QTreeWidgetItem *keyValueIterm = new QTreeWidgetItem(ui->treeWidget_keyValue);
                keyValueIterm->setText(0, (*iter).first.c_str());
                keyValueIterm->setText(2, (*iter).second.c_str());
            }
        }
        break;
    }
    case SSDB_COMMAND_KV_DEL:
    case SSDB_COMMAND_KV_SET:
    case SSDB_COMMAND_KV_SETX:
    {
        resolveResult = SSDBCommand::SetResolve(response.data());
        break;
    }
    default:
    {
        qDebug() << "not send request but reve response, error";
        break;
    }
    }
    if(false == resolveResult)
    {
        NoticeDialog notice{"resolve ssdb response failed"};
        notice.exec();
    }
    currentCommand = SSDB_COMMAND_NONE;
}

void MainWindow::on_pushButton_connect_clicked()
{
    std::string hostString = ui->lineEdit_host->text().toStdString();
    std::string portString = ui->lineEdit_port->text().toStdString();
    std::string passwdString = ui->lineEdit_passwd->text().toStdString();

    std::string host = (hostString.empty() == true) ? "127.0.0.1" : hostString;
    int port = (portString.empty() == true) ? 8888 : std::atoi(portString.c_str());
    qDebug() << "host " << host.c_str() << " port " << port;

    if(true == tcpclient.isConnected())
    {
        if(true == tcpclient.disConnect())
        {
            qDebug() << "disconnect from server success";
            ui->pushButton_connect->setText("Connect");
        }
    }
    else
    {
        if(true == tcpclient.connect(host, port))
        {
            qDebug() << "connect to " << host.c_str() << ":" << port << " success";
            ui->pushButton_connect->setText("DisConect");
        }
    }
}

void MainWindow::on_pushButton_keyValue_update_clicked()
{
    if(currentCommand != SSDB_COMMAND_NONE)
    {
        NoticeDialog notice{"current command is not SSDB_COMMAND_NONE"};
        notice.exec();
        return;
    }

    std::string key_start = ui->lineEdit_keyValue_update_keyStart->text().toStdString();
    std::string key_end   = ui->lineEdit_keyValue_update_keyEnd->text().toStdString();
    std::string key_limit = ui->lineEdit_keyValue_update_keyLimit->text().toStdString();
    std::string cmd = SSDBCommand::ScanBuild(key_start, key_end, key_limit);
    currentCommand = SSDB_COMMAND_KV_SCAN;

    if(sendSSDBCommandRequest(cmd) == false)
    {
        currentCommand = SSDB_COMMAND_NONE;
        NoticeDialog notice{"send cmd failed"};
        notice.exec();
        return;
    }
}

void MainWindow::on_pushButton_keyValue_insert_clicked()
{
    if(currentCommand != SSDB_COMMAND_NONE)
    {
        NoticeDialog notice{"current command is not SSDB_COMMAND_NONE"};
        notice.exec();
        return;
    }

    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();
    std::string value = ui->lineEdit_keyValue_insert_value->text().toStdString();
    std::string expire = ui->lineEdit_keyValue_insert_expire->text().toStdString();

    if(key.empty() == true || value.empty() == true)
    {
        NoticeDialog notice{"key & value can not be null"};
        notice.exec();
        return;
    }

    std::string cmd;
    if(!expire.empty())
    {
        cmd = SSDBCommand::SetxBuild(key, value, expire);
        currentCommand = SSDB_COMMAND_KV_SETX;
    }
    else
    {
        cmd = SSDBCommand::SetBuild(key, value);
        currentCommand = SSDB_COMMAND_KV_SET;
    }

    if(sendSSDBCommandRequest(cmd) == false)
    {
        currentCommand = SSDB_COMMAND_NONE;
        NoticeDialog notice{"send cmd failed"};
        notice.exec();
        return;
    }

    ui->lineEdit_keyValue_insert_key->clear();
    ui->lineEdit_keyValue_insert_value->clear();
    ui->lineEdit_keyValue_insert_expire->clear();
}

void MainWindow::on_pushButton_keyValue_delete_clicked()
{
    if(currentCommand != SSDB_COMMAND_NONE)
    {
        NoticeDialog notice{"current command is not SSDB_COMMAND_NONE"};
        notice.exec();
        return;
    }

    std::string key = ui->lineEdit_keyValue_insert_key->text().toStdString();
    if(key.empty()== true)
    {
        NoticeDialog notice{"key can not be null"};
        notice.exec();
        return;
    }

    std::string cmd = SSDBCommand::DelBuild(key);
    currentCommand = SSDB_COMMAND_KV_DEL;

    if(sendSSDBCommandRequest(cmd) == false)
    {
        currentCommand = SSDB_COMMAND_NONE;
        NoticeDialog notice{"send cmd failed"};
        notice.exec();
        return;
    }
}
