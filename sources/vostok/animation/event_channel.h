////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_EVENT_CHANNEL_H_INCLUDED
#define VOSTOK_ANIMATION_EVENT_CHANNEL_H_INCLUDED

#include <vostok/animation/time_channel.h>

namespace vostok {
namespace animation {

class bi_spline_event_channel_baked;

enum channel_type_enum
{
	channel_type_intervals		= 0x0,
	channel_type_partitions		= 0x1,
	channel_type_events			= 0x2,
};

class event_channel {
public:
	struct domain_data {
		u8 data;
	}; // struct domain_data

public:
	template < class ConfigValueType >
	static inline u32			count_internal_memory_size	( ConfigValueType const& config  );
	static inline u32			count_internal_memory_size	( bi_spline_event_channel_baked const& channel );
	static inline u32			count_internal_memory_size	( event_channel const& channel );

	template < class ConfigValueType >
	inline	void				create_in_place_internals	( ConfigValueType const& config, void* memory_buff );
	inline	void				create_in_place_internals	( bi_spline_event_channel_baked const& channel, void* memory_buff );

#ifndef	MASTER_GOLD
public:
	inline 	void				write						( configs::lua_config_value	&cfg )const;
#endif // #ifndef MASTER_GOLD

public:
	inline	u32					internal_memory_size		( )const;
	inline	u32					domain_id					( float t, u32& current_domain  ) const	{ return m_time_channel.domain( t, current_domain  ); }
	inline	domain_data const&	domain						( u32 id ) const						{ return m_time_channel.domain( id ); }
	inline	u32					domains_count				( ) const								{ return m_time_channel.domains_count(); }

	inline	float				knot						( u32 id ) const						{ return m_time_channel.knot( id ); }
	inline	float const*		knots						( )	const								{ return m_time_channel.knots( ); }
	inline	u32					knots_count					( )	const								{ return m_time_channel.knots_count( ); }

	inline	pcstr				name						( ) const								{ return m_name; }
	inline	channel_type_enum	type						( ) const								{ return m_type; }
private:
	typedef time_channel< domain_data >	time_channel_type;

	string32					m_name;
	time_channel<domain_data>	m_time_channel;
	channel_type_enum			m_type;
}; // class event_channel

} // namespace animation
} // namespace vostok

#ifndef ANIMATION_BI_SPLINE_EVENT_CHANNEL_BAKED_H_INCLUDED
#	include <vostok/animation/event_channel_inline.h>
#endif

#endif // #ifndef VOSTOK_ANIMATION_EVENT_CHANNEL_H_INCLUDED
