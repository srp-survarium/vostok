////////////////////////////////////////////////////////////////////////////
//	Created 	: 05.02.2008
//	Author		: Konstantin Slipchenko
//	Description : element collision interface
////////////////////////////////////////////////////////////////////////////
#ifndef VOSTOK_CONTACT_JOINT_INFO_H_INCLUDED
#define VOSTOK_CONTACT_JOINT_INFO_H_INCLUDED

#include <vostok/collision/contact_info.h>

struct contact_joint_info
{
	vostok::collision::contact_info		c;
	float								integration_step;
}; // struct contact_joint_info

#endif