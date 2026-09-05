// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef GAME_MATERIAL_MANAGER_H_INCLUDED
#define GAME_MATERIAL_MANAGER_H_INCLUDED

#include <vostok/game_core/game_material.h>

namespace survarium {

class game_material;
class game_material_manager_cook;
class material_pair;

class game_material_manager : public resources::unmanaged_resource , private boost::noncopyable {
public:
									game_material_manager		( );
	virtual							~game_material_manager		( );

			game_material const*	get_material				( u16 id ) const;

	inline	u16						get_material_id				( pcstr name ) const
	{
		// sushi@TODO: Verify name-lookup miss policy; returning the loaded default ID follows the numeric getter's fallback model.
		u16 id;
		return material_exist( name, &id ) ? id : m_default_material_id;
	}

			bool					material_exist				( u16 id ) const;
	inline	bool					material_exist				( pcstr name, u16* id ) const
	{
		// sushi@TODO: Verify name comparison, optional-output and miss-output policies in an original lookup consumer.
		for ( material_container::const_iterator it = m_materials.begin( ); it != m_materials.end( ); ++it )
		{
			if ( !strings::equal( it->second->name( ), name ) )
				continue;
			if ( id )
				*id = it->first;
			return true;
		}
		return false;
	}

			material_pair const*	get_pair					( u16 first_mtrl_id, u16 second_mtrl_id ) const;

	inline	bool					pair_exist					( u16 first_mtrl_id, u16 second_mtrl_id ) const
	{
		// sushi@TODO: Verify exact ordered membership versus symmetric/default-substituted pair lookup.
		material_pair_container::const_iterator first_it = m_pairs.find( first_mtrl_id );
		return first_it != m_pairs.end( ) && first_it->second.find( second_mtrl_id ) != first_it->second.end( );
	}

			void					clear_resources				( );

private:
			void					add_game_material			( game_material const* const mtrl );
			void					add_pair					( material_pair const* const pair );

			void					delete_pairs				( );
			void					delete_materials			( );

	typedef map< u16, game_material const* > material_container;
	typedef map< u16, material_pair const* > material_pairs_type;
	typedef map< u16, material_pairs_type > material_pair_container;

private:
	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
	/* 0x0108 */	material_container				m_materials;
	/* 0x0120 */	material_pair_container			m_pairs;
	/* 0x0138 */	u16												m_default_material_id;
private:
	friend class game_material_manager_cook; // sushi@TODO: For `m_default_material_id`, though maybe this comes from some inlined function.
}; // class game_material_manager

STATIC_SIZE_ASSERT(game_material_manager, 0x140);

} // namespace survarium

#endif // #ifndef GAME_MATERIAL_MANAGER_H_INCLUDED
