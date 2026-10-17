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

#include "cellstyle.h"
#include "functions.h"
#include "tag.h"
#include "columnstyle.h"

namespace ods {

ColumnStyle::ColumnStyle(uint index):
    v_style_index(index)
{
    Initialize();
}

ColumnStyle::~ColumnStyle()
{
}

uint ColumnStyle::GetStyleIndex()
{
    return v_style_index;
}

QString ColumnStyle::GetStyleName()
{
    if (v_style_name.isEmpty()){
        return ns::co_ColumnStylePrefix + QString::number(v_style_index);
    }else{
        return v_style_name;
    }
}

void ColumnStyle::Initialize()
{
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Family), ns::co_TableColumn);
}

ods::Tag *ColumnStyle::Save()
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Style, ns::co_Style);

    if (v_style_name.isEmpty()){
        root_tag->AttributeAdd(xml_QualifiedName(ns::co_Style, ns::co_Name), ns::co_ColumnStylePrefix + QString::number(v_style_index));
    }else{
        root_tag->AttributeAdd(xml_QualifiedName(ns::co_Style, ns::co_Name), v_style_name);
    }

    QMapIterator<QString, QString> a(v_attributes);
    while(a.hasNext())
    {
        a.next();
        root_tag->AttributeAdd(a.key(), a.value());
    }

    ods::Tag *table_column_properties = new ods::Tag(ns::co_Style, ns::co_TableColumnProperties);
    QMapIterator<QString, QString> i(v_table_column_properties);
    while(i.hasNext())
    {
        i.next();
        table_column_properties->AttributeAdd(i.key(), i.value());
    }
    root_tag->SubtagAdd(table_column_properties);

    return root_tag;
}

void ColumnStyle::SetBackgroundColor(QColor color)
{
    v_table_column_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BackGroundColor), color.name());
}

void ColumnStyle::SetBreakAfter()
{
    v_table_column_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BreakAfter), ns::co_Auto);
}

void ColumnStyle::SetBreakBefore()
{
    v_table_column_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BreakBefore), ns::co_Column);
}

void ColumnStyle::SetColumnWidth(float width, SizeType unit)
{
    //v_table_column_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BreakBefore), ns::co_Auto);
    switch(unit)
    {
    case SizeType::Cm: v_table_column_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_ColumnWidth), QString::number(width, 'f', 2) + ns::co_Centimeter); break;
    default: v_table_column_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_ColumnWidth), QString::number(width, 'f', 2) + ns::co_Millimeter); break;
    }
}

void ColumnStyle::SetName(QString name)
{
    v_style_name = QString("%1%2%3").arg(name).arg(".").arg(GetExcelColumnName(v_style_index + 1));
}
}
