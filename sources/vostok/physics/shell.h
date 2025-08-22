////////////////////////////////////////////////////////////////////////////
//	Created 	: 17.02.2008
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_SHELL_H_INCLUDED
#define VOSTOK_PHYSICS_SHELL_H_INCLUDED

namespace vostok {
namespace physics {

struct world;

struct VOSTOK_NOVTABLE update_object_interface {

	virtual void	physics_update_transform( float4x4 const* transforms, u32 transfrorms_count ) = 0;

	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( update_object_interface )
}; // struct shell


struct VOSTOK_NOVTABLE shell {

	virtual void	set_update_object	( vostok::physics::update_object_interface *object ) = 0;
	virtual void	deactivate			( world& w ) = 0;
	virtual void	move				( vostok::math::float3 const &delta ) = 0;

	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( shell )
}; // struct shell

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_SHELL_H_INCLUDED