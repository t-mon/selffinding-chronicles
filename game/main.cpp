/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                               *
 * Copyright (C) 2018 Simon Stürz <simon.stuerz@guh.io>                          *
 *                                                                               *
 * This file is part of selffinding-cronicles.                                   *
 *                                                                               *
 * This program is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by          *
 * the Free Software Foundation, either version 3 of the License, or             *
 * (at your option) any later version.                                           *
 *                                                                               *
 * This program is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
 * GNU General Public License for more details.                                  *
 *                                                                               *
 * You should have received a copy of the GNU General Public License             *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.         *
 *                                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <QtQml>
#include <QGuiApplication>
#include <QtQml/QQmlContext>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>

#include "game.h"
#include "debugcategories.h"
#include "audio/gamesoundeffect.h"
#include "conversation/conversationitem.h"
#include "conversation/conversationitems.h"

#include "box2dplugin.h"

#ifndef ANDROID
static const char *const normal = "\033[0m";
static const char *const warning = "\033[33m";
static const char *const error = "\033[31m";
#endif

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

#ifndef ANDROID

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
#endif // ANDROID



int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifndef ANDROID
    qInstallMessageHandler(consoleLogHandler);
#endif

    QGuiApplication app(argc, argv);
    app.setApplicationName("Self finding chronicles");
    app.setApplicationDisplayName("Self finding chronicles");
    app.setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();
    parser.setApplicationDescription(QString("\nFantasy role play game.\n\n"
                                             "Copyright %1 2018 - 2020 Simon Stürz <stuerz.simon@gmail.com>\n"
                                             "Released under the GNU GPLv3.").arg(QChar(0xA9)));

    QCommandLineOption dataOption({"d", "data"}, "Specify the relative or absolut path to the game data folder. Default value relative to executable: ../selffinding-chronicles/data", "datapath", "../selffinding-chronicles/data");
    QCommandLineOption platformOption({"p", "platform"}, "Specify the platform where the game is running. Available platforms are: [desktop, mobile]. \nDefault is \'desktop\'", "platform", "desktop");
    parser.addOption(dataOption);
    parser.addOption(platformOption);
    parser.process(app);

    // Enable debug categories
    s_loggingFilters.insert("Game", true);
    s_loggingFilters.insert("GameInput", true);
    s_loggingFilters.insert("Engine", true);
    s_loggingFilters.insert("EngineData", true);
    s_loggingFilters.insert("Character", true);
    s_loggingFilters.insert("Settings", true);
    s_loggingFilters.insert("PlayerController", true);
    s_loggingFilters.insert("PathController", false);
    s_loggingFilters.insert("Map", true);
    s_loggingFilters.insert("MapScene", true);
    s_loggingFilters.insert("MapEditor", true);
    s_loggingFilters.insert("MapEditorData", true);
    s_loggingFilters.insert("DataManager", true);
    s_loggingFilters.insert("SoundEngine", false);
    s_loggingFilters.insert("Item", true);
    s_loggingFilters.insert("GameObject", false);
    s_loggingFilters.insert("GameModels", false);
    s_loggingFilters.insert("Collision", false);
    s_loggingFilters.insert("Conversation", false);
    s_loggingFilters.insert("Teleportation", false);
    s_loggingFilters.insert("qml", true);

    QLoggingCategory::installFilter(loggingCategoryFilter);

    qCDebug(dcGame()) << "Starting" << app.applicationName() << app.applicationVersion();

    // Load Box2D qml plugin and register it
    Box2DPlugin plugin;
    plugin.registerTypes("Box2D");

    // Game
    qmlRegisterSingletonType<Game>("Chronicles", 1, 0, "Game", Game::qmlInstance);

    qmlRegisterUncreatableType<Engine>("Chronicles", 1, 0, "Engine", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Field>("Chronicles", 1, 0, "Field", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<Map>("Chronicles", 1, 0, "Map", "Can't create this in QML. Get it from the Engine object.");
    qmlRegisterUncreatableType<MapScene>("Chronicles", 1, 0, "MapScene", "Can't create this in QML. Get it from the Engine object.");
    qmlRegisterUncreatableType<Engine>("Chronicles", 1, 0, "Engine", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<DataManager>("Chronicles", 1, 0, "DataManager", "Can't create this in QML. Get it from the Engine object.");
    qmlRegisterUncreatableType<SaveGame>("Chronicles", 1, 0, "SaveGame", "Can't create this in QML. Get it from the DataManager object.");
    qmlRegisterUncreatableType<SaveGames>("Chronicles", 1, 0, "SaveGames", "Can't create this in QML. Get it from the DataManager object.");
    qmlRegisterUncreatableType<PlayerController>("Chronicles", 1, 0, "PlayerController", "Can't create this in QML. Get it from the Engine object.");
    qmlRegisterUncreatableType<GameMapEditor>("Chronicles", 1, 0, "GameMapEditor", "Can't create this in QML. Get it from the Game instance.");

    qmlRegisterUncreatableType<SoundEngine>("Chronicles", 1, 0, "SoundEngine", "Can't create this in QML. Get it from the Game instance.");
    qmlRegisterUncreatableType<AudioDecoderStream>("Chronicles", 1, 0, "AudioDecoderStream", "Can't create this in QML. Create it with the SoundEngine instance.");

    qmlRegisterType<GameSoundEffect>("Chronicles", 1, 0, "GameSoundEffect");

    // Objects
    qmlRegisterUncreatableType<GameObject>("Chronicles", 1, 0, "GameObject", "Can't create this in QML.");
    qmlRegisterUncreatableType<GameObjects>("Chronicles", 1, 0, "GameObjects", "Can't create this in QML.");
    qmlRegisterType<GameObjectsProxy>("Chronicles", 1, 0, "GameObjectsProxy");

    // Items
    qmlRegisterUncreatableType<GameItem>("Chronicles", 1, 0, "GameItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<GameItems>("Chronicles", 1, 0, "GameItems", "Can't create this in QML.");
    qmlRegisterType<GameItemsProxy>("Chronicles", 1, 0, "GameItemsProxy");

    qmlRegisterUncreatableType<TreeItem>("Chronicles", 1, 0, "TreeItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<PlantItem>("Chronicles", 1, 0, "PlantItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<WeaponItem>("Chronicles", 1, 0, "WeaponItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<FirearmItem>("Chronicles", 1, 0, "FirearmItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<LiteratureItem>("Chronicles", 1, 0, "LiteratureItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<TeleporterItem>("Chronicles", 1, 0, "TeleporterItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<ChestItem>("Chronicles", 1, 0, "ChestItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<Character>("Chronicles", 1, 0, "Character", "Can't create this in QML.");
    qmlRegisterUncreatableType<Enemy>("Chronicles", 1, 0, "Enemy", "Can't create this in QML.");

    qmlRegisterUncreatableType<LockItem>("Chronicles", 1, 0, "LockItem", "Can't create this in QML.");
    qmlRegisterUncreatableType<FireItem>("Chronicles", 1, 0, "FireItem", "Can't create this in QML.");
    qmlRegisterType<LightSource>("Chronicles", 1, 0, "LightSource");

    // Weather areas
    qmlRegisterType<WeatherAreaProxy>("Chronicles", 1, 0, "WeatherAreaProxy");
    qmlRegisterUncreatableType<WeatherAreaModel>("Chronicles", 1, 0, "WeatherAreaModel", "Can't create this in QML.");

    // Conversation
    qmlRegisterType<Conversation>("Chronicles", 1, 0, "Conversation");
    qmlRegisterType<ConversationItem>("Chronicles", 1, 0, "ConversationItem");
    qmlRegisterType<ConversationItems>("Chronicles", 1, 0, "ConversationItems");

    // Create qml engine
    QQmlApplicationEngine engine;
#ifdef ANDROID
    engine.rootContext()->setContextProperty("dataDirectory", "assets:");
#else
    QDir dataDirectory(parser.value(dataOption));
    if (!dataDirectory.makeAbsolute()) {
        qCCritical(dcGame()) << "Invalid data path argument:" << parser.value(dataOption);
        exit(-1);
    }

    if (!dataDirectory.exists()) {
        qCCritical(dcGame()) << dataDirectory.path() << "does not exist.";
        exit(-1);
    }

    engine.rootContext()->setContextProperty("dataDirectory", "file://" + dataDirectory.absolutePath());
#endif

    engine.rootContext()->setContextProperty("gameVersion", app.applicationVersion());
    qCDebug(dcGame()) << "Data directory:" << engine.rootContext()->contextProperty("dataDirectory").toString();

    Game::instance()->setDataDirectory(engine.rootContext()->contextProperty("dataDirectory").toString());

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
