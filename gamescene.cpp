#include "gamescene.h"
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsPixmapItem>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}, m_game(), m_level(), m_timer(new QTimer(this)), m_paddleXpos(300), m_paddleYpos(440),
      m_ballXpos(300), m_ballYpos(428), m_moveRight(false), m_moveLeft(false)
{
    loadPixmap();
    loadLevel(":/levels/level.lvl");

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
    Q_ASSERT(m_game_over_bgPixmap.load(m_game.PATH_TO_GAME_OVER_BG_PIXMAP));
    Q_ASSERT(m_pause_bgPixmap.load(m_game.PATH_TO_PAUSE_BG_PIXMAP));
    Q_ASSERT(m_victoryPixmap.load(m_game.PATH_TO_VICTORY_BG_PIXMAP));
}

void GameScene::loadLevel(const QString &pathFile)
{
    m_level.loadLevel(pathFile);
    m_blockItems.clear();
    m_blockItems.resize(m_level.m_levelData->size());
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

void GameScene::checkVictory()
{
    if(m_level.m_levelData->isEmpty())
    {
        m_game.m_state = Game::State::Win;
    }
}

void GameScene::update()
{
    clear();
    m_blockItems.clear();
    m_blockItems.resize(m_level.m_levelData->size());
    QGraphicsPixmapItem *bgIteam = new QGraphicsPixmapItem(m_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
    addItem(bgIteam);

    QGraphicsPixmapItem *paddleIteam = new QGraphicsPixmapItem(m_paddlePixmap);
    addItem(paddleIteam);
    paddleIteam->setPos(m_paddleXpos, m_paddleYpos);

    QGraphicsPixmapItem *ballIteam = new QGraphicsPixmapItem(m_ballPixmap);
    addItem(ballIteam);


    for(int i = 0; i < m_level.m_levelData->size(); ++i)
    {
        m_blockItems[i] = new QGraphicsPixmapItem(QPixmap(m_game.PATH_TO_BLOCKS_PIXMAP[m_level.m_levelData->at(i).colorValue]));
        m_blockItems[i]->setPos(m_level.m_levelData->at(i).position.x(),m_level.m_levelData->at(i).position.y() );
        addItem(m_blockItems[i]);
    }
    if(m_moveLeft)
    {
        m_paddleXpos -= m_game.PADDLE_SPEED;
    }
    else if(m_moveRight)
    {
        m_paddleXpos += m_game.PADDLE_SPEED;
    }
    clampPaddle();
    stuckBall();


    if(!m_game.m_isBallStucked && m_game.m_state == Game::State::Active)
    {
        m_ballXpos += m_game.m_deltaX;
        if (m_ballXpos < 0 || m_ballXpos > m_game.RESOLUTION.width() - m_ballPixmap.width())
        {
            m_game.m_deltaX = -m_game.m_deltaX;
        }
        for(int idx = 0; idx < m_level.m_levelData->count(); ++idx)
        {
            if( QRectF(m_ballXpos, m_ballYpos, m_ballPixmap.width(), m_ballPixmap.height()).intersects(
                        QRectF(m_blockItems[idx]->pos().x(), m_blockItems[idx]->pos().y(), m_blockItems[idx]->boundingRect().width(), m_blockItems[idx]->boundingRect().height()))
                    )
            {
                m_game.m_deltaX = -m_game.m_deltaX;
                m_level.m_levelData->removeAt(idx);
                m_blockItems[idx]->setPos(-100, -100);
            }
        }

        m_ballYpos += m_game.m_deltaY;
        if (m_ballYpos< 0)
        {
            m_game.m_deltaY= -m_game.m_deltaY;
        }
        else if(m_ballYpos > Game::DEATH_Y_LEVEL)
        {
            m_game.m_state = Game::State::GameOver;
        }

        for(int idx = 0; idx < m_level.m_levelData->count(); ++idx)
        {
            if( QRectF(m_ballXpos, m_ballYpos, m_ballPixmap.width(), m_ballPixmap.height()).intersects(
                        QRectF(m_blockItems[idx]->pos().x(), m_blockItems[idx]->pos().y(), m_blockItems[idx]->boundingRect().width(), m_blockItems[idx]->boundingRect().height()))
                    )
            {
                m_game.m_deltaY = -m_game.m_deltaY;
                m_level.m_levelData->removeAt(idx);
                m_blockItems[idx]->setPos(-100,-100);
            }
        }

        checkVictory();

        if (m_ballXpos >= m_paddleXpos &&
                m_ballXpos + m_ballPixmap.width() <= m_paddleXpos + m_paddlePixmap.width() &&
                m_ballYpos + m_ballPixmap.height() >= m_paddleYpos &&
                m_paddleYpos + m_paddlePixmap.height()/2 >= m_ballYpos )
        {
            m_ballYpos = paddleIteam->y() - m_ballPixmap.height();
            m_game.m_deltaY = -(rand()%5+2);
        }

    }
    ballIteam->setPos(m_ballXpos, m_ballYpos);

    if(m_game.m_state == Game::State::GameOver)
    {
        QGraphicsPixmapItem *goBgIteam = new QGraphicsPixmapItem(m_game_over_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(goBgIteam);
    }
    else if(m_game.m_state == Game::State::Pause)
    {
        QGraphicsPixmapItem *pauseBgIteam = new QGraphicsPixmapItem(m_pause_bgPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(pauseBgIteam);
    }
    else if(m_game.m_state == Game::State::Win)
    {
        QGraphicsPixmapItem *victoryBgIteam = new QGraphicsPixmapItem(m_victoryPixmap.scaled(m_game.RESOLUTION.width(), m_game.RESOLUTION.height()));
        addItem(victoryBgIteam);
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{

    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_A:
    {
        if(m_game.m_state == Game::State::Active)
        {
            m_moveLeft = true;
        }
    }
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
    {
        if(m_game.m_state == Game::State::Active)
        {
            m_moveRight = true;
        }
    }
        break;
    case Qt::Key_Space:
    {
        if(m_game.m_state == Game::State::Active)
        {
            if(m_game.m_isBallStucked)
            {
                m_game.m_isBallStucked = false;
            }
        }
    }
        break;
    case Qt::Key_P:
    {
        if(m_game.m_state == Game::State::Active)
        {
            m_game.m_state = Game::State::Pause;
        }
        else if(m_game.m_state == Game::State::Pause)
        {
            m_game.m_state = Game::State::Active;
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
        m_moveLeft = false;
    }
        break;
    case Qt::Key_Right:
    case Qt::Key_D:
    {
        m_moveRight = false;
    }

    }
    QGraphicsScene::keyReleaseEvent(event);
}
