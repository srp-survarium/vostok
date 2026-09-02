// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PLAYER_PARAMETERS_COOKER_DATA_H_INCLUDED
#define PLAYER_PARAMETERS_COOKER_DATA_H_INCLUDED

namespace survarium {

class items_dictionary;
struct player_profile;

struct player_parameters_cooker_data {
	/* 0x0000 */	player_profile const*		profile;
	/* 0x0004 */	items_dictionary*			dictionary;
}; // struct player_parameters_cooker_data

STATIC_SIZE_ASSERT(player_parameters_cooker_data, 0x8);

} // namespace survarium

#endif // #ifndef PLAYER_PARAMETERS_COOKER_DATA_H_INCLUDED
