#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
private:
    int id;
    double intervaloTempo;
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
    void tcpConnect(QString str);
    void temporizador();
    void timerEvent(QTimerEvent *e);
public slots:
  void getData();
  void listarIP();
private slots:
  void on_buttonConnect_clicked();
  void on_buttonDisconnect_clicked();
  void on_buttonUpdate_clicked(bool checked);
  void on_buttonStart_clicked(bool checked);
  void on_buttonStop_clicked(bool checked);
private:
  Ui::MainWindow *ui;
  QTcpSocket *socket;
};

#endif // MAINWINDOW_H
