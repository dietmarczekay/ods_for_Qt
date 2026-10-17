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

#include "column.h"

namespace ods {

Column::Column():
    v_column_index(0),
    v_number_columns_repeated(0)
{
}

Column::Column(uint index):
    v_column_index(index),
    v_number_columns_repeated(0)
{
}

Column::~Column()
{

}

void Column::AttributeAdd(QString type, QString name, QString value)
{
    v_attributes.insert(xml_QualifiedName(type, name), value);
}

void Column::IncColumnIndex()
{
    v_column_index++;
}

void Column::IncColumnIndex(uint count)
{
    v_column_index += count;
}

ods::Tag *Column::Save()
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Table, ns::co_TableColumn);
    QMapIterator<QString, QString> i(v_attributes);
    while(i.hasNext())
    {
        i.next();
        root_tag->AttributeAdd(i.key(), i.value());
    }
    return root_tag;
}

void Column::SetDefaultCellStyle(CellStyle *style)
{
    v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_DefaultCellStyleName), style->GetStyleName());
}

void Column::SetNumberColumnsRepeated(int value)
{
    v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_NumColsRepeated), QString::number(value));

}

void Column::SetStyle(ods::ColumnStyle *style)
{
    v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_StyleName), style->GetStyleName());
}

}
