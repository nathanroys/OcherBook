/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CONTAINER_H
#define OCHER_CONTAINER_H

#include <vector>

class Battery;
class Device;
class Filesystem;
class Options;
class PowerSaver;
class Settings;

#if 0
template<T>
class Dependency {
public:
    Dependency(baseName, isSingleton, a);
    const char *baseName;
    bool singleton;
    T *singletonInstance;
    T *allocate()
    {
        return m_allocator();
    }
    things - to - inject;

protected:
};


get(T * &t) {
    lock();
    Dependency<T> d = find dependency;
    if (d->singleton) {
        if (!d->singletonInstance) {
            d->singletonInstance = d->allocate();
            inject;
        }
        t = d->singletonInstance;
    } else {
        t = d->allocate();
        inject;
    }
    unlock();
}

#endif

/**
 * A container for instances.
 *
 * This is trivial dependency injection.  The dependencies are hardcoded.  The Container owns the
 * instances.  Each instance is a singleton.
 */
class Container {
public:
    Container();
    ~Container();

    /**
     * Requires:
     */
    Device *device;

    /**
     * Requires:
     */
    Battery *battery;

    /**
     * Requires:
     */
    Filesystem *filesystem;

    /**
     * Requires: Filesystem
     */
    Settings *settings;

    Options *options;

    /**
     * Requires: Device
     */
    PowerSaver *powerSaver;
};

extern Container g_container;

#endif
