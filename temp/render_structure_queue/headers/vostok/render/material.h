////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MATERIAL_H_INCLUDED
#define RENDER_MATERIAL_H_INCLUDED

/* INCLUDES */
class vostok::fixed_string<128>;
class vostok::resources::unmanaged_resource;
class vostok::render::enum_vertex_input_type;
typedef vostok::resources::resource_ptr<vostok::configs::binary_config,vostok::resources::unmanaged_intrusive_base>
	vostok::configs::binary_config_ptr;
class vostok::configs::binary_config;

/* FORWARD REFS */
class vostok::configs::binary_config_value;
class vostok::render::material_effects;

namespace vostok {
namespace render {

class material : public resources::unmanaged_resource , public boost::noncopyable {
public:
	inline	explicit								material			( configs::binary_config_ptr in_config ) { /* no source */ }

	inline	pcstr									get_material_name	( ) const { /* no source */ }
	inline	configs::binary_config_value const&		get_config			( ) { /* no source */ }

	virtual											~material			( ) { /* no source */ }

	static	inline	void									initialize_nomaterial_material( ) { /* no source */ }
	static	bool									is_nomaterial_material_ready( );
	static	material_effects&						nomaterial_material	( enum_vertex_input_type vi );
	static	void									finalize_nomaterial_material( );

	/* 0x0000 */	/* resources::unmanaged_resource */
	/* 0x0108 */	/* boost::noncopyable */
private:
	/* 0x0108 */	fixed_string< 128 >				m_material_name;
	/* 0x0194 */	configs::binary_config_ptr		m_config;
}; // class material

STATIC_SIZE_ASSERT(material, 0x198);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MATERIAL_H_INCLUDED
