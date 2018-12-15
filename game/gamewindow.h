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

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QQuickWindow>
#include <QElapsedTimer>

class GameWindow : public QQuickWindow
{
    Q_OBJECT
public:
    GameWindow(QWindow *parent = nullptr);
    ~GameWindow() override;

protected:
    void keyPressEvent(QKeyEvent *keyEvent) override;
    void keyReleaseEvent(QKeyEvent *keyEvent) override;

private:
    QElapsedTimer m_elapsTimer;

private slots:
    void onVisibilityChanged(GameWindow::Visibility visibility);
    void onWidthChanged(int width);
    void onHeightChanged(int height);
    void onXChanged(int x);
    void onYChanged(int y);

    void onBeforeSynchronizing();
    void onAfterSynchronizing();
    void onBeforeRendering();
    void onAfterRendering();
    void onAfterAnimating();
    void onFrameSwapped();

};

#endif // GAMEWINDOW_H
