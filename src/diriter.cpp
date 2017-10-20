#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "diriter.h"
#include "path.h"

DirIter::DirIter()
    : m_flags(0)
    , m_dp(0)
{
}

DirIter::DirIter(const char* dir, unsigned int flags)
    : m_name(dir)
    , m_flags(flags)
    , m_dp(0)
{
}

DirIter::~DirIter()
{
    close();
}

void DirIter::close()
{
    if (m_dp) {
        closedir(m_dp);
        m_dp = 0;
    }
}

void DirIter::setTo(const char* dir, unsigned int flags)
{
    close();
    m_name = dir;
    m_flags = flags;
}

int DirIter::getNext(std::string& entryName)
{
    entryName.clear();
    if (!m_dp && !m_name.length())
        return ENOTDIR;
    if (!m_dp) {
        m_dp = opendir(m_name.c_str());
        if (!m_dp)
            return errno;
    }
    errno = 0;
    struct dirent* de;
    do {
        de = readdir(m_dp);
    } while (de && !(m_flags & IMPLICIT)
        && (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0));
    if (de) {
        entryName = de->d_name;
        return 0;
    } else {
        return errno;
    }
}
