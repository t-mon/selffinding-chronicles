#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include <QtQml/QQmlContext>

#include "game.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    app.setApplicationName("Self finding chronicles");
    app.setApplicationDisplayName("Self finding chronicles");
    app.setApplicationVersion("0.0.1");

    qmlRegisterSingletonType<Game>("Chronicles", 1, 0, "Game", Game::qmlInstance);
    qmlRegisterUncreatableType<World>("Chronicles", 1, 0, "World", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Field>("Chronicles", 1, 0, "Field", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Player>("Chronicles", 1, 0, "Player", "Can't create this in QML. Get it from the Game instance.");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
