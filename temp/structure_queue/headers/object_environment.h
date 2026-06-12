////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef OBJECT_ENVIRONMENT_H_INCLUDED
#define OBJECT_ENVIRONMENT_H_INCLUDED

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

class object_environment : public game_object_ {
public:
			explicit	object_environment	( base_game_scene& w );
	virtual				~object_environment	( );

	virtual	void		load				(
							configs::binary_config_value const&		t,
							pcstr									__formal,
							boost::function< void( game_object_& ) >&	cb
						) override;

	virtual	void		insert				( ) override;
	virtual	void		remove				( ) override;

	inline	void		requery_material	( ) { /* no source */ }

			void		material_ready		( resources::queries_result& data, boost::function< void( game_object_& ) >& cb );

private:
	/* 0x0000 */	/* game_object_ */
	/* 0x0110 */	resources::unmanaged_resource_ptr		m_postprocess;
}; // class object_environment

STATIC_SIZE_ASSERT(object_environment, 0x118);

} // namespace survarium

#endif // #ifndef OBJECT_ENVIRONMENT_H_INCLUDED
