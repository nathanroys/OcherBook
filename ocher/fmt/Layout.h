/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_LAYOUT_H
#define OCHER_LAYOUT_H

/** @file
 * Rough layout of a book.
 */

#include "ocher/util/Buffer.h"

/**
 *  Contains the rough layout of the book's chapters in a file format independent and output device
 *  independent format.  Once the book is laid out in this format, the original file can be
 *  discarded because this is usually more memory efficient.
 *
 *  Derive subclasses per file format; create an "append" function to append the book's chapters
 *  (or spine elements, or whatever) and populate m_data.
 *
 *  The layout class is expected to do much more work than the Render class (for one reason, so that
 *  the pages can be rendered quickly).  The layout is canonicalized:  whitespace is canonicalized,
 *  attributes are properly nested, etc.  The Renderer just blindly follows the output bytecode with
 *  little additional validation.
 *
 *  The bytecode generated by this class must match that expected by the Renderer class.
 *
 *  @todo  Somewhat similar to DVI; perhaps useful to see what could be learned.  (See
 *  http://www.tug.org/TUGboat/Articles/tb03-2/tb06software.pdf.)  But DVI assumes you have a known
 *  font on the final output device, so DVI can do bit-perfect layout.
 */
class Layout {
public:
    enum Op {
        OpPushTextAttr = 0,
        OpPushLineAttr = 1,
        OpCmd          = 2,
        OpSpacing      = 3,
        OpImage        = 4,
    };

    enum TextAttr {
        AttrBold      = 0,  ///< arg: future:
        AttrUnderline = 1,  ///< arg: future: double, strikethrough, etc
        AttrItalics   = 2,  ///< arg: future: slant
        AttrSizeRel   = 3,  ///< arg: -127 - +128 pts
        AttrSizeAbs   = 4,  ///< arg: -127 - +128 pts
        AttrFont      = 5,
        AttrPre       = 6,
    };

    enum LineAttr {
        LineJustifyLeft   = 0,
        LineJustifyCenter = 1,
        LineJustifyFull   = 2,
        LineJustifyRight  = 4,
    };

    enum Cmd {
        CmdPopAttr,            ///< arg: # attrs to pop (0==1)
        CmdOutputStr,          ///< followed by ptr to string
        CmdOutputImg,          ///< followed by ptr to string
        CmdForcePage,          ///< optionally set new title
    };

    enum Spacing {
        Vert,
        Horiz,
    };

    enum Image {
        // href
        // inline vs anchored
        // hr
    };

    Layout();
    ~Layout();

    // virtual void append(...) = 0;

    /** Finishes and returns the laid-out internal buffer.
     * @todo terrible API; buffer holds pointers, freed by the class
     */
    Buffer unlock();

protected:
    void push(unsigned int opType, unsigned int op, unsigned int arg);
    void pushPtr(void *ptr);

    void pushTextAttr(TextAttr attr, uint8_t arg);
    void popTextAttr(unsigned int n = 1);
    void pushLineAttr(LineAttr attr, uint8_t arg);
    void popLineAttr(unsigned int n = 1);

    void _outputChar(char c);
    void outputChar(char c);
    void outputNl();
    void outputBr();
    void flushText();
    void outputPageBreak();

    /** Ensure m_data can hold n additional bytes */
    char *checkAlloc(unsigned int n);

    Buffer m_data;
    unsigned int m_dataLen;

    int nl;
    int ws;
    int pre;
    Buffer *m_text;
    unsigned int m_textLen;

    // Image* m_images[];

    static const unsigned int chunk = 1024;
};

#endif
