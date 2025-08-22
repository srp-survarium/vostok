////////////////////////////////////////////////////////////////////////////
//	Created 	: 20.12.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_READER_H_INCLUDED
#define VOSTOK_RESOURCES_READER_H_INCLUDED

namespace vostok {
namespace resources {

template < typename base_class >
class reader : public base_class {
public:
	inline	reader					( managed_resource_ptr const& resource ) :
		base_class			( get_data( resource ), resource->get_size() ),
		m_resource			( resource )
	{
	}

	inline	~reader					( )
	{
		m_resource->unpin	( const_cast<pbyte>( base_class::data( ) ) );
	}

private:
	static inline pcbyte get_data	( managed_resource_ptr const& resource )
	{
		ASSERT				( resource );
		return				( resource->pin	( ) );
	}

private:
	managed_resource_ptr	m_resource;
}; // class reader

} // namespace resources
} // namespace vostok

#endif // #ifndef VOSTOK_RESOURCES_READER_H_INCLUDED