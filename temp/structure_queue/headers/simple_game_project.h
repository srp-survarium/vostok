////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SIMPLE_GAME_PROJECT_H_INCLUDED
#define SIMPLE_GAME_PROJECT_H_INCLUDED

/* INCLUDES */
class vostok::resources::unmanaged_resource;
class survarium::base_game_scene;
class survarium::base_project;
class survarium::map<unsigned int,survarium::respawn_point_core *,stlp_std::less<unsigned int> >;
class survarium::vector<survarium::artefact_container_core *>;
class survarium::vector<survarium::collision_geometry *>;
class survarium::vector<survarium::game_object_ *>;
class survarium::vector<survarium::generic_anomaly_core *>;
class survarium::vector<survarium::victory_items_container *>;
struct survarium::render_visual;
struct survarium::simple_game_project::loading;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
typedef survarium::vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >
	survarium::vector<survarium::damage_zone_ptr >;
typedef survarium::vector<vostok::resources::resource_ptr<survarium::ladder,vostok::resources::unmanaged_intrusive_base> >
	survarium::vector<survarium::ladder_ptr >;
class vostok::configs::binary_config;
class survarium::damage_zone;
class survarium::ladder;

/* FORWARD REFS */
class vostok::resources::queries_result;
class survarium::scheduler;
class survarium::victory_items_container_core;

namespace survarium {

class simple_game_project : public resources::unmanaged_resource , public base_project {
public:
			explicit							simple_game_project	( base_game_scene& s );
	virtual										~simple_game_project( );

			void								insert				( scheduler& scheduler );
			void								remove				( );

	inline	bool								is_inserted			( ) { /* no source */ }

			void								on_npc_loaded		( resources::queries_result& data );

	inline	pcstr								project_name		( ) { /* no source */ }

			victory_items_container_core*		get_items_container	( u8 id );

			bool								all_loaded			( );

private:
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

} // namespace survarium

#endif // #ifndef SIMPLE_GAME_PROJECT_H_INCLUDED
