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

#ifndef TABLE_H
#define TABLE_H

#include <QMap>
#include "ods.h"

namespace ods {

class Column;
class Content;
class Row;
class TableStyle;

class Table : public DocElement
{
public:
    Table(Content *content, int index, QString name);
    Table(Content *content, QString name);
    ~Table();

    void                        CorrectRowNumbers();
    Column                      *CreateColumn();
    Column                      *CreateColumn(uint index);
    Row                         *CreateRow();
    Row                         *CreateRow(uint index);
//    Table                       *CreateTable();
//    Table                       *CreateTable(QString name);
    QString                     GetName(){ return v_name; }
    Content                     *GetParentContent(){ return p_content; }
    Row                         *GetRow(int index);
    Column                      *InsertColumn(uint index, uint count);
    Row                         *InsertRow(uint index, bool move = true);
    void                        Prepare();
    Tag                         *Save();
    void                        SetStyle(TableStyle *style);
private:
    Content                     *p_content;
    QMap<QString, QString>      v_attributes;
    QList<Column *>             v_columns;
    QString                     v_name;
    QList<Row *>                v_rows;
   // int                         v_row_max_index = 1;
    int                         v_table_index;
    TableStyle                  *v_table_style = nullptr;

    void                        Initialize();
};
}
#endif // TABLE_H
