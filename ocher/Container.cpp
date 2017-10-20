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


Container g_container;

Container::Container() :
    device(0),
    battery(0),
    filesystem(0),
    settings(0),
    options(0),
    powerSaver(0)
{
}

Container::~Container()
{
    delete battery;
    delete filesystem;
    delete settings;
    delete powerSaver;
    delete device;

}

//    m_loop->emitEvent.Connect(this, &PowerSaver::dispatchEvent);

// wire
// unwire
// destroy
