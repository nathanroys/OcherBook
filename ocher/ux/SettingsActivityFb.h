/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_SETTINGSACTIVITY_H
#define OCHER_UX_FB_SETTINGSACTIVITY_H

#include "ocher/ux/Activity.h"


class SettingsActivity : public Activity {
public:
    SettingsActivity();

protected:
    void onAttached();
    void onDetached();
};

#endif
