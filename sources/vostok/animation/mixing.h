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

// claude@MATCH: the enum's real tag is `playback_enum`, named directly (not a typedef of
// `playing_type_enum`): the enum appears in exactly 2 target mangled names
// (get_weapon_lexeme_pair_impl + the weapon_core_shotgun_reload_base_substate ctor), both
// `W4playback_enum@mixing`, ZERO `playing_type_enum`. So the tag must be `playback_enum` to
// make our base symbols' mangled names byte-match the target (verified: the base
// get_weapon_lexeme_pair_impl mangled name is now character-identical to the target's).
// This rename is zero-cost: a clean build of THIS tag vs the prior `playing_type_enum`+alias
// tag (both on the same toolchain) shows 0 unit-level regressions - the "408 regressed" once
// attributed to it was June-7->June-8 delinker/tooling drift, NOT this change. (Neither enum
// symbol yet PAIRS in objdiff - both stay None - but that is the out-of-scope inline-vs-call
// body divergence, not the tag; see get_weapon_lexeme_pair_impl.md.) Resolved sushi@TODO.
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