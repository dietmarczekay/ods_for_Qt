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

#include "functions.h"

namespace ods {

QString xml_QualifiedName(QString arg1, QString arg2)
{
    return QString("%1%2%3").arg(arg1, ":", arg2);
}

QString GetExcelColumnName(int columnNumber)
{
    int dividend = columnNumber;
    QString columnName = "";
    int modulo;

    while (dividend > 0)
    {
        modulo = (dividend - 1) % 26;
        columnName = QChar(65 + modulo).toLatin1() + columnName;
        dividend = (int)((dividend - modulo) / 26);
    }

    return columnName;
}

}
