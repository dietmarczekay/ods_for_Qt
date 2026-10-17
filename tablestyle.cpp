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

#include <QPen>
#include "iostream"
#include "book.h"
#include "constants.h"
#include "content.h"
#include "functions.h"
#include "numberstyle.h"
#include "tag.h"

#include "tablestyle.h"

namespace ods {

TableStyle::TableStyle(const TableStyle& copy_t):
    p_table(copy_t.p_table),
    v_table_properties(copy_t.v_table_properties)
{
}

TableStyle::TableStyle(Table *table):
    p_table(table)
{
    Initialize();
}

TableStyle::~TableStyle()
{

}

TableStyle *TableStyle::Derive()
{
    TableStyle *style = new TableStyle(*this);
    style->Initialize();
    return style;
}

QString TableStyle::GetStyleName()
{
    return p_table->GetName();
}

void TableStyle::Initialize()
{
}

Tag *TableStyle::Save()
{    
    Tag *root_tag = new Tag(ns::co_Style, ns::co_Style);

    root_tag->AttributeAdd(ns::co_Style, ns::co_Name, p_table->GetName());
    root_tag->AttributeAdd(ns::co_Style, ns::co_Family, ns::co_Table);

    Tag *table_properties = new Tag(ns::co_Style, ns::co_TableProperties);

    if (v_table_properties.count() > 0)
    {

        QMapIterator<QString, QString> i(v_table_properties);
        while(i.hasNext())
        {
            i.next();
            table_properties->AttributeAdd(i.key(), i.value());
        }
    }

    root_tag->SubtagAdd(table_properties);

    return root_tag;
}

void TableStyle::SetBackgroundColor(QColor color)
{
    v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BackGroundColor), color.name());
}

void TableStyle::SetBorder(float width, SizeType unit, Qt::PenStyle style, QColor color, Sides sides)
{
    QString unit_t;
    switch(unit)
    {
    case SizeType::Pt: unit_t = "pt"; break;
    case SizeType::In: unit_t = "in"; break;
    case SizeType::Cm: unit_t = "cm"; break;
    default: unit_t = "mm"; break;
    }

    QString style_t;
    switch(style)
    {
    case Qt::DotLine: style_t = "dotted"; break;
    case Qt::DashLine: style_t = "dash"; break;
    case Qt::DashDotLine: style_t = "dot-dash"; break;
    case Qt::DashDotDotLine: style_t = "dot-dot-dash"; break;
    case Qt::CustomDashLine: style_t = "double-thin"; break;
    default: style_t = "solid"; break;
    }

    if (bool(sides & Side::Left))
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderLeft), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderLeft), "none");
    }

    if (bool(sides & Side::Bottom))
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderBottom), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderBottom), "none");
    }

    if (bool(sides & Side::Right))
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderRight), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderRight), "none");
    }

    if (bool(sides & Side::Top))
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderTop), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderTop), "none");
    }
}

void TableStyle::SetHAlign(Qt::Alignment align)
{
    QString str;
    switch (align)
    {
    case Qt::AlignLeft : str = "left"; break;
    case Qt::AlignRight: str = "end"; break;
    case Qt::AlignHCenter: str = "center"; break;
    case Qt::AlignCenter: str = "center"; break;
    default : str = "automatic";
    }
}

void TableStyle::SetTableWidth(float width, SizeType unit)
{
    QString unit_t;
    switch(unit)
    {
    case SizeType::Pt: unit_t = "pt"; break;
    case SizeType::In: unit_t = "in"; break;
    case SizeType::Cm: unit_t = "cm"; break;
    default: unit_t = "mm"; break;
    }
    v_table_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_Width), QString("%1%2").arg(width).arg(unit_t));
    if (!v_table_properties.contains(xml_QualifiedName(ns::co_Table, ns::co_Align))){
        v_table_properties.insert(xml_QualifiedName(ns::co_Table, ns::co_Align), "left");
    }
}

void TableStyle::SetVAlign(Qt::Alignment align)
{
    QString str;
    switch (align)
    {
    case Qt::AlignBottom : str = "bottom"; break;
    case Qt::AlignTop: str = "top"; break;
    case Qt::AlignVCenter: str = "middle"; break;
    case Qt::AlignCenter: str = "middle"; break;
    default : str = "automatic";
    }
}

}
