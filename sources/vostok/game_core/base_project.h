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

	// claude@NOTE: body verified byte-correct (inlined into the anchor's touch() as
	// `add ecx,4` = &m_objects_registry @ +4, `call map::operator[]<char const*>`,
	// then the store - matches target). /GL LTCG inlines it whole-program (anchored
	// only by a synthetic caller), so no standalone body reaches the EXE -> None.
	// STATE[None|PARTIAL]: LTCG inline-vs-call (byte-correct). Target `QAE`.
			void					register_named_object		( pcstr name, base_game_object* obj ) { m_objects_registry[name] = obj; }
	// claude@NOTE: body verified byte-correct (inlined into the anchor's touch() as
	// the vector fast-path append + `_M_insert_overflow_aux` overflow - matches the
	// target's inlined push_back). Same /GL LTCG inline-vs-call; no standalone body
	// reaches the EXE -> None until a real game consumer keeps it standalone.
	// STATE[None|PARTIAL]: LTCG inline-vs-call (byte-correct). Target `QAE`.
			void					register_object_to_resolve	( link_resolver* obj, configs::binary_config_value cfg ) { m_objects_to_resolve.push_back( resolve_link_object( obj, cfg ) ); }

	virtual	void					resolve_links				( );

private:
	typedef map< fixed_string<260>, base_game_object* > objects_registry_type;

	/* 0x0004 */	objects_registry_type				m_objects_registry;
	/* 0x001c */	resolve_link_objects_type			m_objects_to_resolve;
	/* 0x0028 */	static_collision*					m_static_collision_objects;
	/* 0x002c */	u32									m_static_collision_objects_count;
}; // class base_project

STATIC_SIZE_ASSERT(base_project, 0x30);

} // namespace survarium

#endif // #ifndef BASE_PROJECT_H_INCLUDED
