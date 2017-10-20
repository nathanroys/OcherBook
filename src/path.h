/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_PATH_H
#define OCHER_PATH_H

#include <list>
#include <string>

class Path {
public:
    /**
     *  @return  True if the path is an absolute path on this platform.
     */
    static bool isAbsolute(const char* path);

    /**
     *  @param base  The base pathname.
     *  @param leaf  The portion to be appended.  Note that absolute paths override
     *      existing data in base.
     *  @return The combined pathname.
     */
    static std::string join(const char* base, const char* leaf);

    /**
     *  @param base  The base pathname.
     *  @param leaf  The portion to be appended.  Note that absolute paths override
     *      existing data in base.
     *  @return The combined pathname.
     */
    static inline std::string join(const std::string& base, const char* leaf)
    {
        return join(base.c_str(), leaf);
    }

    static void split(const char* path, std::string& base, std::string& file);

    /**
     *  Returns a list of files that match a pattern in a directory.  Note that all matching names
     *  are returned regardless of type (directory, regular file, link, etc).
     *  @param directory  The directory in which to list files
     *  @param glob  The filename globbing pattern, using literal characters, ?, and *.
     *      NULL implies * (all files).
     *  @param files  Returned list of files.
     *  @return 0 on success; nonzero for an error (files list may be partial).  Even on error,
     *      caller is responsbile for cleaning up files list.
     */
    static int list(const char* directory, const char* glob, std::list<std::string>& files);

    static std::string getDirectory(std::string& path);

    /**
     * Checks for the existence of a file or directory at the given path.
     * @param path the path to check
     * @return bool true if it exists, false otherwise
     */
    static bool exists(const char* path);

#if 0
    Path();
    Path(const Path &path);
    Path(const entry_ref *ref);
    Path(const Entry *entry);
    Path(const char *dir, const char *leaf = NULL, bool normalize = false);
    Path(const Directory *dir, const char *leaf = NULL, bool normalize = false);

    ~Path();

    int  InitCheck() const;

    int  SetTo(const entry_ref *ref);
    int  SetTo(const Entry *entry);
    int  SetTo(const char *path, const char *leaf = NULL, bool normalize = false);
    int  SetTo(const Directory *dir, const char *leaf = NULL, bool normalize = false);
    void   Unset();

    int  Append(const char *path, bool normalize = false);

    const char *Leaf() const;
    int  GetParent(Path *path) const;

    bool operator==(const Path &item) const;
    bool operator==(const char *path) const;
    bool operator!=(const Path &item) const;
    bool operator!=(const char *path) const;
    Path &operator=(const Path &item);
    Path &operator=(const char *path);

private:
    int  _SetPath(const char *path);
    static bool   _MustNormalize(const char *path, int *_error);

    std::string m_path;
#endif
};

#endif
