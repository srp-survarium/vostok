#include "pch.h"

// Game-module /OPT:REF reachability anchor for the small self-contained utility
// classes: step_manager, text_translator, empty_hands, swf_input_translator.
//
// These carcass TUs compile into game.lib but no reachable engine call graph
// touches them yet, so /OPT:REF strips them from the EXE and objdiff reports
// their target symbols as "unpaired". use_game_small_utils() constructs each one
// and CALLS its methods so the linker keeps the bodies past /OPT:REF (and the
// delinker can pair them). EVERY argument is sourced through a volatile pointer
// so LTCG cannot prove the calls are side-effect-free and DCE them (a literal
// argument lets the optimiser fold the whole call away - the lesson of the
// use_object_skeleton anchor). Dispatched from anchor_game(); retire once the
// real game call graph reaches these for itself. It is befriended by each class
// so the protected/private members are callable here.

#include "step_manager.h"
#include "text_translator.h"
#include "empty_hands.h"
#include "swf_input_translator.h"
#include <vostok/game_core/base_player.h>
#include <vostok/scaleform/sources/flash_movie.h>
#include <vostok/input/world.h>

namespace vostok
{
	void use_game_small_utils( )
	{
		using namespace survarium;

		static volatile bool s_run = false;
		if ( !s_run )
			return;

		static player const* volatile			s_player	= 0;
		static float3 const* volatile			s_vec		= 0;
		static game_world* volatile				s_world		= 0;
		static input::world* volatile			s_input		= 0;
		static flash_movie* volatile			s_movie		= 0;
		static base_player* volatile			s_base_player	= 0;
		static engine* volatile					s_engine	= 0;
		static mutable_buffer* volatile			s_buffer	= 0;
		static animation::skeleton_ptr const* volatile	s_skel	= 0;
		static float4x4* volatile				s_mat		= 0;
		static animation::animation_player const* volatile	s_anim	= 0;
		static resources::managed_resource_ptr* volatile	s_anims	= 0;
		static pcstr volatile					s_str		= 0;
		static wchar_t* volatile				s_wstr		= 0;
		static resources::queries_result* volatile	s_qr	= 0;
		static input::enum_keyboard volatile	s_key		= input::enum_keyboard( 0 );
		static input::enum_keyboard_action volatile	s_kact	= input::enum_keyboard_action( 0 );
		static input::mouse_button volatile		s_mbtn		= input::mouse_button( 0 );
		static input::enum_mouse_key_action volatile	s_mact	= input::enum_mouse_key_action( 0 );
		static float volatile					s_f			= 0;
		static u32 volatile						s_u			= 0;
		static s32 volatile						s_s			= 0;
		static wchar_t volatile					s_wc		= 0;
		static bool volatile					s_b			= false;
		static dik_to_swf_bind* volatile		s_bind		= 0;

		// ---- step_manager ----------------------------------------------------
		step_manager	step;
		step.on_step( *s_player, *s_vec, *s_vec, *s_world );

		// ---- text_translator -------------------------------------------------
		text_translator	texts;
		texts.translate_text( s_str, s_wstr );
		texts.load_text_localization( );
		texts.on_texts_ready( *s_qr );

		// ---- swf_input_translator --------------------------------------------
		swf_input_translator	swf;
		swf.process_keyboard( s_input, s_key, s_kact, s_movie, s_u );
		swf.process_mouse_btn( s_input, s_mbtn, s_mact, s_f, s_f, s_movie );
		swf.process_mouse_move( s_input, s_f, s_f, s_f, s_movie );
		swf.initialize( );
		swf.get_bind( s_key );
		swf.translate_key_action( s_input, s_b, *s_bind );
		swf.register_ctl_bind( s_key, s_s );
		swf.register_char_bind( s_key, s_s, s_b );
		swf.register_char_bind( s_key, s_wc, s_wc, s_s, s_b );

		// ---- empty_hands -----------------------------------------------------
		empty_hands		hands( s_anims, s_u );
		hands.activate( *s_base_player, *s_engine );
		hands.selected_animations( *s_buffer, s_b );
		hands.update_bones_matrices( *s_skel, s_mat, s_u, s_u, *s_mat, *s_mat, *s_anim );
	}
} // namespace vostok
