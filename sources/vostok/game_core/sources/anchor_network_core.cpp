#include "pch.h"
#include "anchor.h"

namespace vostok
{
	// Shared opaque sink used by the anchor TUs.
	void example_callback(const char *name)
	{
		printf("%s\n", name);
	}
}
