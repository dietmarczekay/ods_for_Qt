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

#ifndef NUMBERSTYLE_H
#define NUMBERSTYLE_H

#include <QMap>
#include <QString>

#include "constants.h"

namespace ods {
class Book;
class Tag;

class NumberStyle
{
public:
    //NumberStyle(ods::Book *book, uint index, number_Type number);
    //NumberStyle(ods::Book *book, uint index, number_Type number, QString format);
    //NumberStyle(ods::Book *book, uint index, number_Type number, QString format, QString appendix);
    NumberStyle(uint index, number_Type number);
    NumberStyle(uint index, number_Type number, QString format, int decimalPlaces, QString prefix);
    NumberStyle(uint index, number_Type number, QString format, QString appendix);
    ~NumberStyle();

    void                    AddAttribute(QString xml, QString value);
    void                    AddMapAttribute(QString xml, QString value);
    void                    AddTextAttribute(QString xml, QString value);
    void                    AddText(QString value);
    uint                    GetStyleIndex() { return v_style_index; }
    QString                 GetStyleName();
    ods::Tag                *Save();

private:
    QString                 v_prefix;
    QString                 v_appendix;
    QMap<QString, QString>  v_attributes;
    QString                 v_format;
    int                     v_decimalPlaces;
    QMap<QString, QString>  v_map_attributes;
    ods::number_Type        v_number_style;
    uint                    v_style_index;
    QMap<QString, QString>  v_text_attributes;
    QString                 v_text;
    void                    Initialize();
};

}
#endif // NUMBERSTYLE_H
