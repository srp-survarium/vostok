////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef SCALEFORM_MOVIE_COOK_H_INCLUDED
#define SCALEFORM_MOVIE_COOK_H_INCLUDED

#include <vostok/resources_cook_classes.h>

namespace survarium {

class flash_factory;

// out-of-line bodies live in game.cpp (translate_query/delete_resource/
// on_raw_data_loaded) and base_game_scene.cpp (ctor) per the PDB
class scaleform_movie_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
			explicit	scaleform_movie_cook	( flash_factory& factory );

	virtual	void		translate_query			( resources::query_result_for_cook& parent ) override;

	virtual	void		delete_resource			( resources::resource_base* resource ) override;

			void		on_raw_data_loaded		( resources::queries_result& data, resources::query_result_for_cook* parent );

	virtual				~scaleform_movie_cook	( ) { /* no source */ }

private:
	/* 0x0000 */	/* resources::translate_query_cook */
	/* 0x0020 */	/* boost::noncopyable */
	/* 0x0020 */	flash_factory&		m_factory;
}; // class scaleform_movie_cook

STATIC_SIZE_ASSERT(scaleform_movie_cook, 0x24);

} // namespace survarium

#endif // #ifndef SCALEFORM_MOVIE_COOK_H_INCLUDED
