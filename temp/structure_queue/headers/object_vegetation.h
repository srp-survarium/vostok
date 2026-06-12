////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_VEGETATION_H_INCLUDED
#define OBJECT_VEGETATION_H_INCLUDED

/* INCLUDES */
class survarium::game_object_;
typedef vostok::resources::resource_ptr<vostok::resources::unmanaged_resource,vostok::resources::unmanaged_intrusive_base>
	vostok::resources::unmanaged_resource_ptr;
class vostok::resources::unmanaged_resource;

/* FORWARD REFS */
class boost::function<void __cdecl(survarium::game_object_ &)>;
class vostok::configs::binary_config_value;
class vostok::resources::queries_result;
class survarium::base_game_scene;

namespace survarium {

class object_vegetation : public game_object_ {
public:
			explicit	object_vegetation	( base_game_scene& s );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									project_resources_path,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert				( ) override;
	virtual	void		remove				( ) override;

			void		on_grass_loaded		( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

	virtual				~object_vegetation	( ) { /* no source */ }

private:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	resources::unmanaged_resource_ptr		m_grass;
}; // class object_vegetation

STATIC_SIZE_ASSERT(object_vegetation, 0x118);

} // namespace survarium

#endif // #ifndef OBJECT_VEGETATION_H_INCLUDED
