/*
 *  Translate.h
 *  Translate AS1
 *
 *  Created by smackay on Tue Jun 15 2004.
 *  Copyright (c) 2004 Flagstone Software Ltd. All rights reserved.
 *
 @license@
 */

#include "Translate.h"

using namespace translate;

#ifdef WIN32
#ifdef _WINDLL

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#endif // _WINDLL
#endif // WIN32
