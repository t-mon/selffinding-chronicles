#ifndef LITERATUREITEM_H
#define LITERATUREITEM_H

#include <QObject>

#include "gameitem.h"

class LiteratureItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(LiteratureType literatureType READ literatureType NOTIFY literatureTypeChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString text READ text NOTIFY textChanged)

public:
    enum LiteratureType {
        LiteratureTypeBook,
        LiteratureTypeScript
    };
    Q_ENUM(LiteratureType)

    explicit LiteratureItem(QObject *parent = nullptr);
    ~LiteratureItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    LiteratureType literatureType() const;
    void setLiteratureType(LiteratureType literatureType);

    QString title() const;
    void setTitle(const QString &title);

    QString description() const;
    void setDescription(const QString &description);

    QString text() const;
    void setText(const QString &text);

private:
    LiteratureType m_literatureType = LiteratureTypeBook;
    QString m_title;
    QString m_description;
    QString m_text;

signals:
    void literatureTypeChanged(LiteratureType literatureType);
    void titleChanged(const QString &title);
    void descriptionChanged(const QString &description);
    void textChanged(const QString &text);

};

QDebug operator<<(QDebug debug, LiteratureItem *literatureItem);


#endif // LITERATUREITEM_H
