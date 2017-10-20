/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/ux/Event.h"

using namespace std;

EventLoop::EventLoop() :
    evLoop(EV_DEFAULT),
    m_epoch(0)
{
    ev_async_init(&m_async, emitInjectedCb);
    m_async.data = this;
    ev_async_start(evLoop, &m_async);
}

EventLoop::~EventLoop()
{
}

int EventLoop::run()
{
    ev_run(evLoop, 0);

    return 0;
}

void EventLoop::stop()
{
    ev_break(evLoop, EVBREAK_ALL);
}

void EventLoop::setEpoch()
{
}

void EventLoop::injectEvent(const struct OcherEvent &event)
{
    {
        lock_guard<mutex> lock(m_lock);
        m_events.push_back(event);
    }

    ev_async_send(evLoop, &m_async);
}

void EventLoop::emitInjectedCb(EV_P_ ev_async *w, int revents)
{
    EventLoop *self = static_cast<EventLoop *>(w->data);

    self->emitInjected();
}

void EventLoop::emitInjected()
{
    while (1) {
        m_lock.lock();

        if (m_events.size() == 0) {
            m_lock.unlock();
            return;
        }

        struct OcherEvent event = m_events[0];
        m_events.erase(m_events.begin());

        m_lock.unlock();

        emitEvent(&event);
    }
}

EventWork::EventWork(EventLoop *loop) :
    Thread("EventWork %p", loop->evLoop),
    m_loop(loop)
{
    ev_async_init(&m_async, completeCb);
    m_async.data = this;
    ev_async_start(m_loop->evLoop, &m_async);
}

EventWork::~EventWork()
{
}

void EventWork::run()
{
    work();
    ev_async_send(m_loop->evLoop, &m_async);
}

void EventWork::completeCb(EV_P_ ev_async *w, int revents)
{
    EventWork *self = static_cast<EventWork *>(w->data);

    self->notifyComplete();
}
