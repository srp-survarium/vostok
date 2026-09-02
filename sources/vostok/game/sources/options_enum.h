// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_ENUM_H_INCLUDED
#define OPTIONS_ENUM_H_INCLUDED

namespace survarium {

enum options_enum
{
	gameplay_options_type		= 0x0,
	controllers_options_type	= 0x1,
	video_options_type			= 0x2,
	sound_options_type			= 0x3,
	options_count				= 0x4,
};

} // namespace survarium

#endif // #ifndef OPTIONS_ENUM_H_INCLUDED
