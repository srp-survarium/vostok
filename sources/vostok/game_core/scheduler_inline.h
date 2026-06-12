////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_INLINE_H_INCLUDED
#define SCHEDULER_INLINE_H_INCLUDED

namespace survarium {

inline scheduler::scheduler( vostok::memory::base_allocator* allocator ) :
	m_inactive_objects	( allocator ),
	m_active_objects	( allocator ),
	m_current_index		( 0 )
{
    m_objects[0] = &m_inactive_objects;
    m_objects[1] = &m_active_objects;
}

// STATE[STUB]
inline scheduler::~scheduler( )
{
	// <1>
	// <2>
}

inline void	scheduler::change_status( scheduler::identifier* identifier, scheduler::records_type& dest, scheduler::records_type& src )
{

}

inline scheduler::record& scheduler::register_object( scheduler::identifier* identifier, scheduler::callback const& callback, bool active )
{
	identifier->m_active = active;
	scheduler::records_type& records = objects( identifier );
	identifier->m_id = records.size( );
	records.push_back( scheduler::record( ) );

	scheduler::record& record = records.back( );
	record.m_id = identifier;
	record.m_callback = callback;

	return record;
}

inline void scheduler::register_on_frame( scheduler::identifier* identifier, scheduler::callback const& callback, bool active )
{
	scheduler::record& record = register_object( identifier, callback, active );
	record.m_type			  = 0;
	record.m_max_update_count = 0;
	record.m_last_update_time = 0;
}

inline void scheduler::register_for_update(
	scheduler::identifier*		identifier,
	scheduler::callback const&	callback,
	bool						active,
	u32							update_delta,
	u32							max_update_count,
	u32							time_start_from
)
{
	scheduler::record& record  = register_object( identifier, callback, active );
	record.m_type			   = 1;
	record.m_update_delta	   = update_delta;
	record.m_max_update_count  = max_update_count;
	record.m_last_update_time  = time_start_from;
}

inline void scheduler::unregister( scheduler::identifier* identifier )
{
	if ( m_current_index >= identifier->m_id )
		m_current_index -= 1;

	scheduler::records_type& records = objects( identifier );
	scheduler::record& src = records.back( );
	scheduler::record& dst = records[identifier->m_id];

    dst = src;
	dst.m_id->m_id = identifier->m_id; 
	records.pop_back( );

	identifier->m_active = false;
}

} // namespace survarium

#endif // #ifndef SCHEDULER_INLINE_H_INCLUDED
