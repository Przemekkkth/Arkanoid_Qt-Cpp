#ifndef GAME_H
#define GAME_H
#include <QSize>
#include <QString>

class Game
{
public:
    Game();
    const QSize RESOLUTION;
    const QString PATH_TO_BACKGROUND_PIXMAP;
    const QString PATH_TO_PADDLE_PIXMAP;
    const QString PATH_TO_BALL_PIXMAP;
    const QString PATH_TO_BLOCKS_PIXMAP[5];
    const size_t COUNT_OF_KIND_OF_BLOCKS;
    const float ITERATION_VALUE;
};

#endif // GAME_H
