/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#define __USE_GNU // for memrchr

#include "fdrender.h"
#include "layout.h"
#include "pagination.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

// TODO:  margins (not the same as margins for fb?)
// TODO:  if isatty: SIGWINCH

FdRender::FdRender(int fd)
    : m_isTty(false)
    , m_fd(fd)
    , m_width(80)
    , m_height(0)
    , m_x(0)
    , m_y(0)
    , ai(1)
{
    if (isatty(m_fd) == -1) {
        m_isTty = 0;
    } else {
        m_isTty = 1;
        struct winsize win;
        if (ioctl(0, TIOCGWINSZ, &win) == 0) {
            m_width = win.ws_col;
            m_height = win.ws_row;
        }
    }
}

void FdRender::clearScreen()
{
    write(m_fd, "\033E", 2);
}

void FdRender::enableUl()
{
    write(m_fd, "\x1b[4m", 4);
}

void FdRender::disableUl()
{
    write(m_fd, "\x1b[24m", 5);
}

void FdRender::enableEm()
{
    write(m_fd, "\x1b[1m", 4);
}

void FdRender::disableEm()
{
    write(m_fd, "\x1b[22m", 5);
}

void FdRender::pushAttrs()
{
    assert(ai < 9);
    a[ai + 1] = a[ai];
    ai++;
}

void FdRender::applyAttrs(int i)
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

void FdRender::popAttrs()
{
    assert(ai > 0);
    ai--;
    applyAttrs(-1);
}

int FdRender::outputWrapped(std::vector<char>* b, unsigned int strOffset, bool doBlit)
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

        if (doBlit)
            write(m_fd, p, n);
        p += n;
        len -= n;
        m_x += n;
        if (nl || m_x >= m_width - 1) {
            if (doBlit)
                write(m_fd, "\n", 1);
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

int FdRender::render(Pagination* pagination, unsigned int pageNum, bool doBlit)
{
    m_x = 0;
    m_y = 0;
    if (m_height) {
        clearScreen();
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
    const char* raw = m_layout.data();
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
                assert(str->size() > 0);
                assert(strOffset <= str->size());
                int breakOffset = outputWrapped(str, strOffset, doBlit);
                strOffset = 0;
                if (breakOffset >= 0) {
                    pagination->set(pageNum, i - 2, breakOffset);
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
    return 1;
}
