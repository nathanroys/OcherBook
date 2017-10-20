/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#define __USE_GNU // for memrchr

#include "cdkrender.h"

#include "layout.h"
#include "pagination.h"

#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <string>
#include <unistd.h>

// TODO:  page size
// TODO:  margins (not the same as margins for fb?)

CdkRender::CdkRender(WINDOW* scr, CDKSCREEN* screen)
    : m_scr(scr)
    , m_screen(screen)
    , m_x(0)
    , m_y(0)
    , ai(1)
{
    getmaxyx(m_scr, m_height, m_width);
}

void CdkRender::enableUl()
{
    // TODO
}

void CdkRender::disableUl()
{
    // TODO
}

void CdkRender::enableEm()
{
    // TODO
}

void CdkRender::disableEm()
{
    // TODO
}

void CdkRender::pushAttrs()
{
    a[ai + 1] = a[ai];
    ai++;
}

void CdkRender::applyAttrs(int i)
{
    if (a[ai].ul && !a[ai - i].ul) {
        enableUl();
    } else if (!a[ai].ul && a[ai - i].ul) {
        disableUl();
    }

    if (a[ai].em && !a[ai - i].em) {
        enableEm();
    } else if (!a[ai].em && a[ai - i].em) {
        disableEm();
    }
}

void CdkRender::popAttrs()
{
    ai--;
    applyAttrs(-1);
}

int CdkRender::outputWrapped(std::vector<char>* b, unsigned int strOffset, bool doBlit)
{
    unsigned int len = b->size();
    const unsigned char* start = (const unsigned char*)b->data();
    const unsigned char* p = start;

    assert(strOffset <= len);
    len -= strOffset;
    p += strOffset;

    do {
        unsigned int w = m_width - m_x;

        // If at start of line, eat spaces
        if (m_x == 0) {
            while (*p != '\n' && isspace(*p)) {
                ++p;
                --len;
            }
        }

        // How many chars should go out on this line?
        const unsigned char* nl = 0;
        unsigned int n = w;
        if (w >= len) {
            n = len;
            nl = (const unsigned char*)memchr(p, '\n', n);
        } else {
            nl = (const unsigned char*)memchr(p, '\n', n);
            if (!nl) {
                // don't break words
                if (!isspace(*(p + n - 1)) && !isspace(*(p + n))) {
                    unsigned char* space = (unsigned char*)memrchr(p, ' ', n);
                    if (space) {
                        nl = space;
                    }
                }
            }
        }
        if (nl)
            n = nl - p;

        if (doBlit) {
            mvwaddnstr(m_scr, m_y, m_x, (const char*)p, n);
        }
        p += n;
        len -= n;
        m_x += n;
        if (nl || m_x >= m_width - 1) {
            m_x = 0;
            m_y++;
            if (nl) {
                p++;
                len--;
            }
            if (m_height > 0 && m_y + 1 >= m_height) {
                return p - start;
            }
        }
    } while (len > 0);
    return -1;
}

int CdkRender::render(Pagination* pagination, unsigned int pageNum, bool doBlit)
{
    m_x = 0;
    m_y = 0;
    if (m_height) {
        wclear(m_scr);
    }

    unsigned int layoutOffset;
    unsigned int strOffset;
    if (!pageNum) {
        layoutOffset = 0;
        strOffset = 0;
    } else if (!pagination->get(pageNum - 1, &layoutOffset, &strOffset)) {
        // Previous page not already paginated?
        // Perhaps at end of book?
        return -1;
    }

    const unsigned int N = m_layout.size();
    const char* raw = &m_layout[0];
    assert(layoutOffset < N);
    for (unsigned int i = layoutOffset; i < N;) {
        assert(i + 2 <= N);
        uint16_t code = *(uint16_t*)(raw + i);
        i += 2;

        unsigned int opType = (code >> 12) & 0xf;
        unsigned int op = (code >> 8) & 0xf;
        unsigned int arg = code & 0xff;
        switch (opType) {
        case Layout::OpPushTextAttr:
            switch (op) {
            case Layout::AttrBold:
                pushAttrs();
                a[ai].b = 1;
                if (doBlit)
                    applyAttrs(1);
                break;
            case Layout::AttrUnderline:
                pushAttrs();
                a[ai].ul = 1;
                if (doBlit)
                    applyAttrs(1);
                break;
            case Layout::AttrItalics:
                pushAttrs();
                a[ai].em = 1;
                if (doBlit)
                    applyAttrs(1);
                break;
            case Layout::AttrSizeRel:
                pushAttrs();
                break;
            case Layout::AttrSizeAbs:
                pushAttrs();
                break;
            default:
                assert(0);
                break;
            }
            break;
        case Layout::OpPushLineAttr:
            switch (op) {
            case Layout::LineJustifyLeft:
                break;
            case Layout::LineJustifyCenter:
                break;
            case Layout::LineJustifyFull:
                break;
            case Layout::LineJustifyRight:
                break;
            default:
                assert(0);
                break;
            }
            break;
        case Layout::OpCmd:
            switch (op) {
            case Layout::CmdPopAttr:
                if (arg == 0)
                    arg = 1;
                while (arg--) {
                    popAttrs();
                }
                break;
            case Layout::CmdOutputStr: {
                assert(i + sizeof(std::vector<char>*) <= N);
                std::vector<char>* str = *(std::vector<char>**)(raw + i);
                assert(strOffset <= str->size());
                int breakOffset = outputWrapped(str, strOffset, doBlit);
                strOffset = 0;
                if (breakOffset >= 0) {
                    pagination->set(pageNum, i - 2, breakOffset);
                    if (doBlit) {
                        refreshCDKScreen(m_screen);
                    }
                    return 0;
                }
                i += sizeof(std::vector<char>*);
                break;
            }
            case Layout::CmdForcePage:
                break;
            default:
                assert(0);
                break;
            }
            break;
        case Layout::OpSpacing:
            break;
        case Layout::OpImage:
            break;
        default:
            assert(0);
            break;
        }
    }
    refreshCDKScreen(m_screen);
    return 1;
}
