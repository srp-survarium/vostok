////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_INLINE_H_INCLUDED
#define SCHEDULER_INLINE_H_INCLUDED

namespace survarium {

// STATE[INLINED]: Shouldn't be generated in target
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

// STATE[INLINED]: Shouldn't be generated in target.
inline void	scheduler::change_status( scheduler::identifier* identifier, scheduler::records_type& dest, scheduler::records_type& src )
{

}

// STATE[4.0%|BLOCKED]
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

	// FUNCTION BODY[0x92cf0]: 27
	// <1>
	// <17>
	// <0x92cf9>|0x000|0x000:'60'
	// <1>
	// <2>
	// <0x92d0e>|0x015|0x015:'63' records.push_back( scheduler::record( ) );
	// <1>
	// <0x92d7c>|0x083|0x06e:'65'
	// <1>
	// <0x92d7f>|0x086|0x003:'67' record.m_callback = callback;
	// <1>
	// <2>
	// ******
}

// STATE[BLOCKED]: 
inline void scheduler::register_on_frame( scheduler::identifier* identifier, scheduler::callback const& callback, bool active )
{
	scheduler::record& record = register_object( identifier, callback, active );
	record.m_type			  = 0;
	record.m_max_update_count = 0;
	record.m_last_update_time = 0;

	// FUNCTION BODY[0x96a60]: 5
	// <0x96a60>|0x000|0x000:'74'
	// <1>
	// <2>
	// <0x96a71>|0x011|0x011:'77'
	// <0x96a7d>|0x01d|0x00c:'78'
	// ******
}

// STATE[BLOCKED]: Didn't 
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

	// FUNCTION BODY[0x92da0]: 8
	// <0x92da0>|0x000|0x000:'88'
	// <1>
	// <2>
	// <3>
	// <4>
	// <5>
	// <0x92db6>|0x016|0x016:'94'
	// <0x92dba>|0x01a|0x004:'95'
	// ******
}

// STATE[0.0%|BLOCKED]: Everything inlined differently. Most likely `dst = src` is `change_status`, but then the structure with the second breakpoint doesn't make sense
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

	// FUNCTION BODY[0x92de0]: 11
	// <0x92de0>|0x000|0x000:'100'	if ( m_current_index >= identifier->m_id )
	// <0x92df7>|0x017|0x017:'101'		m_current_index -= 1;
	// <1>
	// <0x92dfb>|0x01b|0x004:'103'	scheduler::records_type& records = objects( identifier );
	// <1>
	// <2>
	// <0x92e06>|0x026|0x00b:'106'	dst = src;
	// <0x92e3d>|0x05d|0x037:'107'	dst.m_id->m_id = identifier->m_id; 
	// <0x92e55>|0x075|0x018:'108'	records.pop_back( ); // has destroy inside
	// <1>
	// <0x92e83>|0x0a3|0x02e:'110'	identifier->m_active = false;
	// ******
}

} // namespace survarium

#endif // #ifndef SCHEDULER_INLINE_H_INCLUDED
