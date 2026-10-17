/***********************************************************************
 * Copyright (c) 2026 git@czekay.org
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

#ifndef ODS_ZIPWRITER_H
#define ODS_ZIPWRITER_H

#include <QByteArray>
#include <QList>
#include <QString>

class QIODevice;

namespace ods {

/*
 * Minimal ZIP writer for ODF packages, only zlib is needed (no QuaZip, so no
 * Qt6::Core5Compat and no QuaZip port for WebAssembly).
 *
 * Every entry is compressed in memory first, so CRC and sizes are known before
 * the local header is written:
 *   - no data descriptors (LibreOffice rejects stored entries with them)
 *   - no extra fields, no directory entries
 *   - method 0 (stored) or 8 (deflate), file names UTF-8
 * Enough for documents of a few MB (all ODF parts fit in memory anyway).
 */
class ZipWriter
{
public:
    explicit ZipWriter(QIODevice *device);

    /** store = true: uncompressed (required for "mimetype") */
    bool addFile(const QString &name, const QByteArray &data, bool store = false);
    /** writes the central directory; the device stays open */
    bool finish();

    QString errorString() const { return m_error; }

private:
    struct Entry {
        QByteArray name;        // UTF-8
        quint32    crc = 0;
        quint32    compressedSize = 0;
        quint32    size = 0;
        quint16    method = 0;
        quint32    offset = 0;  // of the local header
    };

    bool write(const QByteArray &bytes);
    static bool deflateRaw(const QByteArray &in, QByteArray *out);

    QIODevice    *m_device;
    QList<Entry>  m_entries;
    quint32       m_offset = 0;
    quint16       m_dosTime = 0;
    quint16       m_dosDate = 0;
    bool          m_finished = false;
    QString       m_error;
};

}
#endif // ODS_ZIPWRITER_H
