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

/*
 * the idea for this module is based in the QOds component by f35f22fan@gmail.com
 * one difference is, first the component collects all information and attributes in QMap lists
 * beginning with the writing, the elements trasfered to XML tags with their attributes.
 * This allows to change every element and it's attributes before writing them to XML files.
 * the book is the source for the complete document
 */

#include <QDebug>
#include <QDirIterator>
#include <QSaveFile>

#include "zipwriter.h"

#include "book.h"

namespace ods{

Book::Book(ods::FileType type):
    v_type(type)
{
    v_temp_dir.setAutoRemove(true);
    InitTempDir();
    Initialize();
}

Book::~Book()
{
    delete v_content;
    delete v_styles;
    delete v_manifest;
    delete v_rdf_manifest;
    delete v_meta;
}

SpreadSheet *Book::GetContentAsSpreadSheet()
{
    if (v_type == FileType::spreadsheet){
        return static_cast<SpreadSheet *>(v_content);
    }else{
        //qDebug() << "object is not a spreadsheet";
        return nullptr;
    }
}

void Book::Initialize()
{
    v_meta = new ods::Meta(this);
    v_manifest = new ods::Manifest(this, ns::co_Manifest);
    v_rdf_manifest = new ods::Manifest(this, ns::co_Rdf);
    switch(v_type){
    case ods::FileType::spreadsheet: v_content = new SpreadSheet(this, v_type); break;
    case ods::FileType::text: v_content = new TextDoc(this, v_type); break;
    }
    v_styles = new ods::Styles(this);
}

void Book::InitTempDir()
{
    if (!false)
    {
        v_temp_dir_path = v_temp_dir.path();
        return;
    }
    v_temp_dir_path = QDir(QDir::homePath()).filePath("file");
    QDir dir(v_temp_dir_path);
    if (!dir.mkpath(".")){
        //qDebug() << "Can't create temp dir";
    }
}

QString Book::Save(const QFile &target)
{
    QString save_dir = v_temp_dir_path;
    SaveMimeTypeFile(save_dir);
    QString err;

    if (v_content != nullptr)
        v_content->Save(save_dir, err);
    if (!err.isEmpty())
        return err;

    if (v_styles != nullptr)
        v_styles->Save(save_dir, err);
    if (!err.isEmpty())
        return err;

    if (v_manifest != nullptr)
        v_manifest->Save(save_dir, err);
    if (!err.isEmpty())
        return err;

    if (v_rdf_manifest != nullptr)
        v_rdf_manifest->Save(save_dir, err);
    if (!err.isEmpty())
        return err;

    if (v_meta != nullptr)
        v_meta->Save(save_dir, err);
    if (!err.isEmpty())
        return err;

    return WritePackage(target.fileName(), save_dir);
}

/*
 * ODF package (ODF 1.2 part 3, 3.3):
 *  - "mimetype" is the FIRST entry, STORED (not compressed), no extra field
 *  - no data descriptors: LibreOffice rejects stored entries whose sizes
 *    only follow the data (JlCompress::compressDir wrote them that way,
 *    the resulting file could not be opened)
 *  - no directory entries (not needed, the manifest lists the files)
 * Written with ZipWriter (zlib only).
 */
QString Book::WritePackage(const QString &odsPath, const QString &dir)
{
    QSaveFile out(odsPath);                 // atomic: no half-written file on error
    if (!out.open(QIODevice::WriteOnly))
        return QStringLiteral("Cannot create %1: %2").arg(odsPath, out.errorString());

    ZipWriter zip(&out);

    auto addFile = [&](const QString &relPath, bool store) -> QString {
        QFile in(QDir(dir).filePath(relPath));
        if (!in.open(QIODevice::ReadOnly))
            return QStringLiteral("Cannot read %1").arg(in.fileName());
        if (!zip.addFile(relPath, in.readAll(), store))
            return zip.errorString();
        return QString();
    };

    QString err = addFile(ods::doc_MimeType, true);

    QStringList files;
    QDirIterator it(dir, QDir::Files | QDir::Hidden, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        const QString rel = QDir(dir).relativeFilePath(it.next());
        if (rel != ods::doc_MimeType)
            files << rel;
    }
    files.sort();
    for (const QString &rel : std::as_const(files)) {
        if (!err.isEmpty())
            break;
        err = addFile(rel, false);
    }

    if (err.isEmpty() && !zip.finish())
        err = zip.errorString();

    if (!err.isEmpty()) {
        out.cancelWriting();
        return err;
    }
    if (!out.commit())
        return QStringLiteral("Cannot write %1: %2").arg(odsPath, out.errorString());
    return QString();
}

void Book::SaveMimeTypeFile(const QString &dir_path)
{
    QFile file(dir_path + QStringLiteral("/") + ods::doc_MimeType);
    file.open(QIODevice::WriteOnly);            // no Text mode: exact bytes, no line end
    QTextStream out(&file);
    switch (v_type){
    case FileType::spreadsheet: out << ods::co_MediaType_SpreadSheet; break;
    case FileType::text: out << ods::co_MediaType_Text; break;
    }
    file.close();
}

void Book::SetPageMargins(ods::Side side, double wide, ods::SizeType type)
{
    v_styles->SetPageMargin(side, wide, type);
}

void Book::SetPageOrientation(QPageLayout::Orientation orientation)
{
    v_styles->SetPagesOrientation(orientation);
}

void Book::WriteStartDocument(QXmlStreamWriter &xml)
{
    xml.setAutoFormatting(true);
    xml.setAutoFormattingIndent(-1);
    xml.writeStartDocument(QStringLiteral("1.0"), true);
}

}; //ods
