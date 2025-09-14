////////////////////////////////////////////////////////////////////////////
//	Created 	: 22.12.2009
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "sound_world.h"
#include <vostok/sound/api.h>
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(sound_entry_point)

using vostok::sound::sound_world;

static vostok::uninitialized_reference<sound_world>	s_world;

namespace vostok {
namespace sound {

static bool is_enum_exported = false;

#ifndef MASTER_GOLD
static configs::enum_tuple tuples[] =
{
	std::make_pair( "single_sound_type",		resources::single_sound_class	  ),
	std::make_pair( "composite_sound_type",		resources::composite_sound_class  ),
	std::make_pair( "collection_sound_type",	resources::sound_collection_class )
};
#endif // #ifndef MASTER_GOLD

} // namespace sound
} // namespace vostok

vostok::sound::allocator_type* vostok::sound::g_allocator = 0;

vostok::sound::world* vostok::sound::create_world	( engine& engine, base_allocator_type& logic_allocator, base_allocator_type* editor_allocator )
{
#ifndef MASTER_GOLD	
	if ( !vostok::sound::is_enum_exported )
	{
		vostok::configs::export_enum( "resources", vostok::sound::tuples, vostok::sound::tuples + array_size( tuples ) );
		vostok::sound::is_enum_exported = true;
	}
#endif // #ifndef MASTER_GOLD
	
	VOSTOK_CONSTRUCT_REFERENCE	( s_world, sound_world )( engine, logic_allocator, editor_allocator );
	return						( &*s_world );
}

void vostok::sound::destroy_world	( vostok::sound::world*& world )
{
	R_ASSERT					( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE		( s_world );
	world						= 0;
}

void vostok::sound::set_memory_allocator	( allocator_type& allocator )
{
	ASSERT						( !g_allocator );
	g_allocator					= &allocator;
}
