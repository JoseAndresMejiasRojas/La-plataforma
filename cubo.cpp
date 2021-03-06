#include "Cubo.h"
#include "player.h"
#include "info.h"
#include "score.h"

#include <QTimer>
#include <QList>
#include <QGraphicsScene>
#include <typeinfo>//typeid
#include <stdlib.h> //rand

#include <QDebug>

extern Player* player; //El jugador se hace global ya que será usado en otras clases
extern Score* score; //Se debe acceder en esta clase
extern bool gameOver;
extern bool immunity_variable;
extern bool protection_variable;

Cubo::Cubo() : QObject(), QGraphicsRectItem()
{
    //225 es la mitad del largo de la plataforma.
    int random_number = (rand() % 500) - 225;//[-225, 225] Rango de la plataforma
    //Lo pongo en una posición aleatoria
    setPos( random_number, 0 );

    //Dibujo el objeto
    setRect(0, 0, 15, 15);

    //Cambio de color los cubos
    QBrush brush(Qt::black);
    this->setBrush(brush);

    //Conecto
    QTimer* timer = new QTimer();
    //Cada cierto tiempo, se invocará a move
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));

    timer->start(30);//30 milisegundos
}

void Cubo::move()
{
    QList< QGraphicsItem* > colliding_items = collidingItems();//Lista con todos los Item

    for( int contador = 0; contador < colliding_items.size(); ++contador )
    {
        // Si el cubo choca con el jugador.
        if( typeid( *(colliding_items[contador]) ) == typeid(Player) )
        {
            //Muestra el Game Over
            Info* info = new Info();
            scene()->addItem(info);
            info->setPos(info->x()-100, info->y()+300);

            scene()->removeItem(player); //Elimina el jugador (global)

            scene()->removeItem(this);//Saco de escena al cubo
            delete this;//Elimino el cubo.

            gameOver = true;

            return;
        }
    }

    // la velocidad en que cae el cubo
    setPos( x(), y() + 20 );

    //El cubo sólo puede llegar hasta 493, este valor porque se ve bien a la vista, por la velocidad en que cae.
    if( pos().y() + rect().height() > 493 )
    {
        if(!gameOver)
            score->increase();

        scene()->removeItem(this);
        delete this;
    }
}
