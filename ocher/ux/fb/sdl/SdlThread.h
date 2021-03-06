/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_SDL_LOOP_H
#define OCHER_SDL_LOOP_H

#include "ux/Event.h"

#include <SDL.h>

#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

/**
 * Thread to initialize SDL graphics and gather SDL events.  Note that SDL must do these two things
 * on the same thread, but the main thread of OcherBook knows nothing of SDL.  Therefore SDL runs on
 * its own thread, and injects events back into the EventLoop.
 */
class SdlThread {
public:
    SdlThread();
    ~SdlThread();

    // If a screen is returned, you must set an event loop
    void start(std::promise<SDL_Surface*>& screenPromise);
    void setEventLoop(EventLoop* loop);
    void stop();

protected:
    SDL_Surface* init();
    void run();

    std::thread m_thread;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    bool m_stop;

    EventLoop* m_loop;
    std::promise<SDL_Surface*> m_screenPromise;
};

#endif
