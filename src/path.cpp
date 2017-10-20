#include <dirent.h>
#include <errno.h>
#include <fnmatch.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

#include "path.h"

#define PATH_SEP '/'

bool Path::isAbsolute(const char* path)
{
    return *path == PATH_SEP;
}

std::string Path::join(const char* base, const char* leaf)
{
    std::string path;

    if (!leaf) {
        path = base;
    } else if (isAbsolute(leaf)) {
        path = leaf;
    } else {
        path = base;
        if (path.length() && path[path.length() - 1] != PATH_SEP)
            path += PATH_SEP;
        path += leaf;
    }
    return path;
}

void Path::split(const char* path, std::string& base, std::string& file)
{
    const char* slash = strrchr(path, PATH_SEP);

    if (slash) {
        base.assign(path, slash - path + 1);
        file = slash + 1;
    } else {
        base = "./";
        file = path;
    }
}

int Path::list(const char* dir, const char* glob, std::list<std::string>& files)
{
    int r = -1;

    try {
        DIR* dp;
        if ((dp = opendir(dir)) == NULL) {
            r = errno;
        } else {
            r = 0;
            struct dirent* dir_entry;
            while ((dir_entry = readdir(dp)) != NULL) {
                const char* name = dir_entry->d_name;
                if (!glob || fnmatch(glob, name, 0) == 0) {
                    files.push_back(std::string(name));
                }
            }
            closedir(dp);
        }
    } catch (std::bad_alloc&) {
        r = ENOMEM;
    }
    return r;
}

std::string Path::getDirectory(std::string& path)
{
    std::string dir(path);
    size_t pos = path.rfind(PATH_SEP);

    if (pos != std::string::npos)
        dir.resize(pos + 1);
    return dir;
}

bool Path::exists(const char* path)
{
    struct stat statbuf;
    int r = stat(path, &statbuf);

    return r == 0;
}
