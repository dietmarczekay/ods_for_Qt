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

#ifndef CELL_H
#define CELL_H

#include <QDateTime>
#include <QMap>
#include <QString>
#include <QLocale>

namespace ods {
class CellStyle;
class NumberStyle;
class DrawFrame;
class Row;
class Tag;

class Cell
{
public:
    Cell(Row *row);
    Cell(Row *row, uint index);
    Cell(Row *row, QString text);
    Cell(Row *row, QString text, uint index);
    Cell(Row *row, QTime time, QString format);
    Cell(Row *row, QTime time, QString format, uint index);
    ~Cell();
    void                        AddAttribute(QString type, QString name, QString value);
    void                        AddColSpan(uint count);
    void                        AddPicture(QString filename, float offset_x, float offset_y, float width_x, float width_y);
    void                        AddRowSpan(uint count);
    void                        DeleteAttribute(QString type, QString name);
    QString                     GetCellAdress();
    CellStyle                   *GetCellStyle() { return v_cell_style;}
    uint                        GetColumnNumber(){ return v_col_number;}
    uint                        GetColumnSpan(){ return v_col_span;}
    uint                        GetNumColsRepeated(){ return v_num_col_repeated; }
    uint                        GetNextColumnNumber();
    Row                         *GetParentRow(){ return p_row; }
    QString                     GetTableName();
    QString                     GetText(){  return v_text; }
    void                        IncColumnNumber();
    void                        Initialize();
    Tag                         *Save();
    void                        SetColumnNumber(int index);
    void                        SetCurrency(double value, QLocale locale);
    void                        SetFormula(QString formula, QString cell_value, ods::Cell *ref = nullptr);
    void                        SetNumColsRepeated(uint columns);
    void                        SetStyle(ods::CellStyle *style);
    void                        SetNumberStyle(ods::NumberStyle *style);
    void                        SetText(QString text);
    void                        SetValue(QString text);
    void                        SetValue(QDateTime datetime, QString format);
    void                        SetValue(QTime time, QString format);
    void                        SetValue(double value, int decimals = -1);   // number (office:value-type float)
    /** race time, may exceed 60 min; format of the automatic data style, e.g. "mm:ss.zz" */
    void                        SetDuration(qint64 ms, QString text, QString format = QStringLiteral("mm:ss.zzz"));
    void                        PrepareStyle();                              // before the styles are written

private:
    Cell(const Cell &) = delete;
    Cell &operator=(const Cell &) = delete;
    void                        ClearValue();

    Row                         *p_row = nullptr;
    QMap<QString, QString>      v_cell_properties;
    CellStyle                   *v_cell_style = nullptr;
    NumberStyle                 *v_number_style = nullptr;
    uint                        v_col_number;
    uint                        v_col_span;
    DrawFrame                   *v_draw_frame = nullptr;
    QString                     v_formula;
    Cell                        *v_formula_ref = nullptr;
    uint                        v_num_col_repeated;
    uint                        v_row_span;
    QString                     v_text;
    QString                     v_time_format;      // format of a time value (automatic data style)
};
}
#endif // CELL_H
