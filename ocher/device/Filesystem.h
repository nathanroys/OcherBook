/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_FILESYSTEM_H
#define OCHER_FILESYSTEM_H

#include <QObject>

class Options;

/** Represents the interesting points of the filesystem (namely, where the books exist, and where
 * user settings might be stored.)
 *
 * On some platforms, notifies when the books change.
 */
class Filesystem : public QObject
{
    Q_OBJECT

public:
    Filesystem();
    ~Filesystem();

    Filesystem(const Filesystem &) = delete;
    Filesystem &operator=(const Filesystem &) = delete;

    const char **m_libraries;
    char *m_home;
    char *m_settings;

    void initWatches(Options *options);
    void deinitWatches();
    void fireEvents();

signals:
    void dirChanged(const char *, const char *);

protected:
    int m_infd;
};

#endif
