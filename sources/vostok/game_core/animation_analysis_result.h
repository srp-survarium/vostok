// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_ANALYSIS_RESULT_H_INCLUDED
#define ANIMATION_ANALYSIS_RESULT_H_INCLUDED

#include <vostok/game_core/leg_key_times.h>

namespace survarium {

class animation_analysis_result : public resources::unmanaged_resource {
public:
	typedef buffer_vector< leg_key_times > leg_key_times_buffer_type;

	explicit	animation_analysis_result	( const u32 legs_count );
	virtual		~animation_analysis_result	( );

public:
	inline	leg_key_times_buffer_type&
				key_times	( ) { return m_leg_key_times; }

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	void*							m_buffer;
	/* 0x010c */	leg_key_times_buffer_type		m_leg_key_times;
}; // class animation_analysis_result

STATIC_SIZE_ASSERT(animation_analysis_result, 0x118);

} // namespace survarium

#endif // #ifndef ANIMATION_ANALYSIS_RESULT_H_INCLUDED
