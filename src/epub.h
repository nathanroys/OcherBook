/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_EPUB_H
#define OCHER_EPUB_H

#include "format.h"
#include "logger.h"
#include "unzipcache.h"

#include "mxml.h"

#include <map>
#include <string>
#include <vector>

struct EpubItem {
    std::string href;
    std::string mediaType;
};

/** EPub file format reader
 */
class EpubReader : public Reader {
public:
    /**
     */
    EpubReader(FileCache* fileCache);

    /**
     */
    EpubReader(const std::string& epubFilename, const char* password = 0);

    ~EpubReader();

    std::unique_ptr<std::vector<char>> layout();

    std::string m_title;
    std::string m_epubVersion;
    std::string m_uid;

    std::string getFile(const char* filename)
    {
        TreeFile* f = m_zip->getFile(filename, m_contentPath.c_str());
        std::string b;

        if (f) {
            b = &f->data[0];
        }
        return b;
    }

    int getSpineItemByIndex(unsigned int i, std::string& item);
    int getManifestItemById(unsigned int i, std::string& item);
    int getContentByHref(const char* href, std::string& item);

    /** Parses XML.
     *
     * Caller must call mxml_delete.
     */
    mxml_node_t* parseXml(const std::string& xml);

protected:
    TreeFile* findSpine();
    void parseSpine(TreeFile* spine);

    Logger* m_log;
    FileCache* m_zip;
    std::map<std::string, EpubItem> m_items;
    std::vector<std::string> m_spine;
    std::string m_contentPath; ///< directory of full-path attr
};

#endif
