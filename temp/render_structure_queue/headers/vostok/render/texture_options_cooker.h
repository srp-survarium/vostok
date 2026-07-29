////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_OPTIONS_COOKER_H_INCLUDED
#define RENDER_TEXTURE_OPTIONS_COOKER_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;
enum vostok::resources::class_id_enum;
enum vostok::resources::cook_base::reuse_enum;
class vostok::enum_flags<enum vostok::resources::cook_base::flags_enum>;

/* FORWARD REFS */
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct texture_options_cooker : public resources::translate_query_cook {
					texture_options_cooker	(
						resources::class_id_enum			resource_class,
						resources::cook_base::reuse_enum	reuse_type,
						u32									translate_query_thread,
						enum_flags< enum resources::cook_base::flags_enum >	flags
					);

	virtual	void	delete_resource			( resources::resource_base* __formal ) override;

	virtual			~texture_options_cooker	( ) { /* no source */ }
}; // struct texture_options_cooker

STATIC_SIZE_ASSERT(texture_options_cooker, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_OPTIONS_COOKER_H_INCLUDED
