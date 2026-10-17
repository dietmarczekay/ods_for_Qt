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

#ifndef NAMESPACES_H
#define NAMESPACES_H

namespace ods {


class NameSpaces
{
public:
    NameSpaces(const XML_NS version);
    NameSpaces(QXmlStreamReader *xml, const ods::XML_NS version);
    virtual ~NameSpaces();

private:
    void Initialize();

    const ods::XML_NS   v_XML_VersionId;
    QString             v_version;
    QXmlStreamReader	*v_xml = nullptr;
};

}
#endif // NAMESPACES_H
