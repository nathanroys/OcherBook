/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_SLEEPACTIVITY_H
#define OCHER_UX_FB_SLEEPACTIVITY_H

#include "ocher/ux/Activity.h"


class SleepActivity : public Activity {
public:
    SleepActivity();

    void onAttached();
    void onDetached();

protected:
    Device *m_device;
};

#endif
