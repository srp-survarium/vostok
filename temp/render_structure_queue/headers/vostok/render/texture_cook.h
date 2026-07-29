////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_TEXTURE_COOK_H_INCLUDED
#define RENDER_TEXTURE_COOK_H_INCLUDED

/* INCLUDES */
class vostok::resources::inplace_managed_cook;
class vostok::resources::managed_resource;

/* FORWARD REFS */
class vostok::resources::query_result_for_cook;

namespace vostok {
namespace render {

struct texture_cook : public resources::inplace_managed_cook {
					texture_cook			( );

	virtual	u32		calculate_resource_size	( u32 file_size, u32& out_offset_to_file, bool file_exist ) override;

	virtual	void	create_resource			(
						resources::query_result_for_cook&		in_out_query,
						resources::managed_resource_ptr			in_out_resource,
						u32										raw_file_size,
						u32&									out_final_resource_size
					) override;
	virtual	void	destroy_resource		( resources::managed_resource* dying_resource ) override;

	virtual			~texture_cook			( ) { /* no source */ }
}; // struct texture_cook

STATIC_SIZE_ASSERT(texture_cook, 0x20);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_TEXTURE_COOK_H_INCLUDED
