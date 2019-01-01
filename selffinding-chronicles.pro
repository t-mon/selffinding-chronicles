QT += qml quick

CONFIG += c++11

RESOURCES += gui/gui.qrc \
             data/data.qrc

include(qml-box2d/box2d-static.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = gui/*.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    game/game.h \
    game/field.h \
    game/fields.h \
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
    game/gameworld.h \
    game/items/enemy.h \
    game/items/bullet.h

SOURCES += game/main.cpp \
    game/game.cpp \
    game/field.cpp \
    game/fields.cpp \
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
    game/gameworld.cpp \
    game/items/enemy.cpp \
    game/items/bullet.cpp
