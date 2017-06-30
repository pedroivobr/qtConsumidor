#ifndef PLOTTER_H
#define PLOTTER_H

#include <QWidget>
#include <QVector>
#include <vector>
using namespace std;
class Plotter : public QWidget
{
    Q_OBJECT
private:
    qint64 min,max;
    vector<double> dados;
    double intervalo;
    QColor fundo;
public:
    explicit Plotter(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *e);
    void recebeDados(vector<double> _dados, double intervalo);
signals:

public slots:
};

#endif // PLOTTER_H
