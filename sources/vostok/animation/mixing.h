////////////////////////////////////////////////////////////////////////////
//	Created		: 30.09.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef MIXING_H_INCLUDED
#define MIXING_H_INCLUDED

namespace vostok {
namespace animation {
namespace mixing {

// claude@MATCH: the enum's real tag is `playback_enum` (it is named directly, not a
// typedef of `playing_type_enum`): every target mangled name uses `W4playback_enum@mixing`
// and ZERO use `playing_type_enum`, so the tag drives the symbol name. Naming it
// `playing_type_enum` + aliasing left get_weapon_lexeme_pair_impl and the
// weapon_core_shotgun_reload_base_substate ctor un-pairable (their base symbols carried
// `playing_type_enum@mixing`). Resolved sushi@TODO.
enum playback_enum {
	// play cyclic animation infinitely
	// on animation end just rewind to the start
	play_cyclically,

	// play single animation;
	// on animation end set time scale for animation to 0
	// as a consequence animation will stay at the last frame infinitely
	play_once_and_freeze_at_end,

	// play single animation;
	// on animation end instantly remove animation
	// as a consequence animation will be removed immediately after its end
	play_once_and_remove_at_end,
}; // enum playback_enum

} // namespace mixing
} // namespace animation
} // namespace vostok

#endif // #ifndef MIXING_H_INCLUDED