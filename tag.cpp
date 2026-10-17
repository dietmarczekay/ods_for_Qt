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

#include "attribute.h"
#include "tag.h"

namespace ods{

Tag::Tag(QString type, QString name):
    v_name(name),
    v_type(type)
{
}

Tag::Tag(QString type, QString name, ods::XML namespaces):
    v_namespace(namespaces),
    v_name(name),
    v_type(type)
{
}

Tag::Tag(QString type, QString name, QString text):
    v_name(name),
    v_text(text),
    v_type(type)
{
}

Tag::~Tag()
{
    qDeleteAll(v_attributes);
    qDeleteAll(v_subtags);          // a tag owns its attributes and sub tags
}

void Tag::RemoveSubTags(int from)
{
    while (v_subtags.count() > from)
        delete v_subtags.takeLast();
}

void Tag::AttributeAdd(XML value)
{
    v_attributes.append(new Attribute(value));
}

void Tag::AttributeAdd(QString name, QString value)
{
    v_attributes.append(new Attribute(name, value));
}

void Tag::AttributeAdd(QString type, QString name, QString value)
{
    v_attributes.append(new Attribute(type, name, value));
}

ods::Attribute* Tag::GetAttribute(int index)
{
    if (index >= 0 && index < v_attributes.count())
    {
        return v_attributes[index];
    }
    return nullptr;
}

QString Tag::GetText()
{
    return v_text;
}

ods::Tag* Tag::GetSubTag(int index)
{
    if (index >= 0 && index < v_subtags.count())
    {
        return v_subtags[index];
    }
    return nullptr;
}

void Tag::SubtagAdd(Tag *tag)
{
    v_subtags.append(tag);
}

void Tag::TextAdd(QString text)
{
    v_text = text;
}

void Tag::Write(QXmlStreamWriter &xml, QString &err)
{
    xml.writeStartElement(v_type + ":" + v_name);

    if (!v_namespace.str.isEmpty())
        xml.writeAttribute("xmlns:" + v_type, v_namespace.str_long);

    foreach (Attribute *attribute, v_attributes)
    {
        attribute->write_full_string(xml);
    }

    foreach (Tag *tag, v_subtags)
    {
        if (tag != nullptr)
        {
            tag->Write(xml, err);
        }
    }

    if (!v_text.isEmpty())
    {
        xml.writeCharacters(v_text);
    }
    xml.writeEndElement();
}

}
