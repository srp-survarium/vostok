////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/engine/api.h>
#include "engine_world.h"
#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(engine_entry_point)

typedef vostok::memory::doug_lea_allocator_type	doug_lea_allocator_type;
doug_lea_allocator_type							vostok::engine::g_allocator;

static vostok::uninitialized_reference<vostok::engine::engine_world>	s_world;

void vostok::engine::preinitialize				( vostok::engine_user::module_proxy& proxy, pcstr command_line, pcstr application, pcstr build_date )
{
	VOSTOK_CONSTRUCT_REFERENCE		( s_world, engine_world )	( proxy, command_line, application, build_date );
}

void vostok::engine::initialize		( )
{
	s_world->initialize				( );
}

void vostok::engine::finalize						( )
{
	VOSTOK_DESTROY_REFERENCE			( s_world );
}

void vostok::engine::execute						( )
{
	s_world->run					( );
}

bool vostok::engine::command_line_editor			( )
{
	return							s_world->command_line_editor( );
}

bool vostok::engine::command_line_no_splash_screen( )
{
	return							s_world->command_line_no_splash_screen( );
}

int vostok::engine::get_exit_code					( )
{
	return s_world->get_exit_code	( );
}
