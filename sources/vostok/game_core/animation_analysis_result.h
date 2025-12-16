////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_ANALYSIS_RESULT_H_INCLUDED
#define ANIMATION_ANALYSIS_RESULT_H_INCLUDED

#include <vostok/game_core/leg_key_times.h>

namespace survarium {

class animation_analysis_result : public resources::unmanaged_resource {
public:
	explicit	animation_analysis_result	( u32 legs_count );
	virtual		~animation_analysis_result	( );

public:
	inline	buffer_vector<leg_key_times>&
				key_times	( ) { return m_leg_key_times; }

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	void*							m_buffer;
	/* 0x010c */	buffer_vector<leg_key_times>	m_leg_key_times;
}; // class animation_analysis_result

STATIC_SIZE_ASSERT(animation_analysis_result, 0x118);

} // namespace survarium

#endif // #ifndef ANIMATION_ANALYSIS_RESULT_H_INCLUDED
