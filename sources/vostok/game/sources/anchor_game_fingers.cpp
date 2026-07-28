////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

// Game-module /OPT:REF reachability anchor for survarium::fingers_to_weapon_corrector
// (fingers_to_weapon_corrector.cpp). The corrector is a member of weapon, but
// weapon::process_finger_correction (its only caller) is still a stub, so /OPT:REF
// strips the corrector's out-of-line bodies. use_game_fingers() constructs a corrector
// (self-guarded so the placeholder refs are never dereferenced) and references its
// public/private methods (friend access). The file-local interpolate_hand_matrices is
// kept reachable through process() alone - see the note at the call site below.
//
// Retire once weapon::process_finger_correction (and the activate path) reach these.

#include "pch.h"

#include "fingers_to_weapon_corrector.h"

namespace survarium {

void use_game_fingers( )
{
	static volatile bool s_run = false;
	if( !s_run )
		return;

	static animation::skeleton* volatile			s_skeleton	= 0;
	static render::render_model_instance* volatile	s_model		= 0;
	static float4x4* volatile						s_matrices	= 0;

	animation::skeleton const&				skeleton	= *s_skeleton;
	render::render_model_instance const&	model		= *s_model;
	float4x4*								matrices	= s_matrices;

	static volatile u32 v = 0;
	const u32 any_u32 = v;
	bool b = s_run;

	fingers_to_weapon_corrector corrector;

	corrector.activate( skeleton, render::render_model_instance_ptr( ), b );
	corrector.process( any_u32, matrices );
	corrector.activate_hand( fingers_to_weapon_corrector::left, b, any_u32 );
	corrector.get_hand_coefficient( 0.0f, b );
	corrector.initialize_bones_indices( skeleton );
	corrector.initialize_locators( model, b );

	// interpolate_hand_matrices is reached through process() only; do NOT call it
	// here with a runtime count - a second caller blocks LTCG from specialising the
	// phalanges_count param to the constant 15 the target folds in.
}

} // namespace survarium
