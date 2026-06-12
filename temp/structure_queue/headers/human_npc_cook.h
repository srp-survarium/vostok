////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef HUMAN_NPC_COOK_H_INCLUDED
#define HUMAN_NPC_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
class survarium::game_world;
class survarium::human_npc* const;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace survarium {

class human_npc_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
			explicit	human_npc_cook			( game_world& world );

	virtual	void		translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource			( resources::resource_base* resource ) override;

			void		on_queried_data_received( resources::queries_result& data );
			void		on_npc_options_received	(
							configs::binary_config_value const&		config_value,
							resources::query_result_for_cook&		parent
						);
			void		on_subresources_loaded	( resources::queries_result& data, human_npc* const human );

	virtual				~human_npc_cook			( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	game_world&		m_game_world;
}; // class human_npc_cook

STATIC_SIZE_ASSERT(human_npc_cook, 0x24);

} // namespace survarium

#endif // #ifndef HUMAN_NPC_COOK_H_INCLUDED
