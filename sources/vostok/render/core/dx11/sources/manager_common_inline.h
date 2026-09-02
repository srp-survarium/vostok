// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED
#define VOSTOK_RENDER_CORE_DX11_SOURCES_MANAGER_COMMON_INLINE_H_INCLUDED
namespace vostok {
namespace render {

template < typename resource_type >
bool reclaim( vector< resource_type* >& vec, resource_type const* ptr )
{
	typename vector< resource_type* >::iterator it = vec.begin( ),
		end = vec.end( );

	for ( ; it != end; ++it )
	{
		if ( *it == ptr )
		{
			vec.erase( it );
			return true;
		}
	}

	return false;
}

template < typename resource_type, typename predicate_type >
bool reclaim( set< resource_type*, predicate_type >& container, resource_type const* ptr )
{
	typename set< resource_type*, predicate_type >::iterator const found = container.find( ptr );
	if ( found != container.end( ) ) {
		container.erase( found );
		return true;
	}

	return false;
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
