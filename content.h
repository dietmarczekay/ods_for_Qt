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

#ifndef CONTENT_H
#define CONTENT_H

#include <QString>
#include <QHash>
#include <QRegularExpression>

#include "constants.h"
#include "tag.h"

namespace ods{

class Book;
class CellStyle;
class Column;
class ColumnStyle;
class NumberStyle;
class PictureStyle;
class Row;
class RowStyle;
class Table;
class Text;

class Content
{
public:
    Content(Book *book, ods::FileType type);
    virtual ~Content();         // deleted through Content* (SpreadSheet/TextDoc)
    uint                        NextCellStyleIndex() const;
    /** data style for a time format ("mm:ss.zzz"), one per format */
    QString                     TimeDataStyle(const QString &format);
    /** cell style = base (may be null) + data style; one per combination */
    CellStyle                   *CellStyleWithDataStyle(CellStyle *base, const QString &dataStyle);

    CellStyle                   *CreateCellStyle();
    ColumnStyle                 *CreateColumnStyle();
    QString CreateNumberTimeStyle(number_Type type, QString format, int decimalPlaces, QString prefix);
    QString CreateNumberCurrencyStyle(ods::number_Type type);
    virtual Table               *CreateTable(int index);
    virtual Table               *CreateTable(QString name);
    virtual Table               *CreateTable(int index, QString name);
    QString                     GetValidTableName(QString name);
//    virtual Text                *CreateText();
    Book                        *GetParentBook(){ return p_book; }
    QList<CellStyle *>          *GetCellStyles(){ return &v_cellstyles; }
    int                         GetCellStylesCount(){ return v_cellstyles.count(); }
    QList<PictureStyle *>       *GetPictureStyles(){ return &v_picturestyles; }
    int                         GetPictureStylesCount(){ return v_picturestyles.count(); }
    QList<NumberStyle *>        *GetNumberStyles(){ return &v_numberstyles; }
    QList<ColumnStyle *>        *GetColumnStyles(){ return &v_columnstyles; }
    QList<RowStyle *>           *GetRowStyles(){ return &v_rowstyles; }
    virtual int                 GetTableCount(){ return 0; }
    virtual void                Save(const QString &save_dir, QString &err);

protected:
    Book                        *p_book;

    QList<Table*>               v_tables;
    QList<CellStyle *>          v_cellstyles;
    QList<ColumnStyle *>        v_columnstyles;
    QList<NumberStyle *>        v_numberstyles;
    QList<PictureStyle *>       v_picturestyles;
    QList<RowStyle *>           v_rowstyles;

private:
    void initialize();
    QHash<QString, QString>     v_time_data_styles;     // format -> N...
    QHash<QString, CellStyle *> v_derived_styles;       // "ce3|N108" -> style
};
}
#endif // CONTENT_H
