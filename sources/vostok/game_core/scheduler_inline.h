// SPDX-License-Identifier: GPL-3.0-or-later

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

inline scheduler::~scheduler( )
{
}

inline void	scheduler::change_status( scheduler::id_type* identifier, scheduler::OBJECTS& dest, scheduler::OBJECTS& src )
{

}









inline scheduler::record& scheduler::register_object( scheduler::id_type* identifier, scheduler::callback_type const& callback, const bool active )
{
	identifier->m_active = active;
	scheduler::OBJECTS& records = objects( identifier );
	identifier->m_id = records.size( );
	records.push_back( scheduler::record( ) );

	scheduler::record& record = records.back( );
	record.m_id = identifier;
	record.m_callback = callback;

	return record;
}

inline void scheduler::register_on_frame( scheduler::id_type* const identifier, scheduler::callback_type const& callback, const bool active )
{
	scheduler::record& record = register_object( identifier, callback, active );
	record.m_type = type_on_frame; record.m_update_delta = u32(-1); record.m_max_update_count = 0;
	record.m_last_update_time = 0;
}

inline void scheduler::register_for_update(
	scheduler::id_type*			identifier,
	scheduler::callback_type const& callback,
	const bool					active,
	const u32					update_delta,
	const u32					max_update_count,
	const u32					time_start_from
)
{
	scheduler::record& record  = register_object( identifier, callback, active );
	record.m_type			   = type_fixed_interval;
	record.m_update_delta	   = update_delta;
	record.m_max_update_count  = max_update_count;
	record.m_last_update_time  = time_start_from;
}

inline void scheduler::unregister( scheduler::id_type* identifier )
{
	if ( m_current_index >= identifier->m_id )
		m_current_index -= 1;

	scheduler::OBJECTS& records = objects( identifier );
	scheduler::record& src = records.back( );
	scheduler::record& dst = records[identifier->m_id];

    dst = src;
	dst.m_id->m_id = identifier->m_id; 
	records.pop_back( );

	identifier->m_active = false;
}

} // namespace survarium

#endif // #ifndef SCHEDULER_INLINE_H_INCLUDED
