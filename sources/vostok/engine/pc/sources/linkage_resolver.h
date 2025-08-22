////////////////////////////////////////////////////////////////////////////
//	Created		: 23.04.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef LINKAGE_RESOLVER_H_INCLUDED
#define LINKAGE_RESOLVER_H_INCLUDED

#include <vostok/linkage_helper.h>


VOSTOK_INCLUDE_TO_LINKAGE(core_math_convex)
VOSTOK_INCLUDE_TO_LINKAGE(core_math_sphere)
VOSTOK_INCLUDE_TO_LINKAGE(core_geometry_utils)
VOSTOK_INCLUDE_TO_LINKAGE(core_os_functions_win)
VOSTOK_INCLUDE_TO_LINKAGE(core_memory_writer)
VOSTOK_INCLUDE_TO_LINKAGE(core_text_tree)
VOSTOK_INCLUDE_TO_LINKAGE(core_profiler)

//VOSTOK_INCLUDE_TO_LINKAGE(collision_object_geometry)

VOSTOK_INCLUDE_TO_LINKAGE(sound_entry_point)
VOSTOK_INCLUDE_TO_LINKAGE(sound_library_linkage)
VOSTOK_INCLUDE_TO_LINKAGE(sound_emitter_linkage_id);
VOSTOK_INCLUDE_TO_LINKAGE(sound_debug_stats_id);

VOSTOK_INCLUDE_TO_LINKAGE(engine_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(animation_entry_point)
VOSTOK_INCLUDE_TO_LINKAGE(animation_player_linkage_id)
VOSTOK_INCLUDE_TO_LINKAGE(animation_anim_track)
VOSTOK_INCLUDE_TO_LINKAGE(animation_anim_track_io)

VOSTOK_INCLUDE_TO_LINKAGE(maya_animation_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(input_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(physics_entry_point)
VOSTOK_INCLUDE_TO_LINKAGE(physics_character_controller)
VOSTOK_INCLUDE_TO_LINKAGE(physics_rigid_body_construction_info)

VOSTOK_INCLUDE_TO_LINKAGE(rtp_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(ui_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(ai_navigation_entry_point)

VOSTOK_INCLUDE_TO_LINKAGE(ai_entry_point)

#endif // #ifndef LINKAGE_RESOLVER_H_INCLUDED
