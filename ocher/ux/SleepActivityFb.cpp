/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/device/Device.h"
#include "ocher/ux/fb/FontEngine.h"
#include "ocher/ux/fb/SleepActivityFb.h"
#include "ocher/ux/fb/UxController.h"

#include <unistd.h>


SleepActivity::SleepActivity() :
    m_device(g_container.device)
{
}

void SleepActivity::onAttached()
{
    maximize();

    m_fb->setFg(0xff, 0xff, 0xff);
    m_fb->fillRect(&m_rect);
    m_fb->setFg(0, 0, 0);

    FontEngine fe(m_fb);
    fe.setSize(18);
    fe.setItalic(1);
    fe.apply();

    Pos p;
    p.x = 0;
    p.y = m_rect.h / 2;
    fe.renderString("Sleeping", 8, &p, &m_rect, FE_XCENTER);

    m_fb->update(&m_rect, true); // Full refresh to clear page remnants
    m_fb->sync();
    ::sleep(1);                  // TODO seems hackish but sync doesn't wait long enough!

    m_device->sleep();
}

void SleepActivity::onDetached()
{
}
