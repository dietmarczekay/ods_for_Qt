/***********************************************************************
 * Copyright (c) 2016 dczekay@gmx.de
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

#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <QList>
#include <QXmlStreamWriter>

#include "constants.h"
#include "content.h"

namespace ods {
class Table;
class Tag;

class SpreadSheet : public Content
{
public:
    SpreadSheet(ods::Book *book, FileType type);
    ~SpreadSheet();

    RowStyle                    *CreateRowStyle();
    void                        CorrectRowNumbers();
//    Table                     *CreateTable(int position);
//    Table                     *CreateTable(QString name);
//    Table                     *CreateTable(int position, QString name);
//    QString                   GetValidTableName(QString name);
//    int                       GetTableCount(){ return v_tables.count(); }
    void                        Save(const QString &save_dir, QString &err);

protected:
//    QList<Table*>             v_tables;

    void Initialize();

};
}
#endif // SPREADSHEET_H
