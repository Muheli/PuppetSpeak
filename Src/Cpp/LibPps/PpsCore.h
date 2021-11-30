#if 0 //  BEGIN LICENSE BLOCK
* Version: LGPL 3.0
*
* The contents of this file are subject to The GNU Lesser General Public License
* Version 3.0 (the "License"); you may not use this file except in
* compliance with the License. You may obtain a copy of the License at
* http://www.opensource.org/licenses/lgpl-3.0.html
*
* Software distributed under the License is distributed on an "AS IS" basis,
* WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
* for the specific language governing rights and limitations under the
* License.
*
* Contributors:
* Robit Hairman, Mark Miller, Joe Sandmeyer
*
#endif //  END LICENSE BLOCK
#ifndef HDRSEEN_PpsCore
#define HDRSEEN_PpsCore


#include "Texcerpt.h"


///////////////////////////////
// PpsCore
///////////////////////////////

typedef bool (*DetectParenCallback)(
	int		in_CharInt,
	bool&	out_bIsOpenParen,
	bool&	out_bIsCloseParen); // funcptr

bool DetectParen(int in_CharInt, bool& out_bIsOpenParen, bool& out_bIsCloseParen);
bool DetectWhitespace(int in_CharInt);

bool ParsePps(
	const Texcerpt&		in_Pps,
	Texcerpt			*out_pCmdWord	= NULL,
	Texcerpt			*out_pContent	= NULL,
	Texcerpt			*out_pRemnant	= NULL,
	DetectParenCallback in_pCallback	= NULL);

bool ParsePps(
	const std::string&	 in_Pps,
	int					 in_Len,
	std::string			*out_pCmdWord,
	std::string			*out_pContent	= NULL,
	std::string			*out_pRemnant	= NULL,
	DetectParenCallback in_pCallback	= NULL);

inline bool ParsePps(
	const std::string&	 in_Pps,
	std::string			*out_pCmdWord,
	std::string			*out_pContent	= NULL,
	std::string			*out_pRemnant	= NULL,
	DetectParenCallback in_pCallback	= NULL
){
	return ParsePps(
		in_Pps,
		(int) in_Pps.length(),
		out_pCmdWord,
		out_pContent,
		out_pRemnant,
		in_pCallback);
}


#endif // HDRSEEN_PpsCore