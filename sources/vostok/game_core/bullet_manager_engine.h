////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef BULLET_MANAGER_ENGINE_H_INCLUDED
#define BULLET_MANAGER_ENGINE_H_INCLUDED

namespace survarium {

class bullet_manager_engine {
public:
	virtual	void	play_sound				( resources::unmanaged_resource_ptr const& arg_0, float3 const& arg_1 ) = 0;

	virtual	void	add_decal				(
						resources::unmanaged_resource_ptr const&	arg_0,
						u32									arg_1,
						float								arg_2,
						float								arg_3,
						float3 const&						arg_4,
						float3 const&						arg_5,
						float3 const&						arg_6,
						bool								arg_7
					) = 0;

	virtual	void	play_particle			(
						resources::unmanaged_resource_ptr const&	arg_0,
						float3 const&						arg_1,
						float3 const&						arg_2,
						float3 const&						arg_3
					) = 0;

	virtual	void	remove_decal			( u32 arg_0 ) = 0;

	virtual	bool	attach_tracer			( bullet* arg_0 ) = 0;

	virtual	bool	detach_tracer			( bullet* arg_0 ) = 0;

	virtual	void	update_tracer			(
						u16					arg_0,
						float3 const&		arg_1,
						float3 const&		arg_2,
						float				arg_3
					) = 0;

	inline	bullet_manager_engine	( ) { /* no source */ }


private:
}; // class bullet_manager_engine

STATIC_SIZE_ASSERT(bullet_manager_engine, 0x4);

} // namespace survarium

#endif // #ifndef BULLET_MANAGER_ENGINE_H_INCLUDED
