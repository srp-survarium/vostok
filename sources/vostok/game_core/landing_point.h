////////////////////////////////////////////////////////////////////////////
//	Created 	: 02.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef LANDING_POINT_H_INCLUDED
#define LANDING_POINT_H_INCLUDED

namespace survarium {

class landing_point : public boost::noncopyable {
public:
	inline										landing_point		( float3 const& arg_0, float3 const& arg_1 ) { /* no source */ }

	inline	void								set_start_animation	( resources::managed_resource_ptr const& arg_0 ) { /* no source */ }
	inline	void								set_end_animation	( resources::managed_resource_ptr const& arg_0 ) { /* no source */ }

	inline	bool								use_for_attachment	( ) const { /* no source */ }
	inline	bool								use_for_landing		( ) const { /* no source */ }

	inline	float3 const&						get_position		( ) const { /* no source */ }
	inline	float3 const&						get_rotation		( ) const { /* no source */ }

	inline	resources::managed_resource_ptr	get_start_animation	( ) const { /* no source */ }
	inline	resources::managed_resource_ptr	get_end_animation	( ) const { /* no source */ }

private:
	/* 0x0000 */	/* boost::noncopyable */
	/* 0x0000 */	landing_point*						next;
	/* 0x0004 */	float3								m_position;
	/* 0x0010 */	float3								m_rotation;
	/* 0x001c */	resources::managed_resource_ptr		m_start_animation;
	/* 0x0020 */	resources::managed_resource_ptr		m_end_animation;
}; // class landing_point

STATIC_SIZE_ASSERT(landing_point, 0x24);

} // namespace survarium

#endif // #ifndef LANDING_POINT_H_INCLUDED
