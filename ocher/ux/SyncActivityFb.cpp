/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/fmt/Format.h"
#include "ocher/settings/Options.h"
#include "ocher/ux/fb/SyncActivityFb.h"
#include "ocher/ux/fb/UxController.h"
#include "ocher/util/DirIter.h"
#include "ocher/util/File.h"
#include "ocher/util/Path.h"

#include <sys/stat.h>
#include <sys/types.h>


class SyncActivityWork : public EventWork {
public:
    SyncActivityWork(UxController *controller, const char **files) :
        EventWork(g_container.loop),
        m_files(files),
        m_uxController(controller)
    {
        start();
    }

    ~SyncActivityWork()
    {
        join();
    }

protected:
    const char **m_files;
    UxController *m_uxController;
    void work();
    void processFile(const char *file);
};

void SyncActivityWork::work()
{
    if (m_files) {
        for (const char *file = *m_files; file; file = *++m_files) {
            processFile(file);
        }
    }
}

void SyncActivityWork::processFile(const char *file)
{
    struct stat s;

    if (stat(file, &s)) {
        //Log::warn(LOG_NAME, "%s: stat: %s", file, strerror(errno));
    } else {
        if (S_ISREG(s.st_mode)) {
            Encoding encoding;
            Fmt format = detectFormat(file, &encoding);
            if (format != OCHER_FMT_UNKNOWN) {
                Meta *m = new Meta;
                m->format = format;
                m->relPath = file;
                loadMeta(m);
                m_uxController->ctx.library.add(m);
            }
        } else if (S_ISDIR(s.st_mode)) {
            std::string name;
            DirIter dir(file);
            while (dir.getNext(name) == 0 && name.length()) {
                std::string entryName = Path::join(file, name.c_str());
                processFile(entryName.c_str());
            }
        }
    }
}

void SyncActivity::draw()
{
    m_fb->setFg(0xff, 0xff, 0xff);
    m_fb->fillRect(&m_rect);
}

SyncActivity::SyncActivity()
{
    maximize();

    int w = m_rect.w / 8;
    //m_spinner.setRect(m_rect.w / 2 - w / 2, m_rect.h / 2 - w - 2, w, w);
    //addChild(m_spinner);
}

void SyncActivity::onAttached()
{
    // TODO:  sync files passed on command line once.  Sync filesystem->m_libraries.  etc.
    m_work = new SyncActivityWork(m_uxController, g_container.options->files);
    g_container.options->files = NULL;
    //m_spinner.start();
    invalidate();
}

void SyncActivity::onDetached()
{
    // TODO:  Can be detached not due to work finishing, but being
    // forced out, eg, power saver.  Pause work, don't delete.
    delete m_work;
    //m_spinner.stop();

    m_uxController->ctx.library.notify();
    m_uxController->setNextActivity(ACTIVITY_HOME);
}
