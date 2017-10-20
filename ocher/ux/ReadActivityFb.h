/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_UX_FB_READACTIVITY_H
#define OCHER_UX_FB_READACTIVITY_H

#include "ocher/ux/Activity.h"


class Renderer;
class Layout;

class ReadActivity : public Activity {
public:
    ReadActivity();

protected:
    int evtKey(const struct OcherKeyEvent *);
    int evtMouse(const struct OcherMouseEvent *);

    void onAttached();
    void onDetached();

    void draw();

    Settings *m_settings;

    Layout *m_layout;
    Renderer *m_renderer;
    Meta *meta;
    unsigned int m_pageNum;
    int atEnd;
    unsigned int m_pagesSinceRefresh;
};

#endif
