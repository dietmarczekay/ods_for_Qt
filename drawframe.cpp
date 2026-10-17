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

#include <QCryptographicHash>
#include <QDir>
#include <QMimeDatabase>

#include "book.h"
#include "cell.h"
#include "constants.h"
#include "content.h"
#include "manifest.h"
#include "functions.h"
#include "row.h"
#include "table.h"
#include "tag.h"

#include "drawframe.h"
#include "iostream"

namespace ods {

DrawFrame::DrawFrame(ods::Cell *cell, QString filename, float offset_x, float offset_y, float width_x, float width_y):
    p_cell(cell),
    v_filename(filename),
    v_off_x(offset_x),
    v_off_y(offset_y),
    v_width_x(width_x),
    v_width_y(width_y)
{
    Initialize();
}

DrawFrame::~DrawFrame()
{
    // the cell owns the frame, not the other way round
}

void DrawFrame::Initialize()
{
}

ods::Tag *DrawFrame::Save()
{
    Book *book = p_cell->GetParentRow()->GetParentTable()->GetParentContent()->GetParentBook();
    QDir dir(book->GetTempDir() + QStringLiteral("/") + ods::doc_MediaDirName);
    if (!dir.exists() && !dir.mkpath(QStringLiteral(".")))
        qWarning() << "[DrawFrame] cannot create" << dir.path();

    // file name from the content: every picture gets its own file,
    // the same picture used twice is stored once ("test.png" for all before)
    QFile src(v_filename);
    QByteArray bytes;
    if (src.open(QIODevice::ReadOnly))
        bytes = src.readAll();
    else
        qWarning() << "[DrawFrame] cannot read" << v_filename;

    const QFileInfo info(v_filename);
    const QString hash = QString::fromLatin1(
        QCryptographicHash::hash(bytes, QCryptographicHash::Sha1).toHex().left(16));
    const QString fileName = hash + QStringLiteral(".") + info.suffix().toLower();

    QFile dst(dir.filePath(fileName));
    if (!dst.exists() && dst.open(QIODevice::WriteOnly))
        dst.write(bytes);

    const QString packagePath = ods::doc_MediaDirName + QStringLiteral("/") + fileName;

    ods::Tag *draw_image = new ods::Tag(ns::co_Draw, ns::co_Image);
    draw_image->AttributeAdd(xml_QualifiedName(ns::co_Xlink, ns::co_Href), packagePath);

    // manifest needs a MIME type ("image/png"), not the suffix ("png")
    book->GetManifest()->AddFileEntry(packagePath,
                                      QMimeDatabase().mimeTypeForFile(info).name());

    draw_image->AttributeAdd(xml_QualifiedName(ns::co_Xlink, ns::co_Type), ns::co_Simple);
    draw_image->AttributeAdd(xml_QualifiedName(ns::co_Xlink, ns::co_Show), ns::co_Embed);
    draw_image->AttributeAdd(xml_QualifiedName(ns::co_Xlink, ns::co_Actuate), ns::co_OnLoad);

    ods::Tag *root_tag = new ods::Tag(ns::co_Draw, ns::co_Frame);
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Draw, ns::co_Zindex), "0");
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Draw, ns::co_Name),
                           QStringLiteral("Bild %1").arg(book->NextObjectNumber()));
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Svg, ns::co_Width), QString::number(v_width_x, 'f', 3) + "mm");
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Svg, ns::co_Height), QString::number(v_width_y, 'f', 3) + "mm");
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Svg, ns::co_X), QString::number(v_off_x, 'f', 3) + "mm");
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Svg, ns::co_Y), QString::number(v_off_y, 'f', 3) + "mm");
    root_tag->SubtagAdd(draw_image);

    return root_tag;
}
}
