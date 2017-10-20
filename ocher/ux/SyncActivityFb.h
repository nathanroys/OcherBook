/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_SYNCACTIVITY_H
#define OCHER_UX_SYNCACTIVITY_H

#include "ocher/ux/Activity.h"


class SyncActivityWork;

class SyncActivity : public Activity {
public:
    SyncActivity();

    void draw();

protected:
    void onAttached();
    void onDetached();

    SyncActivityWork *m_work;
    // TODO https://github.com/snowwlex/QtWaitingSpinner
    //Spinner m_spinner;
};

#endif
