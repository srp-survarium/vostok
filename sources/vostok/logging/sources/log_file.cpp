////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.10.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/logging/log_file.h>

#include <vostok/fs/device_utils.h>
#include <vostok/fs/file_type_pointer.h>
#include <vostok/core/core.h>

static const int log_file_group_size = 256;

namespace vostok {
namespace logging {

using namespace fs_new;

// STATE[98.1%|DONE]: core interlocked_exchange_pointer inline-vs-call (same as close).
log_file::log_file				(	memory::base_allocator&		allocator,
									log_file_usage_enum			log_file_usage,
									pcstr						file_name,
									device_file_system_proxy	device )
	:
	m_allocator				( allocator ),
	m_transaction_thread_id ( u32(-1) ),
	m_file					( 0 ),
	m_device				( device )
{
	ASSERT					( file_name && *file_name );
	m_file_name			=	file_name;

	file_mode::mode_enum mode	=	( log_file_usage == logging::create_log ) ?
										file_mode::create_always : file_mode::append_or_create;

	create_folder_r	(m_device, file_name, false);

	file_type *	file				=	(file_type *)& m_file_pointer_storage;
	open_file_params	params			(mode, file_access::read_write, assert_on_fail_false);
	params.file_type_allocated_by_user	=	true;

	if ( m_device->open(& file, file_name, params) )
		threading::interlocked_exchange_pointer	( m_file, file );

	VOSTOK_CONSTRUCT_REFERENCE				(m_line_groups, line_groups_type);
	m_line_groups->push_back (0);

	m_last_line			= 0;
	m_file_size			= 0;
	m_cache_start		= -1;
	m_cache_size		= 0;
	m_current_pos		= 0;

	// STRUCTURE DIFF: target 16 stmts / base 16 stmts
	// SIZE -0x3 | 45 | threading::interlocked_exchange_pointer	( m_file, file );
	// VERDICT: STRUCTURE MATCH (shape ok) - same core interlocked_exchange_pointer call-vs-xchg as close; banked.
}

// STATE[100%|DONE]
log_file::~log_file				( )
{
	close				( );
	VOSTOK_DESTROY_REFERENCE					(m_line_groups);
}

// STATE[75.7%|DONE]: fs-side native_path_string::convert / file_type_pointer ctor shape + core compare_insensitive/c_str inlining. sushi@NOTE: Possibly implementation for them has changed.
void log_file::flush			( pcstr in_file_name )
{
	if ( !m_file )
		return;

	m_device->flush		( m_file );

	// claude@MATCH: target compares the requested name against m_file_name and bails when equal
	// (cmp [ebp+8],0; je return + compare_insensitive(m_file_name.c_str(), in_file_name); je return)
	if ( !in_file_name || !strings::compare_insensitive( m_file_name.c_str(), in_file_name ) )
		return;

	bool const success	= m_device->seek( m_file, 0, seek_file_begin );
	ASSERT_U			( success );

	native_path_string	file_name		=	native_path_string::convert(in_file_name);
	fs_new::create_folder_r					( m_device, file_name, false );

	file_type_pointer file				(file_name, m_device, file_mode::create_always,
										 file_access::write, assert_on_fail_false, notify_watcher_false);

	if ( !file )
		return;

	char				buffer[4096];
	for (;;) {
		file_size_type const	read = m_device->read( m_file, buffer, sizeof( buffer ) );
		if ( read == sizeof( buffer ) ) {
			m_device->write				( file, buffer, sizeof( buffer ) );
			continue;
		}

		m_device->write			( file, buffer, read );
		break;
	}

	// STRUCTURE DIFF: target 19 stmts / base 19 stmts
	// SIZE +0xf  | 74 | if ( !in_file_name || !strings::compare_insensitive( m_file_name.c_str(), in_file_name ) )
	// SIZE +0x2c | 80 | native_path_string	file_name		=	native_path_string::convert(in_file_name);
	// SIZE +0x3d | 84 | 									 file_access::write, assert_on_fail_false, notify_watcher_false);
	// VERDICT: STRUCTURE MATCH (shape ok) - row 74: target calls c_str+compare_insensitive out-of-line (core, flagged);
	// rows 80/84: fs_new convert/file_type_pointer construct differently (fs-side headers, flagged - do not edit from logging).
}

// STATE[100%|DONE]
void log_file::append			( pcstr data, u32 const length )
{
	if ( length == 0 )	// <0x65b5c9>|0x000|0x000:'100'
		return;			// <0x65b5cf>|0x006|0x006:'101'
	ASSERT				( length );
	ASSERT				( data );
	ASSERT				( m_file );
	bool const seek_res	= m_device->seek(m_file, 0, seek_file_end);
	ASSERT_U			( seek_res );

	file_size_type pos	= m_device->tell( m_file );

	file_size_type const	num_written = m_device->write(m_file, data, length);
	ASSERT				( num_written == length );

	m_file_size			+= (int)num_written;

	pcstr const			last_symbol = data + length;
	while ( *data )
	{
		pcstr const		next_line = strchr( data, '\n' );
		size_t const 	line_length = next_line ? ( next_line-data+1 ) : ( last_symbol - data );

		pos				+= (u32)line_length;
		if ( next_line )
		{
			++m_last_line;

			u32 const line_groups_needed = (m_last_line / log_file_group_size) + 1;

			if ( m_line_groups->size() < line_groups_needed &&
				 m_line_groups->size() < m_line_groups->max_size() &&
				 !(m_last_line % log_file_group_size) )
			{
				m_line_groups->push_back((int)pos);
			}
		}

		data			+= line_length;
	}
}

// STATE[92%|DONE]: LTCG for `mutex::lock`.
void log_file::start_transaction	( )
{
	m_log_mutex.lock( );
	R_ASSERT						( m_transaction_thread_id == u32(-1),
									"transaction was not ended or mutex was unlocked by "
									"someone other then end_transaction");
	m_transaction_thread_id		=	threading::current_thread_id();
}

// STATE[100%|DONE]
void log_file::assert_transaction_in_current_thread	( ) const
{
	R_ASSERT						( m_transaction_thread_id != u32(-1),
									 "you must call start_transaction first");
	R_ASSERT						( m_transaction_thread_id == threading::current_thread_id(),
									 "transaction was started in another thread");
}

// STATE[99%|DONE]: LTCG for `mutex::unlock`.
void log_file::end_transaction		( )
{
	assert_transaction_in_current_thread	( );
	m_transaction_thread_id		=	u32(-1);
	m_log_mutex.unlock( );
}

// STATE[100%|DONE]
u32	log_file::get_lines_count		( ) const
{
	assert_transaction_in_current_thread	( );
	return							m_last_line;
}

// STATE[100%|DONE]
void log_file::goto_line		( u32 const line )
{
	assert_transaction_in_current_thread	( );
	ASSERT				(line  <= m_last_line);
	u32 group			= line / log_file_group_size;
	if ( group >= m_line_groups->size() )
		group			= m_line_groups->size() - 1;

	m_current_pos		= (* m_line_groups)[group];

	bool const seek_result = m_device->seek( m_file, m_current_pos, seek_file_begin );
	ASSERT_U			( seek_result );

	u32 num2skip		= line - log_file_group_size*group;

	for ( ; num2skip; --num2skip )
		skip_next_line	( );
}

// STATE[100%|DONE]
char log_file::read_next_char	( )
{
	int const cache_offs= m_current_pos - m_cache_start;

	++m_current_pos;

	if ( ( m_cache_start != -1 ) && ( cache_offs >= 0 ) && ( cache_offs < m_cache_size ) )
		return			( m_cache[cache_offs] );

	m_cache_start		= m_current_pos - 1;
	m_device->seek		( m_file, m_cache_start, seek_file_begin );

	m_cache_size		= (int)m_device->read( m_file, m_cache, cache_size );
	ASSERT				(m_cache_size != 0);
	return				( m_cache[0] );
}

// STATE[76%|DONE]: core math::min called out-of-line in target (both instantiations), inlined in base
// STRUCTURE DIFF (both instantiations): target 14 stmts / base 14 stmts
// SIZE +0x18 | 224 | int const last_pos	= math::min(m_file_size, m_current_pos+buffer_size-1);
// VERDICT: STRUCTURE MATCH (shape ok) - core-side inline-vs-call, banked.
template <typename processor_type>
bool log_file::process_next_line ( u32 const buffer_size, processor_type const& processor )
{
	if ( m_current_pos == m_file_size )
	{
		return			false;
	}

	int const last_pos	= math::min(m_file_size, m_current_pos+buffer_size-1);
	char current_char	= 0;

	while ( (int)m_current_pos < last_pos ) {
		current_char	= read_next_char();
		processor		 (current_char);

		if ( current_char == '\n' )
			break;
	}

	processor			( 0 );

	// if not on eol, read till eol or eof
	while ( ( current_char != '\n' ) && ( m_current_pos != m_file_size ) )
		current_char	= read_next_char( );

	return				true;
}

struct processor {
	mutable pstr buffer_ptr;

