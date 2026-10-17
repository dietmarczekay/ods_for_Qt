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

#ifndef STYLES_H
#define STYLES_H

#include <QList>
#include <QMap>
#include <QPageLayout>

#include "book.h"

namespace ods {

class Styles
{
public:
    Styles(Book *p_book);
    ~Styles();

    void                        AppendNumberStyles();
    int                         AppendNumberCurrencyStyles();
    QString                     CreateNumberCurrencyStyle(number_Type type);
    QString                     CreateNumberTimeStyle(number_Type type, QString format);
    QString                     Save(const QString &save_dir, QString &err);
    void                        SetPageMargin(ods::Side side, double wide, ods::SizeType type);
    void                        SetPagesOrientation(QPageLayout::Orientation orientation);

private:
    void                        Initialize();
    Book                        *p_book;
    bool                        time_styles;
    QMap<QString, QString>      v_page_layout;
//    int                         v_number_index = 108;
    QList<NumberStyle *>        v_numberstyles;
};
}

#endif // STYLES_H
