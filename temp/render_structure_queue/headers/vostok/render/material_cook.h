////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_COOK_H_INCLUDED
#define RENDER_MATERIAL_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::configs::binary_config;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;
class vostok::vfs::vfs_locked_iterator;

namespace vostok {
namespace render {

struct material_cook : public resources::translate_query_cook {
					material_cook					( );

	virtual	void	translate_query					( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource					( resources::resource_base* resource ) override;

			void	on_material_config_loaded		( resources::queries_result& result );
private:
			void	on_material_binary_config_loaded( resources::query_result_for_cook* parent, configs::binary_config* cfg );
	inline	void	on_fs_iterator_ready			( vfs::vfs_locked_iterator const& arg_0, resources::query_result_for_cook* arg_1 ) { /* no source */ }

public:
	virtual			~material_cook					( ) { /* no source */ }
}; // struct material_cook

STATIC_SIZE_ASSERT(material_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_COOK_H_INCLUDED
