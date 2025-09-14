////////////////////////////////////////////////////////////////////////////
//	Created 	: 10.02.2008
//	Author		: Konstantin Slipchenko
//	Description : joint
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_PHYSICS_JOINT_H_INCLUDED
#define VOSTOK_PHYSICS_JOINT_H_INCLUDED

namespace vostok {
namespace render {
	enum debug_user_id;
} // namespace render
} // namespace vostok

namespace vostok {
namespace render {
namespace debug {
	class renderer;
}
	class world;
} // namespace render
} // namespace vostok

class body;
class island;

class joint
{
public:
	virtual	void	connect		( body	*b0, body	*b1 )								=0;
	virtual	void	connect		( body	*b )											=0;
	virtual	void	integrate	( float	time_delta )									=0;
	virtual	void	fill_island ( island	&i )										=0;
	virtual	void	set_anchor  ( const float3	&anchor )								=0;
	virtual	void	get_anchor  ( float3	&anchor )const								=0;
	virtual	void	render		( vostok::render::scene_ptr const& scene, vostok::render::debug::renderer& renderer ) const	=0;
	virtual			~joint		( ) {}

protected:
	joint	(){}

};

#endif