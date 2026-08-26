////////////////////////////////////////////////////////////////////////////
//	Created 	: 06.12.2025
////////////////////////////////////////////////////////////////////////////

#ifndef ANIMATION_ANALYSIS_RESULT_COOK_USER_DATA_H_INCLUDED
#define ANIMATION_ANALYSIS_RESULT_COOK_USER_DATA_H_INCLUDED

namespace vostok {
namespace animation {
	class skeleton;
}
}

namespace survarium {

struct leg_info;

struct animation_analysis_result_cook_user_data {
public:
	/* 0x0000 */	u32									legs_count;
	/* 0x0004 */	leg_info const*						legs;
	/* 0x0008 */	animation::skeleton*				skeleton;
	/* 0x000c */	resources::managed_resource_ptr		animation;
}; // struct animation_analysis_result_cook_user_data

STATIC_SIZE_ASSERT(animation_analysis_result_cook_user_data, 0x10);

} // namespace survarium

#endif // #ifndef ANIMATION_ANALYSIS_RESULT_COOK_USER_DATA_H_INCLUDED
