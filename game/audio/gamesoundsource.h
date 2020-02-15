#ifndef GAMESOUNDSOURCE_H
#define GAMESOUNDSOURCE_H

#include <QObject>

class GameSoundSource : public QObject
{
    Q_OBJECT
public:
    explicit GameSoundSource(QObject *parent = nullptr);



    qreal radius() const;
    void setRadius(qreal radius);

private:
    qreal m_radius = 1;

signals:

};

#endif // GAMESOUNDSOURCE_H
