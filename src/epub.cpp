/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "epub.h"
#include "path.h"

#include <mxml.h>

#include <stdio.h>

TreeFile* EpubReader::findSpine()
{
    TreeFile* mimetype = m_zip->getFile("mimetype");

    if (!mimetype) {
        m_log->warn("Missing '/mimetype'");
    } else {
        stripUtf8Bom(mimetype->data);
        if (mimetype->data.size() < 20
            || strncmp(mimetype->data.c_str(), "application/epub+zip", 20)) {
            m_log->warn("'/mimetype' has incorrect value: '%s' (%u)", mimetype->data.c_str(),
                (unsigned int)mimetype->data.size());
        }
    }

    mxml_node_t* tree = 0;
    const char* fullPath = 0;
    TreeFile* container = m_zip->getFile("META-INF/container.xml");
    if (!container) {
        m_log->error("Missing 'META-INF/container.xml'");
    } else {
        stripUtf8Bom(container->data);
        tree = mxmlLoadString(NULL, container->data.c_str(), MXML_IGNORE_CALLBACK);
        // Must be a "rootfiles" element, with one or more "rootfile" children.
        // First "rootfile" is the default. [OCF 3.0 2.5.1]
        mxml_node_t* rootfile = mxmlFindPath(tree, "container/rootfiles/rootfile");
        if (!rootfile) {
            m_log->error("Missing rootfile node");
        } else {
            fullPath = mxmlElementGetAttr(rootfile, "full-path");
            if (!fullPath) {
                m_log->error("Missing 'full-path' attr");
            } else {
                std::string textPath(fullPath);
                // TODO:  path handling is weak: canonicalization, ...
                m_contentPath = Path::getDirectory(textPath);
                if (m_contentPath == textPath) {
                    m_contentPath = "";
                }
            }
        }
    }

    TreeFile* spine = 0;
    if (fullPath) {
        spine = m_zip->getFile(fullPath);
        if (!spine)
            m_log->error("Missing spine '%s'", fullPath);
    }
    if (tree)
        mxmlDelete(tree);
    return spine;
}

const char* _mxmlElementGetAttr(mxml_node_t* n, const char* name)
{
    for (int i = 0; i < n->value.element.num_attrs; ++i) {
        if (strcmp(n->value.element.attrs[i].name, name) == 0) {
            return n->value.element.attrs[i].value;
        }
    }
    return NULL;
}

void EpubReader::parseSpine(TreeFile* spineFile)
{
    stripUtf8Bom(spineFile->data);

    mxml_node_t* tree = mxmlLoadString(NULL, spineFile->data.c_str(), MXML_IGNORE_CALLBACK);

    mxml_node_t* package = mxmlFindPath(tree, "package");
    if (!package) {
        m_log->warn("Missing 'package' element");
    } else {
        m_uid = _mxmlElementGetAttr(package, "unique-identifier");
        m_epubVersion = _mxmlElementGetAttr(package, "version");
    }

    mxml_node_t* metadata = mxmlFindPath(tree, "package/metadata");
    if (!metadata) {
        m_log->warn("Missing 'metadata' element");
    } else {
        for (mxml_node_t* node = mxmlGetFirstChild(metadata); node;
             node = mxmlGetNextSibling(node)) {
            const char* name = node->value.element.name;
            if (node->type == MXML_ELEMENT) {
                if (strcasecmp(name, "dc:title") == 0) {
                    // m_title = mxmlGetNextSibling(node)->value.opaque;
                } else if (strcasecmp(name, "dc:creator") == 0) {
                    // dc:creator opf:file-as="" opf:role="aut"></dc:creator>
                    // m_author = mxmlGetNextSibling(node)->value.opaque;
                } else if (strcasecmp(name, "dc:language") == 0) {
                    // <dc:language>en-US</dc:language>
                }
            }
        }
        m_title = metadata->value.opaque;
    }

    mxml_node_t* manifest = mxmlFindPath(tree, "package/manifest");
    if (!manifest) {
        m_log->warn("Missing 'manifest' element");
    } else {
        for (mxml_node_t* i = manifest->child; i; i = i->next) {
            if (i->type != MXML_ELEMENT || strcmp(i->value.element.name, "item"))
                continue;
            const char* id = _mxmlElementGetAttr(i, "id");
            const char* href = _mxmlElementGetAttr(i, "href");
            const char* mediaType = _mxmlElementGetAttr(i, "media-type");
            if (id && href) {
                EpubItem item;
                item.href = href;
                item.mediaType = mediaType;
                std::string _id(id);
                m_items.insert(std::pair<std::string, EpubItem>(_id, item));
            }
        }
    }

    mxml_node_t* spine = mxmlFindPath(tree, "package/spine");
    if (!spine) {
        m_log->warn("Missing 'spine' element");
    } else {
        // TODO: toc
        for (mxml_node_t* i = spine->child; i; i = i->next) {
            if (i->type != MXML_ELEMENT || strcmp(i->value.element.name, "itemref"))
                continue;
            const char* idref = _mxmlElementGetAttr(i, "idref");
            if (idref) {
                std::string _idref(idref);
                m_spine.push_back(_idref);
            }
        }
    }
    if (tree)
        mxmlDelete(tree);
}

int EpubReader::getSpineItemByIndex(unsigned int i, std::string& item)
{
    if (i < m_spine.size()) {
        std::string& idref = m_spine[i];
        std::map<std::string, EpubItem>::iterator it = m_items.find(idref);
        if (it != m_items.end()) {
            TreeFile* f = m_zip->getFile((*it).second.href.c_str(), m_contentPath.c_str());
            if (f) {
                item = f->data;
                return 0;
            }
        }
    }
    m_log->warn("Missing spine item #%d", i);
    return -1;
}

int EpubReader::getManifestItemById(unsigned int i, std::string& item)
{
    (void)i;
    (void)item;

    // TODO
    m_log->warn("unimplemented");

    return -1;
}

int EpubReader::getContentByHref(const char* href, std::string& item)
{
    (void)href;
    (void)item;

    // TODO
    m_log->warn("unimplemented");

    return -1;
}

EpubReader::EpubReader(FileCache* fileCache)
    : m_log(Log::get("Epub"))
    , m_zip(fileCache)
{
    TreeFile* spine = findSpine();

    if (spine) {
        parseSpine(spine);
    }
}

EpubReader::EpubReader(const std::string& filename, const char* password)
    : m_log(Log::get("Epub"))
    , m_zip(new UnzipCache(filename.c_str(), password))
{
    TreeFile* spine = findSpine();

    if (spine) {
        parseSpine(spine);
    }
}

EpubReader::~EpubReader()
{
    delete m_zip;
}

mxml_node_t* EpubReader::parseXml(const std::string& xml)
{
    mxml_node_t* tree = mxmlLoadString(NULL, xml.c_str(), MXML_OPAQUE_CALLBACK);

    return tree;
}

std::unique_ptr<std::vector<char>> EpubReader::layout()
{
    // TODO OCHER-1
    EpubLayout* layout = new EpubLayout(this);
    std::string html;
    for (int i = 0;; i++) {
        if (getSpineItemByIndex(i, html) != 0)
            break;
#if 1
        mxml_node_t* tree = parseXml(html);
        if (tree) {
            layout->append(tree);
            mxmlDelete(tree);
        } else {
            m_log->warn("No tree found for spine item %d", i);
        }
#else
        layout->append(html);
#endif
    }
    return layout->unlock();
}
