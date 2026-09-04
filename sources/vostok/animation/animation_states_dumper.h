// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_ANIMATION_STATES_DUMPER_H_INCLUDED
#define ANIMATION_ANIMATION_STATES_DUMPER_H_INCLUDED

/* INCLUDES */
class vostok::core::noncopyable;

/* FORWARD REFS */
class vostok::animation::mixing::animation_interval;

namespace vostok {
namespace animation {

class VOSTOK_ANIMATION_API animation_states_dumper : private core::noncopyable {
public:
	// STATE[UNMATCHABLE]: no client instantiation exposes these base hook bodies.
	virtual			~animation_states_dumper( ) { }

	inline	u32		current_time_in_ms		( ) const { return m_current_time_in_ms; }

	virtual	void	on_started_dump			( u32 current_time_in_ms ) { m_current_time_in_ms = current_time_in_ms; }
	virtual	void	on_finished_dump		( ) { }

	virtual	void	dump_state				(
						pcstr const								arg_0,
						const u8								arg_1,
						const bool								arg_2,
						pcstr const								arg_3,
						pcvoid const							arg_4,
						const u32								arg_5,
						const u32								arg_6,
						const u32								arg_7,
						const u32								arg_8,
						const float								arg_9,
						const float								arg_10,
						const float								arg_11,
						const float								arg_12,
						const float								arg_13,
						mixing::animation_interval const*		arg_14,
						const u32								arg_15,
						const u32								arg_16,
						const float								arg_17,
						const u32								arg_18
					) = 0;

protected:
	inline			animation_states_dumper	( ) { /* no source */ }

	/* 0x0004 */	/* core::noncopyable */
	/* 0x0004 */	u32		m_current_time_in_ms;
}; // class animation_states_dumper

STATIC_SIZE_ASSERT(animation_states_dumper, 0x8);

} // namespace animation
} // namespace vostok

#endif // #ifndef ANIMATION_ANIMATION_STATES_DUMPER_H_INCLUDED
