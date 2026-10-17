/***********************************************************************
 * Copyright (c) 2016 git@czekay.org
 * SPDX-License-Identifier: GPL-3.0-or-later

    This file is part of QODS_Module.

    QODS_Module is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QODS_Module is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QODS_Module.  If not, see <https://www.gnu.org/licenses/>.

************************************************************************/

#ifndef MANIFEST_H
#define MANIFEST_H

#include <QMap>
#include <QList>
#include "tag.h"

namespace ods{
class Book;

class Manifest
{
public:
    Manifest(Book *p_book, QString type);
    ~Manifest();

    /** Additional package file (e.g. Pictures/...). Same path only once. */
    void                AddFileEntry(const QString &fullPath, const QString &mediaType)
                        { v_file_entries.insert(fullPath, mediaType); }
    void                Save(const QString &save_dir, QString &err);

private:
    Book                *p_book;
    ods::Tag            *root_tag = nullptr;
    QString             v_error;
    QMap<QString, QString> v_file_entries;   // full-path -> media-type
    QString             v_type;

    void                initialize_meta();
    void                initialize_rdf();


};
} //ods
#endif // MANIFEST_H
