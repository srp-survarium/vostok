////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef WEAPON_LEXEME_PAIR_H_INCLUDED
#define WEAPON_LEXEME_PAIR_H_INCLUDED

#include <vostok/animation/mixing_animation_lexeme.h>

namespace survarium {

struct weapon_lexeme_pair {
public:
	inline	explicit	weapon_lexeme_pair	(
							animation::mixing::animation_lexeme const& main_lexeme,
							animation::mixing::animation_lexeme const& offset_lexeme
						) : main_lexeme( main_lexeme ), offset_lexeme( offset_lexeme ) { }
	inline				~weapon_lexeme_pair	( ) { /* no source */ }

public:
	/* 0x0000 */	animation::mixing::animation_lexeme	main_lexeme;
	/* 0x0084 */	animation::mixing::animation_lexeme	offset_lexeme;
}; // struct weapon_lexeme_pair

// STATIC_SIZE_ASSERT(weapon_lexeme_pair, 0x108); sushi@TODO

} // namespace survarium

#endif // #ifndef WEAPON_LEXEME_PAIR_H_INCLUDED
