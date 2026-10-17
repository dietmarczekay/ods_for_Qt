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
#include <QXmlStreamWriter>

#include "book.h"
#include "tag.h"
#include "text.h"

namespace ods {

TextDoc::TextDoc(Book *book, FileType type):
    Content(book, type)
{
    Initialize();
}

TextDoc::~TextDoc()
{
    qDeleteAll(v_textelements);     // tables and texts of a text document
    qDeleteAll(v_tablestyles);
    qDeleteAll(v_picturestyles);
}

void TextDoc::CorrectRowNumbers()
{
    for(int lo_int = 0; lo_int < v_tables.count(); lo_int++){
        v_tables[lo_int]->CorrectRowNumbers();
    }
}

ColumnStyle *TextDoc::CreateColumnStyle(Table *table)
{
    ColumnStyle *style = new ColumnStyle(v_columnstyles.count());
    style->SetName(table->GetName());
    v_columnstyles.append(style);
    return style;
}

void TextDoc::CreateHeadline(QString content, int level)
{
    Text *text = new Text(this, content);
    text->IsHeadLine(level);
    v_textelements.append(text);
}

Table *TextDoc::CreateTable(QString name)
{
    QString va_name = GetValidTableName(name);
    Table *table = new Table(this, va_name);
    v_textelements.append(table);
    return table;
}

Table *TextDoc::CreateTable(int position)
{
    QString va_name = GetValidTableName("Tabelle");
    Table *table = new Table(this, v_tables.count(), va_name);
    v_textelements.insert(position, table);
    return table;
}

Table *TextDoc::CreateTable(int position, QString name)
{
    Table *table = new Table(this, name);
    v_textelements.insert(position, table);
    return table;
}

TableStyle *TextDoc::CreateTableStyle(Table *table)
{
    TableStyle *style = new TableStyle(table);
    v_tablestyles.append(style);
    return style;
}


void TextDoc::CreateText(QString content)
{
    Text *text = new Text(this, content);
    v_textelements.append(text);
}

QString TextDoc::GetValidTableName(QString name)
{
#if QT_VERSION < 0x060000
    QRegExp exp("\\(([^\\]]+)\\)");
    int test = QString(exp.cap(1)).toInt();
#else
    QRegularExpression exp("\\(([^\\]]+)\\)");
    int test = exp.captureCount();
#endif
    if (test > 0){
        return QString("%1%2%3").arg("(").arg(test).arg(")");
    }else{
        return name;
    }
}

void TextDoc::Initialize()
{

}

void TextDoc::Save(const QString &save_dir, QString &err)
{
    foreach(Table *table, v_tables)
    {
        table->Prepare();
    }

    QSaveFile out(save_dir + QStringLiteral("/") + doc_Content);
    if (!out.open(QIODevice::WriteOnly)) {
        err = QStringLiteral("[TextDoc] cannot write %1").arg(out.fileName());
        return;
    }

    QXmlStreamWriter xml(&out);
    p_book->WriteStartDocument(xml);

    Tag *root_tag = new Tag(ns::co_Office, ns::co_DocContent);
    root_tag->AttributeAdd(co_OfficeUri);
    root_tag->AttributeAdd(co_StyleUri);
    root_tag->AttributeAdd(co_TextUri);
    root_tag->AttributeAdd(co_TableUri);
    root_tag->AttributeAdd(co_DrawUri);
    root_tag->AttributeAdd(co_FoUri);
    root_tag->AttributeAdd(co_XlinkUri);
    root_tag->AttributeAdd(co_DcUri);
    root_tag->AttributeAdd(co_MetaUri);
    root_tag->AttributeAdd(co_NumberUri);
    //root_tag->AttributeAdd(co_PresentationUri);
    root_tag->AttributeAdd(co_SvgUri);
    root_tag->AttributeAdd(co_ChartUri);
    root_tag->AttributeAdd(co_Dr3d);
    root_tag->AttributeAdd(co_MathUri);
    root_tag->AttributeAdd(co_FormUri);
    root_tag->AttributeAdd(co_ScriptUri);
    root_tag->AttributeAdd(co_OooUri);
    root_tag->AttributeAdd(co_OoowUri);
    root_tag->AttributeAdd(co_OoocUri);
    root_tag->AttributeAdd(co_Ddom);
    root_tag->AttributeAdd(co_Xforms);
    root_tag->AttributeAdd(co_XsdUri);
    root_tag->AttributeAdd(co_XsiUri);
    root_tag->AttributeAdd(co_RptUri);
    root_tag->AttributeAdd(co_OfUri);
    root_tag->AttributeAdd(co_Xhtml);
    root_tag->AttributeAdd(co_Grddl);
    root_tag->AttributeAdd(co_OfficeOoo);
    root_tag->AttributeAdd(co_TableoooUri);
    root_tag->AttributeAdd(co_DrawoooUri);
    root_tag->AttributeAdd(co_CalcExtUri);
    root_tag->AttributeAdd(co_LoExtUri);
    root_tag->AttributeAdd(co_Field);
    root_tag->AttributeAdd(co_FormX);
    root_tag->AttributeAdd(co_Css3tUri);
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Office,"version"), co_xml_version);

    Tag *script_tag = new Tag(ns::co_Office, ns::co_Script);
    root_tag->SubtagAdd(script_tag);

    Tag *font_face_decls_tag = new Tag(ns::co_Office, ns::co_FontFaceDecls);
    font_face_decls_tag->SubtagAdd(new Tag(ns::co_Style, ns::co_FontFace));
    font_face_decls_tag->GetSubTag(font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_Name, "Arial");
    font_face_decls_tag->GetSubTag(font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontFamily, "Arial");
    font_face_decls_tag->GetSubTag(font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontFamilyGen, "swiss");
    font_face_decls_tag->GetSubTag(font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontPitch, "variable");
    root_tag->SubtagAdd(font_face_decls_tag);

    Tag *automatic_styles_tag = new Tag(ns::co_Office, ns::co_AutomaticStyles);
    
    //in order writen to the file column - row - cell
    foreach(TableStyle *style, v_tablestyles)
    {
        automatic_styles_tag->SubtagAdd(style->Save());
    }

    foreach(ColumnStyle *style, v_columnstyles)
    {
        automatic_styles_tag->SubtagAdd(style->Save());
    }

    foreach(RowStyle *style, v_rowstyles)
    {
        automatic_styles_tag->SubtagAdd(style->Save());
    }

    foreach(CellStyle *style, v_cellstyles)
    {
        automatic_styles_tag->SubtagAdd(style->Save());
    }

    root_tag->SubtagAdd(automatic_styles_tag);

    Tag *body_tag = new Tag(ns::co_Office, ns::co_Body);

    Tag *text_tag = new Tag(ns::co_Office, ns::co_Text);
/*
    foreach(Table *table, v_tables)
    {
        text_tag->SubtagAdd(table->Save());
    }
*/
    foreach(DocElement *tag, v_textelements)
    {
        text_tag->SubtagAdd(tag->Save());
    }


    body_tag->SubtagAdd(text_tag);

    root_tag->SubtagAdd(body_tag);



    root_tag->Write(xml, err);
    delete root_tag;                        // the whole tree (was leaked on every save)

    if (!err.isEmpty())
    {
        out.cancelWriting();
    } else {
        xml.writeEndDocument();
        if (!out.commit())
            err = QStringLiteral("[Sheet] QSaveFile:commit() failed");
    }

}

}
