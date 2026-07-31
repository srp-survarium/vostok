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

template<typename T>
bool reclaim_with_id(vector<T*>& vec, const T* ptr)
{
	bool	found = false;
	vector<T*>::iterator it   = vec.begin(),
		end  = vec.end(),
		last = vec.end() - 1;

	for(; it!=end; it++)
	{
		if (*it==ptr)
		{
			found = true;

			if (it+1 != vec.end())
			{
				//set new id
				(*last)->set_id((*it)->id());
				std::swap(*it, *last);
			}

			vec.pop_back();

			break;
		}
	}

	return found;
}

template <class T>
u32 gen_id(const T& res_map, vector<u32>& free_id)
{
	if (free_id.empty())
	{
		return static_cast<u32>(res_map.size());
	}

	u32 id = free_id.back();
	free_id.pop_back();

	return id;
}

template<typename T1, typename T2>
std::pair<T1, T2> mk_pair(const T1& p1, const T2& p2)
{
	return std::pair<T1, T2>(p1, p2);
}

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED
