////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED
#define RENDER_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED

/* INCLUDES */
class vostok::vectora<vostok::collision::object const *>;

/* FORWARD REFS */
class vostok::collision::object;

namespace vostok {
namespace render {

class moved_object_predicate_helper : public boost::noncopyable {
public:
	inline	explicit	moved_object_predicate_helper	( vectora< collision::object const* >& arg_0 ) { /* no source */ }

			void		check_object					( collision::object const& obj );

	inline				~moved_object_predicate_helper	( ) { /* no source */ }

	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	vectora< collision::object const* >&	m_array;
}; // class moved_object_predicate_helper

STATIC_SIZE_ASSERT(moved_object_predicate_helper, 0x4);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED
