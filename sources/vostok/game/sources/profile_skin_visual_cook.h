// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef PROFILE_SKIN_VISUAL_COOK_H_INCLUDED
#define PROFILE_SKIN_VISUAL_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace vostok {
namespace render {
	class skeleton_combined_cook_data;
} // namespace render
} // namespace vostok

namespace survarium {

class game;
class player_profile;

class profile_skin_visual_cook : public resources::translate_query_cook , private boost::noncopyable {
	typedef	resources::translate_query_cook	super;

public:
			explicit	profile_skin_visual_cook	( game& g );

		virtual	void		translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource				( resources::resource_base* __formal ) override;

private:
			void		on_configs_loaded			(
							resources::queries_result&				data,
							resources::query_result_for_cook*		parent,
							player_profile const*					profile
						);
			void		on_visual_loaded			(
							resources::queries_result&				data,
							resources::query_result_for_cook*		parent,
							render::skeleton_combined_cook_data*	cook_data
						);

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	game&		m_game;
}; // class profile_skin_visual_cook

STATIC_SIZE_ASSERT(profile_skin_visual_cook, 0x24);

} // namespace survarium

#endif // #ifndef PROFILE_SKIN_VISUAL_COOK_H_INCLUDED
