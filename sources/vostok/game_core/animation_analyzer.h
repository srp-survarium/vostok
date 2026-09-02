// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef ANIMATION_ANALYZER_H_INCLUDED
#define ANIMATION_ANALYZER_H_INCLUDED

namespace vostok {
namespace animation {
	class skeleton;
}
}

namespace survarium {

struct leg_info;
struct leg_key_times;

class animation_analyzer : private core::noncopyable {
public:
						animation_analyzer		( leg_info const* legs_info, const u32 legs_count, animation::skeleton const& skeleton );
						~animation_analyzer		( );

	inline	void		analyze					( pcstr arg_0, resources::managed_resource_ptr arg_1, leg_key_times* arg_2 ) { /* no source */ }

private:
	struct leg_cycle_sample {
		/* 0x0000 */	float3		heel;
		/* 0x000c */	float3		toetip;
		/* 0x0018 */	float3		middle;
		/* 0x0024 */	float3		foot_base;
	}; // struct leg_cycle_sample

	typedef buffer_vector< leg_cycle_sample > leg_cycle_samples_type;

	inline	void		prepare_samples			( pcstr arg_0, resources::managed_resource_ptr arg_1, leg_cycle_samples_type& arg_2 ) { /* no source */ }
	inline	void		analyse_samples			( leg_cycle_samples_type const& arg_0, const u32 arg_1, leg_key_times* arg_2 ) { /* no source */ }

	inline	float		get_max_range			( leg_cycle_samples_type const& arg_0, const u32 arg_1, const u32 arg_2 ) const { /* no source */ }
	inline	u32			get_stance_index		(
							leg_cycle_samples_type const&	arg_0,
							const u32							arg_1,
							const u32							arg_2,
							const float							arg_3
						) const { /* no source */ }
	inline	float3		get_cycle_center		( leg_cycle_samples_type const& arg_0, const u32 arg_1, const u32 arg_2 ) const { /* no source */ }

	inline	float		find_contact_time		(
							leg_cycle_samples_type const&	arg_0,
							const u32							arg_1,
							const u32							arg_2,
							bool								arg_3,
							u32									arg_4,
							s32									arg_5,
							float								arg_6,
							float								arg_7
						) { /* no source */ }
	inline	float		find_swing_change_time	(
							leg_cycle_samples_type const&	arg_0,
							const u32							arg_1,
							const u32							arg_2,
							u32									arg_3,
							s32									arg_4,
							float								arg_5,
							float3 const&						arg_6
						) { /* no source */ }

private:
	/* 0x0000 */	/* core::noncopyable */
	/* 0x0000 */	leg_info const*					m_legs_info;
	/* 0x0004 */	const u32						m_legs_count;
	/* 0x0008 */	animation::skeleton const&		m_skeleton;
	/* 0x000c */	float							m_ground_height;
}; // class animation_analyzer

STATIC_SIZE_ASSERT(animation_analyzer, 0x10);

} // namespace survarium

#endif // #ifndef ANIMATION_ANALYZER_H_INCLUDED
