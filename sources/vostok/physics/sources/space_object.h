////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.02.2008
//	Author		: Konstantin Slipchenko
//	Description : space_object
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_PHYSICS_SPACE_OBJECT_H_INCLUDED
#define VOSTOK_PHYSICS_SPACE_OBJECT_H_INCLUDED

#include <vostok/render/engine/base_classes.h>

namespace vostok {
namespace render {
	enum debug_user_id;
} // namespace render
} // namespace vostok

namespace vostok {

namespace collision {
	struct space_partitioning_tree;
} // namespace collision

namespace render{

class world;

} // namespace base

namespace physics {
	class space_item;
} // namespace physics

} // namespace vostok


class island;
class collision_space;
class collide_element_interface;

class	space_object
{
public:
	
			void			space_insert		( vostok::collision::space_partitioning_tree & space );
			void			space_remove		( );

public:
	virtual	void			collide				( island& i, space_object& o, bool reverted_order  )						= 0;

	virtual	bool			collide_detail		( island& i, space_object& o, bool reverted_order  )						= 0;
	virtual	bool			collide				( island& i, collide_element_interface& e, bool reverted_order  )			= 0;
	virtual	void			aabb				( float3 &center, float3 &radius )const										= 0;
			void			move				( )																			;
	virtual	bool			aabb_test			( math::aabb const& aabb ) const											= 0;
	virtual	void			render				( render::scene_ptr const& scene, render::debug::renderer& renderer )const	= 0;

private:
	virtual	void			space_check_validity( )const																	= 0;

public:
	inline	vostok::physics::space_item* get_space_item		( )			{ return m_space_item; }
	inline	vostok::physics::space_item const* get_space_item	( ) const	{ return m_space_item; }

protected:
	inline					space_object		( ) : m_space_item( 0 ) { }
							~space_object		( );
private:
	vostok::physics::space_item*	m_space_item;

}; // class	space_object

#endif