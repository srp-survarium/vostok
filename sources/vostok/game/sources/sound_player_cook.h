// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef SOUND_PLAYER_COOK_H_INCLUDED
#define SOUND_PLAYER_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>
#include <vostok/configs.h>	// configs::binary_config_ptr (by-value param)

namespace vostok {
namespace sound {
	struct world;
} // namespace sound
} // namespace vostok

namespace survarium {

class sound_player_cook : public resources::translate_query_cook , private boost::noncopyable {
public:
					sound_player_cook	( sound::world* world, resources::class_id_enum class_id );

	virtual	void	translate_query		( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource		( resources::resource_base* resource ) override;

private:
			void	on_sounds_loaded	( resources::queries_result& data, configs::binary_config_ptr config );
			void	on_config_loaded	( resources::queries_result& data );

public:

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	sound::world*		m_world;
}; // class sound_player_cook

STATIC_SIZE_ASSERT(sound_player_cook, 0x24);

} // namespace survarium

#endif // #ifndef SOUND_PLAYER_COOK_H_INCLUDED
