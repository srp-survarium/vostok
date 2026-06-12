////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.06.2026
////////////////////////////////////////////////////////////////////////////

#ifndef ERASE_OLD_RECEIVERS_H_INCLUDED
#define ERASE_OLD_RECEIVERS_H_INCLUDED

/* INCLUDES */
typedef vostok::resources::resource_ptr<survarium::damage_zone_core,vostok::resources::unmanaged_intrusive_base>
	survarium::damage_zone_core_ptr;
class survarium::damage_zone_core;

/* FORWARD REFS */
class survarium::hit_receiver_info;

namespace survarium {

struct erase_old_receivers : public boost::noncopyable {
	inline	explicit	erase_old_receivers	( damage_zone_core_ptr const& arg_0 ) { /* no source */ }

	inline	bool		operator()			( hit_receiver_info const& arg_0 ) const { /* no source */ }

	inline				~erase_old_receivers( ) { /* no source */ }

public:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	damage_zone_core_ptr const&		m_damage_zone;
}; // struct erase_old_receivers

STATIC_SIZE_ASSERT(erase_old_receivers, 0x4);

} // namespace survarium

#endif // #ifndef ERASE_OLD_RECEIVERS_H_INCLUDED
