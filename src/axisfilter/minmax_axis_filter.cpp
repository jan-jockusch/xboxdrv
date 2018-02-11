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

#include "axisfilter/minmax_axis_filter.hpp"

#include <boost/tokenizer.hpp>
#include <sstream>

#include "helper.hpp"

MinmaxAxisFilter*
MinmaxAxisFilter::from_string(const std::string& str)
{
  int speed = 20000;

  typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
  tokenizer tokens(str, boost::char_separator<char>(":", "", boost::keep_empty_tokens));
  int idx = 0;
  for(tokenizer::iterator t = tokens.begin(); t != tokens.end(); ++t, ++idx)
  {
    switch(idx)
    {
      case 0: speed = str2int(*t); break;
      default: throw std::runtime_error("to many arguments"); break;
    }
  }

  return new MinmaxAxisFilter(speed);
}

MinmaxAxisFilter::MinmaxAxisFilter(int speed) :
  m_speed(speed),
  m_float_speed(0.0f),
  m_value(0),
  m_state(0)
{
}

void
MinmaxAxisFilter::update(int msec_delta)
{
  // JJ: Stolen from relative_axis_filter.  But behaviour is
  // different. With relative_axis you need to keep an eye on the
  // current setting and only your simulator / game can tell you
  // that. With minmax you can develop a feeling for throttle levels
  // like with a throttle lever. You push up to the level you want,
  // then let the stick snap back, or, you pull back the lever to the
  // level you want to slow down to, then let it snap back.

  // This works by mapping the full range [-1..1] onto the upper and
  // the lower half of the axis. With the upper half, you dictate a
  // lower limit for the state, with the lower, you dictate an upper
  // limit.
  
  // Implementation:
  // if m_value >= 0.0
  //    map m_value [0.0 .. 1.0] to [-1.0 .. 1.0] (m_value*2-1)
  //    if m_state < m_value: m_state = m_value
  // else
  //    map m_value [-1.0 .. 0.0] to [-1.0 .. 1.0] (m_value*2+1)
  //    if m_state > m_value: m_state = m_value
  if ((m_value > 0.0) && (m_state < (m_value*2.0-1.0)))
  {
    m_state = m_value*2.0-1.0;
  }
  if ((m_value < 0.0) && (m_state > (m_value*2.0+1.0)))
  {
    m_state = m_value*2.0+1.0;
  }
  m_state = Math::clamp(-1.0f, m_state, 1.0f);
}

int
MinmaxAxisFilter::filter(int value, int min, int max)
{
  m_value = to_float(value, min, max);

  m_float_speed = to_float_no_range_check(m_speed, min, max);

  return from_float(m_state, min, max);
}

std::string
MinmaxAxisFilter::str() const
{
  std::ostringstream out;
  out << "minmaxaxis:" << m_speed;
  return out.str();
}

/* EOF */
