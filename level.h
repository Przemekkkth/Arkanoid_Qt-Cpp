#ifndef LEVEL_H
#define LEVEL_H
#include <QVector>
#include <QPoint>

struct LevelData
{
    int colorValue;
    QPoint position;
};

class Level
{
public:
    Level();
    ~Level();
    void loadLevel(const QString& pathFile);
    QVector<LevelData>* m_levelData;
};

#endif // LEVEL_H
