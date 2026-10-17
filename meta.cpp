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

#include "book.h"
#include "constants.h"
#include "functions.h"

#include "meta.h"

namespace ods {


Meta::Meta(ods::Book *p_book):
    p_book(p_book)
{
    initialize();
}

Meta::~Meta()
{

}

void Meta::initialize()
{
}

void Meta::Save(const QString &save_dir, QString &err)
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Office, ns::co_DocMeta);
    root_tag->AttributeAdd(co_OfficeUri);
    root_tag->AttributeAdd(co_XlinkUri);
    root_tag->AttributeAdd(co_DcUri);
    root_tag->AttributeAdd(co_MetaUri);
    root_tag->AttributeAdd(co_OooUri);
    root_tag->AttributeAdd(co_Grddl);
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Office,ns::co_Version), co_xml_version);

    root_tag->SubtagAdd(new ods::Tag(ns::co_Office, ns::co_Meta));
    ods::Tag *newTag = root_tag->GetSubTag(root_tag->GetSubTagCount() - 1);
    newTag->SubtagAdd(new ods::Tag(ns::co_Meta, ns::co_CreationDate));
    newTag->GetSubTag(newTag->GetSubTagCount() - 1)->TextAdd(QDate::currentDate().toString("yyyy-MM-dd") + "T"+QTime::currentTime().toString("hh:mm:ss.zzz"));

    newTag->SubtagAdd(new ods::Tag(ns::co_Dc, ns::co_Date));
    newTag->GetSubTag(newTag->GetSubTagCount() - 1)->TextAdd(QDate::currentDate().toString("yyyy-MM-dd") + "T"+QTime::currentTime().toString("hh:mm:ss.zzz"));

    newTag->SubtagAdd(new ods::Tag(ns::co_Meta, ns::co_EditingDuration));
    newTag->GetSubTag(newTag->GetSubTagCount() - 1)->TextAdd("PT0M20S");

    newTag->SubtagAdd(new ods::Tag(ns::co_Meta, ns::co_EditingCycles));
    newTag->GetSubTag(newTag->GetSubTagCount() - 1)->TextAdd("1");

    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(ns::co_Meta, ns::co_DocStatistic));

    newTag->SubtagAdd(new ods::Tag(ns::co_Meta, ns::co_Generator));
    newTag->GetSubTag(newTag->GetSubTagCount() - 1)->TextAdd("Regatta_org program");

    QDir dir(save_dir);
    if (!dir.exists())
    {
        if (!dir.mkpath(".")) {
            err = QStringLiteral("[Meta] mkdir failed");
            delete root_tag;
            return;
        }
    }

    QString kFilePath = save_dir + QStringLiteral("/")
            + ods::doc_Meta;

    QSaveFile out(kFilePath);
    if (!out.open(QIODevice::WriteOnly)) {
        err = QStringLiteral("[Meta] cannot write %1").arg(out.fileName());
        delete root_tag;
        return;
    }

    QXmlStreamWriter xml(&out);
    p_book->WriteStartDocument(xml);

    root_tag->Write(xml, err);
    delete root_tag;                        // the whole tree (was leaked on every save)

    if (!err.isEmpty())
    {
        out.cancelWriting();
    } else {
        xml.writeEndDocument();
        if (!out.commit())
            err = QStringLiteral("[Meta] QSaveFile:commit() failed");
    }
}

}
