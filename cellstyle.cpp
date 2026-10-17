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
#include "book.h"
#include "constants.h"
#include "content.h"
#include "functions.h"
#include "tag.h"

#include "cellstyle.h"

namespace ods {

CellStyle::CellStyle(const CellStyle& copy_t):
    p_content(copy_t.p_content),
    v_attributes(copy_t.v_attributes),
    v_paragraph_properties(copy_t.v_paragraph_properties),
    v_style_index(0),
    v_table_cell_properties(copy_t.v_table_cell_properties),
    v_text_properties(copy_t.v_text_properties),
    v_word_wrap(copy_t.v_word_wrap)
{
}

CellStyle::CellStyle(Content *content, uint index):
    p_content(content),
    v_style_index(index)
{
    Initialize();
}

CellStyle::~CellStyle()
{

}

ods::CellStyle *CellStyle::Derive()
{
    ods::CellStyle *style = new CellStyle(*this);
    style->SetStyleIndex(p_content->NextCellStyleIndex());
    style->Initialize();
    //style->SetWordWrap(v_word_wrap);
    p_content->GetCellStyles()->append(style);
    return style;
}

// true if every property in font_property has the same value here
bool CellStyle::FontIsEqual(QMap<QString, QString> *font_property)
{
    bool result = true;                     // was false: never returned true

#if QT_VERSION >= 0x060000
    QMapIterator<QString, QString> j(*font_property);
#else
    QMapIterator<QString, QString> j(*font_property);
#endif
    while(j.hasNext())
    {
        j.next();
        if (v_text_properties.contains(j.key())){
            if (v_text_properties.value(j.key()) != j.value()){
                return false;
            }
        } else {
            return false;
        }
    }

    return result;
}

QString CellStyle::GetStyleName()
{
    return ns::co_CellStylePrefix + QString::number(v_style_index);
}

void CellStyle::Initialize()
{
    v_word_wrap = false;
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Name), ns::co_CellStylePrefix + QString::number(v_style_index));
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_Family), ns::co_TableCell);
}

// same formatting (the name is ignored); was always false and changed v_word_wrap
bool CellStyle::IsEqual(CellStyle *style)
{
    auto withoutName = [](QMap<QString, QString> m) {
        m.remove(xml_QualifiedName(ns::co_Style, ns::co_Name));
        return m;
    };
    return withoutName(v_attributes) == withoutName(style->v_attributes)
           && v_paragraph_properties == style->v_paragraph_properties
           && v_table_cell_properties == style->v_table_cell_properties
           && v_text_properties == style->v_text_properties
           && v_word_wrap == style->v_word_wrap;
}

void CellStyle::RemoveCellProperties()
{
    v_paragraph_properties.clear();
}

void CellStyle::RemoveFontStyle()
{
    v_text_properties.clear();
}

void CellStyle::RemoveParagraphProperties()
{
    v_paragraph_properties.clear();
}

ods::Tag *CellStyle::Save()
{    
    ods::Tag *root_tag = new ods::Tag(ns::co_Style, ns::co_Style);

    QMapIterator<QString, QString> a(v_attributes);
    while(a.hasNext())
    {
        a.next();
        root_tag->AttributeAdd(a.key(), a.value());
    }

    if (v_word_wrap){
        v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_WrapOption), "wrap");
    }

    if (v_table_cell_properties.count() > 0)
    {
        ods::Tag *table_cell_properties = new ods::Tag(ns::co_Style, ns::co_TableCellProperties);

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
        ods::Tag *text_properties = new ods::Tag(ns::co_Style, ns::co_TextProperties);

#if QT_VERSION >= 0x060000
    QMapIterator<QString, QString> j(v_text_properties);
#else
    QMapIterator<QString, QString> j(v_text_properties);
#endif
        while(j.hasNext())
        {
            j.next();
            text_properties->AttributeAdd(j.key(), j.value());
        }
        root_tag->SubtagAdd(text_properties);
    }

    if (v_paragraph_properties.count() > 0)
    {
        ods::Tag *paragraph_properties = new ods::Tag(ns::co_Style, ns::co_ParagraphProperties);

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

void CellStyle::SetBackgroundColor(QColor color)
{
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_BackGroundColor), color.name());
}

void CellStyle::SetBorder(float width, ods::SizeType unit, Qt::PenStyle style, QColor color, Sides sides)
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

void CellStyle::SetFontColor(QColor color)
{
    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_Color), color.name());
}

