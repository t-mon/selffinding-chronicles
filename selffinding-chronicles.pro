QT += qml quick

CONFIG += c++11

RESOURCES += gui/gui.qrc \
             data/data.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = gui/*.qml

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    game/player.h \
    game/game.h \
    game/world.h \
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

SOURCES += game/main.cpp \
    game/player.cpp \
    game/game.cpp \
    game/world.cpp \
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
