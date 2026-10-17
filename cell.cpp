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

#include "ods.h"

#include "cell.h"

namespace ods {

Cell::Cell(Row *row):
    p_row(row)
{
    v_col_number = p_row->GetNextColumnNumber();
    Initialize();
}

Cell::Cell(Row *row, uint index):
    p_row(row),
    v_col_number(index)
{
    Initialize();
}

Cell::Cell(Row *row, QString text):
    p_row(row),
    v_text(text)
{
    // no index given: next free column of the row (columns start at 1)
    v_col_number = p_row->GetNextColumnNumber();
    Initialize();
}

Cell::Cell(Row *row, QString text, uint index):
    p_row(row),
    v_col_number(index),
    v_text(text)
{
    Initialize();
}

Cell::Cell(Row *row, QTime time, QString format):
    p_row(row)
{
    // was: Cells()[count - 1] on a copy of the map -> inserted nullptr and
    // dereferenced it (crash on every call)
    v_col_number = p_row->GetNextColumnNumber();
    Initialize();
    SetValue(time, format);
}

Cell::Cell(Row *row, QTime time, QString format, uint index):
    p_row(row),
    v_col_number(index)
{
    Initialize();
    SetValue(time, format);
}

Cell::~Cell()
{
    delete v_draw_frame;
}

void Cell::AddAttribute(QString type, QString name, QString value)
{
    v_cell_properties.insert(xml_QualifiedName(type, name), value);
}

void Cell::AddColSpan(uint count)
{
    v_col_span = count;
    if (v_row_span == 0){
        v_row_span = 1;
    }
}

void Cell::AddPicture(QString filename, float offset_x, float offset_y, float width_x, float width_y)
{
    delete v_draw_frame;
    v_draw_frame = new ods::DrawFrame(this, filename, offset_x, offset_y, width_x, width_y);
}

void Cell::AddRowSpan(uint count)
{
    v_row_span = count;
    if (v_col_span == 0){
        v_col_span = 1;
    }
}

void Cell::DeleteAttribute(QString type, QString name)
{
    v_cell_properties.remove(xml_QualifiedName(type, name));
}

QString Cell::GetCellAdress()
{
    //qDebug() << QString("'%1'.%2%3").arg(GetParentRow()->GetTableName()).arg(GetExcelColumnName(v_col_number)).arg(GetParentRow()->GetRowNumber());
    return QString("'%1'.%2%3").arg(GetParentRow()->GetTableName(), GetExcelColumnName(v_col_number)).arg( GetParentRow()->GetRowAddressNumber());
}

uint Cell::GetNextColumnNumber()
{
    // if cell is merged, a number_columns_repeated is not allowed
    // but in minimum a one has to be added
    if (v_col_span < 2){
        return v_col_number + qMax((uint)1, v_num_col_repeated);
    }else{
        return v_col_number + qMax((uint)1, v_col_span);
    }
}

QString Cell::GetTableName()
{ return p_row->GetTableName(); }

void Cell::IncColumnNumber()
{
    v_col_number++;
}

void Cell::Initialize()
{
    v_col_span = 0;
    v_formula = "";
    v_num_col_repeated= 0;
    v_row_span = 0;
}

Tag *Cell::Save()
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Table, ns::co_TableCell);

    if (!v_text.isEmpty())
    {
        // ODF collapses whitespace in text:p: a line break became a space and
        // "a  b" became "a b". One paragraph per line, repeated spaces as
        // no-break spaces (displayed the same, not collapsed).
        static const QRegularExpression spaces(QStringLiteral(" {2,}"));
        const QStringList lines = v_text.split(QLatin1Char('\n'));
        for (QString line : lines) {
            line.remove(QLatin1Char('\r'));
            QRegularExpressionMatchIterator it = spaces.globalMatch(line);
            QString out;
            int last = 0;
            while (it.hasNext()) {
                const auto m = it.next();
                out += line.mid(last, m.capturedStart() - last) + QLatin1Char(' ')
                       + QString(m.capturedLength() - 1, QChar(0x00A0));
                last = m.capturedEnd();
            }
            out += line.mid(last);
            root_tag->SubtagAdd(new ods::Tag(ns::co_Text, ns::co_P, out));
        }
    }

    if (v_num_col_repeated > 0){
        root_tag->AttributeAdd(ns::co_Table, ns::co_NumColsRepeated, QString::number(v_num_col_repeated));
    }

    if (v_col_span > 0){
        root_tag->AttributeAdd(ns::co_Table, ns::co_NumColsSpanned, QString::number(v_col_span));
    }

    if (!v_formula.isEmpty()){
        root_tag->AttributeAdd(ns::co_Table, ns::co_Formula,
                               v_formula_ref ? QString(v_formula).arg(v_formula_ref->GetCellAdress())
                                             : v_formula);
    }

    if (v_row_span > 0){
        root_tag->AttributeAdd(ns::co_Table, ns::co_NumRowsSpanned, QString::number(v_row_span));
    }

    QMapIterator<QString, QString> i(v_cell_properties);
    while(i.hasNext())
    {
        i.next();
        root_tag->AttributeAdd(i.key(), i.value());
    }

    // table:style-name must name a table-cell style. A number style is
    // attached through the cell style's style:data-style-name (see
    // PrepareStyle()); writing both produced a duplicate attribute.
    if (v_cell_style != nullptr)
    {
        root_tag->AttributeAdd(ns::co_Table, ns::co_StyleName, v_cell_style->GetStyleName());
    }

    if (v_draw_frame != nullptr)
    {
        root_tag->SubtagAdd(v_draw_frame->Save());
    }

    return root_tag;
}

