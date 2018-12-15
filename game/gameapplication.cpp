#include "gameapplication.h"
#include "debugcategories.h"
#include "game.h"

WindowManager::WindowManager(QObject *parent) :
    QObject(parent),
    m_window(new GameWindow())
{
    m_window->setColor(Qt::black);
}

WindowManager::~WindowManager()
{
    qCDebug(dcGame()) << "Delete WindowManager";
    delete  m_window;
}

void WindowManager::setRootItem(QQuickItem *item)
{
    item->setParent(m_window);
    item->setParentItem(m_window->contentItem());
    item->setWidth(m_window->width());
    item->setHeight(m_window->height());
}

void WindowManager::show()
{
    m_window->show();
}

QQuickWindow *WindowManager::window()
{
    return qobject_cast<QQuickWindow *>(m_window);
}

ViewManager::ViewManager(QObject *parent) : QObject(parent)
{

}

ViewManager::~ViewManager()
{
    qCDebug(dcGame()) << "Delete ViewManager";
}

QQuickItem *ViewManager::rootItem(QQmlEngine *engine)
{
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/main.qml")));
    QQuickItem *item = qobject_cast<QQuickItem *>(component.create());
    item->setParent(this);
    return item;
}

GameApplication::GameApplication(int &argc, char **argv) :
    QGuiApplication(argc, argv),
    m_engine(new QQmlEngine(this)),
    m_windowManager(new WindowManager(this)),
    m_viewManager(new ViewManager(this))
{

}

GameApplication::~GameApplication()
{
    qCDebug(dcGame()) << "Shutting down game application";

    delete m_viewManager;
    delete m_windowManager;
    delete m_engine;
}

QQmlEngine *GameApplication::qmlEngine() const
{
    return m_engine;
}

WindowManager *GameApplication::windowManager() const
{
    return m_windowManager;
}

void GameApplication::init()
{
    qCDebug(dcGame()) << "Init game application";
    m_windowManager->setRootItem(m_viewManager->rootItem(m_engine));
    m_windowManager->show();
}

