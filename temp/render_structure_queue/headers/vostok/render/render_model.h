////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_RENDER_MODEL_H_INCLUDED
#define RENDER_RENDER_MODEL_H_INCLUDED

/* INCLUDES */
class vostok::math::aabb;
class vostok::render::render_surface;
class vostok::resources::unmanaged_resource;
struct vostok::render::model_locator_item;
struct vostok::render::model_lods_descriptor;

/* FORWARD REFS */
class vostok::configs::binary_config_value;

namespace vostok {
namespace render {

class render_model : public resources::unmanaged_resource {
public:
					render_model	( );
	virtual			~render_model	( );

	virtual	void	load_properties	( configs::binary_config_value const& properties );

	virtual	void	set_children	( render_surface** children_in, u8 count, model_lods_descriptor* lods );

	virtual	bool	get_locator		( pcstr locator_name, model_locator_item& result ) const;

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	math::aabb					m_aabbox;
protected:
	/* 0x0120 */	model_locator_item*			m_locators;
	/* 0x0124 */	u16							m_locators_count;
public:
	/* 0x0128 */	model_lods_descriptor*		m_lods_descriptor;
	/* 0x012c */	render_surface**			m_childs;
	/* 0x0130 */	u8							m_childs_count;
}; // class render_model

STATIC_SIZE_ASSERT(render_model, 0x138);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_RENDER_MODEL_H_INCLUDED
