#include <QtQml>
#include <QGuiApplication>
#include <QtQml/QQmlContext>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>

#include "game.h"
#include "debugcategories.h"

static const char *const normal = "\033[0m";
static const char *const warning = "\e[33m";
static const char *const error = "\e[31m";

static QHash<QString, bool> s_loggingFilters;

static void loggingCategoryFilter(QLoggingCategory *category)
{
    // If this is a known category
    if (s_loggingFilters.contains(category->categoryName())) {
        category->setEnabled(QtDebugMsg, s_loggingFilters.value(category->categoryName()));
        category->setEnabled(QtWarningMsg, true);
        category->setEnabled(QtCriticalMsg, true);
        category->setEnabled(QtFatalMsg, true);
    } else {
        //Disable default debug messages, print only >= warnings
        category->setEnabled(QtDebugMsg, false);
        category->setEnabled(QtWarningMsg, true);
        category->setEnabled(QtCriticalMsg, true);
        category->setEnabled(QtFatalMsg, true);
    }
}

static void consoleLogHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    switch (type) {
    case QtInfoMsg:
        fprintf(stdout, " I | %s: %s\n", context.category, message.toUtf8().data());
        break;
    case QtDebugMsg:
        fprintf(stdout, " I | %s: %s\n", context.category, message.toUtf8().data());
        break;
    case QtWarningMsg:
        fprintf(stdout, "%s W | %s: %s%s\n", warning, context.category, message.toUtf8().data(), normal);
        break;
    case QtCriticalMsg:
        fprintf(stdout, "%s C | %s: %s%s\n", error, context.category, message.toUtf8().data(), normal);
        break;
    case QtFatalMsg:
        fprintf(stdout, "%s F | %s: %s%s\n", error, context.category, message.toUtf8().data(), normal);
        break;
    }
    fflush(stdout);
}



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    qInstallMessageHandler(consoleLogHandler);

    QGuiApplication app(argc, argv);
    app.setApplicationName("Self finding chronicles");
    app.setApplicationDisplayName("Self finding chronicles");
    app.setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QString("\nFantasy role play game.\n\n"
                                             "Copyright %1 2018 Simon Stürz <stuerz.simon@gmail.com>\n"
                                             "Released under the GNU GPLv3.").arg(QChar(0xA9)));
    parser.addPositionalArgument("dataPath", "The relative file path where the \"data\" folders can be found (optional).", "[dataPath]");
    parser.process(app);


    // Enable debug categories
    s_loggingFilters.insert("Game", true);
    s_loggingFilters.insert("World", true);
    s_loggingFilters.insert("Player", false);
    s_loggingFilters.insert("PlayerController", true);
    s_loggingFilters.insert("Map", true);
    s_loggingFilters.insert("Item", false);
    s_loggingFilters.insert("Collision", false);

    s_loggingFilters.insert("qml", true);

    QLoggingCategory::installFilter(loggingCategoryFilter);

    // Game
    qmlRegisterSingletonType<Game>("Chronicles", 1, 0, "Game", Game::qmlInstance);
    qmlRegisterUncreatableType<World>("Chronicles", 1, 0, "World", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Field>("Chronicles", 1, 0, "Field", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Player>("Chronicles", 1, 0, "Player", "Can't create this in QML. Get it from the world object.");
    qmlRegisterUncreatableType<PlayerController>("Chronicles", 1, 0, "PlayerController", "Can't create this in QML. Get it from the world object.");
    qmlRegisterUncreatableType<Map>("Chronicles", 1, 0, "Map", "Can't create this in QML. Get it from the World object.");

    // Items
    qmlRegisterUncreatableType<GameObject>("Chronicles", 1, 0, "GameObject", "Can't create this in QML. Get it from the world object.");
    qmlRegisterUncreatableType<GameItem>("Chronicles", 1, 0, "GameItem", "Can't create this in QML. Get it from the world object.");
    qmlRegisterUncreatableType<PlantItem>("Chronicles", 1, 0, "PlantItem", "Can't create this in QML. Get it from the world object.");


    QQmlApplicationEngine engine;
    if (!parser.positionalArguments().isEmpty()) {
        QDir dataDir(QDir::cleanPath(QCoreApplication::applicationDirPath() + "/" + parser.positionalArguments().first()));
        if (!dataDir.exists()) {
            qWarning() << dataDir.path() << "does not exist.";
            exit(-1);
        }
        engine.rootContext()->setContextProperty("dataDirectory", dataDir.path() + "/");
    } else {
        engine.rootContext()->setContextProperty("dataDirectory", QCoreApplication::applicationDirPath() + "/../selffinding-chronicles/data/");
    }

    qCDebug(dcGame()) << "Data path:" << engine.rootContext()->contextProperty("dataDirectory").toString();

    engine.rootContext()->setContextProperty("version", app.applicationVersion());
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