void Cell::PrepareStyle()
{
    Content *content = p_row->GetParentTable()->GetParentContent();
    const QString dataKey = xml_QualifiedName(ns::co_Style, ns::co_DataStyleName);

    QString dataStyle;
    if (v_number_style != nullptr) {
        dataStyle = v_number_style->GetStyleName();
        v_number_style = nullptr;           // done, a second Save() must not derive again
    } else if (!v_time_format.isEmpty()
               && (v_cell_style == nullptr || !v_cell_style->GetAttributes()->contains(dataKey))) {
        // time value without time format: LibreOffice would show the raw
        // day fraction (0.00492...) instead of 07:05.500
        dataStyle = content->TimeDataStyle(v_time_format);
    }
    if (dataStyle.isEmpty())
        return;

    // derived (not changed): other cells may share the base style
    v_cell_style = content->CellStyleWithDataStyle(v_cell_style, dataStyle);
}

void Cell::ClearValue()
{
    v_time_format.clear();
    for (const char *attr : {"value", "date-value", "time-value", "boolean-value",
                             "string-value", "currency", "value-type"})
        v_cell_properties.remove(xml_QualifiedName(ns::co_Office, QString::fromLatin1(attr)));
    v_cell_properties.remove(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType));
}

void Cell::SetValue(double value, int decimals)
{
    ClearValue();
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), QStringLiteral("float"));
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), QStringLiteral("float"));
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_Value), QString::number(value, 'g', 15));
    v_text = decimals < 0 ? QLocale().toString(value) : QLocale().toString(value, 'f', decimals);
}

void Cell::SetColumnNumber(int index)
{
    v_col_number = index;
}

void Cell::SetCurrency(double value, QLocale locale)
{
    ClearValue();
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_Currency);
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_Currency), locale.currencySymbol(QLocale::CurrencyIsoCode));
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_Value), QString::number(value, 'f', 3));
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), ns::co_Currency);

    v_text = locale.toCurrencyString(value);
}

void Cell::SetFormula(QString formula, QString cell_value, Cell *ref)
{
    v_formula = formula;
    v_formula_ref = ref;
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_String);
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), ns::co_String);
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_StringValue), cell_value);
    v_text = cell_value;
}

void Cell::SetNumColsRepeated(uint columns)
{
    v_num_col_repeated = columns;
}

void Cell::SetStyle(CellStyle *style)
{
    v_cell_style = style;
}

void Cell::SetNumberStyle(NumberStyle *style)
{
    v_number_style = style;
}

void Cell::SetText(QString text)
{
    ClearValue();
    v_text = text;
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_String);
}

void Cell::SetValue(QString text)
{
    ClearValue();
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_String);
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), ns::co_String);
    v_text = text;
}

void Cell::SetValue(QDateTime datetime, QString format)
{
    // a date/time is value-type "date" with office:date-value (xsd:dateTime);
    // before: value-type time with an unreadable time-value
    ClearValue();
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_Date);
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), ns::co_Date);
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_DateValue),
                             datetime.toString(QStringLiteral("yyyy-MM-ddTHH:mm:ss.zzz")));

    v_text = datetime.toString(format);
}

void Cell::SetValue(QTime time, QString format)
{
    SetDuration(QTime(0, 0).msecsTo(time), time.toString(format));
    v_time_format = format;                 // for an automatic time format
}

void Cell::SetDuration(qint64 ms, QString text, QString format)
{
    // xsd:duration "PT0H14M10.123S": milliseconds were dropped before
    // (14:10.123 was stored as 14:10), hours may exceed 24
    ClearValue();
    const bool neg = ms < 0;
    const qint64 a = neg ? -ms : ms;
    const QString value = QStringLiteral("%1PT%2H%3M%4.%5S")
                              .arg(neg ? QStringLiteral("-") : QString())
                              .arg(a / 3600000)
                              .arg((a / 60000) % 60, 2, 10, QLatin1Char('0'))
                              .arg((a / 1000) % 60, 2, 10, QLatin1Char('0'))
                              .arg(a % 1000, 3, 10, QLatin1Char('0'));
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_ValueType), ns::co_Time);
    v_cell_properties.insert(xml_QualifiedName(ns::co_CalcExt, ns::co_ValueType), ns::co_Time);
    v_cell_properties.insert(xml_QualifiedName(ns::co_Office, ns::co_TimeValue), value);
    v_text = text;
    // minutes are not truncated at 60 (truncate-on-overflow=false), so
    // "mm:ss.zzz" also shows 75:03.123; SetValue(QTime, format) replaces it
    v_time_format = format;
}

}//ods
