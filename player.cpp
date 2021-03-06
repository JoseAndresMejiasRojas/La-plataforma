#include "player.h"
#include "info.h"
#include "cubo.h"
#include "game.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QMediaPlayer>

bool jumping = false;
bool gameOver = false;
bool immunity_variable = false;
bool protection_variable = false;

extern Game* game;
extern QMediaPlayer* music;

Player::Player()
{
    setRect(0,420,30,60); //Se crea el jugador en las coordenadas 0,420 con tamaño 30,60
}

void Player::keyPressEvent(QKeyEvent *event)
{
    timer = new QTimer();

    if(event->key() == Qt::Key_Left) //Izquierda, si esta agachado se levanta.
    {
        if(rect().height() == 30)
        {
            setRect(0,0,30,60); //Cambia el tamaño.
            setY(420); //Coordenada de la plataforma en y.
        }

        setPos(x()-10, y());

        if(x() == -280)
        {
            connect(timer, SIGNAL(timeout()), this, SLOT(fallDown()));
            timer->start(10);
        }
    }
    else if(event->key() == Qt::Key_Right) //Derecha, si esta agachado se levanta
    {
        if(rect().height() == 30)
        {
            setRect(0,0,30,60); //Aumenta el tamaño
            setY(420); //Coordenada de la plataforma en y.
        }

        setPos(x()+10, y());

        if(x() == 300)
        {
            connect(timer, SIGNAL(timeout()), this, SLOT(fallDown()));
            timer->start(10);
        }
    }
    else if(event->key() == Qt::Key_Down) //Si está levantado se agacha.
    {
        if(!gameOver)
        {
            if(rect().height() != 30) //Si la altura no es 30 ( es 60 )
            {
                if(y() == 0)
                    setRect(0,y()+450,30,30);
                else
                    setRect(0,30,30,30);
            }
        }

    }
    else if(event->key() == Qt::Key_Up) //Brinca
    {
        if(y() == 0 || y() == 420)
        {
            if(jumping == false)
            {
                connect(timer, SIGNAL(timeout()), this, SLOT(ascend()));
                timer->start(10);

                jumping = true;

            }

        }

    }
}

void Player::ascend()
{
    if(y() <= 0 && y() > -60) //Si esta en este rango, se mueve
        this->setPos(x(), y()-2);
    else if(y() <= 420 && y() > 360) //Si esta en este rango se mueve
        this->setPos(x(), y()-2);

    //qDebug() << y();

    if(y()==-60 || y() == 360) //Cuando esta en el cielo debe bajar, se llama fall()
    {
        timer->stop();

        connect(timer, SIGNAL(timeout()), this, SLOT(fall()));
        timer->start(20);
    }
}

void Player::fall()
{
    if(y() < 420) //Rango de movimiento
        this->setPos(x(), y()+2);

    if(y() == 0 || y() == 420) //Cuando llega al suelo se detiene
        timer->stop();

    jumping = false;
}

void Player::fallDown()
{
    this->setPos(x(),y()+2);

    if(y() == 600 || y() == 160)
    {
        Info* info = new Info();
        scene()->addItem(info);
        info->setPos(info->x()-100, info->y()+300);

        delete this;
        gameOver = true;
        music->stop();
    }

}

