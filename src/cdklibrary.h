/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#ifndef OCHER_CDKLIBRARY_H
#define OCHER_CDKLIBRARY_H

#include "cdkactivity.h"

#include <vector>

class EBook;
class CdkController;

class CdkLibrary : public CdkActivity {
public:
    CdkLibrary(CdkController* controller);

    ~CdkLibrary() {}

    void onAttached();
    void onDetached();

    EBook* browse(std::vector<EBook*>& ebook);
    void read(EBook* ebook);

    CdkController* m_controller;
};

#endif
