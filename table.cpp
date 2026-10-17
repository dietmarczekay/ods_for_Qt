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

#include "table.h"

namespace ods {

Table::Table(Content *content, int index, QString name):
    DocElement(),
    p_content(content),
    v_name(name),
    //v_row_max_index(-1),
    v_table_index(index)
{
    // the name was overwritten with "Table" + GetTableCount(), which is always
    // 0 -> every numbered sheet was called "Table0" (duplicate sheet names)
    if (v_name.isEmpty())
        v_name = QStringLiteral("Tabelle%1").arg(index + 1);
    Initialize();
}

Table::Table(Content *content, QString name):
    p_content(content),
    v_name(name),
    v_table_index(0)
{
    Initialize();
}

Table::~Table()
{
    qDeleteAll(v_columns.begin(), v_columns.end());
    v_columns.clear();

    qDeleteAll(v_rows.begin(), v_rows.end());
    v_rows.clear();
}

void Table::CorrectRowNumbers()
{
    if (v_rows.count() > 0){
        v_rows[0]->SetRowNumber(0);
    }
    for (int lo_int = 1; lo_int < v_rows.count(); lo_int++) {
        v_rows[lo_int]->SetRowNumber(v_rows[lo_int - 1]->GetNextRowNumber());
    }
}

ods::Column *Table::CreateColumn()
{
    int index = v_columns.count() > 0 ? static_cast<int>(v_columns[v_columns.count() - 1]->GetColumnIndex()) : -1;
    ods::Column *v_column = new ods::Column(static_cast<uint>(index + 1));
    v_columns.append(v_column);
    return v_column;
}

ods::Column *Table::CreateColumn(uint index)
{
    return InsertColumn(index, 1);
}

ods::Row *Table::CreateRow()
{
    uint v_row_max_index = 0;
    if (v_rows.count() > 0)
  //      v_row_max_index = 1;
  //  else
        v_row_max_index = v_rows[v_rows.count() - 1]->GetNextRowNumber();
    //use rowindex of last row for the new row
    ods::Row *v_row = new ods::Row(this, v_row_max_index);
    v_rows.append(v_row);
    return v_row;
}

ods::Row *Table::CreateRow(uint index)
{
    return InsertRow(index);
}


ods::Column *Table::InsertColumn(uint index, uint count)
{
    int pos = 0;
    for (int counter = 0; counter < v_columns.count(); counter++)
    {
        if (index > v_columns[counter]->GetColumnIndex())
        {
            pos = counter + 1;
        }
        else
        {
            v_columns[counter]->IncColumnIndex(count);
        }
    }
    ods::Column *v_column = new ods::Column(index);
    v_columns.insert(pos, v_column);
    return v_column;
}

//move indicates, following rowidices are increased while inserting a row before
ods::Row *Table::InsertRow(uint index, bool move)
{
    //in any case, the row is created, following rows moved downwards

   // wenn Reihe eingefügt wird in Rowrepeated eine Reihe einfügen und den Style danach fortsetzen
    int pos = 0;
    int counter = 0;
    for (counter = 0; counter < v_rows.count(); counter++)
    {
        if (index > v_rows[counter]->GetRowNumber())
        {
            pos = counter + 1;
        }
        else
        {
            if (move)
                v_rows[counter]->IncRowIndex();
        }
    }

    //if following rows should be moved down, increase the row number

    ods::Row *v_row = new ods::Row(this, index);
    v_rows.insert(pos, v_row);

    //v_row_max_index = v_rows[v_rows.count() - 1]->GetRowNumber();
    return v_row;
}

void Table::Initialize()
{
    if ( v_name.isEmpty())
    {
        v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_Name), QString::number(v_table_index));
    }else
    {
         v_attributes.insert(xml_QualifiedName(ns::co_Table, ns::co_Name), v_name);
    }
}

Row *Table::GetRow(int index)
{
    if (index >= 0 && index < v_rows.count()){
        return v_rows[index];
    }
    return nullptr;
}

void Table::Prepare()
{

    // if index from first row is larger than zero, add row at zero
    ods::Row *row;
    if (v_rows.count() > 0 && v_rows[0]->GetRowNumber() > 0)
    {
        row = InsertRow(0, false);
        //row->AttributeAdd(ns::co_Table, ns::co_NumRowsRepeated, QString::number(v_rows[1]->GetRowNumber() - 1));
        row->SetNumberRowsRepeated(v_rows[1]->GetRowNumber());
    }

    uint last_row = 0;

    for (int i_row = 0; i_row < v_rows.count(); ++i_row){
        //qDebug() << (*i_row)->GetRowNumber();
        if(v_rows[i_row]->GetRowNumber() != last_row){
            ods::Row *new_row = new ods::Row(this, last_row);
            new_row->SetNumberRowsRepeated(v_rows[i_row]->GetRowNumber() - last_row - 1);
            v_rows.insert(i_row, new_row);
        }
        last_row = v_rows[i_row]->GetNextRowNumber();
    }

    CorrectRowNumbers();

    foreach(Row *row, v_rows)
    {
        row->Prepare();
    }
}

ods::Tag *Table::Save()
{

    ods::Tag *root_tag = new ods::Tag(ns::co_Table, ns::co_Table);
    if (!v_name.isEmpty()){
        root_tag->AttributeAdd(ns::co_Table, ns::co_Name, v_name);
    }

    if (v_table_style != nullptr){
        root_tag->AttributeAdd(ns::co_Table, ns::co_StyleName, v_table_style->GetStyleName());
    }
    ods::Column*column;

    if (v_columns.count() > 0 && v_columns[0]->GetColumnIndex() > 0)
    {
        ods::Column filler;
        filler.AttributeAdd(ns::co_Table, ns::co_NumColsRepeated, QString::number(v_columns[0]->GetColumnIndex()));
        root_tag->SubtagAdd(filler.Save());                 // was leaked
    }

    ods::Column *next_col;
    ods::Column *temp_col = nullptr;
    int counter;
    for(counter = 0; counter < v_columns.count() - 1; counter++)
    {
        column = v_columns[counter];
        next_col = v_columns[counter + 1];
        /* if there is a gap in the columns, marked by the columnindex
         * the gap has to be filled with an empty column
         * first the current column has to be inserted, after that the filling column
         */
        if ((next_col->GetColumnIndex() - column->GetColumnIndex()) > 1)
        {
            temp_col = new ods::Column();
            temp_col->AttributeAdd(ns::co_Table, ns::co_NumColsRepeated, QString::number(next_col->GetColumnIndex() - column->GetColumnIndex() - 1));
        }
        root_tag->SubtagAdd(column->Save());
        if (temp_col != nullptr)
        {
            root_tag->SubtagAdd(temp_col->Save());
            delete temp_col;
            temp_col = nullptr;
        }
    }

    if (v_columns.count() > 0)
    {
        root_tag->SubtagAdd(v_columns[counter]->Save());
    }

    for(counter = 0; counter < v_rows.count(); counter++)
    {
        root_tag->SubtagAdd(v_rows[counter]->Save());
    }

    return root_tag;
}

void Table::SetStyle(TableStyle *style)
{
    v_table_style = style;
}
}
