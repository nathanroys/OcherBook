/*
 * Copyright (c) 2016, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_MAINWINDOW_H
#define OCHER_UX_MAINWINDOW_H

#include "ocher/ux/Activity.h"

#include <QMainWindow>
#include <QStackedWidget>

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    void setNextActivity(enum ActivityType a);

protected:
    void contextMenuEvent(QContextMenuEvent *event);

private slots:
    void newFile();
    void open();
    void save();
    void newOrg();
    void about();

private:
    void createActions();
    void createMenus();

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *exitAct;
    QAction *newOrgAct;
    QAction *aboutAct;

    QStackedWidget *m_view;
};

#endif
