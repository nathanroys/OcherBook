/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "cdkread.h"

#include "cdkcontroller.h"
#include "ebook.h"
#include "render.h"

#if 0
int CdkRead::evtKey(const struct OcherKeyEvent* evt)
{
    if (evt->subtype == OEVT_KEY_DOWN) {
        if (evt->key == OEVTK_HOME) {
            // TODO  visually turn page down
            m_controller->setNextActivity(ACTIVITY_HOME);
            return -1;
        } else if (evt->key == OEVTK_LEFT || evt->key == OEVTK_UP || evt->key == OEVTK_PAGEUP) {
            if (m_pageNum > 0) {
                m_pageNum--;
                m_controller->m_systemBar->hide();
                m_controller->m_navBar->hide();
                invalidate();
            }
            return -1;
        } else if (evt->key == OEVTK_RIGHT || evt->key == OEVTK_DOWN
                || evt->key == OEVTK_PAGEDOWN) {
            if (!atEnd) {
                m_pageNum++;
                m_controller->m_systemBar->hide();
                m_controller->m_navBar->hide();
                invalidate();
            }
            return -1;
        }
    }
    return -2;
}

int CdkRead::evtMouse(const struct OcherMouseEvent* evt)
{
    SystemBar* systemBar = m_controller->m_systemBar;
    NavBar* navBar = m_controller->m_navBar;

    if (evt->subtype == OEVT_MOUSE1_UP) {
        Pos pos(evt->x, evt->y);
        if (systemBar->m_rect.contains(&pos) || navBar->m_rect.contains(&pos)) {
            if (systemBar->m_flags & WIDGET_HIDDEN) {
                systemBar->show();
                m_fb->update(&systemBar->m_rect);
                navBar->show();
                m_fb->update(&navBar->m_rect);
            } else {
                // TODO interact
            }
        } else {
            if (!(systemBar->m_flags & WIDGET_HIDDEN)) {
                systemBar->hide();
                navBar->hide();
                invalidate();
            } else {
                if (evt->x < m_fb->width() / 2) {
                    if (m_pageNum > 0) {
                        m_pageNum--;
                        invalidate();
                    }
                } else {
                    if (!atEnd) {
                        m_pageNum++;
                        invalidate();
                    }
                }
            }
        }
        return -1;
    }
    return -2;
}
#endif

CdkRead::CdkRead(CdkController* c)
    : m_log(Log::get("CdkRead"))
    , m_controller(c)
    , atEnd(1)
{
}

void CdkRead::draw()
{
    atEnd = m_controller->m_renderer->render(&ebook->m_pagination, m_pageNum, true);
}

void CdkRead::onAttached()
{
    ebook = m_controller->ctx.selected;
    assert(ebook);

    m_controller->m_renderer->set(memLayout);

// Optionally, run through all pages without blitting to get an accurate
// page count.  Alternative is to do some sort of "idealize" page layout that might be faster.
#if 0
    if (ebook->m_pagination.numPages() == 0) {
        for (int pageNum = 0;; pageNum++) {
            m_log->info("Paginating page %d", pageNum);
            int r = m_controller->m_renderer->render(&ebook->m_pagination, pageNum, false);

            if (r != 0) {
                ebook->pages = pageNum + 1;
                break;
            }
        }
    }
#endif

    ebook->record.touch();
    m_pageNum = ebook->record.activePage;
    m_log->info("Starting on page %u", m_pageNum);
    invalidate();
}

void CdkRead::onDetached()
{
    ebook->record.activePage = m_pageNum;
}
