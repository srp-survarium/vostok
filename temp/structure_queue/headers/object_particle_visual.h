////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_PARTICLE_VISUAL_H_INCLUDED
#define OBJECT_PARTICLE_VISUAL_H_INCLUDED

/* INCLUDES */
class survarium::game_object_static;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class vostok::configs::binary_config_value;
class vostok::resources::queries_result;
class survarium::base_game_scene;

namespace survarium {

class object_particle_visual : public game_object_static {
public:
			explicit	object_particle_visual	( base_game_scene& w );

	virtual	void		load					(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert					( ) override;
	virtual	void		remove					( ) override;

			void		on_visual_ready			( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

	virtual				~object_particle_visual	( ) { /* no source */ }

private:
	/* 0x0000 */	/* game_object_static */
	/* 0x0150 */	resources::unmanaged_resource_ptr		m_particle_system_instance_ptr;
}; // class object_particle_visual

STATIC_SIZE_ASSERT(object_particle_visual, 0x158);

} // namespace survarium

#endif // #ifndef OBJECT_PARTICLE_VISUAL_H_INCLUDED
