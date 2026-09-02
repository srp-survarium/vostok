// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LEGS_IK_PROCESSOR_H_INCLUDED
#define LEGS_IK_PROCESSOR_H_INCLUDED

#include <vostok/game_core/ik_processor.h>
#include <vostok/animation/api.h>
#include <vostok/animation/fermi_interpolator.h>

namespace vostok {
namespace physics {
	class bt_character_controller;
}
namespace animation {
	class skeleton_bone;
}
}

namespace survarium {

class legs_ik_drawer;

class legs_ik_processor : public ik_processor {
public:
			explicit	legs_ik_processor		( );
						~legs_ik_processor		( );

			void		activate				( animation::skeleton const& skeleton );

	inline	void		set_drawer				( legs_ik_drawer* drawer ) { /* no source */ }
	inline	void		set_character_controller( physics::bt_character_controller* controller ) { m_character_controller = controller; }

			void		process					( float4x4* matrices, float4x4 const& transform );

			void		set_left_heel_on_ground	( bool value );
			void		set_left_toe_on_ground	( bool value );
			void		set_right_heel_on_ground( bool value );
			void		set_right_toe_on_ground	( bool value );

			void		tick					( const u32 current_time_in_ms );

	inline	bool		is_right_supporting_leg	( ) const { /* no source */ }

public:
	struct leg_params {
	public:
				explicit	leg_params			( );

				void	activate				( animation::skeleton const& skeleton, pcstr foot_bone_name );
				void	tick					( float dt );

				void	set_heel_transition_time( float tr_time );
				void	set_toe_transition_time	( float tr_time );
		inline	void	set_rotation_axis		( float3 const& arg_0 ) const { /* no source */ }
				void	set_heel_on_ground		( const bool value );
				void	set_toe_on_ground		( const bool value );

		inline	bool	is_heel_on_ground		( ) const { return m_heel_on_ground; }
		inline	bool	is_toe_on_ground		( ) const { return m_toe_on_ground; }
		inline	bool	is_on_ground			( ) const { return is_heel_on_ground( ) || is_toe_on_ground( ); }
		inline	bool	is_full_on_ground		( ) const { return is_heel_on_ground( ) && is_toe_on_ground( ); }

		inline	bool	is_more_supporting_then	( legs_ik_processor::leg_params const& arg_0 ) const { /* no source */ }


	public:
		/* 0x0000 */	u32			foot_bone_index;
		/* 0x0004 */	u32			toe_bone_index;
		/* 0x0008 */	u32			leg_bone_index;
		/* 0x000c */	u32			knee_bone_index;
		/* 0x0010 */	u32			up_leg_bone_index;
		/* 0x0014 */	float		heel_transition_time;
		/* 0x0018 */	float		toe_transition_time;
		/* 0x001c */	float3		rotation_axis;
	private:
		/* 0x0028 */	float		m_time_since_stance;
		/* 0x002c */	bool		m_heel_on_ground;
		/* 0x002d */	bool		m_toe_on_ground;
	}; // struct leg_params

private:
	class transition_time_calculator {
	public:
		inline	explicit	transition_time_calculator	( ) : m_value( 0.1f ) { }

		inline	void		reset						( ) { m_value = 0.0f; }
		inline	void		tick						( float arg_0 ) { m_value += arg_0; }
		inline	float		get_value					( ) const { return m_value; }

	private:
		/* 0x0000 */	float		m_value;
	}; // class transition_time_calculator

private:
			void		set_heel_on_ground		( legs_ik_processor::leg_params& params, bool value );
			void		set_toe_on_ground		( legs_ik_processor::leg_params& params, bool value );

			float4x4	get_foot_fixed_transform(
							legs_ik_processor::leg_params const&	params,
							float4x4 const&						hip_world_matrix,
							float4x4 const*						matrices,
							float&								delta_len
						) const;

			void		process_leg				(
							legs_ik_processor::leg_params&		params,
							float4x4 const&						target_foot_obj_matrix,
							float4x4 const&						hip_obj_matrix,
							float4x4*							matrices,
							float4x4 const&						transform
						);


private:
	/* 0x0000 */	/* ik_processor */
	/* 0x0008 */	legs_ik_drawer*						m_drawer;
	/* 0x000c */	physics::bt_character_controller*	m_character_controller;
	/* 0x0010 */	legs_ik_processor::leg_params		m_left_leg_params;
	/* 0x0040 */	legs_ik_processor::leg_params		m_right_leg_params;

private:
	/* 0x0070 */	animation::fermi_interpolator		m_heel_interpolator;
	/* 0x007c */	animation::fermi_interpolator		m_toe_interpolator;

	/* 0x0088 */	legs_ik_processor::transition_time_calculator	m_heel_transition_time_calculator;
	/* 0x008c */	legs_ik_processor::transition_time_calculator	m_toe_transition_time_calculator;

private:
	/* 0x0090 */	animation::skeleton_bone const*		m_hip_bone;
	/* 0x0094 */	float								m_heel_transition_time;
	/* 0x0098 */	float								m_toe_transition_time;
}; // class legs_ik_processor

STATIC_SIZE_ASSERT(legs_ik_processor, 0x9C);

} // namespace survarium

#endif // #ifndef LEGS_IK_PROCESSOR_H_INCLUDED
