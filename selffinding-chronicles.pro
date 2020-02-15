QT += core qml quick quickcontrols2 svg multimedia

CONFIG += c++11 -Wall -Werror

# Add Box2D as static library, disable warnings from upstream
QMAKE_CFLAGS_WARN_ON -= -Wall
QMAKE_CXXFLAGS_WARN_ON -= -Wall
include(qml-box2d/box2d-static.pri)
QMAKE_CFLAGS_WARN_ON
QMAKE_CXXFLAGS_WARN_ON

#QMAKE_CXXFLAGS += -isystem "$$PWD/qml-box2d/box2d_lib" -Werror -std=c++11 -g
QMAKE_LFLAGS *= -std=c++11

RESOURCES += gui/gui.qrc \
             gamedata/gamedata.qrc


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
QML_FILES = $$PWD/gui/*.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    game/audio/audiodecoderstream.h \
    game/audio/audiomixer.h \
    game/audio/gamemusic.h \
    game/audio/gamesoundeffect.h \
    game/audio/gamesoundsource.h \
    game/audio/soundengine.h \
    game/game.h \
    game/field.h \
    game/fields.h \
    game/gameobjects.h \
    game/gameobjectsproxy.h \
    game/items/boxitem.h \
    game/items/literatureitem.h \
    game/items/teleporteritem.h \
    game/items/weatherareaproxy.h \
    game/map.h \
    game/debugcategories.h \
    game/playercontroller.h \
    game/collisiondetector.h \
    game/gameobject.h \
    game/items/gameitem.h \
    game/items/plantitem.h \
    game/items/treeitem.h \
    game/items/gameitems.h \
    game/items/gameitemsproxy.h \
    game/items/weaponitem.h \
    game/items/character.h \
    game/dataloader.h \
    game/conversation/conversation.h \
    game/conversation/conversationitem.h \
    game/conversation/conversationchoise.h \
    game/conversation/conversationaction.h \
    game/conversation/conversationitems.h \
    game/gamesettings.h \
    game/items/chestitem.h \
    game/items/enemy.h \
    game/items/firearmitem.h \
    game/gamemapeditor.h \
    game/engine.h \
    game/datamanager.h \
    game/path.h \
    game/pathsegment.h \
    game/pathcontroller.h \
    game/items/staticitem.h \
    game/savegame.h \
    game/savegameproxy.h \
    game/savegames.h \
    game/teleportationhandler.h


SOURCES += game/main.cpp \
    game/audio/audiodecoderstream.cpp \
    game/audio/audiomixer.cpp \
    game/audio/gamemusic.cpp \
    game/audio/gamesoundeffect.cpp \
    game/audio/gamesoundsource.cpp \
    game/audio/soundengine.cpp \
    game/game.cpp \
    game/field.cpp \
    game/fields.cpp \
    game/gameobjects.cpp \
    game/gameobjectsproxy.cpp \
    game/items/boxitem.cpp \
    game/items/literatureitem.cpp \
    game/items/teleporteritem.cpp \
    game/items/weatherareaproxy.cpp \
    game/map.cpp \
    game/debugcategories.cpp \
    game/playercontroller.cpp \
    game/collisiondetector.cpp \
    game/gameobject.cpp \
    game/items/gameitem.cpp \
    game/items/plantitem.cpp \
    game/items/treeitem.cpp \
    game/items/gameitems.cpp \
    game/items/gameitemsproxy.cpp \
    game/items/weaponitem.cpp \
    game/items/character.cpp \
    game/dataloader.cpp \
    game/conversation/conversation.cpp \
    game/conversation/conversationitem.cpp \
    game/conversation/conversationaction.cpp \
    game/conversation/conversationitems.cpp \
    game/gamesettings.cpp \
    game/items/chestitem.cpp \
    game/items/enemy.cpp \
    game/items/firearmitem.cpp \
    game/gamemapeditor.cpp \
    game/engine.cpp \
    game/datamanager.cpp \
    game/path.cpp \
    game/pathsegment.cpp \
    game/pathcontroller.cpp \
    game/items/staticitem.cpp \
    game/savegame.cpp \
    game/savegameproxy.cpp \
    game/savegames.cpp \
    game/teleportationhandler.cpp


unix:!android {
    message("Building linux version")
}

android {
    message("Building android version")

    QT += androidextras
    DEFINES += QT_DEPRECATED_WARNINGS ANDROID

    COMMON_DATA.path = /assets
    COMMON_DATA.files = $$files($$PWD/data/*)
    INSTALLS += COMMON_DATA

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/packaging/android

    DISTFILES += \
        $$ANDROID_PACKAGE_SOURCE_DIR/AndroidManifest.xml \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.jar \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradlew \
        $$ANDROID_PACKAGE_SOURCE_DIR/res/values/libs.xml \
        $$ANDROID_PACKAGE_SOURCE_DIR/build.gradle \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradle/wrapper/gradle-wrapper.properties \
        $$ANDROID_PACKAGE_SOURCE_DIR/gradlew.bat
}


