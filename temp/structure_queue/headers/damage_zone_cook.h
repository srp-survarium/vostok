////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef DAMAGE_ZONE_COOK_H_INCLUDED
#define DAMAGE_ZONE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
class survarium::game_world;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace survarium {

class damage_zone_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
			explicit	damage_zone_cook		( game_world& game_world );

	virtual	void		translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource			( resources::resource_base* resource ) override;

			void		on_sub_resources_loaded	( resources::queries_result& data, configs::binary_config_value const& cfg_val );

	virtual				~damage_zone_cook		( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	game_world&		m_game_world;
}; // class damage_zone_cook

STATIC_SIZE_ASSERT(damage_zone_cook, 0x24);

} // namespace survarium

#endif // #ifndef DAMAGE_ZONE_COOK_H_INCLUDED
