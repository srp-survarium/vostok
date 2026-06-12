////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

/* INCLUDES */
typedef stlp_std::vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base>,survarium::std_allocator<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> > >
	stlp_std::vector<survarium::damage_zone_ptr >;
class survarium::damage_zone;

namespace survarium {

class vector< damage_zone_ptr > : public std::vector< damage_zone_ptr > {
public:
	inline								vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
	inline								vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0, damage_zone_ptr const& arg_1 ) { /* no source */ }
	inline	explicit					vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >( u32 arg_0 ) { /* no source */ }

	inline	damage_zone_ptr&			operator[]	( u32 arg_0 ) { /* no source */ }
	inline	damage_zone_ptr const&		operator[]	( u32 arg_0 ) const { /* no source */ }

	inline	u32							size		( ) const { /* no source */ }

	inline								~vector<vostok::resources::resource_ptr<survarium::damage_zone,vostok::resources::unmanaged_intrusive_base> >( ) { /* no source */ }
}; // class vector< damage_zone_ptr >

STATIC_SIZE_ASSERT(vector< damage_zone_ptr >, 0xC);

} // namespace survarium

#endif // #ifndef VECTOR_H_INCLUDED
