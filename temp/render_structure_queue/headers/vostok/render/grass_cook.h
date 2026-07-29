////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_GRASS_COOK_H_INCLUDED
#define RENDER_GRASS_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::grass_cook_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

class grass_cook : public resources::translate_query_cook {
public:
					grass_cook		( );

	virtual	void	translate_query	( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource	( resources::resource_base* resource ) override;

private:
			void	on_layers_loaded( resources::queries_result& data, grass_cook_data* cook_data );

public:
	virtual			~grass_cook		( ) { /* no source */ }
}; // class grass_cook

STATIC_SIZE_ASSERT(grass_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_GRASS_COOK_H_INCLUDED
