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

#include "game.h"
#include "gamewindow.h"
#include "debugcategories.h"

GameWindow::GameWindow(QWindow *parent) :
    QQuickWindow(parent)
{
    setMinimumSize(QSize(800, 600));

    // Init with last settings
    setWidth(Game::instance()->settings()->windowSize().width());
    setHeight(Game::instance()->settings()->windowSize().height());
    setX(Game::instance()->settings()->windowPosition().x());
    setY(Game::instance()->settings()->windowPosition().y());
    setVisibility(Game::instance()->settings()->visibility());

    // Connect the setting
    connect(Game::instance()->settings(), &GameSettings::visibilityChanged, this, &GameWindow::setVisibility);


    // Window size/position connections
    connect(this, &QQuickWindow::widthChanged, this, &GameWindow::onWidthChanged);
    connect(this, &QQuickWindow::heightChanged, this, &GameWindow::onHeightChanged);
    connect(this, &QQuickWindow::xChanged, this, &GameWindow::onXChanged);
    connect(this, &QQuickWindow::yChanged, this, &GameWindow::onYChanged);
    connect(this, &QQuickWindow::visibilityChanged, this, &GameWindow::onVisibilityChanged);

    // Rendering connections
    connect(this, &QQuickWindow::beforeSynchronizing, this, &GameWindow::onBeforeSynchronizing, Qt::DirectConnection);
    connect(this, &QQuickWindow::afterSynchronizing, this, &GameWindow::onAfterSynchronizing, Qt::DirectConnection);
    connect(this, &QQuickWindow::beforeRendering, this, &GameWindow::onBeforeRendering, Qt::DirectConnection);
    connect(this, &QQuickWindow::afterRendering, this, &GameWindow::onAfterRendering, Qt::DirectConnection);
    connect(this, &QQuickWindow::afterAnimating, this, &GameWindow::onAfterAnimating, Qt::DirectConnection);
    connect(this, &QQuickWindow::frameSwapped, this, &GameWindow::onFrameSwapped, Qt::DirectConnection);

    m_elapsTimer.start();
}

GameWindow::~GameWindow()
{

}

void GameWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    qCDebug(dcGame()) << "Key pressed" << keyEvent;
    Game::instance()->keyPressed(static_cast<Qt::Key>(keyEvent->key()), keyEvent->isAutoRepeat());
}

void GameWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    qCDebug(dcGame()) << "Key released" << keyEvent;
    Game::instance()->keyReleased(static_cast<Qt::Key>(keyEvent->key()), keyEvent->isAutoRepeat());
}

void GameWindow::onVisibilityChanged(QWindow::Visibility visibility)
{
    Game::instance()->settings()->setVisibility(visibility);
}

void GameWindow::onWidthChanged(int width)
{
    Game::instance()->settings()->setWindowSize(QSize(width, height()));
}

void GameWindow::onHeightChanged(int height)
{
    Game::instance()->settings()->setWindowSize(QSize(width(), height));
}

void GameWindow::onXChanged(int x)
{
    Game::instance()->settings()->setWindowPosition(QPoint(x, y()));
}

void GameWindow::onYChanged(int y)
{
    Game::instance()->settings()->setWindowPosition(QPoint(x(), y));
}

void GameWindow::onBeforeSynchronizing()
{
    qCDebug(dcRender()) << "---------------------------------------------------";
    qCDebug(dcRender()) << "Before synchronizing";
    Game::instance()->gameTick();
    m_elapsTimer.restart();
}

void GameWindow::onAfterSynchronizing()
{
    qCDebug(dcRender()) << "After synchronizing";
}

void GameWindow::onBeforeRendering()
{
    qCDebug(dcRender()) << "Before rendering";
}

void GameWindow::onAfterRendering()
{
    qCDebug(dcRender()) << "After rendering" << m_elapsTimer.elapsed() << "ms";
    Game::instance()->gameTick();
}

void GameWindow::onAfterAnimating()
{
    qCDebug(dcRender()) << "After animating";
}

void GameWindow::onFrameSwapped()
{
    qCDebug(dcRender()) << "Frame swapped" << m_elapsTimer.elapsed() << "ms";
}


