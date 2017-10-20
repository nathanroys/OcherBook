/*
 * Copyright (c) 2015, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "ocher/Container.h"
#include "ocher/fmt/epub/Epub.h"
#include "ocher/fmt/epub/LayoutEpub.h"
#include "ocher/fmt/epub/TreeMem.h"
#include "ocher/settings/Settings.h"

#include "mxml.h"


void LayoutEpub::processNode(mxml_node_t *node)
{
    Settings *settings = g_container.settings;

    if (node->type == MXML_ELEMENT) {
        const char *name = node->value.element.name;
        if (strcasecmp(name, "div") == 0) {
            processSiblings(node->child);
        } else if (strcasecmp(name, "title") == 0) {

        } else if (strcasecmp(name, "link") == 0) {
            // load CSS
            const char *type = mxmlElementGetAttr(node, "type");
            if (type && strcmp(type, "text/css") == 0) {
                const char *href = mxmlElementGetAttr(node, "href");
                if (href) {
                    std::string css;
                    css = m_epub->getFile(href);
                    // TODO: parse CSS
                }
            }
        } else if (strcasecmp(name, "p") == 0) {
            outputNl();
            processSiblings(node->child);
            outputNl();
            outputBr();
        } else if (strcasecmp(name, "br") == 0) {
            outputBr();
        } else if ((name[0] == 'h' || name[0] == 'H') && isdigit(name[1]) && !name[2]) {
            outputNl();
            pushTextAttr(AttrBold, 0);
            // TODO CSS: text size, ...
            int inc = 3 - (name[1] - '0');
            if (inc < 0)
                inc = 0;
            pushTextAttr(AttrSizeAbs, settings->fontPoints + inc * 2);
            processSiblings(node->child);
            popTextAttr(2);
            outputNl();
        } else if (strcasecmp(name, "b") == 0) {
            pushTextAttr(AttrBold, 0);
            processSiblings(node->child);
            popTextAttr();
        } else if (strcasecmp(name, "ul") == 0) {
            pushTextAttr(AttrUnderline, 0);
            processSiblings(node->child);
            popTextAttr();
        } else if (strcasecmp(name, "em") == 0) {
            pushTextAttr(AttrItalics, 0);
            processSiblings(node->child);
            popTextAttr();
        } else if (strcasecmp(name, "img") == 0) {
            const char *src = mxmlElementGetAttr(node, "src");
            if (src) {
                std::string img;
                img = m_epub->getFile(src);
                // TODO width, height, scale, ...
                // pushImage(img, 0, index);
            }
        } else {
            processSiblings(node->child);
        }
    } else if (node->type == MXML_OPAQUE) {
        for (char *p = node->value.opaque; *p; ++p) {
            outputChar(*p);
        }
        flushText();
    }
}

void LayoutEpub::processSiblings(mxml_node_t *node)
{
    for (; node; node = mxmlGetNextSibling(node)) {
        processNode(node);
    }
}

#if 1
void LayoutEpub::append(mxml_node_t *tree)
{
    // TODO:  "html/body" matches nothing if the root node is "html" (no ?xml) so using "*/body"
    mxml_node_t *body = mxmlFindPath(tree, "*/body");

    if (body) {
        // mxmlFindPath returns the first child node.  Ok, so processSiblings.
        processSiblings(body);
        outputPageBreak();
    }
}
#else
void LayoutEpub::append(std::string &html)
{
    mxml_node_t *tree = epub.parseXml(html);

    if (tree) {
        ((LayoutEpub *)m_layout)->append(tree);
        mxmlDelete(tree);
    }
}
#endif
