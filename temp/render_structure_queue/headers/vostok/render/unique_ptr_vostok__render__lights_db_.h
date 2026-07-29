////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_UNIQUE_PTR_H_INCLUDED
#define RENDER_UNIQUE_PTR_H_INCLUDED

/* INCLUDES */
class vostok::render::lights_db;
class vostok::render::lights_db* (vostok::render::unique_ptr<vostok::render::lights_db>::*);

namespace vostok {
namespace render {

class unique_ptr< lights_db > : public boost::noncopyable {
public:
	inline	explicit		unique_ptr<vostok::render::lights_db>( lights_db* arg_0 ) { /* no source */ }
	inline					~unique_ptr<vostok::render::lights_db>( ) { /* no source */ }

	inline	lights_db&		operator*	( ) const { /* no source */ }

	inline	lights_db*		operator->	( ) const { /* no source */ }

	inline	bool			operator!	( ) const { /* no source */ }

	inline	lights_db* ( unique_ptr< lights_db >::* )	operator class vostok::render::lights_db *(__thiscall vostok::render::unique_ptr<class vostok::render::lights_db>::* )(void) const( ) const { /* no source */ }

	inline	lights_db*		c_ptr		( ) const { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
private:
	/* 0x0000 */	lights_db*		m_object;
}; // class unique_ptr< lights_db >

STATIC_SIZE_ASSERT(unique_ptr< lights_db >, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_UNIQUE_PTR_H_INCLUDED
