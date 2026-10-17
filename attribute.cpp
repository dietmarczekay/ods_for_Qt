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

#include "iostream"
#include "attribute.h"

namespace ods{

Attribute::Attribute(QString parent_tag, QString name, QString value):
    v_qualifiedname(parent_tag + ":" + name),
    v_value(value)
{

}

Attribute::Attribute(QString name, QString value):
    v_qualifiedname(name),
    v_value(value)
{

}

Attribute::Attribute(XML value):
    v_qualifiedname("xmlns:" + value.str),
    v_value(value.str_long)
{

}

Attribute::~Attribute()
{

}

void Attribute::write_full_string(QXmlStreamWriter &xml)
{
    xml.writeAttribute(v_qualifiedname, v_value);
}

}
