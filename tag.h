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

#ifndef TAG_H
#define TAG_H

#include <QXmlStreamWriter>
#include "constants.h"


namespace ods{
class Attribute;
//class NameSpaces;


class Tag
{
public:
    Tag(QString type, QString name);
    Tag(QString type, QString name, ods::XML namespaces);
    Tag(QString type, QString name, QString text);
    ~Tag();
    Tag(const Tag &) = delete;
    Tag &operator=(const Tag &) = delete;

    void                   AttributeAdd(XML value);
    void                   AttributeAdd(QString name, QString value);
    void                   AttributeAdd(QString type, QString name, QString value);
    Attribute*             GetAttribute(int index);
    Tag*                   GetSubTag(int index);
    int                    GetSubTagCount(){return v_subtags.count();}
    QString                GetText();
    void                   SubtagAdd(ods::Tag *tag);          // takes ownership
    void                   RemoveSubTags(int from);           // delete sub tags [from..end]
    void                   TextAdd(QString text);

    void                   Write(QXmlStreamWriter &xml, QString &err);

private:
    QVector<Attribute*>    v_attributes;
    ods::XML               v_namespace;
    QString                v_name;
    QVector<Tag*>          v_subtags;
    QString                v_text;
    QString                v_type;
};
}
#endif // TAG_H
