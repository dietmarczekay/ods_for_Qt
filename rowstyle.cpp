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

#include "rowstyle.h"

namespace ods {

RowStyle::RowStyle(uint index):
    v_style_index(index)
{
    Initialize();
}

RowStyle::~RowStyle()
{

}

uint RowStyle::GetStyleIndex()
{
    return v_style_index;
}

QString RowStyle::GetStyleName()
{
    return ns::co_RowStylePrefix + QString::number(v_style_index);
}

void RowStyle::Initialize()
{
    attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Name), ns::co_RowStylePrefix + QString::number(v_style_index));
    attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Family), ns::co_TableRow);
}

ods::Tag *RowStyle::Save()
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Style, ns::co_Style);

    QMapIterator<QString, QString> a(attributes);
    while(a.hasNext())
    {
        a.next();
        root_tag->AttributeAdd(a.key(), a.value());
    }

    if (row_properties.count() > 0)
    {
        ods::Tag *v_table_row_properties = new ods::Tag(ns::co_Style, ns::co_TableRowProperties);

        QMapIterator<QString, QString> i(row_properties);
        while(i.hasNext())
        {
            i.next();
            v_table_row_properties->AttributeAdd(i.key(), i.value());
        }
        root_tag->SubtagAdd(v_table_row_properties);
    }
    if (v_font_properties.count() > 0)
    {
        ods::Tag *v_text_properties = new ods::Tag(ns::co_Style, ns::co_TextProperties);

#if QT_VERSION >= 0x060000
        QMapIterator<QString, QString> j(v_font_properties);
#else
        QMapIterator<QString, QString> j(v_font_properties);
#endif
        while(j.hasNext())
        {
            j.next();
            v_text_properties->AttributeAdd(j.key(), j.value());
        }
        root_tag->SubtagAdd(v_text_properties);
    }


    return root_tag;
}

void RowStyle::SetBackgroundColor(QColor color)
{
    row_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BackGroundColor), color.name());
}

void RowStyle::SetBreakAfter()
{
    row_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BreakAfter), ns::co_Page);
}

void RowStyle::SetBreakBefore()
{
    row_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BreakBefore), ns::co_Page);
}

void RowStyle::SetFontSize(uint size, ods::SizeType unit)
{
    QString str = QString::number(size, 'f', 0);
    if (unit == ods::SizeType::Cm)
        str += "cm";
    else if (unit == ods::SizeType::In)
        str += "in";
    else if(unit == ods::SizeType::Mm)
        str += "mm";
    else if (unit == ods::SizeType::Pt)
        str += "pt";

    v_font_properties.insert(xml_QualifiedName(ns::co_Fo,  ns::co_FontSize), str);
}

void RowStyle::SetVAlign(Qt::Alignment align)
{
    Q_UNUSED(align);
    // has to be done
    // all cells in that row need an extra valign entry
    // recreate styles and assign them t the cells or so
}

void RowStyle::SetRowHeight(uint size, ods::SizeType unit)
{
    QString str = QString::number(size, 'f', 0);
    if (unit == ods::SizeType::Cm)
        str += "cm";
    else if (unit == ods::SizeType::In)
        str += "in";
    else if(unit == ods::SizeType::Mm)
        str += "mm";
    else if (unit == ods::SizeType::Pt)
        str += "pt";

    row_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_RowHeight), str);
    row_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_UseOptimalRowHeight), "false");
}



}
