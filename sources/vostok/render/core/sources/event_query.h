#ifndef VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED
#define VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED

#include <vostok/detail_noncopyable.h>

struct ID3D11Query;

namespace vostok {
namespace render {

class event_query : public core::noncopyable {
public:
	event_query( );
	~event_query( );

	void issue( );
	void wait( );

private:
	void init_query( );
	void release_query( );

private:
	ID3D11Query* m_query;
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_CORE_SOURCES_EVENT_QUERY_H_INCLUDED