	inline	processor		( pstr buffer ) : buffer_ptr(buffer) {}

	inline void operator( ) ( char const character ) const
	{
		*buffer_ptr = character;
		++buffer_ptr;
	}
};

STATIC_SIZE_ASSERT(processor, 0x4);

// STATE[99.8%|DONE]: frame 0x18 vs target 0x10 (slot allocation only, identical statements/instructions)
bool log_file::read_next_line	(pstr const buffer, const u32 buffer_size)
{
	assert_transaction_in_current_thread	( );

	return				( process_next_line( buffer_size, processor( buffer ) ) );

	// STRUCTURE DIFF: target 2 stmts / base 2 stmts (no diverging rows, sizes equal)
	// VERDICT: STRUCTURE MATCH - residual is frame-size/slot allocation (sub esp,18h vs 10h), LTCG; banked.
}

// STATE[99.8%|DONE]: same slot-allocation-only residual as read_next_line
bool log_file::skip_next_line	( )
{
	struct processor {
		static void dummy (char const character) { VOSTOK_UNREFERENCED_PARAMETER(character); }
	};

	return				( process_next_line ( 0, &processor::dummy ) );

	// STRUCTURE DIFF: target 1 stmt / base 1 stmt (no diverging rows, sizes equal)
	// VERDICT: STRUCTURE MATCH - residual is slot allocation in the call shape, LTCG; banked.
}

} // namespace logging
} // namespace vostok

