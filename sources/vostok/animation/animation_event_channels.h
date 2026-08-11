////////////////////////////////////////////////////////////////////////////
//	Created		: 27.09.2010
//	Author		: Konstantin Slipchenko
//	Copyright (C) GSC Game World - 2010
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_H_INCLUDED
#define VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_H_INCLUDED

#include <vostok/animation/event_channel.h>

namespace vostok {
namespace animation {

class bi_spline_event_channel_baked;

//static const u32	max_event_channels = 10;

class VOSTOK_ANIMATION_API animation_event_channels {

public:
	animation_event_channels();
	~animation_event_channels();

public:
template	<class ConfigValueType>
static	size_t	count_internal_memory_size( ConfigValueType const& config );
static	u32		count_internal_memory_size(
						bi_spline_event_channel_baked const* channels,
						u32 channels_count
					);
static inline u32 count_internal_memory_size( animation_event_channels const& event_channels );

public:
inline	void	create_in_place_internals ( animation_event_channels const& event_channels, void* memory_buff );

public:
template	<class ConfigValueType>
void create_in_place_internals ( ConfigValueType const& config, void* memory_buff );
		void	create_in_place_internals(
						bi_spline_event_channel_baked const* channels,
						u32 channels_count,
						void* memory_buffer
					);

#ifndef	MASTER_GOLD
public:
		void				write( configs::lua_config_value &cfg )const;
#endif

public:
	inline	size_t				internal_memory_size	()const;
	inline	void				copy_internals			( void* mem_buff )const;

public:
inline	u32						channels_count		( )	const		{ return m_channels_count; }
		event_channel			&channel			( u32 id )	;
		event_channel	const	&channel			( u32 id ) const ;
		u32						get_channel_id		( pcstr name ) const ;

private:
	u32					m_channels_count;
	u32					m_internal_memory_position;

}; // class animation_event_channels

STATIC_SIZE_ASSERT(animation_event_channels, 0x8);

} // namespace animation
} // namespace vostok

#include <vostok/animation/animation_event_channels_inline.h>

#endif // #ifndef VOSTOK_ANIMATION_ANIMATION_EVENT_CHANNELS_H_INCLUDED
