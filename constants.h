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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QFlags>
#include <QString>

#ifndef con_ods
#define con_ods const char * const
#endif


namespace ods		{ // ods::
/*
enum class IndexType : quint8
{
    cell,
    column,
    row
};
*/
enum class FileType : quint8
{
    spreadsheet = 0x1,
    text = 0x2
};

enum class Side
{
    Left = 0x1,
    Top = 0x2,
    Bottom = 0x4,
    Right = 0x8
};

Q_DECLARE_FLAGS(Sides, Side);
Q_DECLARE_OPERATORS_FOR_FLAGS(Sides);

enum class SizeType : quint8
{
    Pt,
    In,
    Cm,
    Mm,
    NotSet
};

enum class StyleFile :quint8
{
    styleFile,
    contentFile
};

enum class number_Type : quint8
{
    number_style,
    number_currency,
    number_time
};

con_ods doc_Content         = "content.xml";
con_ods doc_MediaDirName	= "Pictures";
con_ods doc_Manifest		= "manifest.xml";
con_ods doc_Manifest_rdf	= "manifest.rdf";
con_ods doc_Meta			= "meta.xml";
con_ods doc_MimeType		= "mimetype";
con_ods doc_Settings		= "settings.xml";
con_ods doc_Styles          = "styles.xml";

con_ods co_MediaType_SpreadSheet  = "application/vnd.oasis.opendocument.spreadsheet";
con_ods co_MediaType_Text  = "application/vnd.oasis.opendocument.text";
con_ods co_MediaType_text_xml  = "text/xml";
con_ods co_MediaType_rdf_xml  = "application/rdf+xml";

con_ods co_xml_version    = "1.2";

struct XML
{
    const QString	str;
    const QString   str_long;
};

const XML co_AnimationUri { "anim",
            "urn:oasis:names:tc:opendocument:xmlns:animation:1.0" };
const XML co_CalcExtUri { "calcext",
            "urn:org:documentfoundation:names:experimental:calc:xmlns:calcext:1.0" };
const XML co_ChartUri { "chart",
            "urn:oasis:names:tc:opendocument:xmlns:chart:1.0" };
const XML co_ConfigUri { "config",
            "urn:oasis:names:tc:opendocument:xmlns:config:1.0" };
const XML co_Css3tUri { "css3t",
            "http://www.w3.org/TR/css3-text/" };
const XML co_DatabaseUri { "database",
            "urn:oasis:names:tc:opendocument:xmlns:database:1.0" };
const XML co_DcUri { "dc",
            "http://purl.org/dc/elements/1.1/" };
const XML co_Ddom { "dom",
            "http://www.w3.org/2001/xml-events" };
const XML co_Dr3d { "dr3d",
            "urn:oasis:names:tc:opendocument:xmlns:dr3d:1.0" };
const XML co_DrawUri { "draw",
            "urn:oasis:names:tc:opendocument:xmlns:drawing:1.0" };
const XML co_DrawoooUri { "drawooo",
            "http://openoffice.org/2010/draw" };
const XML co_Field { "field",
            "urn:openoffice:names:experimental:ooo-ms-interop:xmlns:field:1.0" };
const XML co_FoUri { "fo",
            "urn:oasis:names:tc:opendocument:xmlns:xsl-fo-compatible:1.0" };
const XML co_FormUri { "form",
            "urn:oasis:names:tc:opendocument:xmlns:form:1.0" };
const XML co_FormX { "formx",
            "urn:openoffice:names:experimental:ooxml-odf-interop:xmlns:form:1.0" };
const XML co_Grddl { "grddl",
            "http://www.w3.org/2003/g/data-view#" };
const XML co_LoExtUri { "loext",
            "urn:org:documentfoundation:names:experimental:office:xmlns:loext:1.0"};
const XML co_ManifestUri { "manifest",
            "urn:oasis:names:tc:opendocument:xmlns:manifest:1.0" };
const XML co_MathUri { "math",
            "http://www.w3.org/1998/Math/MathML" };
const XML co_MetaUri { "meta",
            "urn:oasis:names:tc:opendocument:xmlns:meta:1.0" };
const XML co_NumberUri { "number",
            "urn:oasis:names:tc:opendocument:xmlns:datastyle:1.0" };
const XML co_OdfUri { "odf",
            "http://docs.oasis-open.org/ns/office/1.2/meta/odf#" };
const XML co_OfUri { "of",
            "urn:oasis:names:tc:opendocument:xmlns:of:1.2" };
const XML co_OfficeOoo { "officeooo",
            "http://openoffice.org/2009/office" };
const XML co_OfficeUri { "office",
            "urn:oasis:names:tc:opendocument:xmlns:office:1.0" };
const XML co_OooUri { "ooo",
            "http://openoffice.org/2004/office" };
const XML co_OoowUri { "ooow",
            "http://openoffice.org/2004/writer" };
const XML co_OoocUri { "oooc",
            "http://openoffice.org/2004/calc" };
const XML co_PkgUri { "pkg",
            "http://docs.oasis-open.org/ns/office/1.2/meta/pkg#" };
const XML co_PresentationUri { "presentation",
            "urn:oasis:names:tc:opendocument:xmlns:presentation:1.0" };
const XML co_RdfUri { "rdf",
            "http://www.w3.org/1999/02/22-rdf-syntax-ns#" };
const XML co_RptUri { "rpt",
            "http://openoffice.org/2005/report" };
const XML co_ScriptUri { "script",
            "urn:oasis:names:tc:opendocument:xmlns:script:1.0" };
const XML co_SmilUri { "smil",
            "urn:oasis:names:tc:opendocument:xmlns:smil-compatible:1.0" };
const XML co_StyleUri { "style",
            "urn:oasis:names:tc:opendocument:xmlns:style:1.0" };
const XML co_SvgUri { "svg",
            "urn:oasis:names:tc:opendocument:xmlns:svg-compatible:1.0" };
const XML co_TableUri { "table",
            "urn:oasis:names:tc:opendocument:xmlns:table:1.0" };
const XML co_TableoooUri { "tableooo",
            "http://openoffice.org/2009/table" };
const XML co_TextUri { "text",
            "urn:oasis:names:tc:opendocument:xmlns:text:1.0" };
const XML co_Xforms { "xforms",
            "http://www.w3.org/2002/xforms" };
const XML co_Xhtml { "xhtml",
            "http://www.w3.org/1999/xhtml" };
const XML co_XlinkUri { "xlink",
            "http://www.w3.org/1999/xlink" };
const XML co_XsdUri { "xsd",
            "http://www.w3.org/2001/XMLSchema" };
const XML co_XsiUri { "xsi",
            "http://www.w3.org/2001/XMLSchema-instance" };

namespace ns	{ // ods::ns::

con_ods co_About				= "about";
con_ods co_Actuate				= "actuate";
con_ods co_Align				= "align";
con_ods co_ApplyStyleName		= "apply-style-name";
con_ods co_Auto					= "auto";
con_ods co_AutomaticFindLabels	= "automatic-find-labels";
con_ods co_AutomaticStyles  	= "automatic-styles";
con_ods co_BackGroundColor		= "background-color";
con_ods co_Body					= "body";
con_ods co_Bool					= "boolean";
con_ods co_BorderBottom			= "border-bottom";
con_ods co_BorderTop    		= "border-top";
con_ods co_BorderLeft			= "border-left";
con_ods co_BorderRight			= "border-right";
con_ods co_BreakBefore			= "break-before";
con_ods co_BreakAfter			= "break-after";
con_ods co_CalcExt          	= "calcext";
con_ods co_CaseSensitive		= "case-sensitive";
con_ods co_Cell     			= "cell";
con_ods co_CellCount			= "cell-count";
con_ods co_CellStylePrefix		= "ce";
con_ods co_Chart				= "chart";
con_ods co_Centimeter			= "cm";
con_ods co_Color				= "color";
con_ods co_Column				= "column";
con_ods co_ColumnStylePrefix	= "co";
con_ods co_ColumnWidth			= "column-width";
con_ods co_Condition			= "condition";
con_ods co_ConfigItem			= "config-item";
con_ods co_ConfigItemMapEntry	= "config-item-map-entry";
con_ods co_ConfigItemMapIndexed	= "config-item-map-indexed";
con_ods co_ConfigItemMapNamed	= "config-item-map-named";
con_ods co_ConfigItemSet		= "config-item-set";
con_ods co_Country				= "country";
con_ods co_CountryAsian			= "country-asian";
con_ods co_CountryComplex		= "country-complex";
con_ods co_CoveredTableCell		= "covered-table-cell";
con_ods co_CreationDate			= "creation-date";
con_ods co_Creator				= "creator";
con_ods co_Currency				= "currency";
con_ods co_CurrencyStyle		= "currency-style";
con_ods co_CurrencySymbol		= "currency-symbol";
con_ods co_DataStyleName		= "data-style-name";
con_ods co_Date					= "date";
con_ods co_DateValue			= "date-value";
con_ods co_Day					= "day";
con_ods co_Dc					= "dc";
con_ods co_DecimalPlaces		= "decimal-places";
con_ods co_DefaultCellStyleName = "default-cell-style-name";
con_ods co_Desc					= "desc"; // <svg::desc> in <draw:frame>
con_ods co_Description			= "Description";
con_ods co_DocContent			= "document-content";
con_ods co_DocMeta				= "document-meta";
con_ods co_DocSettings			= "document-settings";
con_ods co_DocStatistic			= "document-statistic";
con_ods co_DocStyles			= "document-styles";
con_ods co_Double				= "float";
con_ods co_Draw 				= "draw";
con_ods co_DrawingPage			= "drawing-page";
con_ods co_Embed    			= "embed";
con_ods co_Frame				= "frame";
con_ods co_FullPath				= "full-path";
con_ods co_EditingCycles		= "editing-cycles";
con_ods co_EditingDuration		= "editing-duration";
con_ods co_False				= "false";
con_ods co_Family				= "family";
con_ods co_FileEntry			= "file-entry";
con_ods co_Fo                	= "fo";
con_ods co_FontFace         	= "font-face";
con_ods co_FontFaceDecls    	= "font-face-decls";
con_ods co_FontFamily       	= "font-family";
con_ods co_FontFamilyGen       	= "font-family-generic";
con_ods co_FontPitch           	= "font-pitch";
con_ods co_FontSize           	= "font-size";
con_ods co_FontWeight          	= "font-weight";
con_ods co_FontStyle           	= "font-style";
con_ods co_TruncateOnOverflow   = "truncate-on-overflow";
con_ods co_Formula				= "formula";
con_ods co_Generator			= "generator";
con_ods co_Grouping				= "grouping";
con_ods co_H                  	= "h";
con_ods co_HasPart          	= "hasPart";
con_ods co_Header				= "header";
con_ods co_Height				= "height";
con_ods co_HorizontalAlign      = "horizontal-align";
con_ods co_Hours				= "hours";
con_ods co_Href					= "href";
con_ods co_Id					= "id";
con_ods co_Image        		= "image";
con_ods co_InitialCreator		= "initial-creator";
con_ods co_Language				= "language";
con_ods co_LanguageAsian		= "language-asian";
con_ods co_LanguageComplex		= "language-complex";
con_ods co_LoExt        		= "loext";
con_ods co_Manifest				= "manifest";
con_ods co_MarginBottom			= "margin-bottom";
con_ods co_MarginLeft   		= "margin-left";
con_ods co_MarginRight			= "margin-right";
con_ods co_MarginTop			= "margin-top";
con_ods co_Map					= "map";
con_ods co_MasterPage			= "master-page";
con_ods co_MasterStyles			= "master-styles";
con_ods co_MediaType			= "media-type";
con_ods co_Meta					= "meta";
con_ods co_Millimeter			= "mm";
con_ods co_MinDecimalPlaces		= "min-decimal-places";
con_ods co_MinIntegerDigits		= "min-integer-digits";
con_ods co_Minutes				= "minutes";
con_ods co_Month				= "month";
con_ods co_Name					= "name";
con_ods co_Number				= "number";
con_ods co_NumberMinIntDigits	= "number-min-integer-digits";
con_ods co_NumberStyle  		= "number-style";
con_ods co_NumberStylePrefix	= "N";
con_ods co_NumColsRepeated		= "number-columns-repeated";
con_ods co_NumColsSpanned		= "number-columns-spanned";
con_ods co_NumRowsRepeated		= "number-rows-repeated";
con_ods co_NumRowsSpanned		= "number-rows-spanned";
con_ods co_Ns0          		= "ns0";
con_ods co_ObjectCount			= "object-count";
con_ods co_Office   			= "office";
con_ods co_OnLoad     			= "onLoad";
con_ods co_OutlineLevel     	= "outline-level";
con_ods co_P					= "p";
con_ods co_Page     			= "page";
con_ods co_PageCount			= "page-count";
con_ods co_PageHeight			= "page-height";
con_ods co_PageLayout          	= "page-layout";
con_ods co_PageLayoutName   	= "page-layout-name";
con_ods co_PageLayoutProperties	= "page-layout-properties";
con_ods co_PageNumber			= "page-number";
con_ods co_PageUsage			= "page-usage";
con_ods co_PageWidth			= "page-width";
con_ods co_ParagraphProperties	= "paragraph-properties";
con_ods co_Percentage			= "percentage";
con_ods co_PercentageStyle		= "percentage-style";
con_ods co_PrintOrientation		= "print-orientation";
con_ods co_Rdf                  = "rdf";
con_ods co_RDF                  = "RDF";
con_ods co_RepeatContent        = "repeat-content";
con_ods co_Ressource            = "resource";
con_ods co_Row                  = "row";
con_ods co_RowHeight            = "row-height";
con_ods co_RowStylePrefix		= "ro";
con_ods co_Script               = "script";
con_ods co_SearchWholeCell		= "search-criteria-must-apply-to-whole-cell";
con_ods co_Seconds				= "seconds";
con_ods co_Settings				= "settings";
con_ods co_Sheet				= "table";
con_ods co_SheetCell			= "table-cell";
con_ods co_SheetColumn			= "table-column";
con_ods co_SheetCount			= "table-count";
con_ods co_SheetRow				= "table-row";
con_ods co_Show					= "show";
con_ods co_Simple				= "simple";
con_ods co_Span					= "span";
con_ods co_Spreadsheet			= "spreadsheet";
con_ods co_String				= "string";
con_ods co_StringValue  		= "string-value";
con_ods co_Style				= "style";
con_ods co_Styles				= "styles";
con_ods co_StyleName			= "style-name";
con_ods co_Svg      			= "svg";
con_ods co_Table    			= "table";
con_ods co_TableCell   			= "table-cell";
con_ods co_TableCellProperties	= "table-cell-properties";
con_ods co_TableColumn 			= "table-column";
con_ods co_TableColumnProperties= "table-column-properties";
con_ods co_TableCount			= "table-count";
con_ods co_TableProperties      = "table-properties";
con_ods co_TableRow   			= "table-row";
con_ods co_TableRowProperties	= "table-row-properties";
con_ods co_Text					= "text";
con_ods co_TextAlign			= "text-align";
con_ods co_TextAlignSource		= "text-align-source";
con_ods co_TextStrikeThrough    = "text-line-through-style";
con_ods co_TextLineStyle        = "text-line-through-type";
con_ods co_TextProperties		= "text-properties";
con_ods co_TextXml				= "text/xml";
con_ods co_Time					= "time";
con_ods co_TimeStyle			= "time-style";
con_ods co_TimeValue			= "time-value";
con_ods co_Title				= "title";
con_ods co_True					= "true";
con_ods co_Type					= "type";
con_ods co_UseOptimalRowHeight	= "use-optimal-row-height";
con_ods co_UseRegularExpressions = "use-regular-expressions";
con_ods co_UseWildcards			= "use-wildcards";
con_ods co_Value				= "value";
con_ods co_ValueType			= "value-type";
con_ods co_Variable				= "variable";
con_ods co_Version				= "version";
con_ods co_VerticalAlign		= "vertical-align";
con_ods co_Volatile				= "volatile";
con_ods co_Width				= "width";
con_ods co_WrapOption			= "wrap-option";
con_ods co_X					= "x";
con_ods co_Xlink                = "xlink";
con_ods co_Y					= "y";
con_ods co_Year					= "year";
con_ods co_Zindex				= "z-index";
} // ods::ns::

} // ods:
#endif // CONSTANTS_H
