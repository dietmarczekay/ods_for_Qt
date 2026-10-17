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

#ifndef ROWSTYLE_H
#define ROWSTYLE_H

#include <QColor>
#include <QMap>
#include <QString>

#include "constants.h"

namespace ods {
class CellStyle;
class Tag;

class RowStyle
{
public:
    RowStyle(uint index);
    ~RowStyle();

    QMap<QString, QString>  *GetFontProperties(){ return &v_font_properties; }
    uint                    GetStyleIndex();
    QString                 GetStyleName();
    Tag                     *Save();
    void                    SetBackgroundColor(QColor color);
    void                    SetBreakAfter();
    void                    SetBreakBefore();
    void                    SetFontSize(uint size, ods::SizeType unit);
    void                    SetRowHeight(uint size, ods::SizeType unit);
    void                    SetVAlign(Qt::Alignment align);

private:
    uint                    v_style_index;
    QMap<QString, QString>  attributes;
    QMap<QString, QString>  row_properties;
    QMap<QString, QString>  v_font_properties;

    void Initialize();
};
}
#endif // ROWSTYLE_H
