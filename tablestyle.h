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

#ifndef TABLESTYLE_H
#define TABLESTYLE_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QString>

#include "constants.h"

namespace ods {

class Table;
class Tag;

class TableStyle
{
public:
    TableStyle(Table *table);
    ~TableStyle();

    ods::TableStyle         *Derive();
    QString                 GetStyleName();
    ods::Tag                *Save();
    void                    SetBackgroundColor(QColor color);
    void                    SetBorder(float width, SizeType unit, Qt::PenStyle style, QColor color, Sides sides);
    void                    SetHAlign(Qt::Alignment align);
    void                    SetTableWidth(float width, SizeType unit);
    void                    SetVAlign(Qt::Alignment align);

private:
    TableStyle(const TableStyle& copy_t);
    Table                   *p_table;
    QMap<QString, QString>  v_table_properties;

    void Initialize();
};
}

#endif // TABLESTYLE_H
