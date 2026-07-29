////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_USER_RENDER_SURFACE_H_INCLUDED
#define RENDER_USER_RENDER_SURFACE_H_INCLUDED

/* INCLUDES */
class vostok::render::render_surface;

/* FORWARD REFS */
class vostok::memory::chunk_reader;
class vostok::render::material_effects_instance_cook_data;
class vostok::resources::queries_result;

namespace vostok {
namespace render {

class user_render_surface : public render_surface {
public:
	virtual	void	load_from_chunk_data( memory::chunk_reader& arg_0 ) = 0;

protected:
			void	material_ready		(
						resources::queries_result&		data,
						material_effects_instance_cook_data*	cook_data,
						char*							material_name
					);

	virtual	bool	is_dynamic			( ) const override { /* no source */ }

public:
	inline			user_render_surface	( ) { /* no source */ }
	virtual			~user_render_surface( ) { /* no source */ }
}; // class user_render_surface

STATIC_SIZE_ASSERT(user_render_surface, 0x9C);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_USER_RENDER_SURFACE_H_INCLUDED
