////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_SPEEDTREE_COOK_H_INCLUDED
#define RENDER_SPEEDTREE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::translate_query_cook;

/* FORWARD REFS */
class vostok::render::speedtree_data;
class vostok::resources::queries_result;
class vostok::resources::query_result_for_cook;
class vostok::resources::resource_base;

namespace vostok {
namespace render {

struct speedtree_cook : public resources::translate_query_cook {
					speedtree_cook				( );

	virtual	void	translate_query				( resources::query_result_for_cook& parent ) override;

	virtual	void	delete_resource				( resources::resource_base* resource ) override;

private:
			void	on_speedtree_raw_data_loaded( resources::queries_result& data, speedtree_data* creation_data );
			void	on_model_materials_loaded	( resources::queries_result& data, speedtree_data* d );
	inline	void	on_material_effects_created	( resources::queries_result& arg_0, speedtree_data* arg_1 ) { /* no source */ }

			void	query_materail_effects		( speedtree_data* cook_data );

			void	finish_model_creation		( speedtree_data* d );

public:
	virtual			~speedtree_cook				( ) { /* no source */ }
}; // struct speedtree_cook

STATIC_SIZE_ASSERT(speedtree_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_SPEEDTREE_COOK_H_INCLUDED
