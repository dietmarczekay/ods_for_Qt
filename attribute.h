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

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QString>
#include <QXmlStreamWriter>

#include "constants.h"

namespace ods{

class Attribute
{
public:
    Attribute(QString parent_tag, QString name, QString value);
    Attribute(QString name, QString value);
    Attribute(XML value);
    ~Attribute();
    QString getName() {return v_qualifiedname;}

    void            write_full_string(QXmlStreamWriter &xml);
private:
    QString         v_qualifiedname;
    QString         v_value;
};
}
#endif // ATTRIBUTE_H
