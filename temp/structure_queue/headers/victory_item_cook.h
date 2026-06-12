////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VICTORY_ITEM_COOK_H_INCLUDED
#define VICTORY_ITEM_COOK_H_INCLUDED

/* INCLUDES */
class survarium::game_world;
class survarium::victory_item_core_cook;

/* FORWARD REFS */
class vostok::resources::queries_result;
class survarium::victory_item;
class survarium::victory_item_core;

namespace survarium {

class victory_item_cook : public victory_item_core_cook {
public:
			explicit				victory_item_cook		( game_world& game_world );

	virtual	victory_item_core*		create_resource			( ) override;

	virtual	void					on_config_loaded		( resources::queries_result& data ) override;
	virtual	void					on_subresources_loaded	( resources::queries_result& data, victory_item* object_to_cook );

	virtual							~victory_item_cook		( ) { /* no source */ }

private:
	/* 0x0000 */	/* victory_item_core_cook */
	/* 0x0020 */	game_world&		m_game_world;
}; // class victory_item_cook

STATIC_SIZE_ASSERT(victory_item_cook, 0x24);

} // namespace survarium

#endif // #ifndef VICTORY_ITEM_COOK_H_INCLUDED
