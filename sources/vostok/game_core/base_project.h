////////////////////////////////////////////////////////////////////////////
//	Created 	: 03.11.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BASE_PROJECT_H_INCLUDED
#define BASE_PROJECT_H_INCLUDED


namespace survarium {

struct static_collision;
class link_resolver;
class base_game_object;

class base_project : public boost::noncopyable {
public:
	struct resolve_link_object {
		inline	explicit				resolve_link_object	( link_resolver* object, configs::binary_config_value config ) : config( config ), object( object ) {}

		configs::binary_config_value	config;
		link_resolver*					object;
	};
	typedef vector< base_project::resolve_link_object > resolve_link_objects_type;

public:
	explicit						base_project				( );
	virtual							~base_project				( );

	virtual	base_game_object*		get_object_by_name			( pcstr name );

			void					register_named_object		( pcstr name, base_game_object* obj ) { m_objects_registry[name] = obj; }
			void					register_object_to_resolve	( link_resolver* obj, configs::binary_config_value cfg ) { m_objects_to_resolve.push_back( resolve_link_object( obj, cfg ) ); }

	virtual	void					resolve_links				( );

private:
	// simple_game_project::insert/remove walk m_static_collision_objects directly;
	// PDB does not record friendship, codegen-neutral
	friend class simple_game_project;
	// project_cooker_simple constructs the static-collision array from the project config;
	// PDB does not record friendship, codegen-neutral
	friend class project_cooker_simple;

	typedef map< fixed_string<260>, base_game_object* > objects_registry_type;

	/* 0x0004 */	objects_registry_type				m_objects_registry;
	/* 0x001c */	resolve_link_objects_type			m_objects_to_resolve;
	/* 0x0028 */	static_collision*					m_static_collision_objects;
	/* 0x002c */	u32									m_static_collision_objects_count;
}; // class base_project

STATIC_SIZE_ASSERT(base_project, 0x30);

// free helper used by project_cooker_simple::create_game_objects.
void read_transform( configs::binary_config_value const& cfg, float4x4& result );

} // namespace survarium

#endif // #ifndef BASE_PROJECT_H_INCLUDED
