/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_DEV_POWER_SAVER_H
#define OCHER_DEV_POWER_SAVER_H

#include <QObject>
#include <QTimer>

class Device;


class PowerSaver : public QObject
{
    Q_OBJECT

public:
    PowerSaver();

    void inject(Device *device);

    void setTimeout(unsigned int seconds);

//    void sleep();

    void onAttached();
    void onDetached();

signals:
    void wantToSleep();
public slots:
    void timeout();

protected:
    void resetTimeout();

//    void dispatchEvent(const struct OcherEvent *evt);

    QTimer m_timer;
    unsigned int m_seconds;
    Device *m_device;
};

#endif
