/*
 *  Translate.h
 *  Translate AS1
 *
 *  Created by smackay on Tue Jun 15 2004.
 *  Copyright (c) 2004 Flagstone Software Ltd. All rights reserved.
 *
 @license@
 */

#ifndef __FSTranslate_h__
#define __FSTranslate_h__ 1

#ifdef WIN32
#  define CALL __stdcall
#else
#  define CALL
#endif

#include <stdlib.h>
#include "DLLInterface.h"

namespace translate {
/** 
 * The Translate class defines constants that are used throughout the package.
 */
class Translate 
{
public:
    /** 
     * VERSION is used to identify the version of ActionScript that the framework supports.
     */
	static const int VERSION = 1;
    /** 
     * MAJOR is used to identify the current version of the framework. This
     * is incremented for each new version of ActionScript supported.
     */
    static const int MAJOR = 1;
    /** 
     * MINOR is used to identify the current minor version of the framework. This
     * is incremented when new functionality is added or API changes are made.
     */
    static const int MINOR = 0;
    /** 
     * The RELEASE number is used to differentiate between different releases. 
     * This number is incremented when an enhancement or bug fix has been made 
     * and the API is unchanged.
     */
    static const int RELEASE = 6;
};
}
#endif
