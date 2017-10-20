/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "layout.h"

#include <ctype.h>

Layout::Layout()
    : m_dataLen(0)
    , nl(0)
    , ws(0)
    , pre(0)
    , m_buffer(new std::vector<char>)
    , m_bufferLen(0)
{
    m_buffer->resize(chunk);
    m_data.resize(chunk);
}

Layout::~Layout()
{
    // Walk the bytecode and delete embedded strings
    const unsigned int N = m_data.size();
    const char* raw = m_data.data();

    for (unsigned int i = 0; i < N;) {
        uint16_t code = *(uint16_t*)(raw + i);
        i += 2;
        unsigned int opType = (code >> 12) & 0xf;
        unsigned int op = (code >> 8) & 0xf;
        if (opType == OpCmd && op == CmdOutputStr) {
            delete *(std::vector<char>**)(raw + i);
            i += sizeof(std::vector<char>*);
        }
    }

    delete m_buffer;
}

std::unique_ptr<std::vector<char>> Layout::unlock()
{
    flushText();
    delete m_buffer;
    m_buffer = 0;

    m_data.resize(m_dataLen);
    return std::unique_ptr<std::vector<char>>{ new std::vector<char>(m_data) };
}

char* Layout::checkAlloc(unsigned int n)
{
    if (m_dataLen + n > m_data.size()) {
        m_data.resize(m_dataLen + n + chunk);
    }
    char* p = m_data.data() + m_dataLen;
    m_dataLen += n;
    return p;
}

void Layout::push(unsigned int opType, unsigned int op, unsigned int arg)
{
    char* p = checkAlloc(2);
    uint16_t i = (opType << 12) | (op << 8) | arg;

    *(uint16_t*)p = i;
}

void Layout::pushPtr(void* ptr)
{
    int n = sizeof(ptr);
    char* p = checkAlloc(n);

    *((char**)p) = (char*)ptr;
}

void Layout::pushTextAttr(TextAttr attr, uint8_t arg)
{
    push(OpPushTextAttr, attr, arg);
}

void Layout::popTextAttr(unsigned int n)
{
    push(OpCmd, CmdPopAttr, n);
}

void Layout::pushLineAttr(LineAttr attr, uint8_t arg)
{
    push(OpPushLineAttr, attr, arg);
}

void Layout::popLineAttr(unsigned int n)
{
    push(OpCmd, CmdPopAttr, n);
}

// void Layout::pushImage()
// {
//	push(OpImage, );
// }

inline void Layout::_outputChar(char c)
{
    if (m_bufferLen == chunk) {
        flushText();
    }
    (*m_buffer)[m_bufferLen++] = c;
}

void Layout::outputChar(char c)
{
    if (isspace(c)) {
        if (!ws) {
            ws = 1;
            _outputChar(' ');
        }
    } else {
        nl = 0;
        ws = 0;
        _outputChar(c);
    }
}

void Layout::outputNl()
{
    if (!nl) {
        _outputChar('\n');
        nl = 1;
    }
}

void Layout::outputBr()
{
    _outputChar('\n');
    nl = 1;
}

void Layout::flushText()
{
    if (m_bufferLen) {
        push(OpCmd, CmdOutputStr, 0);
        m_buffer->resize(m_bufferLen);
        pushPtr(m_buffer);
        // m_buffer pointer is now owned by the layout bytecode.
        m_buffer = new std::vector<char>;
        m_buffer->resize(chunk);
        m_bufferLen = 0;
    }
}

void Layout::outputPageBreak()
{
    push(OpCmd, CmdForcePage, 0);
}
