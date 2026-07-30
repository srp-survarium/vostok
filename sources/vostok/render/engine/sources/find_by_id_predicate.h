#ifndef VOSTOK_RENDER_ENGINE_FIND_BY_ID_PREDICATE_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_FIND_BY_ID_PREDICATE_H_INCLUDED

namespace vostok {
namespace render {

template < typename T >
struct find_by_id_predicate {
	explicit find_by_id_predicate( u32 id ) :
		m_id( id )
	{
	}

	bool operator()( T const* const object )
	{
		return object->m_id == m_id;
	}

	u32	m_id;
};

struct environment_probe;

struct find_environment_probe_predicate {
	explicit find_environment_probe_predicate( u32 id ) :
		m_id( id )
	{
	}

	bool operator()( environment_probe const* const object );

	u32	m_id;
};

STATIC_SIZE_ASSERT( find_environment_probe_predicate, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_FIND_BY_ID_PREDICATE_H_INCLUDED
