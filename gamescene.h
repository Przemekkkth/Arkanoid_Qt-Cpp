#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QPixmap>
#include "game.h"

class QTimer;
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:
private slots:
    void update();

private:
    void loadPixmap();
    void clampPaddle();
    void stuckBall();

    QPixmap m_paddlePixmap;
    QPixmap m_blocksPixmap[5];
    QPixmap m_bgPixmap;
    QPixmap m_ballPixmap;

    Game m_game;
    QTimer* m_timer;

    int m_paddleXpos;
    int m_paddleYpos;
    int m_ballXpos;
    int m_ballYpos;

    bool m_moveRight, m_moveLeft;
    // QGraphicsScene interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // GAMESCENE_H
