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

#include "book.h"
#include "functions.h"
#include "tag.h"

#include "numberstyle.h"

namespace ods {

NumberStyle::NumberStyle(uint index, ods::number_Type number):
    v_number_style(number),
    v_style_index(index)
{
    v_prefix = "";
    v_format = "";
    v_decimalPlaces = 0;
    v_appendix = "";
    Initialize();
}

NumberStyle::NumberStyle(uint index, ods::number_Type number, QString format, int decimalPlaces, QString prefix):
    v_prefix(prefix),
    v_format(format),
    v_decimalPlaces(decimalPlaces),
    v_number_style(number),
    v_style_index(index)
{
    v_appendix = "";
    Initialize();
}

NumberStyle::NumberStyle(uint index, ods::number_Type number, QString format, QString appendix):
    v_appendix(appendix),
    v_format(format),
    v_number_style(number),
    v_style_index(index)
{
    v_prefix = "";
    v_decimalPlaces = 0;
    Initialize();
}

NumberStyle::~NumberStyle()
{

}

void NumberStyle::AddAttribute(QString xml, QString value)
{
    v_attributes.insert(xml, value);
}

void NumberStyle::AddMapAttribute(QString xml, QString value)
{
    v_map_attributes.insert(xml, value);
}

void NumberStyle::AddTextAttribute(QString xml, QString value)
{
    v_text_attributes.insert(xml, value);
}

void NumberStyle::AddText(QString value)
{
    v_text = value;
}

QString NumberStyle::GetStyleName()
{
    // with appendix ("N108P0"), as written in style:name
    return ns::co_NumberStylePrefix + QString::number(v_style_index) + v_appendix;
}

void NumberStyle::Initialize()
{
    v_attributes.insert(
        xml_QualifiedName(ns::co_Style, ns::co_Name),
        ns::co_NumberStylePrefix + QString::number(v_style_index) + v_appendix
        );
}

ods::Tag *NumberStyle::Save()
{
    ods::Tag *root_tag = nullptr;
    switch (v_number_style)
    {
    case ods::number_Type::number_currency: root_tag = new ods::Tag(ns::co_Number, ns::co_CurrencyStyle); break;
    case ods::number_Type::number_time: root_tag = new ods::Tag(ns::co_Number, ns::co_TimeStyle); break;
    case ods::number_Type::number_style: root_tag = new ods::Tag(ns::co_Number, ns::co_NumberStyle); break;
    };

    QMapIterator<QString, QString> a(v_attributes);
    while(a.hasNext())
    {
        a.next();
        root_tag->AttributeAdd(a.key(), a.value());
    }

    // schema order: style:text-properties, content, style:map last
    if (v_text_attributes.count() > 0){
        ods::Tag *tag = new ods::Tag(ns::co_Style, ns::co_TextProperties);
        QMapIterator<QString, QString> a(v_text_attributes);   // was v_map_attributes
        while(a.hasNext())
        {
            a.next();
            tag->AttributeAdd(a.key(), a.value());
        }
        root_tag->SubtagAdd(tag);
    }

    if (!v_text.isEmpty()){
        ods::Tag *tag = new ods::Tag(ns::co_Number, ns::co_Text);
        tag->TextAdd(v_text);
        root_tag->SubtagAdd(tag);
    }

    ods::Tag *number;
    if (!v_prefix.isEmpty()){
        number = new ods::Tag(ns::co_Number, ns::co_Text);
        number->TextAdd(v_prefix);
        root_tag->SubtagAdd(number);
    }

    if (v_number_style == ods::number_Type::number_time)
    {
        // "hh:mm:ss.zzz", "mm:ss.zzz", "h:mm" ... ':' only BETWEEN the parts
        // (was also after the last one); the leading unit is not truncated,
        // so 75 minutes show as 75:00 and hours may exceed 24
        root_tag->AttributeAdd(ns::co_Number, ns::co_TruncateOnOverflow, "false");

        const QStringList parts = v_format.split(QRegularExpression("[:,.]"), Qt::SkipEmptyParts);
        bool first = true;
        for (const QString &part : parts)
        {
            QString element;
            if (part.compare("hh", Qt::CaseInsensitive) == 0 || part.compare("h", Qt::CaseInsensitive) == 0)
                element = ns::co_Hours;
            else if (part == "mm" || part == "m")
                element = ns::co_Minutes;
            else if (part == "ss" || part == "s")
                element = ns::co_Seconds;
            else
                continue;                           // "zzz": decimal places of the seconds

            if (!first) {
                number = new ods::Tag(ns::co_Number, ns::co_Text);
                number->TextAdd(":");
                root_tag->SubtagAdd(number);
            }
            first = false;

            number = new ods::Tag(ns::co_Number, element);
            number->AttributeAdd(ns::co_Number, ns::co_Style, "long");
            if (element == ns::co_Seconds && v_decimalPlaces > 0)
                number->AttributeAdd(ns::co_Number, ns::co_DecimalPlaces, QString::number(v_decimalPlaces));
            root_tag->SubtagAdd(number);
        }
    }
    if (v_number_style == ods::number_Type::number_currency){
        number = new ods::Tag(ns::co_Number, ns::co_Number);
        number->AttributeAdd(ns::co_Number, ns::co_DecimalPlaces, "2");
        number->AttributeAdd(ns::co_LoExt, ns::co_MinDecimalPlaces, "2");
        number->AttributeAdd(ns::co_Number, ns::co_MinIntegerDigits, "1");
        number->AttributeAdd(ns::co_Number, ns::co_Grouping, "true");
        root_tag->SubtagAdd(number);
        number = new ods::Tag(ns::co_Number, ns::co_Text);
        number->TextAdd(" ");
        root_tag->SubtagAdd(number);
        number = new ods::Tag(ns::co_Number, ns::co_CurrencySymbol);
        // language/country of the same locale as the symbol (was fixed de/DE)
        const QStringList lc = QLocale().name().split(QLatin1Char('_'));
        number->AttributeAdd(ns::co_Number, ns::co_Language, lc.value(0));
        if (lc.size() > 1)
            number->AttributeAdd(ns::co_Number, ns::co_Country, lc.value(1));
        number->TextAdd(QLocale().currencySymbol(QLocale::CurrencySymbol));
        root_tag->SubtagAdd(number);
    }

    if (v_map_attributes.count() > 0){
        ods::Tag *tag = new ods::Tag(ns::co_Style, ns::co_Map);
        QMapIterator<QString, QString> a(v_map_attributes);
        while(a.hasNext())
        {
            a.next();
            tag->AttributeAdd(a.key(), a.value());
        }
        root_tag->SubtagAdd(tag);
    }
    return root_tag;
}

}//ods
