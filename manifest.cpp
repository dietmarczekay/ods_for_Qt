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

#include "book.h"
#include "constants.h"
#include "functions.h"

#include "manifest.h"

static const auto kMetaInf = "META-INF";

namespace ods {
Manifest::Manifest(ods::Book *p_book, QString type):
    p_book(p_book),
    v_type(type)
{
    if(v_type == ns::co_Manifest)
    {
        initialize_meta();
    }else if (v_type == ns::co_Rdf)
    {
        initialize_rdf();
    }

}
Manifest::~Manifest()
{
    delete root_tag;
}

void Manifest::initialize_meta()
{
    root_tag = new ods::Tag(v_type, ns::co_Manifest, co_ManifestUri);
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_Version), co_xml_version);
    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_FileEntry));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_FullPath), "/");
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_Version), co_xml_version);
    switch(p_book->GetFileType())
    {
        case FileType::spreadsheet: root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_SpreadSheet); break;
        case FileType::text:root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_Text); break;
    }

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_FileEntry));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_FullPath), doc_Content);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_text_xml);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_FileEntry));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_FullPath), doc_Styles);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_text_xml);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_FileEntry));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_FullPath), doc_Manifest_rdf);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_rdf_xml);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_FileEntry));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_FullPath), doc_Meta);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Manifest,ns::co_MediaType), co_MediaType_text_xml);

}

void Manifest::initialize_rdf()
{
    root_tag = new ods::Tag(v_type, ns::co_RDF, co_RdfUri);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_Description));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_About), doc_Styles);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(v_type, ns::co_Type));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTag(root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_Ressource), co_OdfUri.str_long + "StylesFile");

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_Description));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_About), "");
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(ns::co_Ns0, ns::co_HasPart, co_PkgUri));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTag(root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTagCount() - 1)->AttributeAdd(ns::co_Rdf, ns::co_Ressource, doc_Styles);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_Description));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_About), doc_Content);
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(v_type, ns::co_Type));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTag(root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_Ressource), co_OdfUri.str_long + "ContentFile");

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_Description));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_About), "");
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(ns::co_Ns0, ns::co_HasPart, co_PkgUri));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTag(root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTagCount() - 1)->AttributeAdd(ns::co_Rdf, ns::co_Ressource, doc_Content);

    root_tag->SubtagAdd(new ods::Tag(v_type, ns::co_Description));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_About), "");
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->SubtagAdd(new ods::Tag(v_type, ns::co_Type));
    root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTag(root_tag->GetSubTag(root_tag->GetSubTagCount() - 1)->GetSubTagCount() - 1)->AttributeAdd(xml_QualifiedName(ns::co_Rdf,ns::co_Ressource), co_PkgUri.str_long + "Document");
}

void Manifest::Save(const QString &save_dir, QString &err)
{
    if (root_tag == nullptr)
    {
        err = QStringLiteral("[Manifest::Save] root_ = nullptr");
        return;
    }
    QString kDirPath;
    if (v_type == ns::co_Manifest)
    {
        kDirPath = save_dir + QStringLiteral("/") + kMetaInf;
    }
    else if (v_type == ns::co_Rdf)
    {
        kDirPath = save_dir;
    }
    QDir dir(kDirPath);
    if (!dir.exists())
    {
        if (!dir.mkpath(".")) {
            v_error = QStringLiteral("[Manifest] mkdir failed");
            return;
        }
    }
    QString kFilePath;
    if (v_type == ns::co_Manifest)
    {
        kFilePath = kDirPath + QStringLiteral("/")
            + ods::doc_Manifest;
    }
    else if (v_type == ns::co_Rdf)
    {
        kFilePath = kDirPath + QStringLiteral("/")
            + ods::doc_Manifest_rdf;
    }

    QSaveFile out(kFilePath);
    if (!out.open(QIODevice::WriteOnly)) {
        err = QStringLiteral("[Manifest] cannot write %1").arg(kFilePath);
        return;
    }

    QXmlStreamWriter xml(&out);
    p_book->WriteStartDocument(xml);
    // package files (pictures) only for the manifest, added for this write
    // and removed again, so a second Save() does not duplicate them
    const int fixedEntries = root_tag->GetSubTagCount();
    if (v_type == ns::co_Manifest) {
        for (auto it = v_file_entries.cbegin(); it != v_file_entries.cend(); ++it) {
            auto *tag = new ods::Tag(ns::co_Manifest, ns::co_FileEntry);
            tag->AttributeAdd(xml_QualifiedName(ns::co_Manifest, ns::co_FullPath), it.key());
            tag->AttributeAdd(xml_QualifiedName(ns::co_Manifest, ns::co_MediaType), it.value());
            root_tag->SubtagAdd(tag);
        }
    }

    root_tag->Write(xml, err);
    root_tag->RemoveSubTags(fixedEntries);

    if (!err.isEmpty())
    {
        out.cancelWriting();
    } else {
        xml.writeEndDocument();
        if (!out.commit())
            err = QStringLiteral("[Manifest] QSaveFile:commit() failed");
    }
}
} //ods
