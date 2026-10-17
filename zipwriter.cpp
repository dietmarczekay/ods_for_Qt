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

#include <QDateTime>
#include <QIODevice>
#include <QtEndian>

#include <zlib.h>

#include "zipwriter.h"

namespace ods {

namespace {
// little endian helpers
void put16(QByteArray &b, quint16 v) { char c[2]; qToLittleEndian(v, c); b.append(c, 2); }
void put32(QByteArray &b, quint32 v) { char c[4]; qToLittleEndian(v, c); b.append(c, 4); }

constexpr quint32 SIG_LOCAL   = 0x04034b50;
constexpr quint32 SIG_CENTRAL = 0x02014b50;
constexpr quint32 SIG_END     = 0x06054b50;
constexpr quint16 VERSION     = 20;          // 2.0: deflate
constexpr quint16 FLAG_UTF8   = 0x0800;      // bit 11: file name is UTF-8
}

ZipWriter::ZipWriter(QIODevice *device)
    : m_device(device)
{
    // one timestamp for all entries (DOS format, local time, 2 s resolution)
    const QDateTime now = QDateTime::currentDateTime();
    const QDate d = now.date();
    const QTime t = now.time();
    m_dosTime = quint16((t.hour() << 11) | (t.minute() << 5) | (t.second() / 2));
    m_dosDate = quint16(((qMax(d.year(), 1980) - 1980) << 9) | (d.month() << 5) | d.day());
}

bool ZipWriter::write(const QByteArray &bytes)
{
    if (m_device->write(bytes) != bytes.size()) {
        m_error = QStringLiteral("write failed: %1").arg(m_device->errorString());
        return false;
    }
    m_offset += quint32(bytes.size());
    return true;
}

// raw deflate stream (no zlib/gzip header), as used inside ZIP files
bool ZipWriter::deflateRaw(const QByteArray &in, QByteArray *out)
{
    z_stream zs{};
    if (deflateInit2(&zs, Z_BEST_COMPRESSION, Z_DEFLATED, -MAX_WBITS, 8, Z_DEFAULT_STRATEGY) != Z_OK)
        return false;

    out->resize(int(deflateBound(&zs, uLong(in.size()))));
    zs.next_in   = reinterpret_cast<Bytef *>(const_cast<char *>(in.constData()));
    zs.avail_in  = uInt(in.size());
    zs.next_out  = reinterpret_cast<Bytef *>(out->data());
    zs.avail_out = uInt(out->size());

    const int rc = deflate(&zs, Z_FINISH);
    out->resize(int(zs.total_out));
    deflateEnd(&zs);
    return rc == Z_STREAM_END;
}

bool ZipWriter::addFile(const QString &name, const QByteArray &data, bool store)
{
    if (m_finished) {
        m_error = QStringLiteral("zip already finished");
        return false;
    }

    Entry e;
    e.name   = name.toUtf8();
    e.size   = quint32(data.size());
    e.crc    = quint32(crc32(crc32(0L, Z_NULL, 0),
                             reinterpret_cast<const Bytef *>(data.constData()), uInt(data.size())));
    e.offset = m_offset;

    QByteArray payload;
    if (store) {
        e.method = 0;
        payload = data;
    } else {
        e.method = 8;
        if (!deflateRaw(data, &payload)) {
            m_error = QStringLiteral("deflate failed for %1").arg(name);
            return false;
        }
    }
    e.compressedSize = quint32(payload.size());

    QByteArray h;
    put32(h, SIG_LOCAL);
    put16(h, VERSION);
    put16(h, FLAG_UTF8);                // no bit 3: sizes are in this header
    put16(h, e.method);
    put16(h, m_dosTime);
    put16(h, m_dosDate);
    put32(h, e.crc);
    put32(h, e.compressedSize);
    put32(h, e.size);
    put16(h, quint16(e.name.size()));
    put16(h, 0);                        // no extra field
    h.append(e.name);

    if (!write(h) || !write(payload))
        return false;

    m_entries.append(e);
    return true;
}

bool ZipWriter::finish()
{
    if (m_finished)
        return true;

    const quint32 cdOffset = m_offset;
    QByteArray cd;
    for (const Entry &e : std::as_const(m_entries)) {
        put32(cd, SIG_CENTRAL);
        put16(cd, VERSION);             // made by
        put16(cd, VERSION);             // needed
        put16(cd, FLAG_UTF8);
        put16(cd, e.method);
        put16(cd, m_dosTime);
        put16(cd, m_dosDate);
        put32(cd, e.crc);
        put32(cd, e.compressedSize);
        put32(cd, e.size);
        put16(cd, quint16(e.name.size()));
        put16(cd, 0);                   // extra
        put16(cd, 0);                   // comment
        put16(cd, 0);                   // disk
        put16(cd, 0);                   // internal attributes
        put32(cd, 0);                   // external attributes
        put32(cd, e.offset);
        cd.append(e.name);
    }
    if (!write(cd))
        return false;

    QByteArray end;
    put32(end, SIG_END);
    put16(end, 0);                      // this disk
    put16(end, 0);                      // disk with central directory
    put16(end, quint16(m_entries.size()));
    put16(end, quint16(m_entries.size()));
    put32(end, quint32(cd.size()));
    put32(end, cdOffset);
    put16(end, 0);                      // comment length
    if (!write(end))
        return false;

    m_finished = true;
    return true;
}

}
