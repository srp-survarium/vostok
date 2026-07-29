////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_EVENT_QUERY_H_INCLUDED
#define RENDER_EVENT_QUERY_H_INCLUDED

/* INCLUDES */
struct ID3D11Query;
class vostok::core::noncopyable;

namespace vostok {
namespace render {

struct event_query : public core::noncopyable {
			event_query		( );
			~event_query	( );

	void	issue			( );

	void	wait			( );

	void	init_query		( );
	void	release_query	( );

	/* 0x0000 */	/* core::noncopyable */
private:
	/* 0x0000 */	ID3D11Query*	m_query;
}; // struct event_query

STATIC_SIZE_ASSERT(event_query, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_EVENT_QUERY_H_INCLUDED
