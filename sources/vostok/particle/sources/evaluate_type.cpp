////////////////////////////////////////////////////////////////////////////
//	Created		: 03.03.2011
//	Author		: Nikolay Partas
//	Copyright (C) GSC Game World - 2011
////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include <vostok/strings_functions.h>
#include "evaluate_type.h"

namespace vostok {
namespace particle {

enum_evaluate_type string_to_evaluate_type(pcstr name)
{
		 if	(vostok::strings::equal(name, "Age"))	return age_evaluate_type;
	else if (vostok::strings::equal(name, "Random"))	return random_evaluate_type;
	
	return age_evaluate_type;
}

} // namespace particle
} // namespace vostok
