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
#include "constants.h"

#include "content.h"

namespace ods{

Content::Content(Book *book, FileType type):
    p_book(book)
{
    Q_UNUSED(type);
    initialize();
}

Content::~Content()
{
    qDeleteAll(v_tables);
    qDeleteAll(v_cellstyles);
    qDeleteAll(v_columnstyles);
    qDeleteAll(v_numberstyles);
    qDeleteAll(v_picturestyles);
    qDeleteAll(v_rowstyles);
}

ods::CellStyle *Content::CreateCellStyle()
{
    // one numbering for all cell styles (CreateCellStyle, Derive, row styles)
    ods::CellStyle *style = new ods::CellStyle(this, NextCellStyleIndex());
    v_cellstyles.append(style);
    return style;
}

ods::ColumnStyle *Content::CreateColumnStyle()
{
    ods::ColumnStyle *style = nullptr;
    if (v_columnstyles.count() > 0)
    {
        style = new ods::ColumnStyle(v_columnstyles.at(v_columnstyles.count() - 1)->GetStyleIndex() + 1);
    }
    else
    {
        style = new ods::ColumnStyle(0);
    }
    v_columnstyles.append(style);
    return style;
}

// "N<i>P0" = positive format (volatile), "N<i>" = negative in red with "-"
// and a map to N<i>P0 for values >= 0. The cell uses "N<i>".
// Before: the map pointed to N<i>P0, the positive style was N<i+1>P0 and
// the returned name N<i+1> did not exist at all.
QString Content::CreateNumberCurrencyStyle(ods::number_Type type)
{
    const int i = p_book->GetNumStyleNumber();

    auto *positive = new ods::NumberStyle(i, type, "", "P0");
    positive->AddAttribute(xml_QualifiedName(ns::co_Style, ns::co_Volatile), "true");
    v_numberstyles.append(positive);        // referenced style first

    auto *style = new ods::NumberStyle(i, type);
    style->AddMapAttribute(xml_QualifiedName(ns::co_Style, ns::co_Condition), QString("value()>=0"));
    style->AddMapAttribute(xml_QualifiedName(ns::co_Style, ns::co_ApplyStyleName), positive->GetStyleName());
    style->AddTextAttribute(xml_QualifiedName(ns::co_Fo, ns::co_Color), "#ff0000");
    style->AddText("-");
    v_numberstyles.append(style);

    return style->GetStyleName();
}

// prefix wird dem Zeitformat vorangestellt
QString Content::CreateNumberTimeStyle(ods::number_Type type, QString format, int decimalPlaces, QString prefix)
{
    ods::NumberStyle *style = nullptr;
    if (format.isEmpty()){
        style = new ods::NumberStyle(p_book->GetNumStyleNumber(), type);
    } else {
        style = new ods::NumberStyle(p_book->GetNumStyleNumber(), type, format, decimalPlaces, prefix);
    }
    v_numberstyles.append(style);
    return style->GetStyleName();
}

ods::Table *Content::CreateTable(QString name)
{
    QString va_name = GetValidTableName(name);
    Table *table = new ods::Table(this, va_name);
    v_tables.append(table);
    return table;
}

ods::Table *Content::CreateTable(int position)
{
    QString va_name = GetValidTableName(QStringLiteral("Tabelle%1").arg(v_tables.count() + 1));
    ods::Table *table = new ods::Table(this, v_tables.count(), va_name);
    v_tables.insert(position, table);
    return table;
}

ods::Table *Content::CreateTable(int position, QString name)
{
    QString va_name = GetValidTableName(name);
    Table *table = new ods::Table(this, va_name);
    v_tables.insert(position, table);
    return table;
}

// Sheet names: no [ ] * ? : / \, not starting/ending with an apostrophe,
// unique within the document (duplicates make the file invalid)
QString Content::GetValidTableName(QString name)
{
    static const QRegularExpression forbidden(QStringLiteral("[\\[\\]*?:/\\\\]"));
    QString result = name;
    result.replace(forbidden, QStringLiteral("-"));
    result.replace(QLatin1Char('$'), QLatin1Char('s'));
    while (result.startsWith(QLatin1Char('\'')))
        result.remove(0, 1);
    while (result.endsWith(QLatin1Char('\'')))
        result.chop(1);
    if (result.trimmed().isEmpty())
        result = QStringLiteral("Tabelle");

    auto used = [this](const QString &n) {
        for (Table *t : std::as_const(v_tables))
            if (t->GetName().compare(n, Qt::CaseInsensitive) == 0)
                return true;
        return false;
    };
    const QString base = result;
    for (int i = 2; used(result); ++i)
        result = QStringLiteral("%1 %2").arg(base).arg(i);
    return result;
}

QString Content::TimeDataStyle(const QString &format)
{
    auto it = v_time_data_styles.constFind(format);
    if (it != v_time_data_styles.cend())
        return *it;
    // decimal places of the seconds from the format: "ss.zzz" -> 3
    const int dot = format.lastIndexOf(QRegularExpression(QStringLiteral("[.,]")));
    const int decimals = dot >= 0 ? int(format.size() - dot - 1) : 0;
    const QString name = CreateNumberTimeStyle(ods::number_Type::number_time, format, decimals, QString());
    v_time_data_styles.insert(format, name);
    return name;
}

CellStyle *Content::CellStyleWithDataStyle(CellStyle *base, const QString &dataStyle)
{
    const QString key = (base ? base->GetStyleName() : QString()) + QLatin1Char('|') + dataStyle;
    if (CellStyle *s = v_derived_styles.value(key))
        return s;
    CellStyle *style = base ? base->Derive() : CreateCellStyle();
    style->GetAttributes()->insert(xml_QualifiedName(ns::co_Style, ns::co_DataStyleName), dataStyle);
    v_derived_styles.insert(key, style);
    return style;
}

uint Content::NextCellStyleIndex() const
{
    uint next = 0;
    for (CellStyle *s : v_cellstyles)
        next = qMax(next, s->GetStyleIndex() + 1);
    return next;
}

void Content::initialize()
{
    //
}

void Content::Save(const QString &save_dir, QString &err)
{
    Q_UNUSED(save_dir);
    Q_UNUSED(err);
    qDebug() << "content Save";
}

}//ods
