////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

/* sushi@NOTE:
 * - `m_max_update_count` seems useless. It doesn't specify how many times the callback should run,
 *	instead it specifies maximum bound in case of "overshoot".
 * - `scheduler::on_frame(u32, u32)` is consufing, since it iterates over `active_objects` with inactive objects size.
 * - `scheduler::activate` and `scheduler::deactivate` seems to be inlined at usage sites.
 * - Matching anything completely failed. Though the logic seems to be trivial enough.
 */

namespace survarium {

class scheduler : public boost::noncopyable {
public:
	// u32 const time_delta_ms, u32 const current_time_ms
	typedef boost::function< void( u32, u32 ) >	callback;

	struct identifier {
		u32		m_id	: 31;
		u32		m_active: 1;
	};

	struct callback_record {
		scheduler::identifier*	m_id;
		scheduler::callback		m_callback;
	};

	struct scheduler_record {
		u32		m_update_delta	: 31;	/// periodic interval
		u32		m_type			: 1;	/// 0/1: per-frame/periodic
		u32		m_max_update_count;		/// upper bound in case of overshoot
		u32		m_last_update_time;		/// misnomer, next time the task should be fired
	};

	struct record : public scheduler::callback_record, public scheduler::scheduler_record { };
	typedef vostok::vectora< scheduler::record >	records_type;

private:
	inline	void						change_status		( scheduler::identifier* identifier, scheduler::records_type& dest, scheduler::records_type& src );
	inline	scheduler::records_type&	objects				( scheduler::identifier* identifier ) { return *m_objects[identifier->m_active]; }
			scheduler::record&			register_object		( scheduler::identifier* identifier, scheduler::callback const& callback, const bool active );

public:
			void						on_frame			( const u32 frame_delta, const u32 current_time );
private:
			void						on_frame			( scheduler::record& record, const u32 frame_delta, const u32 current_time );

public:
	inline explicit						scheduler			( vostok::memory::base_allocator* allocator );
										~scheduler			( );

			void						register_on_frame	( scheduler::identifier* identifier, scheduler::callback const& callback, const bool active );
			void						register_for_update	(
											scheduler::identifier*		identifier,
											scheduler::callback const&	callback,
											const bool					active,
											const u32					update_delta,
											const u32					max_update_count,
											const u32					time_start_from
										);
			void						unregister			( scheduler::identifier* identifier );

	inline	void						activate			( scheduler::identifier* identifier ) { /* no source */ }
	inline	void						deactivate			( scheduler::identifier* identifier ) { /* no source */ }


private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	scheduler::records_type		m_inactive_objects;
	/* 0x0010 */	scheduler::records_type		m_active_objects;
	/* 0x0020 */	scheduler::records_type*	m_objects[2];
	/* 0x0028 */	u32							m_current_index;
}; // class scheduler

STATIC_SIZE_ASSERT(scheduler, 0x2C);

} // namespace survarium

#include <vostok/game_core/scheduler_inline.h>

#endif // #ifndef SCHEDULER_H_INCLUDED
