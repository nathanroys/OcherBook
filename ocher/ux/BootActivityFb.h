/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_BOOTACTIVITY_H
#define OCHER_UX_FB_BOOTACTIVITY_H

#include "ocher/ux/Activity.h"

#include <QRect>


class BootActivity : public Activity {
public:
    BootActivity();
    ~BootActivity();

    void draw();

    int evtMouse(const struct OcherMouseEvent *);

    void onAttached();
    void onDetached();

protected:
    void highlight(int i);
    QRect apps[2];
};

#endif
