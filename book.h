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

#ifndef BOOK_H
#define BOOK_H

#include <QtCore>
#include <QPageLayout>

#include "ods.h"

namespace ods	{

class Content;
class Manifest;
class Meta;
class Styles;

class Book
{
public:
    explicit Book(FileType type);
    virtual ~Book();

    Content                 *GetContent(){ return v_content; }
    SpreadSheet             *GetContentAsSpreadSheet();
    FileType                GetFileType(){ return v_type; }
    Manifest                *GetManifest(){ return v_manifest; }
    Styles                  *GetStyleFile(){ return v_styles; }
    QString                 GetTempDir(){ return v_temp_dir_path; }
    int                     GetNumStyleNumber(){ v_numberStyles++; return v_numberStyles; }
    int                     NextObjectNumber(){ return ++v_objects; }   // unique draw:name
    QString                 Save(const QFile &targetOdsFile);

    void                    SetFileType(FileType type){ v_type = type; }
    void                    SetPageMargins(Side side, double wide, ods::SizeType type);
    void                    SetPageOrientation(QPageLayout::Orientation orientation);
    void                    WriteStartDocument(QXmlStreamWriter&);

private:
    void                    InitTempDir();
    void                    PrepareDir(const QString &save_dir, QString &err);
    void                    SaveMimeTypeFile(const QString &dir_path);
    QString                 WritePackage(const QString &odsPath, const QString &dir);

    FileType                v_type;
    QTemporaryDir           v_temp_dir;
    QString                 v_temp_dir_path;
    Content                 *v_content;
    Manifest                *v_manifest;
    Manifest                *v_rdf_manifest;
    Meta                    *v_meta;
    Styles                  *v_styles;
    int                     v_numberStyles = 107;
    int                     v_objects = 0;
    void                    Initialize();
signals:

public slots:
};
}
#endif // BOOK_H