#include <vostok/os_include.h>	// for MemoryBarrier

namespace vostok {
namespace logging {

// STATE[78.8%|DONE]: core threading::interlocked_exchange_pointer called out-of-line in target, inlined to xchg in base
void log_file::close		( )
{
	if ( !m_file )
		return;

	file_type* file	= m_file;

	threading::interlocked_exchange_pointer( m_file, (file_type*)0 );
	VOSTOK_MEMORY_BARRIER_FULL();

	m_device->flush		( file );
	m_device->close		( file );

	// STRUCTURE DIFF: target 7 stmts / base 7 stmts
	// SIZE -0x3 | 292 | threading::interlocked_exchange_pointer( m_file, (file_type*)0 );
	// VERDICT: STRUCTURE MATCH (shape ok) - target calls core interlocked_exchange_pointer (ecx/eax conv), base inlines xchg; core-side, banked.
}

// STATE[STUB]
void log_file::on_terminate			( )
{
	close			( );
}

// STATE[92%|DONE]: LTCG for malloc
log_file* new_log_file(
	memory::base_allocator&				allocator,
	fs_new::device_file_system_proxy&	device,
	pcstr								log_file_name,
	log_file_usage_enum					log_file_usage
)
{
	return VOSTOK_NEW_IMPL( allocator, log_file )( allocator, log_file_usage, log_file_name, device ); 	// <0x65bcd6>|0x000|0x000:'297'
}

// STATE[100%|DONE]
void delete_log_file( log_file*& log_file )
{
	if ( log_file )												// <0x65b9d6>|0x000|0x000:'302'
		VOSTOK_DELETE_IMPL( log_file->allocator(), log_file );	// <0x65b9de>|0x008|0x008:'303'
}

} // namespace logging
} // namespace vostok