////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_PROJECT_H_INCLUDED
#define GAME_PROJECT_H_INCLUDED

#include <vostok/resources_unmanaged_resource.h>
#include <vostok/configs.h>	// configs::binary_config_ptr (value member)
#include <vostok/game_core/base_project.h>

namespace vostok {
namespace resources {
	class queries_result;
} // namespace resources
} // namespace vostok

namespace survarium {

class artefact_container_core;
class base_game_scene;
class collision_geometry;
class damage_zone;
class game_object_;
class generic_anomaly_core;
class ladder;
class scheduler;
class victory_items_container;
class victory_items_container_core;
struct render_visual;
struct respawn_point_core;

// canonical UDT aliases - damage_zone/ladder themselves are batch-11/game_core
// types, only the resource_ptr is held by value here
typedef resources::resource_ptr<
	damage_zone,
	resources::unmanaged_intrusive_base
> damage_zone_ptr;
typedef resources::resource_ptr<
	ladder,
	resources::unmanaged_intrusive_base
> ladder_ptr;

class simple_game_project : public resources::unmanaged_resource , public base_project {
public:
	struct loading {
		inline		loading	( ) :
			loaded_count( 0 ),
			all_queried( false ),
			visuals_loaded( false ),
			collision_loaded( false )
		{ }

	public:
		/* 0x0000 */	u32		loaded_count;
		/* 0x0004 */	bool	all_queried;
		/* 0x0005 */	bool	visuals_loaded;
		/* 0x0006 */	bool	collision_loaded;
	}; // struct loading

public:
			explicit						simple_game_project	( base_game_scene& s );
	virtual									~simple_game_project( );

			void							insert				( scheduler& scheduler );
			void							remove				( );

	inline	bool							is_inserted			( ) { /* no source */ return m_is_inserted; }

			void							on_npc_loaded		( resources::queries_result& data );

	inline	pcstr							project_name		( ) { /* no source */ return NULL; }

			victory_items_container_core*	get_items_container	( u8 id );

			bool							all_loaded			( );

private:
	// game_world walks m_respawn_points directly (clear_player_spawn_info);
	// PDB does not record friendship, codegen-neutral
	friend class game_world;
	// game_world_ui reads m_config directly (initialize_minimap project_name lookup);
	// PDB does not record friendship, codegen-neutral
	friend class game_world_ui;
	// project_cooker_simple reads m_loaded / m_objects directly (on_object_loaded);
	// PDB does not record friendship, codegen-neutral
	friend class project_cooker_simple;
	// network_client walks m_victory_items_containers directly (on_world_sync_request);
	// PDB does not record friendship, codegen-neutral
	friend class network_client;
	// lobby_menu inserts the lobby project's objects, visuals, and collisions directly.
	friend class lobby_menu;

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* base_project */
	/* 0x0138 */	base_game_scene&						m_game_scene;
	/* 0x013c */	configs::binary_config_ptr				m_config;
	/* 0x0140 */	vector< game_object_* >					m_objects;
	/* 0x014c */	map< u32, respawn_point_core*, std::less< u32 > >	m_respawn_points;
	/* 0x0164 */	vector< collision_geometry* >			m_collision_geometries;
	/* 0x0170 */	vector< damage_zone_ptr >				m_damage_zones;
	/* 0x017c */	vector< artefact_container_core* >		m_artefact_containers;
	/* 0x0188 */	vector< ladder_ptr >					m_ladders;
	/* 0x0194 */	vector< generic_anomaly_core* >			m_anomalies;
	/* 0x01a0 */	vector< victory_items_container* >		m_victory_items_containers;
	/* 0x01ac */	render_visual*							m_render_visuals;
	/* 0x01b0 */	u32										m_render_visuals_count;
	/* 0x01b4 */	bool									m_is_inserted;
	/* 0x01b8 */	simple_game_project::loading			m_loaded;
}; // class simple_game_project

STATIC_SIZE_ASSERT(simple_game_project, 0x1C0);
STATIC_SIZE_ASSERT(simple_game_project::loading, 0x8);

} // namespace survarium

#endif // #ifndef GAME_PROJECT_H_INCLUDED
