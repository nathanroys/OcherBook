/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_HOMEACTIVITY_H
#define OCHER_UX_FB_HOMEACTIVITY_H

#include "ux/fb/ActivityFb.h"


class HomeActivityFb : public ActivityFb {
public:
    HomeActivityFb(UxControllerFb* c);

    void draw();

    int evtKey(const struct OcherKeyEvent*);
    int evtMouse(const struct OcherMouseEvent*);

protected:
    void onAttached();
    void onDetached();

    float coverRatio;
#define NUM_CLUSTER_BOOKS 5
    Rect books[NUM_CLUSTER_BOOKS];
    Rect shortlist[5];
    Rect m_browseLabel;
};

#endif
