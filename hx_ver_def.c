#ifndef __HX_VER_H__
#define __HX_VER_H__

#include "hx_ver_def.h"

//=============================================================================
// these defines use for find version string to fix bin file name. just a trick.
#ifndef __VERSION__	
#define __VERSION__		"16091201"
#endif
#ifndef __PROG_NAME__
#define __PROG_NAME__	"prog"
#endif

const char g_version[] = __VERSION__;
const char g_prog_name[] = __PROG_NAME__;

#define __BUILD_TIME__	"Build " __DATE__ " " __TIME__

const char *__version_str__ = \
	__SEARCH_FLAG__ "$$" __VERSION__ "$$" __RELTYPE__ "$$" __BUILD_TIME__ "$$" __PROG_NAME__;

const char * hx_ver_init(void)
{
	return __version_str__;
}

//=============================================================================

#endif


