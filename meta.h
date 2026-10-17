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

#ifndef META_H
#define META_H

#include "tag.h"

namespace ods{
class Book;


class Meta
{
public:
    Meta(Book *p_book);
    ~Meta();

    void            Save(const QString &save_dir, QString &err);
private:
    Book            *p_book;
    QString         v_type;
    QString			v_error;

    void initialize();
};
}
#endif // META_H
