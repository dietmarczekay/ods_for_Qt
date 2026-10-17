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

#ifndef COLUMN_H
#define COLUMN_H

#include <QMap>
#include "ods.h"

namespace ods {
class Table;
class Column
{
public:
    Column();
    Column(uint index);
    ~Column();

    void                    AttributeAdd(QString type, QString name, QString value);
    uint                    GetColumnIndex(){return v_column_index;}
    uint                    GetNumberColumnRepeated(){return v_number_columns_repeated;}
    void                    IncColumnIndex();
    void                    IncColumnIndex(uint count);
    Tag                     *Save();
    void                    SetDefaultCellStyle(ods::CellStyle *style);
    void                    SetNumberColumnsRepeated(int value);
    void                    SetStyle(ods::ColumnStyle *style);

private:
    //Table                   *p_table;
    QMap<QString, QString>  v_attributes;
    uint                    v_column_index;
    int                     v_number_columns_repeated;
};
}

#endif // COLUMN_H
