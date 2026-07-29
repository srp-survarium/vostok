////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK\RENDER\ENGINE\SOURCES\RENDER_COLLISION_OBJECT_H_INCLUDED
#define VOSTOK\RENDER\ENGINE\SOURCES\RENDER_COLLISION_OBJECT_H_INCLUDED

namespace vostok {
namespace render {

// STATE[STUB]
inline void render_collision_object<vostok::render::render_model_instance_impl>::render( base_scene_ptr const& __formal, debug::renderer& __formal ) const
{
	// OTHER SYMBOLS
	// Label(LabelSymbol { offset: PdbInternalSectionOffset { section: 0x1, offset: 0x111e20 }, flags: ProcedureFlags { nofpo: false, int: false, far: false, never: false, notreached: true, cust_call: false, noinline: false, optdbginfo: false }, name: RawString("$LN3") })
	// ******

	// FUNCTION BODY[0x122e20]
	// ******
}

// STATE[STUB]
inline bool render_collision_object<vostok::render::render_model_instance_impl>::aabb_query( math::aabb const& __formal, vectora< collision::triangle_result >& __formal ) const
{
	return false;

	// FUNCTION BODY[0xb53e0]
	// ******
}

// STATE[STUB]
inline bool render_collision_object<vostok::render::render_model_instance_impl>::ray_query(
	float3 const&		__formal,
	float3 const&		__formal,
	float				__formal,
	float&				__formal,
	vectora< collision::ray_triangle_result >&	__formal,
	fastdelegate::FastDelegate< bool( collision::ray_triangle_result const& ) > const&	__formal
) const
{
	return false;

	// FUNCTION BODY[0x6f130]
	// ******
}

// STATE[STUB]
inline bool render_collision_object<vostok::render::render_model_instance_impl>::aabb_test( math::aabb const& __formal ) const
{
	return false;

	// FUNCTION BODY[0x6f140]
	// ******
}

// STATE[STUB]
inline bool render_collision_object<vostok::render::render_model_instance_impl>::ray_test(
	float3 const&		__formal,
	float3 const&		__formal,
	float				__formal,
	float&				__formal
) const
{
	return false;

	// FUNCTION BODY[0x6f150]
	// ******
}

// STATE[STUB]
inline void* render_collision_object<vostok::render::render_model_instance_impl>::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x244c0]
	// ******
}

// STATE[STUB]
inline void* render_surface::`scalar deleting destructor'( u32 arg_0 )
{
	return NULL;

	// FUNCTION BODY[0x71550]
	// ******
}

	// TYPEDEFS
	// typedef
	// 	pbyte
	// 	iterator_type;

	// typedef
	// 	pcvoid*
	// 	iterator_type;

	// typedef
	// 	u16*
	// 	iterator_type;

	// typedef
	// 	void**
	// 	iterator_type;

	// typedef
	// 	vostok::fixed_string< 32 >*
	// 	iterator_type;

	// typedef
	// 	vostok::render::geometry_batch*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_pass_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::res_shader_technique_ptr*
	// 	iterator_type;

	// typedef
	// 	vostok::render::shader_constant_binding*
	// 	iterator_type;

	// typedef
	// 	vostok::render::signature_layout_pair*
	// 	iterator_type;

	// typedef
	// 	vostok::render::texture_named_instance*
	// 	iterator_type;

	// ******

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK\RENDER\ENGINE\SOURCES\RENDER_COLLISION_OBJECT_H_INCLUDED
