// SPDX-License-Identifier: GPL-3.0-or-later
////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////
#ifndef HUMAN_NPC_COOK_H_INCLUDED
#define HUMAN_NPC_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace configs {
	class binary_config_value;
} // namespace configs
} // namespace vostok

namespace survarium {

class game_world;
class human_npc;

class human_npc_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
			explicit	human_npc_cook			( game_world& world );

	virtual	void		translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource			( resources::resource_base* resource ) override;

private:
			void		on_queried_data_received( resources::queries_result& data );
			void		on_npc_options_received	(
							configs::binary_config_value const&		config_value,
							resources::query_result_for_cook&		parent
						);
			void		on_subresources_loaded	( resources::queries_result& data, human_npc* const human );

public:

	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
private:
	/* 0x0020 */	game_world&		m_game_world;
}; // class human_npc_cook

STATIC_SIZE_ASSERT(human_npc_cook, 0x24);

} // namespace survarium

#endif // #ifndef HUMAN_NPC_COOK_H_INCLUDED
