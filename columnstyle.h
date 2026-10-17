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

#ifndef COLUMNSTYLE_H
#define COLUMNSTYLE_H

#include <QColor>
#include <QMap>
#include <QString>

#include "constants.h"

namespace ods {
class CellStyle;
class Tag;

class ColumnStyle
{
public:
    ColumnStyle(uint index);
    ~ColumnStyle();
    uint                    GetStyleIndex();
    QString                 GetStyleName();
    Tag                     *Save();
    void                    SetBackgroundColor(QColor color);
    void                    SetBreakAfter();
    void                    SetBreakBefore();
    void                    SetColumnWidth(float width, SizeType unit = SizeType::Mm);
    void                    SetName(QString name);

private:
    QMap<QString, QString>  v_attributes;
    uint                    v_style_index;
    QString                 v_style_name = "";
    QMap<QString, QString>  v_table_column_properties;

    void Initialize();
};
}
#endif // COLUMNSTYLE_H
