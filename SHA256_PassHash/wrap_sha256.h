/*==============================================================================
	SHA256_PassHash standalone implementation.

	Copyright (c) 2018 Stanislav Gromov.

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the
use of this software.

Permission is granted to anyone to use this software for
any purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1.	The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software in
	a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

2.	Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

3.	This notice may not be removed or altered from any source distribution.
==============================================================================*/


#ifndef INC_WRAP_SHA256_H
#define INC_WRAP_SHA256_H

#include "amx.h"

class AMXSHA256
{
public:
	static int AMXEXPORT AMXAPI Init(AMX *amx) throw();
	static int AMXEXPORT AMXAPI Cleanup(AMX *amx) throw();
}; // class AMXSHA256


#endif // INC_WRAP_SHA256_H
