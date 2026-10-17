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

#ifndef CELLSTYLE_H
#define CELLSTYLE_H

#include <QColor>
#include <QFont>
#include <QMap>
#include <QString>

#include "constants.h"

namespace ods {

class Content;
class NumberStyle;
class Tag;

class CellStyle
{
public:
    CellStyle(Content *content, uint index);
    ~CellStyle();

    ods::CellStyle          *Derive();
    bool                    FontIsEqual(QMap<QString, QString> *font_property);
    QMap<QString, QString>  *GetAttributes() { return &v_attributes; }
    QString                 GetStyleName();
    uint                    GetStyleIndex() { return v_style_index; }
    QMap<QString, QString>  *GetTextProperties() { return &v_text_properties; }
    bool                    IsEqual(CellStyle *style);
    void                    RemoveCellProperties();
    void                    RemoveFontStyle();
    void                    RemoveParagraphProperties();
    Tag                     *Save();
    void                    SetBackgroundColor(QColor color);
    void                    SetBorder(float width, SizeType unit, Qt::PenStyle style, QColor color, Sides sides);
    void                    SetFontColor(QColor color);
    void                    SetFontSize(uint size, ods::SizeType unit);
    void                    SetFontSize(QString key, QString value);
    void                    SetFontStyle(QFont::Style style);
    void                    SetFontWeight(QFont::Weight weight);
    void                    SetHAlign(Qt::Alignment align);
    void                    SetNumberTimeStyle(QString format, int decimalPlaces = 0, QString prefix = "");
    void                    SetNumberMoneyStyle();
    void                    SetVAlign(Qt::Alignment align);
    void                    SetWordWrap(bool wrap);
    void                    SetStrikeThrough(bool striked);

private:
    CellStyle(const CellStyle& copy_t);
    Content                *p_content;
    QMap<QString, QString>  v_attributes;
    QMap<QString, QString>  v_paragraph_properties;
    uint                    v_style_index;
    QMap<QString, QString>  v_table_cell_properties;
    QMap<QString, QString>  v_text_properties;
    bool                    v_word_wrap;

    void                    Initialize();
    void                    SetStyleIndex(uint index);
};
}

#endif // CELLSTYLE_H
