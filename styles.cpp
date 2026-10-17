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

#include <QPageLayout>
#include <QPageSize>

#include "styles.h"

namespace ods {

Styles::Styles(Book *p_book):
    p_book(p_book),
    time_styles(false)
{
    Initialize();
}

Styles::~Styles()
{
    qDeleteAll(v_numberstyles);
}

void Styles::Initialize()
{
}

void Styles::AppendNumberStyles()
{
    ods::NumberStyle *style = new ods::NumberStyle(p_book->GetNumStyleNumber(), ods::number_Type::number_style);
    v_numberstyles.append(style);
}

int Styles::AppendNumberCurrencyStyles()
{
    int v_number_index = p_book->GetNumStyleNumber();
    ods::NumberStyle *style = new ods::NumberStyle(v_number_index, ods::number_Type::number_style);
    v_numberstyles.append(style);
    return v_number_index;
}

QString Styles::CreateNumberCurrencyStyle(ods::number_Type type)
{
    ods::NumberStyle *style = nullptr;
    int v_number_index = p_book->GetNumStyleNumber();
    style = new ods::NumberStyle(p_book->GetNumStyleNumber(), type);

    style->AddMapAttribute(xml_QualifiedName(ns::co_Style, ns::co_Condition), QString("value()>=0"));
    style->AddMapAttribute(xml_QualifiedName(ns::co_Style, ns::co_ApplyStyleName), QString("%1%2%3").arg("N").arg(v_number_index).arg("P0"));

    style->AddTextAttribute(xml_QualifiedName(ns::co_Fo, ns::co_Color), "#ff0000");

    style->AddText("-");

    v_numberstyles.append(style);

    style = new ods::NumberStyle(v_number_index, type, "", "P0");
    style->AddAttribute(xml_QualifiedName(ns::co_Style, ns::co_Volatile), "true");
    v_numberstyles.append(style);
    //++v_number_index;
    return style->GetStyleName();
}

QString Styles::CreateNumberTimeStyle(ods::number_Type type, QString format)
{
    ods::NumberStyle *style = nullptr;
    style = new ods::NumberStyle(p_book->GetNumStyleNumber(), type, format, 0);
    v_numberstyles.append(style);
    return style->GetStyleName();
}

QString Styles::Save(const QString &save_dir, QString &err)
{
    ods::Tag *root_tag = new ods::Tag(ns::co_Office, ns::co_DocStyles);
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
    root_tag->AttributeAdd(co_PresentationUri);
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
    root_tag->AttributeAdd(co_TableoooUri);
    root_tag->AttributeAdd(co_DrawoooUri);
    root_tag->AttributeAdd(co_CalcExtUri);
    root_tag->AttributeAdd(co_LoExtUri);
    root_tag->AttributeAdd(co_FormX);
    root_tag->AttributeAdd(co_Field);
    root_tag->AttributeAdd(co_Css3tUri);
    root_tag->AttributeAdd(xml_QualifiedName(ns::co_Office, ns::co_Version), co_xml_version);

    ods::Tag *v_font_face_decls_tag = new ods::Tag(ns::co_Office, ns::co_FontFaceDecls);
    v_font_face_decls_tag->SubtagAdd(new ods::Tag(ns::co_Style, ns::co_FontFace));
    v_font_face_decls_tag->GetSubTag(v_font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_Name, "Arial");
    v_font_face_decls_tag->GetSubTag(v_font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontFamily, "Arial");
    v_font_face_decls_tag->GetSubTag(v_font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontFamilyGen, "swiss");
    v_font_face_decls_tag->GetSubTag(v_font_face_decls_tag->GetSubTagCount() - 1)->AttributeAdd(ns::co_Style, ns::co_FontPitch, "variable");
    root_tag->SubtagAdd(v_font_face_decls_tag);

    ods::Tag *v_styles_tag = new ods::Tag(ns::co_Office, ns::co_Styles);
/*
    if (time_styles)
    {
        ods::Tag *tag = new ods::Tag(ns::co_Number, ns::co_TimeStyle);
        tag->AttributeAdd(ns::co_Style, ns::co_Name, "N109");
        tag->SubtagAdd(new ods::Tag(ns::co_Number, ns::co_Minutes));
        tag->GetSubTag(0)->AttributeAdd(ns::co_Number, ns::co_Style, "long");
        tag->SubtagAdd(new ods::Tag(ns::co_Number, ns::co_Text));
        tag->GetSubTag(1)->TextAdd(":");
        tag->SubtagAdd(new ods::Tag(ns::co_Number, ns::co_Seconds));
        tag->GetSubTag(2)->AttributeAdd(ns::co_Number, ns::co_Style, "long");
        v_styles_tag->SubtagAdd(tag);
    }
*/
    foreach(NumberStyle *style, *p_book->GetContent()->GetNumberStyles())
    {
        v_styles_tag->SubtagAdd(style->Save());
    }

    root_tag->SubtagAdd(v_styles_tag);

    QDir dir(save_dir);
    if (!dir.exists())
    {
        if (!dir.mkpath(".")) {
            return QStringLiteral("[Style] mkdir failed");
        }
    }

    QString kFilePath = save_dir + QStringLiteral("/")
            + ods::doc_Styles;

    QSaveFile out(kFilePath);
    if (!out.open(QIODevice::WriteOnly)) {
        err = QStringLiteral("[Styles] cannot write %1").arg(out.fileName());
        delete root_tag;
        return QString();
    }

    QXmlStreamWriter xml(&out);
    p_book->WriteStartDocument(xml);

    foreach(ods::NumberStyle *style, v_numberstyles)
    {
        v_styles_tag->SubtagAdd(style->Save());
    }

    ods::Tag *office_automatic_style = new ods::Tag(ns::co_Office, ns::co_AutomaticStyles);
    {   // always written: the master page refers to "Mpm1"
        ods::Tag *page_layout = new ods::Tag(ns::co_Style, ns::co_PageLayout);
        page_layout->AttributeAdd(ns::co_Style, ns::co_Name, "Mpm1");
        ods::Tag *page_layout_property = new ods::Tag(ns::co_Style, ns::co_PageLayoutProperties);

        QMapIterator<QString, QString> a(v_page_layout);
        while(a.hasNext())
        {
            a.next();
            page_layout_property->AttributeAdd(a.key(), a.value());
        }
        page_layout->SubtagAdd(page_layout_property);
        office_automatic_style->SubtagAdd(page_layout);
    }
    root_tag->SubtagAdd(office_automatic_style);

    ods::Tag *master_style = new ods::Tag(ns::co_Office, ns::co_MasterStyles);
    ods::Tag *master_page = new ods::Tag(ns::co_Style, ns::co_MasterPage);
    master_page->AttributeAdd(ns::co_Style, ns::co_Name, "Default");
    master_page->AttributeAdd(ns::co_Style, ns::co_PageLayoutName, "Mpm1");
    master_style->SubtagAdd(master_page);

    root_tag->SubtagAdd(master_style);

    root_tag->Write(xml, err);
    delete root_tag;                        // the whole tree (was leaked on every save)

    if (!err.isEmpty())
    {
        out.cancelWriting();
    } else {
        xml.writeEndDocument();
        if (!out.commit())
            err = QStringLiteral("[Styles] QSaveFile:commit() failed");
    }
    return err;
}

void Styles::SetPageMargin(ods::Side side, double wide, ods::SizeType type)
{
    QString wide_type = "";
    switch (type){
    case ods::SizeType::Pt: wide_type = "pt"; break;
    case ods::SizeType::In: wide_type = "in"; break;
    case ods::SizeType::Cm: wide_type = "cm"; break;
    case ods::SizeType::Mm: wide_type = "mm"; break;
    default: wide_type = ""; break;
    }

    switch (side){
    case ods::Side::Bottom: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_MarginBottom), QString::number(wide) + wide_type); break;
    case ods::Side::Left: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_MarginLeft), QString::number(wide) + wide_type); break;
    case ods::Side::Right: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_MarginRight), QString::number(wide) + wide_type); break;
    case ods::Side::Top: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_MarginTop), QString::number(wide) + wide_type); break;
    }
}

void Styles::SetPagesOrientation(QPageLayout::Orientation orientation)
{
    // QPageSize has no orientation: width/height stayed portrait (210x297)
    // with "landscape". QPageLayout gives the oriented size; no QPrinter
    // (PrintSupport is not available in WebAssembly).
    const QPageLayout layout(QPageSize(QPageSize::A4), orientation, QMarginsF());
    const QRectF page = layout.fullRect(QPageLayout::Millimeter);
    switch (orientation){
    case QPageLayout::Landscape: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Style, ns::co_PrintOrientation), "landscape"); break;
    case QPageLayout::Portrait: v_page_layout.insert(ods::xml_QualifiedName(ns::co_Style, ns::co_PrintOrientation), "portrait"); break;
    }
    v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_PageWidth), QString::number(page.width()) + "mm");
    v_page_layout.insert(ods::xml_QualifiedName(ns::co_Fo, ns::co_PageHeight), QString::number(page.height()) + "mm");
}

}
