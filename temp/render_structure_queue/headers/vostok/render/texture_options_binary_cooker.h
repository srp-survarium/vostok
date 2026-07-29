////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_OPTIONS_BINARY_COOKER_H_INCLUDED
#define RENDER_TEXTURE_OPTIONS_BINARY_COOKER_H_INCLUDED

/* INCLUDES */
struct vostok::render::texture_options_cooker;

/* FORWARD REFS */
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;

namespace vostok {
namespace render {

struct texture_options_binary_cooker : public texture_options_cooker {
					texture_options_binary_cooker	( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

private:
			void	on_lua_options_loaded			( resources::queries_result& result );
			void	on_binary_config_loaded			( resources::queries_result& result );

public:
	virtual			~texture_options_binary_cooker	( ) { /* no source */ }
}; // struct texture_options_binary_cooker

STATIC_SIZE_ASSERT(texture_options_binary_cooker, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_OPTIONS_BINARY_COOKER_H_INCLUDED
