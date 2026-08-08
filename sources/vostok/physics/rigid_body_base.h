////////////////////////////////////////////////////////////////////////////
//	Created		: 21.09.2025
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_RIGID_BODY_BASE_H_INCLUDED
#define VOSTOK_PHYSICS_RIGID_BODY_BASE_H_INCLUDED

#include <vostok/physics/base_physics_object.h>

class btRigidBody;

namespace vostok {
namespace physics {

class  bt_rigid_body_base : public base_physics_object
{
public:
	virtual					~bt_rigid_body_base		( )														= 0;
	virtual btRigidBody*	get_rigid_body			( )														= 0;

	virtual u16				get_triangle_material	( s32 triangle_id, bool is_shape_index ) const			= 0;
	virtual void			apply_impulse			( float3 const& impulse, float3 const& point_in_world )	= 0;

	virtual void			set_transform			( float4x4 const& transform )							= 0;
	virtual float4x4		get_transform			( ) const												= 0;
}; // class bt_rigid_body_base

inline bt_rigid_body_base::~bt_rigid_body_base( ) { }

STATIC_SIZE_ASSERT(bt_rigid_body_base, 0xC);

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_RIGID_BODY_BASE_H_INCLUDED
