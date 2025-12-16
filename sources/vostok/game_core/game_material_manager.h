////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef GAME_MATERIAL_MANAGER_H_INCLUDED
#define GAME_MATERIAL_MANAGER_H_INCLUDED

namespace survarium {

class game_material;
class material_pair;

class game_material_manager : public resources::unmanaged_resource , public boost::noncopyable {
public:
									game_material_manager		( );
	virtual							~game_material_manager		( );

			game_material const*	get_material				( u16 id ) const;

	inline	u16						get_material_id				( pcstr arg_0 ) const { /* no source */ }

	inline	bool					material_exist				( pcstr arg_0, u16* arg_1 ) const { /* no source */ }
			bool					material_exist				( u16 id ) const;

			material_pair const*	get_pair					( u16 first_mtrl_id, u16 second_mtrl_id ) const;

	inline	bool					pair_exist					( u16 arg_0, u16 arg_1 ) const { /* no source */ }

			void					clear_resources				( );

			void					add_game_material			( game_material const* mtrl );
			void					add_pair					( material_pair const* pair );

			void					delete_pairs				( );
			void					delete_materials			( );

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	map< u16, game_material const * >				m_materials;
	/* 0x0120 */	map< u16, map< u16, material_pair const * > >	m_pairs;
	/* 0x0138 */	u16												m_default_material_id;
}; // class game_material_manager

STATIC_SIZE_ASSERT(game_material_manager, 0x140);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_MANAGER_H_INCLUDED
