#ifndef VOSTOK_RENDER_ENGINE_RENDER_COLLISION_OBJECT_H_INCLUDED
#define VOSTOK_RENDER_ENGINE_RENDER_COLLISION_OBJECT_H_INCLUDED

#include <vostok/collision/object.h>

namespace vostok {
namespace render {

template <typename T>
class render_collision_object : public collision::object {
public:
	explicit render_collision_object( T* owner ) :
		m_owner( owner )
	{
		ASSERT( owner );
		set_type( 1 );
	}

	virtual void render( scene_ptr const&, debug::renderer& ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0x122e20]
	}

	virtual bool aabb_query( math::aabb const&, collision::triangles_type& ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0xb53e0]
		return false;
	}

	virtual bool cuboid_query( math::cuboid const&, collision::triangles_type& ) const
	{
		return false;
	}

	virtual bool ray_query(
		float3 const&,
		float3 const&,
		float,
		float&,
		collision::ray_triangles_type&,
		collision::triangles_predicate_type const&
	) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0x6f130]
		return false;
	}

	virtual bool aabb_test( math::aabb const& ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0x6f140]
		return false;
	}

	virtual bool cuboid_test( math::cuboid const& ) const
	{
		return false;
	}

	virtual bool ray_test( float3 const&, float3 const&, float, float& ) const
	{
		// STATE[STUB]
		// FUNCTION BODY[0x6f150]
		return false;
	}

	virtual void add_triangles( collision::triangles_type& ) const
	{
	}

	virtual math::aabb update_aabb( float4x4 const& local_to_world )
	{
		return m_owner->get_aabb( ).modify( local_to_world );
	}

	T* owner( ) const
	{
		return m_owner;
	}

	virtual ~render_collision_object( )
	{
	}

private:
	T* m_owner;
};

} // namespace render
} // namespace vostok

#endif // #ifndef VOSTOK_RENDER_ENGINE_RENDER_COLLISION_OBJECT_H_INCLUDED
