#include "plotter.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <cmath>
#include <QDebug>
#include <QMouseEvent>
#include <QColor>
#include <vector>
using namespace std;
//Metodo construtor
Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    fundo.setRgb(0,255,0);
    startTimer(1000);
    dados.push_back(0);
}

//Função que faz os ajustes e realize o desenho
void Plotter::paintEvent(QPaintEvent *e){
    float x1,x2,y1,y2;

    // cria um objeto pintor
    QPainter p(this);

    // faz com que a linha seja desenhada
    // com tracos suaves
    p.setRenderHint(QPainter::Antialiasing);

    // cria um objeto de caneta
    QPen pen;
    // cria um objeto de pincel
    QBrush brush;
    // define a cor da caneta
    pen.setColor(QColor(255,0,0));
    pen.setWidth(4);

    // informa ao painter qual a caneta a ser
    // usada
    p.setPen(pen);

    // define a cor do pincel
    brush.setColor(fundo);
    brush.setStyle(Qt::SolidPattern);

    // informa oa painter qual o pincel a ser
    // usado
    p.setBrush(brush);
    p.drawRect(0,0,width(),height());
    // (x1,y1) -> (x2,y2)
    pen.setColor(QColor(255,255,0));
    p.setPen(pen);

    y1 = dados[0];
    x1 = 0.0;
    y2 = dados[1];
    x2 = intervalo;
    qDebug() << x2;
    p.drawLine(x1*width(),(1 - y1)*height(), x2*width(),(1 - y2)*height());
    float nDados = dados.size();
    for(int i=1; i<nDados; i++){
        qDebug() << nDados;
        x1 =x2;
        y1 = y2;
        y2 = dados[i];
        x2 += intervalo;
        qDebug() << width() << x1 << " : " << y1 <<" " << x2 << " : " <<y2;
        p.drawLine(x1*width(),(1 - y1)*height(), x2*width(),(1 - y2)*height());
    }
}

//Função que recebe os dados
void Plotter::recebeDados(vector<double> _dados,double _intervalo){
    dados = _dados;
    intervalo = _intervalo;
    repaint();
}
