// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////
#ifndef WEAPON_LEXEME_PAIR_H_INCLUDED
#define WEAPON_LEXEME_PAIR_H_INCLUDED

#include <vostok/animation/mixing_animation_lexeme.h>

namespace survarium {

struct weapon_lexeme_pair {
public:
	typedef animation::mixing::animation_lexeme lexeme_type;

	inline	explicit	weapon_lexeme_pair	(
							lexeme_type const& main_lexeme,
							lexeme_type const& offset_lexeme
						) : main_lexeme( main_lexeme ), offset_lexeme( offset_lexeme ) { }
public:
	/* 0x0000 */	lexeme_type	main_lexeme;
	/* 0x0084 */	lexeme_type	offset_lexeme;
}; // struct weapon_lexeme_pair

// STATIC_SIZE_ASSERT(weapon_lexeme_pair, 0x108); sushi@TODO

} // namespace survarium

#endif // #ifndef WEAPON_LEXEME_PAIR_H_INCLUDED
