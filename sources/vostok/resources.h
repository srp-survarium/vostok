////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.10.2008
//	Author		: Sergey Chechin, Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#ifndef VOSTOK_RESOURCES_H_INCLUDED
#define VOSTOK_RESOURCES_H_INCLUDED

#ifndef MASTER_GOLD
#	define DEBUG_RESOURCES					1
#else
#	define DEBUG_RESOURCES					0
#endif

#include <vostok/fs/native_path_string.h>
#include <vostok/fs/virtual_path_string.h>
#include <vostok/resources_types.h>
#include <vostok/resources_queries_result.h>
#include <vostok/resources_cook_base.h>
#include <vostok/resources_resource_ptr.h>
#include <vostok/resources_fs.h>
#include <vostok/resources_memory.h>

namespace vostok {
namespace resources {

enum enable_fs_watcher_bool { enable_fs_watcher_false, enable_fs_watcher_true };

struct request
{
	static char const	path_separator	=	'|';

	pcstr				path;
	class_id_enum		id;

	void				set				( pcstr in_path, class_id_enum in_id ) { path = in_path; id = in_id; }
};

class VOSTOK_CORE_API creation_request
{
public:
			creation_request			( pcstr name, const_buffer data, class_id_enum id );
			creation_request			( pcstr name, u32 buffer_size, class_id_enum id );

	inline	pcstr			get_name	( ) const	{ return m_name; }
	inline	const_buffer	get_data	( ) const	{ return m_data; }
	inline	class_id_enum	get_id		( ) const	{ return m_id; }

private:	
	pcstr				m_name;
	const_buffer		m_data;
	class_id_enum		m_id;
};

char const				physical_path_char	=	'@';
pcstr const				sources_mount	=	"sources";
pcstr const				exported_mount	=	"exported";
pcstr const				converted_mount	=	"converted";

VOSTOK_CORE_API	void   	start_query_transaction	( );
VOSTOK_CORE_API	void   	end_query_might_destroy_this_transaction	( );

struct VOSTOK_CORE_API query_resource_params
{
	request	const *				requests;
	creation_request const *	requests_create;
	u32							requests_count;
	query_callback				callback;
	memory::base_allocator *	allocator;
	float *						target_satisfactions;
	math::float4x4 const * *	transforms;
	user_data_variant const * *	user_data;
	query_result_for_cook *		parent;
	query_type_enum				query_type;
	pcstr						request_mask;
	query_flag_enum				flags;
	u32	const *					quality_indexes;
	bool const *				disable_cache;
	u32 *						out_queries_id;
	autoselect_quality_bool *	autoselect_quality;
	assert_on_fail_bool			assert_on_fail;

