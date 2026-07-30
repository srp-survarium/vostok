#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED

namespace vostok {
namespace render {

template < typename resource_type >
inline bool reclaim( vector< resource_type* >& resources, resource_type const* resource )
{
	typename vector< resource_type* >::iterator const found =
		std::find( resources.begin( ), resources.end( ), resource );

	if ( found == resources.end( ) )
		return false;

	resources.erase( found );
	return true;

	// FUNCTION BODY[0x1266f0] for res_state
}

template < typename resource_type, typename predicate_type >
inline bool reclaim(
	set< resource_type*, predicate_type >& resources,
	resource_type const* resource
)
{
	typename set< resource_type*, predicate_type >::iterator const found =
		resources.find( const_cast< resource_type* >( resource ) );

	if ( found == resources.end( ) )
		return false;

	resources.erase( found );
	return true;

	// FUNCTION BODY[0x1265e0] for shader_constant_table/resource_manager::constant_table_predicate
	// FUNCTION BODY[0x12a540] for res_pass/effect_manager::compare_predicate<res_pass>
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED
