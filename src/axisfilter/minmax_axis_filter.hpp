/*
**  Xbox360 USB Gamepad Userspace Driver
**  Copyright (C) 2011 Ingo Ruhnke <grumbel@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// JJ Notes: This is nicked from relative_axis_filter.  Therefore,
// some unnecessary things are still here to make patching easier. See
// notes below.

#ifndef HEADER_XBOXDRV_AXISFILTER_MINMAX_AXIS_FILTER_HPP
#define HEADER_XBOXDRV_AXISFILTER_MINMAX_AXIS_FILTER_HPP

#include "axis_filter.hpp"

class MinmaxAxisFilter : public AxisFilter
{
public:
  static MinmaxAxisFilter* from_string(const std::string& str);

public:
  MinmaxAxisFilter(int speed); /* JJ: speed not necessary */

  void update(int msec_delta);
  int filter(int value, int min, int max);
  std::string str() const;

private:
  int m_speed; /* JJ: speed not necessary */

  float m_float_speed; /* JJ: speed not necessary */
  float m_value;
  float m_state;
};

#endif

/* EOF */
