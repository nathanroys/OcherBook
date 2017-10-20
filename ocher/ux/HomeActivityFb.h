/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_HOMEACTIVITY_H
#define OCHER_UX_FB_HOMEACTIVITY_H

#include "ocher/ux/Activity.h"

#include <QRect>


class HomeActivity : public Activity {
public:
    HomeActivity();
    ~HomeActivity();

    void draw();

    int evtKey(const struct OcherKeyEvent *);
    int evtMouse(const struct OcherMouseEvent *);

protected:
    void onAttached();
    void onDetached();

    float coverRatio;
#define NUM_CLUSTER_BOOKS 5
    QRect books[NUM_CLUSTER_BOOKS];
    QRect shortlist[5];
    QRect m_browseLabel;
};

#endif
