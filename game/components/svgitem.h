#ifndef SVGITEM_H
#define SVGITEM_H

#include <QObject>
#include <QImage>
#include <QColor>
#include <QDomDocument>
#include <QSvgRenderer>
#include <QQuickPaintedItem>

class SvgItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit SvgItem(QQuickItem *parent = nullptr);

    QUrl source() const;
    void setSource(const QUrl &source);

    QColor color() const;
    void setColor(const QColor &color);

protected:
    virtual void paint(QPainter *painter);

private:
    QSvgRenderer *m_renderer = nullptr;
    QUrl m_source;
    QColor m_color;
    QDomDocument m_svgDom;

signals:
    void sourceChanged(const QUrl &source);
    void colorChanged(const QColor &color);

private slots:
    void render();

};

#endif // SVGITEM_H
