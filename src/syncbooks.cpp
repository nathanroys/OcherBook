/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "syncbooks.h"

#include "controller.h"
#include "diriter.h"
#include "ebook.h"
#include "format.h"
#include "logger.h"
#include "options.h"
#include "path.h"

#include <sys/stat.h>
#include <sys/types.h>

static void processFile(Controller* controller, const char* file)
{
    struct stat s;

    if (stat(file, &s)) {
        Log::warn("SyncBooks", "%s: stat: %s", file, strerror(errno));
    } else {
        if (S_ISREG(s.st_mode)) {
            Encoding encoding;
            Fmt format = detectFormat(file, &encoding);
            if (format != OCHER_FMT_UNKNOWN) {
                EBook* ebook = new EBook;
                ebook->format = format;
                ebook->relPath = file;
                controller->ctx.library.add(ebook);
            }
        } else if (S_ISDIR(s.st_mode)) {
            std::string name;
            DirIter dir(file);
            while (dir.getNext(name) == 0 && name.length()) {
                std::string entryName = Path::join(file, name.c_str());
                processFile(controller, entryName.c_str());
            }
        }
    }
}

void SyncBooks(Controller* controller, const char** files)
{
    if (files) {
        for (const char* file = *files; file; file = *++files) {
            processFile(controller, file);
        }
    }
}
