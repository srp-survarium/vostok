#ifndef VOSTOK_RENDER_ENGINE_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED

namespace vostok {

namespace collision {

class object;

} // namespace collision

namespace render {

class moved_object_predicate_helper : private boost::noncopyable {
public:
	explicit moved_object_predicate_helper( vectora< collision::object const* >& array ) :
		m_array( array )
	{
	}

	void check_object( collision::object const& object );

private:
	vectora< collision::object const* >& m_array;
};

STATIC_SIZE_ASSERT( moved_object_predicate_helper, 0x4 );

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_MOVED_OBJECT_PREDICATE_HELPER_H_INCLUDED
