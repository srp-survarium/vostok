////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef PROFILE_SKIN_VISUAL_COOK_H_INCLUDED
#define PROFILE_SKIN_VISUAL_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
class survarium::game;

/* FORWARD REFS */
class vostok::render::skeleton_combined_cook_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class survarium::player_profile;

namespace survarium {

class profile_skin_visual_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
			explicit	profile_skin_visual_cook	( game& g );

	virtual	void		translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource				( resources::resource_base* __formal ) override;

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

	virtual				~profile_skin_visual_cook	( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	game&		m_game;
}; // class profile_skin_visual_cook

STATIC_SIZE_ASSERT(profile_skin_visual_cook, 0x24);

} // namespace survarium

#endif // #ifndef PROFILE_SKIN_VISUAL_COOK_H_INCLUDED
