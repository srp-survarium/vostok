////////////////////////////////////////////////////////////////////////////
//	Created		: 28.07.2010
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PARTICLE_BASE_PARTICLE_H_INCLUDED
#define VOSTOK_PARTICLE_BASE_PARTICLE_H_INCLUDED

namespace vostok {
namespace particle {

struct base_particle {
	vostok::math::float4	color;
	
	// TODO: remove target_color_y_position!
	float				target_color_y_position; // Used in color_matrix by color_over_life action
	
	vostok::math::float3	position;
	vostok::math::float3	spawn_position;
	vostok::math::float3	old_position;
	
	vostok::math::float3	velocity;
	vostok::math::float3	start_velocity;
	
	vostok::math::float3	size;
	vostok::math::float3	start_size;
	
	base_particle*		next;
	
#pragma warning( push )
#pragma warning( disable : 4201 )
	union {
		float			rotation;		// By X axis, uses for sprites
		struct {
			float		rotationX;		// For meshes
			float		rotationY;		// For meshes
			float		rotationZ;		// For meshes
		};
	};
#pragma warning( pop )
	// TODO: rotation_rateXYZ
	vostok::math::float3	rotation_rate;
	vostok::math::float3	start_rotation_rate;
	
	float				lifetime;
	float				duration;
	
	// TODO: maybe as emitter instance dynamic data?
	float				subimage_index;
	float				next_subimage_index;
	
	vostok::math::float3	prev_offset_position;
	
	float				get_duration		() const;
	float				get_linear_lifetime	() const;
	float				get_lifetime		() const;
	u32					seed				() const;
	bool				is_dead				() const;
	void				kill				();
	void				set_defaults		();
	
private:
	friend class		particle_emitter_instance;
	u32					m_seed;

}; // struct base_particle

typedef intrusive_list< particle::base_particle, particle::base_particle*, &particle::base_particle::next >	particle_list_type;

} // namespace particle
} // namespace vostok

#include "base_particle_inline.h"

#endif // #ifndef VOSTOK_PARTICLE_BASE_PARTICLE_H_INCLUDED
