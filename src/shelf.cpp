/*
 * Copyright (c) 2017, Chuck Coffing
 * OcherBook is released under the GPLv3.  See COPYING.
 */

#include "shelf.h"

#include "ebook.h"

void GroupOfBooks::attach(GroupOfBooks* observer)
{
    m_observers.push_back(observer);
}

void GroupOfBooks::detach(GroupOfBooks* observer)
{
    for (auto it = m_observers.begin(); it < m_observers.end(); ++it) {
        if ((*it) == observer) {
            m_observers.erase(it);
            break;
        }
    }
}

void GroupOfBooks::notify()
{
    unsigned int n = m_observers.size();

    for (unsigned int i = 0; i < n; ++i) {
        GroupOfBooks* group = m_observers[i];
        group->update(this);
    }
}

Shelf::Shelf()
{
}

Shelf::~Shelf()
{
}

Library::~Library()
{
    size_t n = m_books.size();

    for (size_t i = 0; i < n; ++i) {
        delete m_books[i];
    }

    notify();
}

void Library::add(EBook* ebook)
{
    m_books.push_back(ebook);
}

void ShortList::update(GroupOfBooks* changed)
{
    m_books.clear();
    const std::vector<EBook*>* books = changed->getList();
    unsigned int n = books->size();
    for (unsigned int i = 0; i < n; ++i) {
        EBook* ebook = (*books)[i];
        if (ebook->record.shortlist) {
            m_books.push_back(ebook);
        }
    }
}
