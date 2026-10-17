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

#ifndef DRAWFRAME_H
#define DRAWFRAME_H

#include <QString>
namespace ods {

class Cell;
class Tag;

class DrawFrame
{
public:
    DrawFrame(ods::Cell *cell, QString filename, float offset_x, float offset_y, float width_x, float width_y);
    ~DrawFrame();

    ods::Tag        *Save();
private:
    Cell            *p_cell = nullptr;
    QString         v_filename;
    float           v_off_x;
    float           v_off_y;
    float           v_width_x;
    float           v_width_y;

    void Initialize();
};

}

#endif // DRAWFRAME_H
