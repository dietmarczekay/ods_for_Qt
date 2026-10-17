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

#ifndef TEXTDOC_H
#define TEXTDOC_H

#include <QList>

#include "constants.h"
#include "content.h"

namespace ods {
class DocElement;
class PictureStyle;
class Table;
class TableStyle;
class Tag;

class TextDoc : public Content
{
public:
    TextDoc(Book *book, FileType type);
    ~TextDoc();

    void                    CorrectRowNumbers();
    ColumnStyle             *CreateColumnStyle(ods::Table *table);
    void                    CreateHeadline(QString content, int level);
    PictureStyle            *CreatePictureStyle();
    Table                   *CreateTable(int position);
    Table                   *CreateTable(QString name);
    Table                   *CreateTable(int position, QString name);
    TableStyle              *CreateTableStyle(ods::Table *table);
    void                    CreateText(QString content);
    QString                 GetValidTableName(QString name);
    int                     GetTableCount(){ return v_tables.count(); }
    void                    Save(const QString &save_dir, QString &err);

protected:
    QList<DocElement *>     v_textelements;
    //QList<Tag *>            v_paragraphs;
    QList<PictureStyle *>   v_picturestyles;
    QList<TableStyle *>     v_tablestyles;

    void Initialize();

};
}
#endif // TEXTDOC_H
