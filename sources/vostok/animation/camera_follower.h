////////////////////////////////////////////////////////////////////////////
//	Created		: 16.12.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_CAMERA_FOLLOWER_H_INCLUDED
#define VOSTOK_ANIMATION_CAMERA_FOLLOWER_H_INCLUDED

#include <vostok/animation/api.h>

namespace vostok {

namespace input { struct world; }

class VOSTOK_ANIMATION_API camera_effector {
public:
	virtual void	process_camera	( float4x4 &view_inverted ) = 0;
	virtual void	on_attach		( )							= 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( camera_effector )
};

namespace animation {

struct camera_follower_interface {
	virtual	float4x4	get_object_matrix_for_camera	( ) const = 0;

protected:
	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( camera_follower_interface )
}; // struct camera_follower_interface

class  VOSTOK_ANIMATION_API camera_follower :public vostok::camera_effector 
{
public:
					camera_follower	( );
	virtual			~camera_follower( ) { }
			void	set_object		( camera_follower_interface const *follow_object );
			void	move_control	( float4x4 &view_inverted, input::world& input_world );
			void	update_position	( float delta_height, float delta_dist );
	virtual void	process_camera	( float4x4 &view_inverted );
	virtual void	on_attach		( );

private:
	camera_follower& operator = (camera_follower const &) { NODEFAULT( return *this ); }

public:
	camera_follower_interface const	*m_follow_object;

private:
	float			m_cam_height;
	float			m_cam_dist;
	float3			m_focus_point_position;
	float const		factor;
}; // class camera_follower

} // namespace animation
} // namespace vostok

#endif // #ifndef VOSTOK_ANIMATION_CAMERA_FOLLOWER_H_INCLUDED