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

#ifndef ROW_H
#define ROW_H

#include <QMap>

class QTime;

namespace ods {
class Cell;
class CellStyle;
class RowStyle;
class Table;
class Tag;

class Row
{
public:
    Row(Table *table);
    Row(Table *table, uint index);
    ~Row();

    void                        AttributeAdd(QString type, QString name, QString value);
    int                         CellCount() {return v_cells.count();}
    QMap<int, Cell*>            Cells(){return v_cells;}
    void                        CorrectColumnNumbers();
    Cell                        *CreateCell(uint index, uint count = 0);
    Cell                        *CreateCell(QString text);
    Cell                        *CreateCell(QString text, uint index);
    Cell                        *CreateCell(QTime time, QString format);
    Cell                        *CreateCell(QTime time, QString format, uint index);
    Cell                        *GetCell(uint index);
    uint                        GetNextColumnNumber();
    uint                        GetNextRowNumber();
    uint                        GetNumRowsRepeated(){ return v_number_rows_repeated; }
    uint                        GetRowNumber();
    uint                        GetRowAddressNumber();
    Table                       *GetParentTable(){ return p_table; }
    QString                     GetTableName();
    void                        IncRowIndex();
    Cell                        *InsertCell(uint index);
    void                        Prepare();
    Tag                         *Save();
    void                        SetCellStyle(CellStyle *style);
    void                        SetNumberRowsRepeated(uint index){ v_number_rows_repeated = index; }
    void                        SetRowNumber(uint index){ v_row_number = index; }
    void                        SetStyle(ods::RowStyle *style);

private:
    Table                       *p_table = nullptr;
    QMap<QString, QString>      v_attributes;
    QMap<int, Cell*>            v_cells;
    uint                        v_number_rows_repeated = 0;
    //uint                        v_row_index = 0;
    uint                        v_row_number = 0; //takes into account, number_rows_repeated of former rows increase the row number
    RowStyle                    *v_row_style = nullptr;

    void Initialize();
};
}
#endif // ROW_H
