////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SCENE_COOK_H_INCLUDED
#define RENDER_SCENE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::scene;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

class scene_cook : public resources::translate_query_cook , public boost::noncopyable {
public:
					scene_cook					( );

	virtual	void	translate_query				( resources::query_result_for_cook& in_out_query ) override;

	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_particle_world_created	(
						resources::queries_result&				result,
						scene*									created_resource,
						resources::query_result_for_cook*		in_out_query
					);

public:
	virtual			~scene_cook					( ) { /* no source */ }
}; // class scene_cook

STATIC_SIZE_ASSERT(scene_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SCENE_COOK_H_INCLUDED
