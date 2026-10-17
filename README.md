# ods_for_Qt
Qt class to write ods files in desktop apps and webassemlby apps


ods::Book book(ods::FileType::spreadsheet);
ods::SpreadSheet *sheet = book.GetContentAsSpreadSheet();
ods::Table *table = sheet->CreateTable("Results");

ods::Row *row = table->CreateRow();
row->CreateCell("Anna");                                          // text
row->CreateCell(row->GetNextColumnNumber())->SetValue(12.5, 2);   // number
row->CreateCell(QTime(0, 1, 23, 450), "mm:ss.zzz");               // time

QString err = book.Save(QFile("example.ods"));                    // "" = OK


example code:

/***********************************************************************
 * Copyright (c) 2026 git@czekay.org
 * SPDX-License-Identifier: GPL-3.0-or-later

    Minimal example for QODS_Module: writes a small spreadsheet.
    Desktop:     ./ods_example [file.ods]
    WebAssembly: the browser offers "example.ods" as a download.
************************************************************************/

#include <QDir>
#include <QFile>
#include <QTemporaryDir>
#include <QTime>

#ifdef Q_OS_WASM
#  include <QApplication>
#  include <QFileDialog>
#else
#  include <QCoreApplication>
#  include <QTextStream>
#endif

#include "ods.h"

// Builds the spreadsheet and saves it to filePath. Returns "" on success.
static QString writeOds(const QString &filePath)
{
    ods::Book book(ods::FileType::spreadsheet);
    ods::SpreadSheet *sheet = book.GetContentAsSpreadSheet();
    ods::Table *table = sheet->CreateTable(QStringLiteral("Results"));

    // bold header with grey background
    ods::CellStyle *head = sheet->CreateCellStyle();
    head->SetFontWeight(QFont::Bold);
    head->SetBackgroundColor(QColor(0xDD, 0xDD, 0xDD));

    ods::Row *row = table->CreateRow();
    for (const char *title : {"Name", "Points", "Time"})
        row->CreateCell(QString::fromLatin1(title))->SetStyle(head);

    struct Entry { const char *name; double points; QTime time; };
    const Entry data[] = {
        { "Anna",  12.5, QTime(0, 1, 23, 450) },
        { "Ben",    9.0, QTime(0, 1, 31, 120) },
        { "Clara", 14.25, QTime(0, 1, 19, 980) },
    };
    for (const Entry &e : data) {
        row = table->CreateRow();
        row->CreateCell(QString::fromUtf8(e.name));            // text
        row->CreateCell(row->GetNextColumnNumber())           // number, 2 decimals
            ->SetValue(e.points, 2);                           // (columns are 1-based)
        row->CreateCell(e.time, QStringLiteral("mm:ss.zzz"));  // time
    }

    return book.Save(QFile(filePath));     // all objects belong to the Book
}

int main(int argc, char *argv[])
{
#ifdef Q_OS_WASM
    QApplication app(argc, argv);

    // the browser has no real file system: write into Emscripten's
    // in-memory FS, then hand the bytes to the browser as a download
    QTemporaryDir tmp;
    const QString path = tmp.filePath(QStringLiteral("example.ods"));
    const QString err = writeOds(path);
    if (!err.isEmpty()) {
        qWarning("%s", qPrintable(err));
        return 1;
    }
    QFile f(path);
    if (f.open(QIODevice::ReadOnly))
        QFileDialog::saveFileContent(f.readAll(), QStringLiteral("example.ods"));
    return 0;   // with Qt for wasm main() returns, the page keeps running
#else
    QCoreApplication app(argc, argv);
    const QString path = argc > 1 ? QString::fromLocal8Bit(argv[1])
                                  : QDir::current().filePath(QStringLiteral("example.ods"));
    const QString err = writeOds(path);
    QTextStream out(stdout);
    if (!err.isEmpty()) {
        out << "Error: " << err << Qt::endl;
        return 1;
    }
    out << "Written: " << path << Qt::endl;
    return 0;
#endif
}
