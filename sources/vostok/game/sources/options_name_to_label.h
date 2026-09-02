// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef OPTIONS_NAME_TO_LABEL_H_INCLUDED
#define OPTIONS_NAME_TO_LABEL_H_INCLUDED

namespace survarium {

// aggregate: the file-static label tables are brace-initialized
// ( { "name", "label" } ) in game_options.cpp, one entry per statement
struct options_name_to_label {
public:
	/* 0x0000 */	fixed_string< 32 >		name;
	/* 0x002c */	fixed_string< 32 >		label;
}; // struct options_name_to_label

STATIC_SIZE_ASSERT(options_name_to_label, 0x58);

} // namespace survarium

#endif // #ifndef OPTIONS_NAME_TO_LABEL_H_INCLUDED
