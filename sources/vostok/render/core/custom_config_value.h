#ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED
#define VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED

namespace vostok {
namespace render {

class custom_config_value {
public:
	typedef custom_config_value const* const_iterator;

	custom_config_value( ) :
		id			( 0 ),
		data		( 0 ),
		id_crc		( 0 ),
		type		( 0 ),
		count		( 0 ),
		destroyer	( 0 )
	{
	}

	u32 size( ) const
	{
		return count;
	}

	const_iterator begin( ) const
	{
		return static_cast< const_iterator >( data );
	}

	const_iterator end( ) const
	{
		return begin( ) + count;
	}

	custom_config_value const& operator[]( pcstr key ) const;

	bool operator<( u32 ) const
	{
		return false;
	}

	pcvoid get_data_ptr( ) const
	{
		return data;
	}

	bool value_exists( pcstr key ) const;

private:
	void call_data_destructor( ) const;

public:
	pcstr	id;
	pcvoid	data;
	u32		id_crc;
	u16		type;
	u16		count;
	pcvoid	destroyer;
};

STATIC_SIZE_ASSERT( custom_config_value, 0x14 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_CUSTOM_CONFIG_VALUE_H_INCLUDED
