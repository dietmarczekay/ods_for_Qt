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

#ifndef TEXT_H
#define TEXT_H

#include <QList>
#include <QMap>
#include <QString>

#include "docelement.h"

namespace ods {

class Content;
class DocElement;
class Tag;

class Text: public DocElement
{
public:
    Text(Content *content);
    Text(Content *content, QString text);
    ~Text();

    void                    IsHeadLine(int level);
    Tag                     *Save();

protected:
    Content                 *p_content;
    QMap<QString, QString>  v_attributes;
    bool                    v_isheader = false;
    QString                 v_text;

    void Initialize();

};
}
#endif // TEXT_H
