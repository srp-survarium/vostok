#include "pch.h"

// Game-module /OPT:REF reachability anchor for the NPC / animation cluster
// (human_npc, animations_selector, ai_sound_player). The real
// game call graph does not yet reach these out-of-line bodies, so without a
// reference /OPT:REF strips them and the delinker has nothing to pair against
// the target (every function shows `unpaired`). use_game_npc() touches each
// symbol behind a volatile self-guard (the heavy ctors never actually run), so
// the compiler emits the references but no NULL deref happens.
//
// Dispatched from anchor_game() (anchor_game.cpp). Retire once human_npc &
// friends are constructed by the real game world / ai cooks.

#include "human_npc.h"
#include "animations_selector.h"
#include "ai_sound_player.h"
#include "game_world.h"

#include <vostok/math_float4x4.h>

namespace vostok
{
	void use_game_npc( )
	{
		static volatile bool s_run = false;
		if ( !s_run )
			return;

		// placeholders sourced through volatile pointers so LTCG cannot fold the
		// constant-null args into the ctors (which would drop member stores).
		static survarium::game_world* volatile							s_world		= 0;
		static vostok::animation::animation_player* volatile			s_player	= 0;
		static survarium::animation_space_graph_ptr const* volatile		s_graph		= 0;
		static vostok::resources::managed_resource_ptr const* volatile	s_anim		= 0;
		static vostok::ai::navigation::world const* volatile			s_navworld	= 0;
		static survarium::human_npc* volatile							s_npc		= 0;
		static vostok::resources::unmanaged_resource_ptr* volatile		s_scene		= 0;
		static vostok::sound::world_user* volatile						s_user		= 0;

		survarium::game_world&								world	= *s_world;
		vostok::animation::animation_player&				player	= *s_player;
		survarium::animation_space_graph_ptr const&			graph	= *s_graph;
		vostok::resources::managed_resource_ptr const&		anim	= *s_anim;
		vostok::ai::navigation::world const&				navworld= *s_navworld;
		survarium::human_npc&								npc		= *s_npc;

		// ----- human_npc : ctor + vtable keep all virtuals; member-ptr sinks keep
		// the non-virtual out-of-line methods.
		survarium::human_npc								human( world );

		static pcvoid volatile s_sink = 0;
		survarium::object_weapon* ( survarium::human_npc::* const hp_pop )( )	= &survarium::human_npc::pop_weapon;
		void ( survarium::human_npc::* const hp_avail )( vectora< ai::weapon* >& ) const	= &survarium::human_npc::get_available_weapons;
		void ( survarium::human_npc::* const hp_selgoal )( )						= &survarium::human_npc::select_new_goal;
		void ( survarium::human_npc::* const hp_onhit )( survarium::hit_object const& )	= &survarium::human_npc::on_hit_event;
		s_sink = *( pcvoid const* )&hp_pop;
		s_sink = *( pcvoid const* )&hp_avail;
		s_sink = *( pcvoid const* )&hp_selgoal;
		s_sink = *( pcvoid const* )&hp_onhit;

		// npc_game_attributes ctor + operator=
		survarium::human_npc::npc_game_attributes			attrs;
		survarium::human_npc::npc_game_attributes			attrs2;
		attrs = attrs2;

		// ----- animations_selector : embeds the single_position/simple animation
		// controllers; constructing it forces their (matched-later) vtables to emit, so
		// the controller TUs supply minimal buildability returns for their non-void
		// stubs (see those files' claude@NOTE). The ctor itself is an addressed target
		// symbol, so it must be constructed (a ctor cannot be member-ptr-referenced).
		survarium::animations_selector						selector( player, graph, anim, navworld, world, npc );
		void ( survarium::animations_selector::* const s_settgt_a )( ai::animation_item const& )	= &survarium::animations_selector::set_target;
		void ( survarium::animations_selector::* const s_settgt_m )( ai::movement_target const& )	= &survarium::animations_selector::set_target;
		void ( survarium::animations_selector::* const s_dbg )( render::game::renderer&, render::scene_ptr const& ) const	= &survarium::animations_selector::debug_draw;
		vostok::animation::callback_return_type_enum ( survarium::animations_selector::* const s_onend )( animation::animation_callback_params& )	= &survarium::animations_selector::on_animation_interval_end;
		void ( survarium::animations_selector::* const s_onset )( )		= &survarium::animations_selector::on_set_target;
		s_sink = *( pcvoid const* )&s_settgt_a;
		s_sink = *( pcvoid const* )&s_settgt_m;
		s_sink = *( pcvoid const* )&s_dbg;
		s_sink = *( pcvoid const* )&s_onend;
		s_sink = *( pcvoid const* )&s_onset;

		// ----- ai_sound_player : ctor + vtable (play / play_once / tick / clear) plus
		// the non-virtual find / on_finish_playing / serialize handlers.
		survarium::ai_sound_player							sound_player( *s_scene, 0, *s_user, NULL, NULL );
		survarium::ai_sound_player::sounds_collection_type const* ( survarium::ai_sound_player::* const sp_find )( ai::sound_collection_types ) const	= &survarium::ai_sound_player::find;
		vostok::sound::command_result_enum ( survarium::ai_sound_player::* const sp_finish )( )	= &survarium::ai_sound_player::on_finish_playing;
		void ( survarium::ai_sound_player::* const sp_ser )( memory::writer*, memory::writer* )	= &survarium::ai_sound_player::on_active_sound_serialized;
		void ( survarium::ai_sound_player::* const sp_deser )( memory::reader*, void* )	= &survarium::ai_sound_player::on_active_sound_deserialized;
		s_sink = *( pcvoid const* )&sp_find;
		s_sink = *( pcvoid const* )&sp_finish;
		s_sink = *( pcvoid const* )&sp_ser;
		s_sink = *( pcvoid const* )&sp_deser;
	}
}
