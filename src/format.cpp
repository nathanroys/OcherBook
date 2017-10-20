/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "format.h"

const char* fmtToStr(Fmt fmt)
{
    switch (fmt) {
    case OCHER_FMT_EPUB:
        return "EPUB";
    case OCHER_FMT_TEXT:
        return "TEXT";
    case OCHER_FMT_HTML:
        return "HTML";
    default:
        return "";
    }
}

bool stripUtf8Bom(std::string& data)
{
    // Unicode standard does not recommend BOM for UTF8.
    // UTF8 is assumed anyway.
    if (data.size() >= 3 && (unsigned char)data[0] == 0xef && (unsigned char)data[1] == 0xbb
        && (unsigned char)data[2] == 0xbf) {
        data.erase(data.begin(), data.begin() + 3);
        return true;
    }
    return false;
}

Fmt detectFormat(const char* file, Encoding* encoding)
{
    Fmt format = OCHER_FMT_UNKNOWN;

    *encoding = OCHER_ENC_UNKNOWN;

    FILE* fp;
    fp = fopen(file, "r");
    if (fp != NULL) {
        unsigned char buf[4];
        size_t r = fread((char*)buf, 1, 4, fp);
        if (r >= 4 && buf[0] == 'P' && buf[1] == 'K' && buf[2] == 0x03 && buf[3] == 0x04) {
            format = OCHER_FMT_EPUB;
        } else {
            int skip;
            if (r >= 3 && buf[0] == 0xef && buf[1] == 0xbb && buf[2] == 0xbf) {
                skip = 3;
                *encoding = OCHER_ENC_UTF8;
            } else if (r >= 4 && buf[0] == 0xff && buf[1] == 0xfe && (buf[2] || buf[3])) {
                skip = 2;
                *encoding = OCHER_ENC_UTF16LE;
            } else if (r >= 4 && buf[0] == 0xfe && buf[1] == 0xff && (buf[2] || buf[3])) {
                skip = 2;
                *encoding = OCHER_ENC_UTF16BE;
            } else if (r >= 4 && buf[0] == 0xff && buf[1] == 0xfe && !buf[2] && !buf[3]) {
                skip = 4;
                *encoding = OCHER_ENC_UTF32LE;
            } else if (r >= 4 && !buf[0] && !buf[1] && buf[2] == 0xfe && buf[3] == 0xff) {
                skip = 4;
                *encoding = OCHER_ENC_UTF32BE;
            } else {
                // Recommended for UTF8 to not have BOM.  UTF8 also handles plain ascii.
                skip = 0;
                *encoding = OCHER_ENC_UTF8;
            }

            char buf2[512];
            fseek(fp, skip, SEEK_SET);
            r = fread(buf2, 1, sizeof(buf2), fp);
            if (r > 0) {
                // Simple heuristics.  Should look at what the file command does.
                // TODO:  convert to UTF8 based on encoding so heuristics have a chance
                size_t i = 0;

                // HTML ought to start out something like: \s*<\s*html
                while (i < r && isspace(buf2[i])) {
                    ++i;
                }
                if (i < r && buf2[i] == '<') {
                    ++i;
                    while (i < r && isspace(buf2[i])) {
                        ++i;
                    }
                    if (strncasecmp(buf2 + i, "html", r - i) == 0)
                        format = OCHER_FMT_HTML;
                }

                // Unrecognized formatting?  Call it plain text.
                if (format == OCHER_FMT_UNKNOWN) {
                    size_t len = strlen(file);
                    if (len >= 4 && strcasecmp(file + len - 4, ".txt") == 0) {
                        format = OCHER_FMT_TEXT;
                    }
                }

#if 0
                // But if more than a few binary chars, it's an unknown format.
                int bin = 0;
                for (i = 0; i < r; ++i) {
                    char c = buf2[i];
                    if (c < 8 || (c > 13 && c < 32)) {
                        bin++;
                    }
                }
                if (bin > 1)
                    format = OCHER_FMT_UNKNOWN;
#endif
            }
        }
    }

    return format;
}
