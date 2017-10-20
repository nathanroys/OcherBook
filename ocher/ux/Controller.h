/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_CONTROLLER_H
#define OCHER_UX_CONTROLLER_H

#include "ocher/shelf/Shelf.h"
#include "ocher/ux/Activity.h"
#include "ocher/ux/MainWindow.h"

#include <QObject>

class Device;
class Filesystem;
class Options;
class PowerSaver;
class Renderer;

/** Holds the current reading state.
 */
class ReadingContext {
public:
    ReadingContext() :
        shortList(&library),
        selected(0)
    {
    }

    Library library;
    ShortList shortList;

    Meta *selected;
};

/**
 * A UxController is a loose coupling between the Activities and the display mechanism used.
 *
 * The UxController is told what it should be doing (via an Activity) and in return gathers events and
 * passes them to its implementation-dependent children.
 *
 * Derive from UxController (perhaps) per toolkit.
 */
class UxController : public QObject
{
    Q_OBJECT

public:
    UxController();
    virtual ~UxController();

    virtual bool init() = 0;

    // TODO: in general, need more control (when running a new activity, is the prior
    // one destroyed?  or suspended?
    virtual void setNextActivity(enum ActivityType a) = 0;

    void onWantToSleep();
    void onDirChanged(const char *dir, const char *file);

    ReadingContext ctx;

protected:
    Filesystem *m_filesystem;
    PowerSaver *m_powerSaver;
    MainWindow *m_mainWindow;
};

/**
 * Top-level controller, managing startup and shutdown.  During startup, it initializes the Container
 * and builds an appropriate UxController.  Delegates the user-interaction to a UxController.
 */
class Controller {
public:
    Controller(Options *options);
    ~Controller();

    void run();

protected:
    void initCrash();
    void initDebug();

    UxController *m_uxController;
};

#endif
