////////////////////////////////////////////////////////////////////////////
//	Created 	: 29.07.2026
////////////////////////////////////////////////////////////////////////////

#ifndef RENDER_LIGHTS_DB_H_INCLUDED
#define RENDER_LIGHTS_DB_H_INCLUDED

/* INCLUDES */
enum const vostok::render::lights_db::tree_operation_enum;
class vostok::render::vector<vostok::render::light_data>;
struct vostok::collision::space_partitioning_tree;
typedef vostok::intrusive_ptr<vostok::render::light,vostok::render::resource_intrusive_base,vostok::threading::single_threading_policy>
	vostok::render::light_ptr;
class vostok::render::light;

/* FORWARD REFS */
class vostok::render::light_data;
class vostok::render::light_props;

namespace vostok {
namespace render {

class lights_db {
public:
													lights_db		( );
													~lights_db		( );

			void									initialize_sun	( light_data& light_to_add );

			void									add_light		( u32 id, light_props* props );
			void									update_light	( u32 id, light_props* props );
			void									remove_light	( u32 id );

			void									tick			( float time_delta );

			light_ptr								get_sun			( ) const;
	inline	vector< light_data > const&				get_lights		( ) const { /* no source */ }

	inline	collision::space_partitioning_tree&		lights_tree		( ) const { /* no source */ }

private:
			light*									create			(
														const lights_db::tree_operation_enum	arg_0 /* lights_db::tree_operation_enum operation */
													) const;

	/* 0x0000 */	vector< light_data >					m_lights;
	/* 0x000c */	light_ptr								m_sun;
	/* 0x0010 */	collision::space_partitioning_tree*		m_lights_tree;
}; // class lights_db

STATIC_SIZE_ASSERT(lights_db, 0x14);

} // namespace render
} // namespace vostok

#endif // #ifndef RENDER_LIGHTS_DB_H_INCLUDED
