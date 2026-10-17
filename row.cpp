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

#include <QList>
#include <QTime>
#include "ods.h"

#include "row.h"

/* The row is the container for all cells in the row
 * While inserting a cell, all cells from the position to the right end are moved one
 * position to the right. Column index of the cell is increased.
 *
 */
namespace ods {

Row::Row(Table *table):
    p_table(table),
    v_row_number(0)
{
    Initialize();
}
Row::Row(Table *table, uint index):
    p_table(table),
    v_row_number(index)
{
    Initialize();
}

Row::~Row()
{
    qDeleteAll(v_cells.begin(), v_cells.end());
    v_cells.clear();
}

void Row::AttributeAdd(QString type, QString name, QString value)
{
    v_attributes.insert(xml_QualifiedName(type, name), value);
}

// Fill gaps between cells with empty (repeated) cells and resolve overlaps.
// The map is keyed by column number (1-based). Rebuilt instead of inserting
// while iterating (the old version modified v_cells during the iteration).
void Row::CorrectColumnNumbers()
{
    const QList<Cell *> cells = v_cells.values();     // sorted by column
    QMap<int, Cell *> result;
    uint expected = 1;

    for (Cell *cell : cells) {
        if (cell->GetColumnNumber() > expected) {
            // gap: one empty cell covering the missing columns
            auto *filler = new ods::Cell(this, expected);
            filler->SetNumColsRepeated(cell->GetColumnNumber() - expected);
            result.insert(static_cast<int>(expected), filler);
        } else if (cell->GetColumnNumber() < expected && !result.isEmpty()) {
            Cell *prev = result.last();
            if (prev->GetColumnSpan() > 1)
                cell->SetColumnNumber(static_cast<int>(expected));   // spanned: move behind
            else
                prev->SetNumColsRepeated(cell->GetColumnNumber() - prev->GetColumnNumber());
        }
        result.insert(static_cast<int>(cell->GetColumnNumber()), cell);
        expected = cell->GetNextColumnNumber();
    }
    v_cells = result;
}

ods::Cell *Row::CreateCell(QString text)
{
    uint index = GetNextColumnNumber();
    //the cell is created and appended in the current row
    ods::Cell *cell = new ods::Cell(this, text, index);
    v_cells.insert(static_cast<int>(index), cell);     // key = column number (was: count + 1)
    return cell;
}

Cell *Row::CreateCell(uint index, uint count)
{
    //the cell is created and inserted into the row, all cells from the position
    //to the end are moved one position to the right and the index incremented
    //int counter;
    //int pos = 0;
    //uint numcolsrepeated = 0;

    if (v_cells.contains(static_cast<int>(index))){
        return v_cells.value(static_cast<int>(index));
    }

    ods::Cell *cell = new ods::Cell(this, index);
    if (count != 0)
    {
        cell->SetNumColsRepeated(count);
    }
    v_cells.insert(static_cast<int>(index), cell);
    return cell;
}

ods::Cell *Row::CreateCell(QString text, uint index)
{
    ods::Cell *cell = CreateCell(index);
    cell->SetText(text);
    return cell;
}

ods::Cell *Row::CreateCell(QTime time, QString format)
{
    const uint index = GetNextColumnNumber();
    ods::Cell *cell = new ods::Cell(this, index);
    if (time.isValid())
        cell->SetValue(time, format);
    else
        cell->SetValue(QString());
    v_cells.insert(static_cast<int>(index), cell);
    return cell;
}

ods::Cell *Row::CreateCell(QTime time, QString format, uint index)
{
    //the cell is created and inserted into the row, all cells from the position
    //to the end are moved one position to the right and the index incremented
    ods::Cell *cell;

    if (v_cells.contains(static_cast<int>(index)))
    {
        cell = v_cells.value(static_cast<int>(index));
    }else{
        cell = new ods::Cell(this, index);
        v_cells.insert(static_cast<int>(index), cell);
    }

    if (time.isValid())
    {
        cell->SetValue(time, format);
    }
    else
    {
        cell->SetValue("");
    }
    return cell;
}

Cell *Row::GetCell(uint index)
{
    //createCell checks if cell exists and returns this cell
    return CreateCell(index);
}

uint Row::GetNextColumnNumber()
{
    if (v_cells.count() == 0){
        return 1;
    }
    //returns the next free column number from last inserted cells
    ods::Cell *cell =  v_cells.last();
    return cell->GetNextColumnNumber();
}

uint Row::GetNextRowNumber()
{
    //number_rows_repeated includes the current row, number_rows_repeated = 4 means for row 5, 5 and 6 and 7 and 8 are the same
    //so 5 + 4 returens correctly 9 as next number
    //if number_rows_repeated is zero, v_row_number has to be added by one to get the correct nex row number
    return (v_row_number + qMax(v_number_rows_repeated, static_cast<uint>(1)));
}

uint Row::GetRowNumber()
{
    return v_row_number;
}

uint Row::GetRowAddressNumber()
{
    //qDebug() << "R " << v_row_number + 1;
    return v_row_number + 1;
}

QString Row::GetTableName()
{
    return p_table->GetName();
}

void Row::IncRowIndex()
{
    v_row_number++;
}

void Row::Initialize()
{
    //adress of cells start with A1, so getRowAdressNumber gives the rownumber for formulas etc
}

// insert a cell at column index, cells from there on move one column right
ods::Cell *Row::InsertCell(uint index)
{
    QMap<int, Cell *> moved;
    for (auto it = v_cells.begin(); it != v_cells.end(); ++it) {
        Cell *c = it.value();
        if (c->GetColumnNumber() >= index)
            c->IncColumnNumber();
        moved.insert(static_cast<int>(c->GetColumnNumber()), c);
    }
    v_cells = moved;
    ods::Cell *cell = new ods::Cell(this, index);
    v_cells.insert(static_cast<int>(index), cell);      // key = column number (was: position)
    return cell;
}

void Row::Prepare()
{
    CorrectColumnNumbers();

    // number styles -> derived cell styles (must exist before styles are written)
    for (Cell *cell : std::as_const(v_cells))
        cell->PrepareStyle();

    CellStyle *dummy_style = new CellStyle(p_table->GetParentContent(),
                                           p_table->GetParentContent()->NextCellStyleIndex());

    if (v_row_style != nullptr){
        dummy_style->GetTextProperties()->insert(*v_row_style->GetFontProperties());
    }

    bool insert_in_sheet_styles = false;

    QMapIterator<int, Cell*> i_cell(v_cells);
    while(i_cell.hasNext())
    {
        i_cell.next();

        // row font only for cells without an own style (as it effectively
        // was: FontIsEqual() always returned false; a styled cell would lose
        // its background/borders if it got the dummy style)
        if (v_row_style != nullptr &&
                v_row_style->GetFontProperties()->count() > 0){
            if (i_cell.value()->GetCellStyle() == nullptr){
                i_cell.value()->SetStyle(dummy_style);
                insert_in_sheet_styles = true;
            }
        }
    }
    if (insert_in_sheet_styles){
        p_table->GetParentContent()->GetCellStyles()->append(dummy_style);
    } else {
        delete dummy_style;                 // was leaked for every row
    }
}

ods::Tag *Row::Save()
{
    //row and cell start with index one NOT zero
    Tag *root_tag = new Tag(ns::co_Table, ns::co_TableRow);

    QMapIterator<QString, QString> i(v_attributes);
    while(i.hasNext())
    {
        i.next();
        root_tag->AttributeAdd(i.key(), i.value());
    }

    if (v_number_rows_repeated > 0)
        root_tag->AttributeAdd(ns::co_Table, ns::co_NumRowsRepeated, QString::number(v_number_rows_repeated));

    QMapIterator<int, Cell*> i_cell(v_cells);
    while(i_cell.hasNext())
    {
        i_cell.next();
        root_tag->SubtagAdd(i_cell.value()->Save());
        if (i_cell.value()->GetColumnSpan() > 1){   // span 1 wrote repeated="0" (invalid)
            //TODO: CoveredTableCell muss an Anzahl der ColSpan angepasst werden
            Tag *tag = new Tag(ns::co_Table, ns::co_CoveredTableCell);
            tag->AttributeAdd(xml_QualifiedName(ns::co_Table, ns::co_NumColsRepeated), QString::number(i_cell.value()->GetColumnSpan() -1));
            root_tag->SubtagAdd(tag);
        }
    }
    return root_tag;
}

void Row::SetStyle(RowStyle *style)
{
    v_row_style = style;
    v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_StyleName), style->GetStyleName());
}
}

