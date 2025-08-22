////////////////////////////////////////////////////////////////////////////
//	Created		: 29.07.2010
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_AI_NAVIGATION_WORLD_H_INCLUDED
#define VOSTOK_AI_NAVIGATION_WORLD_H_INCLUDED

#include <vostok/ai_navigation/sources/navigation_mesh_types.h>

namespace vostok {

namespace input {
	struct world;
} // namespace input

namespace ai {
namespace navigation {

class graph_generator;

struct VOSTOK_NOVTABLE world {
	virtual	void	tick			( ) = 0;
	virtual	void	clear_resources	( ) = 0;
	virtual void	load_navmesh		( pcstr filename ) = 0;
	virtual graph_generator* get_graph_generator ( ) = 0;

#ifdef DEBUG
	virtual	void	debug_render		( input::world& input, float3 const& position, float3 const& direction, math::frustum const& frustum ) = 0;

#endif // #ifdef DEBUG


	virtual u32		get_node_id_at		( float3 const& position ) = 0;
	virtual u32		get_node_id_after_move	( u32 old_node_id, float3 const& old_position, float3 const& new_position ) = 0;
	virtual bool	find_path (
						u32 start_node,
						vostok::math::float3 const& start_position,
						u32 goal_node,
						vostok::math::float3 const& goal_position,
						float agent_radius,
						vostok::ai::navigation::path_type& path
					) = 0;
	virtual	bool	can_stand( 
						float3 const& vertex,
						u32 triangle_id,
						float agent_radius
					) = 0;

	VOSTOK_DECLARE_PURE_VIRTUAL_DESTRUCTOR( world );
}; // class world

} // namespace navigation
} // namespace ai
} // namespace vostok

#endif // #ifndef VOSTOK_AI_NAVIGATION_WORLD_H_INCLUDED