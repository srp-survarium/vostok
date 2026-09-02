// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GRAPHIC_PRESET_H_INCLUDED
#define GRAPHIC_PRESET_H_INCLUDED

#include "video_options_enum.h"

namespace survarium {

struct graphic_preset {

public:
	/* 0x0000 */	video_options_enum		option_id;
	/* 0x0004 */	u8						option_value;
}; // struct graphic_preset

STATIC_SIZE_ASSERT(graphic_preset, 0x8);

} // namespace survarium

#endif // #ifndef GRAPHIC_PRESET_H_INCLUDED
