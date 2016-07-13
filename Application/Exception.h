/**
 * 
 * Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
 *  
 * This software is provided 'as-is', without any 
 * express or implied warranty. In no event will 
 * the authors be held liable for any damages 
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose, 
 * including commercial applications, and to alter it and redistribute 
 * it freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; 
 *    you must not claim that you wrote the original software. 
 *    If you use this software in a product, an acknowledgment 
 *    in the product documentation would be appreciated but 
 *    is not required.
 * 
 * 2. Altered source versions must be plainly marked as such, 
 *    and must not be misrepresented as being the original software.
 *  
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef __ERROR_H__
#define __ERROR_H__

#include <exception>
#include <string>

enum SystemError {
	LABEL, TEXTURE, NETWORKING, GAME, WINDOW, RENDERER, 
	AUDIO, FILE_IO, SYSTEM
};

enum SystemProgrammer {
	William
};

class Error : public std::exception
{
private:
	// The person who programmed the Error
	SystemProgrammer ErrorsOwner;
	// The component the Error took place in
	SystemError systemErrorLocation;
public:
	// Constructor & Deconstructor
	Error(SystemError sys, std::string msg, SystemProgrammer user = William);
	virtual ~Error() throw();
	
	// Get functions for the title and the error message
	const char * title();
	const char * what();
private:
	// The error message passed the programmer
	std::string error_message;
};

#endif
