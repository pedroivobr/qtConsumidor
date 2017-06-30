#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "plotter.h"
#include <string>
#include <QDateTime>
#include <QString>
#include <QStringList>

using namespace std;
//Metodo construtor da classe
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect(ui->pushButtonGet,
            SIGNAL(clicked(bool)),
            this,
            SLOT(getData()));
}
//Metodo de conexão com o servidor
void MainWindow::tcpConnect(QString str){
    socket->connectToHost(str,1234);
    if(socket->waitForConnected(3000)){
        qDebug() << "Connected";
    }
    else{
        qDebug() << "Disconnected";
    }
}

//Metodo que envia o get e recebe as informações do servidor
void MainWindow::getData(){
    QString str,write;
    QByteArray array;
    QStringList list;
    qint64 thetime;

    std::vector<double> dados,tmp,dadosNormalizado,tempo,tempoNormalizado;
    std::vector<double>::iterator min,max;

    qDebug() << "to get data...";
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            write = "get " + ui->listWidget->currentItem()->text() + " 30\r\n";
            qDebug() << "write value: " << write;
            socket->write(write.toStdString().c_str());
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            qDebug() << socket->bytesAvailable();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                list = str.split(" ");
                if(list.size() == 2){
                    bool ok;
                    str = list.at(0);
                    thetime = str.toLongLong(&ok); //tempo
                    str = list.at(1);              //dado
                    qDebug() << thetime << ": " << str;
                    dados.push_back(str.toDouble());
                    tmp.push_back(thetime);
                }
            }
            for(unsigned int i=0; i<tmp.size();i++){
                tempo.push_back(tmp[i] - tmp[0]);
            }
        }
    }
    tmp.clear();

    max = max_element(dados.begin(),dados.end());
    min = min_element(dados.begin(),dados.end());
    qDebug() << dados.size();
    for(unsigned int i=0; i<dados.size(); i++){
        tmp[i] = (dados[i] - *min)/(*max-*min);
        dadosNormalizado.push_back(tmp[i]);
    }
    intervaloTempo = 1.0/tempo.size();
    /*
    tmp.clear();
    max = tempo.end();
    min = tempo.begin();
    tempoNormalizado.push_back(tempo[0]);
    for(unsigned int i=1; i<dados.size(); i++){
        //tmp[i] = (tempo[i] - *min)/(*max - *min);
        tmp[i] = 1/max;
        tempoNormalizado.push_back(tmp[i]);
    }*/
    ui->widgetPlotter->recebeDados(dadosNormalizado,intervaloTempo);
}


//Metodo que faz a listagem de IP
void MainWindow::listarIP()
{
    ui->listWidget->clear();
    QString str;
    if(socket->state() == QAbstractSocket::ConnectedState){
        if(socket->isOpen()){
            qDebug() << "reading...";
            socket->write("list");
            socket->waitForBytesWritten();
            socket->waitForReadyRead();
            while(socket->bytesAvailable()){
                str = socket->readLine().replace("\n","").replace("\r","");
                ui->listWidget->addItem(str);
            }
        }
    }
}


MainWindow::~MainWindow()
{
    delete socket;
    delete ui;
}

//Ação do botão para conectar
void MainWindow::on_buttonConnect_clicked()
{
    tcpConnect(ui->lineEdit->text());
    if(socket->isOpen())
        listarIP();
}

//Ação do botão para disconectar
void MainWindow::on_buttonDisconnect_clicked()
{
    socket->disconnectFromHost();
}
//Ação do botão para atualizar lista de ip
void MainWindow::on_buttonUpdate_clicked(bool checked)
{
    listarIP();
}

//Função padrão de temporizador
void MainWindow::timerEvent(QTimerEvent *e){
    getData();
}

//Função com o start do temporizador
void MainWindow::temporizador(){
    id = startTimer((ui->horizontalSlider->value())*1000);
}

//Inicia o envio de dados e o temporizador
void MainWindow::on_buttonStart_clicked(bool checked)
{
    if(socket->isOpen()){
        temporizador();
    }
}

//Para o envio
void MainWindow::on_buttonStop_clicked(bool checked)
{
    killTimer(id);
}
