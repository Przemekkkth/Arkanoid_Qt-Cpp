#include "gamescene.h"
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, m_game(), m_timer(new QTimer(this)), m_paddleXpos(300), m_paddleYpos(440),
      m_ballXpos(300), m_ballYpos(428)
{
    loadPixmap();
    stuckBall();
    setSceneRect(0, 0, m_game.RESOLUTION.width(), m_game.RESOLUTION.height());
    connect(m_timer, &QTimer::timeout, this, &GameScene::update);
    m_timer->start(m_game.ITERATION_VALUE);
}

void GameScene::loadPixmap()
{
    Q_ASSERT(m_paddlePixmap.load(m_game.PATH_TO_PADDLE_PIXMAP));
    for(size_t i = 0; i < m_game.COUNT_OF_KIND_OF_BLOCKS; ++i)
    {
        Q_ASSERT(m_blocksPixmap[i].load(m_game.PATH_TO_BLOCKS_PIXMAP[i]));
    }
    Q_ASSERT(m_bgPixmap.load(m_game.PATH_TO_BACKGROUND_PIXMAP));
    Q_ASSERT(m_ballPixmap.load(m_game.PATH_TO_BALL_PIXMAP));
}

void GameScene::clampPaddle()
{
    if(m_paddleXpos < 0)
    {
        m_paddleXpos = 0;
    }
    else if(m_paddleXpos + m_paddlePixmap.width() > m_game.RESOLUTION.width())
    {
        m_paddleXpos = m_game.RESOLUTION.width() - m_paddlePixmap.width();
    }
}

void GameScene::stuckBall()
{
    if(m_game.m_isBallStucked)
    {
        m_ballXpos = m_paddleXpos + m_paddlePixmap.width()/2 - m_ballPixmap.width()/2;
    }
}

void GameScene::update()
{
    clear();
    QGraphicsPixmapItem *bgIteam = new QGraphicsPixmapItem(m_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
    addItem(bgIteam);

    QGraphicsPixmapItem *paddleIteam = new QGraphicsPixmapItem(m_paddlePixmap);
    addItem(paddleIteam);
    paddleIteam->setPos(m_paddleXpos, m_paddleYpos);

    QGraphicsPixmapItem *ballIteam = new QGraphicsPixmapItem(m_ballPixmap);
    addItem(ballIteam);
    ballIteam->setPos(m_ballXpos, m_ballYpos);


}

void GameScene::keyPressEvent(QKeyEvent *event)
{

        switch (event->key()) {
        case Qt::Key_Left:
        case Qt::Key_A:
        {
            m_paddleXpos -= 6;
            clampPaddle();
            stuckBall();
        }
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
        {
            m_paddleXpos += 6;
            clampPaddle();
            stuckBall();
        }
            break;
        case Qt::Key_Space:
        {
            if(m_game.m_isBallStucked)
            {
                m_game.m_isBallStucked = false;
            }
        }
            break;
        }


    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
    {

    }
        break;
    }
    QGraphicsScene::keyReleaseEvent(event);
}