	query_resource_params		(request const 				requests[], 
								 creation_request const 	requests_create[], 
								 u32						count,
								 query_callback const &		callback,
								 memory::base_allocator *	allocator,
								 float *					target_satisfactions	=	NULL,
								 math::float4x4 const *		transforms[]	=	NULL,
								 user_data_variant const *	user_data[]		=	NULL,
								 query_result_for_cook *	parent			=	NULL,
								 bool const *				disable_cache	=	NULL,
								 u32 const *				quality_indexes	=	NULL,
								 pcstr						request_mask	=	NULL,
								 query_flag_enum			flag			=	0,
								 query_type_enum			query_type		=	query_type_normal,
								 u32 *						out_queries_id	=	NULL,
								 autoselect_quality_bool *	autoselect_quality	=	NULL,
								 assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);
};

VOSTOK_CORE_API	long   	query_resources			( query_resource_params const & params );
VOSTOK_CORE_API	void   	query_resources_and_wait( query_resource_params const & params );

VOSTOK_CORE_API	long   	query_resources_autoselect_quality
												( request const 			requests[], 
												  u32						count, 
												  query_callback const &	, 
												  memory::base_allocator *	,
												  math::float4x4 const *	transform[],
												  user_data_variant const *	user_data[] = 	NULL,
												  query_result_for_cook *	parent		= 	NULL );

VOSTOK_CORE_API	long   	query_resources			( request const 			requests[], 
												  u32						count, 
												  query_callback const &	, 
												  memory::base_allocator *	,
												  user_data_variant const *	user_data[] =	NULL,
												  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);

VOSTOK_CORE_API	long   	query_resource			( pcstr						request_path, 
												  class_id_enum				class_id, 
												  query_callback const &	, 
												  memory::base_allocator *	,
												  user_data_variant	const *	user_data	=	NULL,
												  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);

VOSTOK_CORE_API	void   	query_resources_and_wait( request const *			requests, 
												  u32						count, 
												  query_callback const &	, 
												  memory::base_allocator *	,
												  user_data_variant const *	user_data[] =	NULL,
												  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);

VOSTOK_CORE_API	void   	query_resource_and_wait	( pcstr						request_path, 
												  class_id_enum				class_id, 
												  query_callback const &	, 
												  memory::base_allocator *	,
												  user_data_variant const *	user_data	=	NULL,
												  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);

VOSTOK_CORE_API	long   	query_create_resources	( creation_request const *  requests, 
											 	  u32						count, 
											 	  query_callback const &	, 
											 	  memory::base_allocator *	,
												  user_data_variant const *	user_data[] =	NULL,
											 	  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail	=	assert_on_fail_true);

VOSTOK_CORE_API	void   	query_create_resources_and_wait
												( creation_request const *  requests, 
											 	  u32						count, 
											 	  query_callback const &	, 
											 	  memory::base_allocator *	,
												  user_data_variant	const *	user_data[] =	NULL,
											 	  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool const	assert_on_fail	= assert_on_fail_true);

VOSTOK_CORE_API	long   	query_create_resource	( pcstr						request_name,
												  const_buffer				src_data, 
											 	  class_id_enum				class_id, 
											 	  query_callback const &	, 
											 	  memory::base_allocator *	,
												  user_data_variant	const *	user_data	=	NULL,
											 	  query_result_for_cook *	parent		=	NULL,
												  assert_on_fail_bool		assert_on_fail = assert_on_fail_true);

// request_mask can contain * and ?
VOSTOK_CORE_API	void   	query_resources_by_mask	( pcstr				request_mask, 
												  class_id_enum		class_id	, 
												  query_callback const &		, 
												  memory::base_allocator *		,
												  query_flag_enum	flags		=	0, 
												  query_result_for_cook *	parent	=	NULL);

VOSTOK_CORE_API	void	finalize_thread_usage ( bool calling_from_main_thread );
VOSTOK_CORE_API	void	wait_and_dispatch_callbacks	( bool calling_from_main_thread );

VOSTOK_CORE_API	void	start_cooks_registration	( );
VOSTOK_CORE_API	void	finish_cooks_registration	( );

VOSTOK_CORE_API	void	dispatch_callbacks			( );
VOSTOK_CORE_API	void	tick						( );

VOSTOK_CORE_API	void	on_resources_thread_started	( );
VOSTOK_CORE_API	void	resources_thread_tick		( );
VOSTOK_CORE_API	void	resources_thread_yield		( u32 thread_sleep_period );
VOSTOK_CORE_API	void	on_resources_thread_ending	( );
VOSTOK_CORE_API	void	cooker_thread_tick			( );
VOSTOK_CORE_API	void	cooker_thread_yield			( u32 thread_sleep_period );

#if VOSTOK_FS_NEW_WATCHER_ENABLED
VOSTOK_CORE_API	void	watcher_thread_tick			( );
#endif

VOSTOK_CORE_API	void	start_resources_threads		( );
VOSTOK_CORE_API	void	finish_resources_threads	( );


VOSTOK_CORE_API	u32		pending_queries_count ( );

template < int count >
inline			long   	query_resources		( request const				(& requests)[ count ], 
											  query_callback const &	callback, 
											  memory::base_allocator *	allocator,
											  user_data_variant const *	user_data[] =	NULL,
											  query_result_for_cook *	parent		=	NULL,
											  assert_on_fail_bool		assert_on_fail = assert_on_fail_true)
{
	VOSTOK_UNREFERENCED_PARAMETER				(parent);
	return query_resources (requests, array_size(requests), callback, allocator, user_data, parent, assert_on_fail);
}

template < int count >
inline			long   	query_create_resources		( creation_request const	(& requests)[ count ], 
													  query_callback const &	callback, 
													  memory::base_allocator *	allocator,
													  user_data_variant	const *	user_data[]	=	NULL,
													  query_result_for_cook *	parent		=	NULL)
{
	VOSTOK_UNREFERENCED_PARAMETER				(parent);
	return query_create_resources(requests, array_size(requests), callback, allocator, user_data, parent);
}

} // namespace resources
} // namespace vostok

#endif// VOSTOK_CORE_RESOURCES_H_INCLUDED