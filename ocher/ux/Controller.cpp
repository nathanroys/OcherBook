/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/device/Battery.h"
#include "ocher/device/Device.h"
#include "ocher/device/Filesystem.h"
#include "ocher/settings/Options.h"
#include "ocher/settings/Settings.h"
#include "ocher/ux/Controller.h"
#include "ocher/ux/PowerSaver.h"
#include "ocher/util/Debug.h"

#include "airbag_fd/airbag_fd.h"

#include <stdexcept>

#define LOG_NAME "ocher.controller"


UxController::UxController() :
    m_powerSaver(0)
{
    m_filesystem = g_container.filesystem;
    m_powerSaver = g_container.powerSaver;

    connect(m_filesystem, SIGNAL(dirChanged()), this, SLOT(onDirChanged()));
    m_filesystem->initWatches(g_container.options);

    connect(m_powerSaver, SIGNAL(wantToSleep()), this, SLOT(onWantToSleep()));
}

UxController::~UxController()
{
}

void UxController::onDirChanged(const char *dir, const char *file)
{
    // TODO
}

void UxController::onWantToSleep()
{
    // TODO
}

Controller::Controller(Options *options)
{
    g_container.options = options;

    g_container.settings = new Settings();
    g_container.filesystem = new Filesystem();
    g_container.powerSaver = new PowerSaver();
    g_container.device = new Device();
    g_container.battery = new Battery();

    // Wire up
    //   TODO setters vs constructors vs hitting g_container
    //   TODO here or in Container?
    g_container.settings->inject(g_container.filesystem);
    g_container.powerSaver->inject(g_container.device);

    initDebug();

    g_container.settings->load();

    initCrash();
}

Controller::~Controller()
{
}

void Controller::initCrash()
{
#ifdef OCHER_AIRBAG_FD
    airbag_init_fd(2, 0);
#endif
}

#if 0
void Controller::initLog()
{
    static LogAppenderCFile appender(stderr);
    Logger *l = Log::get("");

    l->setAppender(&appender);

    Options *opt = g_container.options;
    if (opt->verbose < 0)
        l->setLevel(Log::Fatal);
    else if (opt->verbose == 0)
        l->setLevel(Log::Error);
    else if (opt->verbose == 1)
        l->setLevel(Log::Warn);
    else if (opt->verbose == 2)
        l->setLevel(Log::Info);
    else if (opt->verbose == 3)
        l->setLevel(Log::Debug);
    else
        l->setLevel(Log::Trace);
    Log::info("ocher", "Log initialized");
}
#endif

void Controller::initDebug()
{
#if 0
    // Before proceeding with startup and initializing the framebuffer, check for a killswitch.
    // Useful when needing to bail to native stack (such as OcherBook vs native stack init-ing
    // framebuffer in incompatible ways).
    if (g_container.device->fs.m_libraries) {
        for (int i = 0;; ++i) {
            const char *lib = g_container.device->fs.m_libraries[i];
            if (!lib)
                break;
            std::string killswitch(1, "%s/.ocher/kill", lib);
            if (access(killswitch.c_str(), F_OK) == 0) {
                fprintf(stderr, "OcherBook exiting because of '%s' killswitch\n", killswitch.c_str());
                exit(1);
            }
        }
    }
#endif
}

void Controller::run()
{
#ifdef OCHER_TARGET_KOBO
    // Kobo rc scripts start animate.sh, which shows an animation while nickel is starting.
    // Kill that here (so it doesn't overlay the boot menu) to simplify installation steps.
    // TODO: Remove when more closely integrated.
    system("killall on-animator.sh");
    sleep(1);
#endif

    ActivityType a = ACTIVITY_SYNC;
    m_uxController->setNextActivity(a);

    // TODO: sync state out
}
