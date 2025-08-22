////////////////////////////////////////////////////////////////////////////
//	Created 	: 28.10.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "xbox360_world.h"
#include <vostok/render/api.h>
#include <vostok/render/base/api.h>

using vostok::render::xbox360::world;

static vostok::uninitialized_reference<world>	s_world;

vostok::render::world* vostok::render::xbox360::create_world	( render::engine::wrapper& wrapper )
{
	VOSTOK_CONSTRUCT_REFERENCE( s_world, xbox360::world )	( wrapper );
	return					( &*s_world );
}

void vostok::render::xbox360::destroy_world							( vostok::render::world*& world )
{
	R_ASSERT				( &*s_world == world );
	VOSTOK_DESTROY_REFERENCE	( s_world );
	world					= 0;
}