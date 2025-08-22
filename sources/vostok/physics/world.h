////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.01.2008
//	Author		: Dmitriy Iassenev
//	Description : default world interface
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_PHYSICS_WORLD_H_INCLUDED
#define VOSTOK_PHYSICS_WORLD_H_INCLUDED

#include <vostok/animation/type_definitions.h> 
#include <vostok/render/engine/base_classes.h>
#include <vostok/physics/ray_result.h>

class btDynamicsWorld;

namespace vostok {
namespace render {

namespace debug {
	class renderer;
} // namespace debug
} // namespace render

namespace collision {
	class geometry_instance;
	struct space_partitioning_tree;
} // namespace collision

namespace physics {

struct handler;
struct shell;
class bt_rigid_body;
class bt_soft_body_rope;

struct VOSTOK_NOVTABLE world {
	virtual										~world				( )												{};
	virtual	void								tick				( )												= 0;
	virtual	void								clear_resources		( )												= 0;
	virtual void								initialize			( )												= 0;
	//virtual	non_null<shell>::ptr				new_physics_shell	( const configs::binary_config_value& cfg )		= 0;
	//virtual	non_null<shell>::ptr				new_physics_shell	( non_null<collision::geometry_instance>::ptr g_instance ) = 0;

	//virtual	void								delete_shell		( non_null<shell>::ptr sh )						= 0;
	//virtual	non_null<shell>::ptr				new_static_shell	( const configs::binary_config_value& cfg )		= 0;
	//virtual	non_null<shell>::ptr				new_static_shell	( non_null<collision::geometry_instance>::ptr g_instance ) = 0;

	virtual	void								debug_render		( vostok::render::scene_ptr const& scene, vostok::render::debug::renderer& renderer ) const = 0;
	virtual	void								set_ready			( bool v )										= 0;
//	virtual	void								set_space			( vostok::collision::space_partitioning_tree *space )	= 0;

	virtual	void								create_test_scene( )											= 0;
	
	virtual	void		add_rigid_body			( bt_rigid_body* )	=0;
	virtual	void		remove_rigid_body		( bt_rigid_body* )	=0;
	virtual	void		add_soft_body			( bt_soft_body_rope* )	=0;
	virtual	void		remove_soft_body		( bt_soft_body_rope* )	=0;
	virtual closest_ray_result ray_test			( float3 const& ray_from, float3 const& ray_dir, float const ray_length ) = 0;
//	virtual	 collision::space_partitioning_tree* get_collision_tree	( ) const										= 0;

}; // class world

} // namespace physics
} // namespace vostok

#endif // #ifndef VOSTOK_PHYSICS_WORLD_H_INCLUDED
