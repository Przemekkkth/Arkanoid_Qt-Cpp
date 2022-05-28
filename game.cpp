#include "game.h"

Game::Game()
    : RESOLUTION(QSize(520, 450)), PATH_TO_BACKGROUND_PIXMAP(":/images/background.jpg"),
      PATH_TO_PADDLE_PIXMAP(":/images/paddle.png"), PATH_TO_BALL_PIXMAP(":/images/ball.png"),
      PATH_TO_BLOCKS_PIXMAP{":/images/block01.png", ":/images/block02.png", ":/images/block03.png",
                            ":/images/block04.png", ":/images/block05.png"},
      COUNT_OF_KIND_OF_BLOCKS(5), ITERATION_VALUE(1000.0f/60.0f), PADDLE_SPEED(16), m_isBallStucked(true), m_deltaX(5),
      m_deltaY(-6)
{

}
