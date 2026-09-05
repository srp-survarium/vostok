// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

namespace survarium {

class scheduler : private boost::noncopyable {
public:
	struct identifier {
		u32		m_id	: 31;
		u32		m_active: 1;
	};
	typedef identifier id_type;

	// u32 const time_delta_ms, u32 const current_time_ms
	typedef boost::function< void( u32, u32 ) > callback_type;

	struct callback_record {
		scheduler::identifier*	m_id;
		scheduler::callback_type	m_callback;
	};

	enum type {
		type_on_frame = 0,
		type_fixed_interval = 1,
	};

	struct scheduler_record {
		u32		m_update_delta	: 31;	/// periodic interval
		u32		m_type			: 1;	/// 0/1: per-frame/periodic
		u32		m_max_update_count;		/// upper bound in case of overshoot
		u32		m_last_update_time;
	};

	struct record : public scheduler::callback_record, public scheduler::scheduler_record { };
	typedef vostok::vectora< scheduler::record > OBJECTS;

private:
	/* 0x0000 */	OBJECTS		m_inactive_objects;
	/* 0x0010 */	OBJECTS		m_active_objects;
	/* 0x0020 */	OBJECTS*	m_objects[2];
	/* 0x0028 */	u32			m_current_index;

private:
	inline	void						change_status		( scheduler::id_type* identifier, scheduler::OBJECTS& dest, scheduler::OBJECTS& src );
	inline	scheduler::OBJECTS&		objects				( scheduler::id_type* identifier ) { return *m_objects[identifier->m_active]; }
			scheduler::record&			register_object		( scheduler::id_type* identifier, scheduler::callback_type const& callback, const bool active );

private:
			void						on_frame			( scheduler::record& record, const u32 frame_delta, const u32 current_time );
public:
			void						on_frame			( const u32 frame_delta, const u32 current_time );

public:
	inline explicit						scheduler			( vostok::memory::base_allocator* allocator );
										~scheduler			( );

			void						register_on_frame	( scheduler::id_type* identifier, scheduler::callback_type const& callback, const bool active );
			void						register_for_update	(
											scheduler::id_type*			identifier,
											scheduler::callback_type const& callback,
											const bool					active,
											const u32					update_delta,
											const u32					max_update_count,
											const u32					time_start_from
										);
			void						unregister			( scheduler::id_type* identifier );

	inline	void						activate			( scheduler::id_type* identifier );
	inline	void						deactivate			( scheduler::id_type* identifier );
}; // class scheduler

STATIC_SIZE_ASSERT(scheduler, 0x2C);

} // namespace survarium

#include <vostok/game_core/scheduler_inline.h>

#endif // #ifndef SCHEDULER_H_INCLUDED