void CellStyle::SetFontSize(uint size, ods::SizeType unit)
{
    QString str = QString::number(size);    // number(uint, 'f') used 'f' (=102) as number base
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

void CellStyle::SetFontSize(QString key, QString value)
{
    v_text_properties.insert(key, value);
}

void CellStyle::SetFontStyle(QFont::Style style)
{
    QString style_t;
    switch (style)
    {
    case QFont::StyleItalic: style_t = "italic"; break;
    case QFont::StyleOblique: style_t = "oblique"; break;
    default: style_t = "normal"; break;
    }

    // was written as fo:font-weight: italic was lost and overwrote bold
    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_FontStyle), style_t);
}

void CellStyle::SetFontWeight(QFont::Weight weight)
{
    QString weight_t;
    switch (weight)
    {
    // CSS/ODF weights: Medium 500, DemiBold 600, Bold 700, ExtraBold 800, Black 900
    case QFont::Thin: weight_t = "100"; break;
    case QFont::ExtraLight: weight_t = "200"; break;
    case QFont::Light: weight_t = "300"; break;
    case QFont::Medium: weight_t = "500"; break;
    case QFont::DemiBold: weight_t = "600"; break;
    case QFont::Bold: weight_t = "bold"; break;
    case QFont::ExtraBold: weight_t = "800"; break;
    case QFont::Black: weight_t = "900"; break;
    default: weight_t = "normal"; break;
    }

    v_text_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_FontWeight), weight_t);
}

void CellStyle::SetHAlign(Qt::Alignment align)
{
    // only the horizontal part: AlignRight | AlignVCenter fell to "automatic",
    // which is not a valid fo:text-align (automatic = attribute omitted)
    QString str;
    switch (align & Qt::AlignHorizontal_Mask)
    {
    case Qt::AlignLeft : str = "start"; break;
    case Qt::AlignRight: str = "end"; break;
    case Qt::AlignHCenter: str = "center"; break;
    case Qt::AlignJustify: str = "justify"; break;
    default: break;
    }
    const QString key = xml_QualifiedName(ns::co_Fo, ns::co_TextAlign);
    if (str.isEmpty()) {
        v_paragraph_properties.remove(key);
        v_table_cell_properties.remove(xml_QualifiedName(ns::co_Style, ns::co_TextAlignSource));
        return;
    }
    v_paragraph_properties.insert(key, str);
    v_paragraph_properties.insert(xml_QualifiedName(ns::co_Fo, ns::co_MarginLeft), "0mm");
    // without text-align-source="fix" LibreOffice keeps the value-dependent alignment
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_TextAlignSource), "fix");
}

void CellStyle::SetNumberTimeStyle(QString format, int decimalPlaces, QString prefix)
{
    v_attributes.insert(
        xml_QualifiedName(
            ns::co_Style,
            ns::co_DataStyleName),
        p_content->CreateNumberTimeStyle(
            ods::number_Type::number_time,
            format,
            decimalPlaces,
            prefix)
        );
}

void CellStyle::SetNumberMoneyStyle()
{
    //v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_DataStyleName), p_content->GetParentBook()->GetStyleFile()->CreateNumberCurrencyStyle(ods::number_Type::number_currency));
    v_attributes.insert(xml_QualifiedName(ns::co_Style, ns::co_DataStyleName), p_content->CreateNumberCurrencyStyle(ods::number_Type::number_currency));
}

void CellStyle::SetStyleIndex(uint index)
{
    v_style_index = index;
}

void CellStyle::SetVAlign(Qt::Alignment align)
{
    QString str;
    switch (align & Qt::AlignVertical_Mask)
    {
    case Qt::AlignBottom : str = "bottom"; break;
    case Qt::AlignTop: str = "top"; break;
    case Qt::AlignVCenter: str = "middle"; break;
    default : str = "automatic";
    }

    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_RepeatContent), "false");
    v_table_cell_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_VerticalAlign), str);
}

void CellStyle::SetWordWrap(bool wrap)
{
    v_word_wrap = wrap;
}

void CellStyle::SetStrikeThrough(bool striked)
{
    // text property, not a table-cell property (was ignored there)
    if (striked)
    {
        v_text_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_TextStrikeThrough), "solid");
        v_text_properties.insert(xml_QualifiedName(ns::co_Style, ns::co_TextLineStyle), "single");
    }
    else
    {
        v_text_properties.remove(xml_QualifiedName(ns::co_Style, ns::co_TextStrikeThrough));
        v_text_properties.remove(xml_QualifiedName(ns::co_Style, ns::co_TextLineStyle));
    }
}

}
