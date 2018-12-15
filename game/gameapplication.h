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

#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

#include <QQuickItem>
#include <QQmlEngine>
#include <QGuiApplication>

#include "gamewindow.h"

class WindowManager : public QObject
{
    Q_OBJECT
public:
    explicit WindowManager(QObject *parent = nullptr);
    ~WindowManager();

    void setRootItem(QQuickItem *item);
    void show();

    QQuickWindow *window();

private:
    GameWindow *m_window = nullptr;
};


class ViewManager : public QObject
{
    Q_OBJECT
public:
    explicit ViewManager(QObject *parent = nullptr);
    ~ViewManager();

    QQuickItem* rootItem(QQmlEngine *engine);
};



class GameApplication : public QGuiApplication
{
    Q_OBJECT
public:
    explicit GameApplication(int &argc, char **argv);
    ~GameApplication();

    QQmlEngine *qmlEngine() const;
    WindowManager *windowManager() const;

    void init();

private:
    void setRootItem(QQuickItem *item);
    void show();

    QQuickWindow *window();

private:
    QQmlEngine *m_engine = nullptr;
    WindowManager *m_windowManager = nullptr;
    ViewManager *m_viewManager = nullptr;

};


#endif // GAMEAPPLICATION_H
