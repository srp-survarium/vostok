////////////////////////////////////////////////////////////////////////////
//	Created 	: 26.08.2008
//	Author		: Dmitriy Iassenev
//	Copyright (C) GSC Game World - 2009
////////////////////////////////////////////////////////////////////////////

#include "pch.h"

namespace vostok {

template fixed_string<32>::fixed_string();
template fixed_string<32>::fixed_string(fixed_string<32> const&);
template fixed_string<32>::fixed_string(buffer_string const&);
template fixed_string<32>::fixed_string(pcstr);
template fixed_string<32> const& fixed_string<32>::operator=(pcstr const);
template fixed_string<32> const& fixed_string<32>::operator=(
	fixed_string<32> const&
);

} // namespace vostok
