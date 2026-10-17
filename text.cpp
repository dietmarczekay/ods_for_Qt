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

#include <QSaveFile>

#include "constants.h"
#include "functions.h"
#include "tag.h"
#include "text.h"

namespace ods {

Text::Text(Content *content):
    p_content(content)
{
    Initialize();
}
Text::Text(Content *content, QString text):
    p_content(content),
    v_text(text)
{
    Initialize();
}

Text::~Text()
{
    //
}

void Text::Initialize()
{
    //
}

void Text::IsHeadLine(int level)
{
    if (level > 0){
        v_isheader = true;
        v_attributes.insert(xml_QualifiedName(ns::co_Text, ns::co_OutlineLevel), QString("%1").arg(level));
    }else{
        v_attributes.remove(xml_QualifiedName(ns::co_Text, ns::co_OutlineLevel));
    }
}

Tag *Text::Save()
{
    Tag *root_tag;
    if (v_isheader){
        root_tag = new Tag(ns::co_Text, ns::co_H);
    }else{
        root_tag = new Tag(ns::co_Text, ns::co_P);
    }

    QMapIterator<QString, QString> i(v_attributes);
    while(i.hasNext())
    {
        i.next();
        root_tag->AttributeAdd(i.key(), i.value());
    }

    if (!v_text.isEmpty()){
        root_tag->TextAdd(v_text);
    }

    return root_tag;
}

}
