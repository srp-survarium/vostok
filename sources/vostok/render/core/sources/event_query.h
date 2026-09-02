// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED
#define VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED
#include <vostok/detail_noncopyable.h>

struct ID3D11Query;

namespace vostok {
namespace render {

struct event_query : public core::noncopyable {
	event_query( );
	~event_query( );

	void issue( );
	void wait( );

	void init_query( );
	void release_query( );

private:
	ID3D11Query* m_query;
};

STATIC_SIZE_ASSERT( event_query, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED
