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

#include <QPen>
#include "iostream"
#include "book.h"
#include "constants.h"
#include "content.h"
#include "functions.h"
#include "tag.h"

#include "picturestyle.h"

namespace ods {

PictureStyle::PictureStyle(const PictureStyle& copy_t):
    p_content(copy_t.p_content),
    v_attributes(copy_t.v_attributes),
    v_paragraph_properties(copy_t.v_paragraph_properties),
    v_style_index(0),
    v_table_cell_properties(copy_t.v_table_cell_properties),
    v_text_properties(copy_t.v_text_properties)
{
}

PictureStyle::PictureStyle(Content *content, uint index):
    p_content(content),
    v_style_index(index)
{
    Initialize();
}

PictureStyle::~PictureStyle()
{

}

PictureStyle *PictureStyle::Derive()
{
    PictureStyle *style = new PictureStyle(*this);
    style->SetStyleIndex(p_content->GetCellStylesCount() + 1);
    style->Initialize();
    p_content->GetPictureStyles()->append(style);
    return style;
}

bool PictureStyle::FontIsEqual(QMap<QString, QString> *font_property)
{
    bool result = false;

    QMapIterator<QString, QString> j(*font_property);
    while(j.hasNext())
    {
        j.next();
        if (v_text_properties.contains(j.key())){
            if (v_text_properties.value(j.key()) != j.value()){
                return false;
            }
        }
    }

    return result;
}

QString PictureStyle::GetStyleName()
{
    return ns::co_CellStylePrefix + QString::number(v_style_index);
}

void PictureStyle::Initialize()
{
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Name), ns::co_CellStylePrefix + QString::number(v_style_index));
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Family), ns::co_TableCell);
}

bool PictureStyle::IsEqual(PictureStyle *style)
{
    bool isequal = false;
    isequal = isequal && (v_attributes == style->v_attributes);
    //v_paragraph_properties;
    //v_table_cell_properties;
    //v_text_properties;

    return isequal;
}

void PictureStyle::RemoveCellProperties()
{
    v_paragraph_properties.clear();
}

void PictureStyle::RemoveFontStyle()
{
    v_text_properties.clear();
}

void PictureStyle::RemoveParagraphProperties()
{
    v_paragraph_properties.clear();
}

Tag *PictureStyle::Save()
{    
    Tag *root_tag = new Tag(ns::co_Style, ns::co_Style);

    QMapIterator<QString, QString> a(v_attributes);
    while(a.hasNext())
    {
        a.next();
        root_tag->AttributeAdd(a.key(), a.value());
    }

    if (v_table_cell_properties.count() > 0)
    {
        Tag *table_cell_properties = new Tag(ns::co_Style, ns::co_TableCellProperties);

        QMapIterator<QString, QString> i(v_table_cell_properties);
        while(i.hasNext())
        {
            i.next();
            table_cell_properties->AttributeAdd(i.key(), i.value());
        }
        root_tag->SubtagAdd(table_cell_properties);
    }

    if (v_text_properties.count() > 0)
    {
        Tag *text_properties = new Tag(ns::co_Style, ns::co_TextProperties);

        QMapIterator<QString, QString> j(v_text_properties);
        while(j.hasNext())
        {
            j.next();
            text_properties->AttributeAdd(j.key(), j.value());
        }
        root_tag->SubtagAdd(text_properties);
    }

    if (v_paragraph_properties.count() > 0)
    {
        Tag *paragraph_properties = new Tag(ns::co_Style, ns::co_ParagraphProperties);

        QMapIterator<QString, QString> k(v_paragraph_properties);
        while(k.hasNext())
        {
            k.next();
            paragraph_properties->AttributeAdd(k.key(), k.value());
        }
        root_tag->SubtagAdd(paragraph_properties);
    }
    return root_tag;
}

void PictureStyle::SetBackgroundColor(QColor color)
{
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BackGroundColor), color.name());
}

void PictureStyle::SetBorder(float width, ods::SizeType unit, Qt::PenStyle style, QColor color, Sides sides)
{
    QString unit_t;
    switch(unit)
    {
    case ods::SizeType::Pt: unit_t = "pt"; break;
    case ods::SizeType::In: unit_t = "in"; break;
    case ods::SizeType::Cm: unit_t = "cm"; break;
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

    if (bool(sides & ods::Side::Left))
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderLeft), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderLeft), "none");
    }

    if (bool(sides & ods::Side::Bottom))
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderBottom), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderBottom), "none");
    }

    if (bool(sides & ods::Side::Right))
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderRight), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderRight), "none");
    }

    if (bool(sides & ods::Side::Top))
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderTop), QString("%1%2 %3 %4").arg(width).arg(unit_t).arg(style_t).arg(color.name()));
    }
    else
    {
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BorderTop), "none");
    }
}

void PictureStyle::SetFontSize(uint size, ods::SizeType unit)
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

    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_FontSize), str);
}

void PictureStyle::SetFontSize(QString key, QString value)
{
    v_text_properties.insert(key, value);
}

void PictureStyle::SetFontStyle(QFont::Style style)
{
    QString style_t;
    switch (style)
    {
    case QFont::StyleItalic: style_t = "italic"; break;
    case QFont::StyleOblique: style_t = "oblique"; break;
    default: style_t = "normal"; break;
    }

    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_FontWeight), style_t);
}

void PictureStyle::SetFontWeight(QFont::Weight weight)
{
    QString weight_t;
    switch (weight)
    {
    case QFont::Bold: weight_t = "bold"; break;
    case QFont::Thin: weight_t = "100"; break;
    case QFont::ExtraLight: weight_t = "200"; break;
    case QFont::Light: weight_t = "300"; break;
    case QFont::Medium: weight_t = "400"; break;
    case QFont::DemiBold: weight_t = "500"; break;
        //case QFont::Bold: weight_t = "600"; break;
    case QFont::ExtraBold: weight_t = "700"; break;
    case QFont::Black: weight_t = "800"; break;
    default: weight_t = "normal"; break;
    }

    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_FontWeight), weight_t);
}

void PictureStyle::SetHAlign(Qt::Alignment align)
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
    v_paragraph_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_TextAlign), str);
    v_paragraph_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_MarginLeft), "0mm");
}

void PictureStyle::SetStyleIndex(uint index)
{
    v_style_index = index;
}

void PictureStyle::SetVAlign(Qt::Alignment align)
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

    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_TextAlignSource), "fix");
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_RepeatContent), "false");
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_VerticalAlign), str);
}

}
