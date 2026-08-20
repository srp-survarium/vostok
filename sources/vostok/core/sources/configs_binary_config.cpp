////////////////////////////////////////////////////////////////////////////
//	Created		: 11.02.2009
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "configs_binary_config.h"
#include "configs_binary_config_cook.h"
#include "resources_manager.h"
#include <vostok/configs.h>
#include <vostok/resources.h>

using vostok::core::configs::binary_config;
using vostok::configs::binary_config_value;
using vostok::memory::base_allocator;

binary_config::binary_config		(pcbyte const buffer, u32 const buffer_size, base_allocator* allocator) :
	m_allocator		( allocator )
{
	m_root			= 0;
	load			( buffer, buffer_size );
}

binary_config::~binary_config()
{
	VOSTOK_FREE_IMPL	( m_allocator, m_root );
}

void binary_config::load			( pcbyte const buffer, u32 const buffer_size )
{
	pcbyte reader	= buffer;

	m_root			= static_cast<binary_config_value*>( VOSTOK_MALLOC_IMPL(m_allocator, buffer_size, "binary_config") );
	memory::copy	( m_root, buffer_size, reader, buffer_size );

	m_root->fix_up	( horrible_cast<binary_config_value*,size_t>(m_root).second );
}

vostok::configs::binary_config_ptr vostok::configs::create_binary_config ( mutable_buffer const& buffer, memory::base_allocator& allocator )
{
	vostok::core::configs::binary_config* const result =
		VOSTOK_NEW_IMPL( allocator, vostok::core::configs::binary_config ) (
		(pcbyte)buffer.c_ptr(),
		buffer.size(),
		&allocator
		);

	resources::g_resources_manager->get_binary_config_cook().register_object_to_delete( result, threading::current_thread_id() );
	result->set_creation_source (
		resources::resource_base::creation_source_created_by_user,
		"user generated", resources::memory_usage_type( resources::nocache_memory, buffer.size() ));
	return result;
}
