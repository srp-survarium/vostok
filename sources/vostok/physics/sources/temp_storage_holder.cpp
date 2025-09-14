////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.01.2008
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2009
//	all vectors that clear after every island step
////////////////////////////////////////////////////////////////////////////
#include "pch.h"
#include "temp_storage_holder.h"
#include "contact_storage.h"

temp_storage_holder::temp_storage_holder( )
:m_contacts					( VOSTOK_NEW_IMPL( vostok::physics::g_ph_allocator, contact_storage ) ),
m_space_collision_results	( VOSTOK_NEW_IMPL( vostok::physics::g_ph_allocator, vostok::collision::objects_type )( vostok::physics::g_ph_allocator ) ),
m_island_objects_pointers	( vostok::physics::g_ph_allocator ),
m_joints					( vostok::physics::g_ph_allocator ),
m_bodies					( vostok::physics::g_ph_allocator )
{
}

temp_storage_holder::~temp_storage_holder()
{
	VOSTOK_DELETE_IMPL( vostok::physics::g_ph_allocator, m_contacts );
	VOSTOK_DELETE_IMPL( vostok::physics::g_ph_allocator, m_space_collision_results );
}

