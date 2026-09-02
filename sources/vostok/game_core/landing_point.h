// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef LANDING_POINT_H_INCLUDED
#define LANDING_POINT_H_INCLUDED

namespace survarium {

class landing_point : private boost::noncopyable {
public:
	inline	explicit							landing_point		( float3 const& position, float3 const& rotation ) : m_position( position ), m_rotation ( rotation ) { }

	inline	void								set_start_animation	( resources::managed_resource_ptr const& start_animation )	{ m_start_animation = start_animation; }
	inline	void								set_end_animation	( resources::managed_resource_ptr const& end_animation )	{ m_end_animation = end_animation; }

	inline	bool								use_for_attachment	( ) const { /* no source */ }
	inline	bool								use_for_landing		( ) const { /* no source */ }

	inline	float3 const&						get_position		( ) const { return m_position; }
	inline	float3 const&						get_rotation		( ) const { return m_rotation; }

	inline	resources::managed_resource_ptr		get_start_animation	( ) const { return m_start_animation; }
	inline	resources::managed_resource_ptr		get_end_animation	( ) const { return m_end_animation; }

	/* 0x0000 */	landing_point*						next;

private:
	/* 0x0004 */	const float3						m_position;
	/* 0x0010 */	const float3						m_rotation;
	/* 0x001c */	resources::managed_resource_ptr		m_start_animation;
	/* 0x0020 */	resources::managed_resource_ptr		m_end_animation;
}; // class landing_point

STATIC_SIZE_ASSERT(landing_point, 0x24);

} // namespace survarium

#endif // #ifndef LANDING_POINT_H_INCLUDED
