////////////////////////////////////////////////////////////////////////////
//	Created		: 11.05.2010
//	Author		: Andrew Kolomiets
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/memory_writer.h>
#include <vostok/fs/synchronous_device_interface.h>
#include <vostok/fs/file_type_pointer.h>
#include <vostok/core_entry_point.h>

#include <vostok/linkage_helper.h>

VOSTOK_DECLARE_LINKAGE_ID(core_memory_writer)

namespace vostok {
namespace memory {

writer_base::writer_base( memory::base_allocator* allocator )
:
m_allocator		( allocator ),
m_chunk_pos		( allocator )			
{
}

writer_base::~writer_base( )	
{
	ASSERT	( m_chunk_pos.empty(), "Opened chunk not closed." );	
}


void writer_base::open_chunk	(u32 type)
{
	write_u32				( type );
	m_chunk_pos.push_back	( tell() );
	write_u32				( 0 );	// the place for 'size'
}

void writer_base::close_chunk	( )
{
	ASSERT					( !m_chunk_pos.empty() );

	int pos					= tell( );
	seek					( m_chunk_pos.back() );
	write_u32				( pos - m_chunk_pos.back() - 4 );
	seek					( pos );
	m_chunk_pos.pop_back	( );
}

u32 writer_base::chunk_size	( )					// returns size of currently opened chunk, 0 otherwise
{
	if (m_chunk_pos.empty())	
		return 0;

	return tell() - m_chunk_pos.back() - 4;
}

void writer_base::write_chunk(u32 type, pvoid data, u32 size)
{
	open_chunk	( type );
	write		( data, size );
	close_chunk	( );
}


writer::writer(memory::base_allocator* allocator) 
:writer_base( allocator )
{
	m_data			= 0;
	m_position		= 0;
	m_mem_size		= 0;
	m_file_size		= 0;
	external_data	= false;
}

writer::~writer() 
{	free_();	}

void writer::free_()			
{
	m_file_size		= 0; 
	m_position		= 0; 
	m_mem_size		= 0; 
	if( !external_data ) 
		VOSTOK_FREE_IMPL( m_allocator, m_data );
}

void writer::write	( pcvoid ptr, u32 count )
{
	if ( m_position + count > m_mem_size ) 
	{

		// reallocate
		if ( m_mem_size == 0 )
			m_mem_size = 128;

		while ( m_mem_size <= ( m_position + count ))
			m_mem_size *= 2;

		if ( 0 == m_data )
			m_data = (pbyte)VOSTOK_MALLOC_IMPL	( m_allocator, m_mem_size, "memory_writer" );
		else				
			m_data = (pbyte)VOSTOK_REALLOC_IMPL	( m_allocator, m_data, m_mem_size, "memory_writer" );
	}

	vostok::memory::copy		( m_data + m_position, m_mem_size - m_position, ptr, count );
	m_position				+= count;

	if ( m_position > m_file_size ) 
		m_file_size = m_position;
}

bool writer::save_to	(pcstr fn, bool deassociate_resource)
{
	using namespace							fs_new;
	synchronous_device_interface const &	device	=	core::get_core_synchronous_device( );
	file_type_pointer	f					(native_path_string::convert(fn), 
											 device, 
 											 file_mode::create_always, 
											 file_access::write, 
											 assert_on_fail_true, 
											 (notify_watcher_bool)deassociate_resource);
	if ( !f )
		return								false;
	
	device->write							(f, pointer(), size());
	return									true;
}

} // namespace memory
} // namespace vostok
