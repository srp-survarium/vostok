////////////////////////////////////////////////////////////////////////////
//	Created 	: 07.10.2008
//	Author		: Sergey Chechin
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef LOGGING_LOG_FILE_H_INCLUDED
#define LOGGING_LOG_FILE_H_INCLUDED

#include <vostok/platform_pointer.h>
#include <vostok/threading_functions.h>
#include <vostok/fs/file_type.h>
#include <vostok/fs/synchronous_device_interface.h>

namespace vostok {

namespace logging {
	class log_file;
} // namespace logging

namespace core {
	enum log_file_usage;
	extern logging::log_file* g_log_file;
} // namespace core

namespace memory {
	class base_allocator;
} // namespace memory

namespace logging {

enum log_file_usage_enum
{
	no_log,
	create_log,
	append_log,
}; // enum error_mode

enum { max_line_groups	=	1024*4 };
typedef fixed_vector< int, max_line_groups >	line_groups_type;

template class VOSTOK_LOGGING_API uninitialized_reference<line_groups_type>;

class VOSTOK_LOGGING_API log_file : private core::noncopyable {
public:
						log_file				(	memory::base_allocator&				allocator,
													log_file_usage_enum					log_file_usage,
													pcstr								file_name,
													fs_new::device_file_system_proxy	device);
						~log_file				( );

    // writing data
			void		append					( pcstr data, u32 length );
			void		flush					( pcstr file_name );
			void		close					( );

	// reading lines
			void		start_transaction		( );
			void		end_transaction			( );
			void		goto_line				( u32 line );

	template <int buffer_size>
	inline	bool		read_next_line			( char (&buffer)[buffer_size]) { return read_next_line(buffer, buffer_size); }
			bool		read_next_line			( pstr const buffer, const u32 buffer_size);

			u32			get_lines_count			( ) const;
	inline	pcstr		file_name				( ) const { return m_file_name.c_str(); }
	inline	bool		initialized				( ) const { return m_file != 0; }

			void		on_terminate			( );

	inline	memory::base_allocator&		allocator				( ) const { return m_allocator; }
private:
	void				assert_transaction_in_current_thread	( ) const;

	enum { cache_size	= 1024 };
	char				m_cache[cache_size];

private:
	threading::atomic_ptr<fs_new::file_type>::type	m_file;
	fs_new::synchronous_device_interface			m_device;
	fs_new::native_path_string						m_file_name;

	char											m_file_pointer_storage[sizeof(fs_new::file_type) + fs_new::max_path_length];

	uninitialized_reference<line_groups_type>		m_line_groups;	// m_line_groups[x] = group_size*x-th line pos

	threading::mutex			m_log_mutex;
	memory::base_allocator&		m_allocator;

	u32							m_last_line;
	u32							m_current_pos;
	u32							m_file_size;
	int							m_cache_start;
	int							m_cache_size;
	u32							m_transaction_thread_id;

private:
			char		read_next_char			( );

	template <typename processor_type>
	inline	bool		process_next_line		( u32 buffer_size, processor_type const& processor );

			bool		skip_next_line			( );
}; // class log_file

STATIC_SIZE_ASSERT(log_file, 0x4680);

VOSTOK_LOGGING_API	log_file*		new_log_file		(
										memory::base_allocator&				allocator,
										fs_new::device_file_system_proxy&	device,
										pcstr								log_file_name,
										log_file_usage_enum					log_file_usage
									);
VOSTOK_LOGGING_API	void			delete_log_file		( log_file*& log_file );
inline log_file* get_log_file( ) { return core::g_log_file; }


} // namespace logging
} // namespace vostok

#endif // #ifndef LOGGING_LOG_FILE_H_INCLUDED
