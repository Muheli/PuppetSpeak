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


#include "PpsCore.h"


bool DetectParen(int in_CharInt, bool& out_bIsOpenParen, bool& out_bIsCloseParen){
	bool a_bIsDelimeter = false;
	out_bIsOpenParen = false;
	out_bIsCloseParen = false;

	switch(in_CharInt){
		case '(':
			out_bIsOpenParen = true;
			a_bIsDelimeter = true;
			break;
		case ')':
			out_bIsCloseParen = true;
			a_bIsDelimeter = true;
			break;
	}
	return a_bIsDelimeter;
}


bool DetectWhitespace(int in_CharInt){
	if((' ' == in_CharInt) || ('\t' == in_CharInt) || ('\r' == in_CharInt) || ('\n' == in_CharInt))
		return true;
	else
		return false;
}


bool ParsePps(
	const Texcerpt&		in_Pps,
	//
	Texcerpt			*out_pCmdWord,
	Texcerpt			*out_pContent,
	Texcerpt			*out_pRemnant,
	DetectParenCallback in_pCallback
){
	int			 a_ErrorCt = 0;
	const char	*a_pBuf = in_Pps.m_pBuf;
	int 		 a_TextEndPos = in_Pps.m_nLen;
	int			 a_CursorPos = 0;
	bool		 a_bIsOpenParen		= false;
	bool		 a_bIsCloseParen	= false;
	int			 a_ParenDepth = 0;
	int			 a_CmdWordStartPos = 0;
	int			 a_CmdWordEndPos = 0;
	int			 a_ContentStartPos = 0;
	int			 a_ContentEndPos = 0;
	int			 a_RemnantStartPos = 0;
	int			 a_RemnantEndPos = 0;
	DetectParenCallback a_pCallback = &DetectParen;

	if (in_pCallback){
		a_pCallback = in_pCallback;
	}
	
	a_CmdWordStartPos = a_CursorPos;
	a_CmdWordEndPos = a_TextEndPos;

	while(a_CursorPos < a_CmdWordEndPos){
		if (!DetectWhitespace(a_pBuf[a_CursorPos])){
			break;
		}
		a_CursorPos = a_CursorPos + 1;
		a_CmdWordStartPos = a_CursorPos;
	}

	while(a_CursorPos < a_CmdWordEndPos){
		if((*a_pCallback)(a_pBuf[a_CursorPos],a_bIsOpenParen,a_bIsCloseParen)){
			a_CmdWordEndPos = a_CursorPos;
			if (a_bIsOpenParen){
				a_ParenDepth = a_ParenDepth + 1;
			}else{
				a_ErrorCt = a_ErrorCt + 1;
			}
		}
		a_CursorPos = a_CursorPos + 1;
	}

	a_ContentStartPos = a_CursorPos;
	a_ContentEndPos = a_TextEndPos;
	while(a_CursorPos < a_ContentEndPos){
		if((*a_pCallback)(a_pBuf[a_CursorPos],a_bIsOpenParen,a_bIsCloseParen)){
			if(a_bIsOpenParen){
				a_ParenDepth++;
			}else if (a_bIsCloseParen){
				a_ParenDepth--;
				if (0 == a_ParenDepth){
					a_ContentEndPos = a_CursorPos;
				}
			}else{
				a_ErrorCt = a_ErrorCt + 1;
			}
		}
		a_CursorPos = a_CursorPos + 1;
	}// while

	a_RemnantStartPos = a_CursorPos;
	a_RemnantEndPos = a_TextEndPos;
		
	if (0 < a_ParenDepth){
		a_ErrorCt = a_ErrorCt + a_ParenDepth;
	}else if (0 > a_ParenDepth){		
		a_ErrorCt = a_ErrorCt - a_ParenDepth;
	}
	
	if ( out_pCmdWord ){
		out_pCmdWord->m_pBuf = &a_pBuf[a_CmdWordStartPos];
		if ( a_CmdWordStartPos < a_CmdWordEndPos ){
			// Trim back trailing whitespace.
			for (
				a_CursorPos = a_CmdWordEndPos - 1;
				a_CursorPos > a_CmdWordStartPos;
				a_CursorPos --
			){
				if(DetectWhitespace(a_pBuf[a_CursorPos])){
					a_CmdWordEndPos--;
				}else{
					break;
				}
			}
			out_pCmdWord->m_nLen = a_CmdWordEndPos - a_CmdWordStartPos;
		}else{
			out_pCmdWord->m_nLen = 0;
		}
	}

	if ( out_pContent ){
		out_pContent->m_pBuf = &a_pBuf[a_ContentStartPos];
		if(a_ContentStartPos < a_ContentEndPos){
			out_pContent->m_nLen = a_ContentEndPos - a_ContentStartPos;
		}else{
			out_pContent->m_nLen = 0;
		}
	}

	if ( out_pRemnant ){
		out_pRemnant->m_pBuf = &a_pBuf[a_RemnantStartPos];
		if(a_RemnantStartPos < a_RemnantEndPos){
			out_pRemnant->m_nLen = a_RemnantEndPos - a_RemnantStartPos;
		}else{
			out_pRemnant->m_nLen = 0;
		}
	}

	if (a_ErrorCt <= 0){
		return true;
	}else{
		return false;
	}
}// ParsePps


////////////////////////////////

bool ParsePps(
	const std::string&	 in_Pps,
	int					 in_Len,
	std::string			*out_pCmdWord,
	std::string			*out_pContent,
	std::string			*out_pRemnant,
	DetectParenCallback in_pCallback
){
	int			 a_ErrorCt = 0;
	const char	*a_pBuf = in_Pps.c_str();
	int 		 a_TextEndPos = in_Len;
	int			 a_CursorPos = 0;
	bool		 a_bIsOpenParen		= false;
	bool		 a_bIsCloseParen	= false;
	int			 a_ParenDepth = 0;
	int			 a_CmdWordStartPos = 0;
	int			 a_CmdWordEndPos = 0;
	int			 a_ContentStartPos = 0;
	int			 a_ContentEndPos = 0;
	int			 a_RemnantStartPos = 0;
	int			 a_RemnantEndPos = 0;
	DetectParenCallback a_pCallback = &DetectParen;

	if (in_pCallback){
		a_pCallback = in_pCallback;
	}
	
	a_CmdWordStartPos = a_CursorPos;
	a_CmdWordEndPos = a_TextEndPos;

	while(a_CursorPos < a_CmdWordEndPos){
		if (!DetectWhitespace(a_pBuf[a_CursorPos])){
			break;
		}
		a_CursorPos = a_CursorPos + 1;
		a_CmdWordStartPos = a_CursorPos;
	}

	while(a_CursorPos < a_CmdWordEndPos){
		if((*a_pCallback)(a_pBuf[a_CursorPos],a_bIsOpenParen,a_bIsCloseParen)){
			a_CmdWordEndPos = a_CursorPos;
			if (a_bIsOpenParen){
				a_ParenDepth = a_ParenDepth + 1;
			}else{
				a_ErrorCt = a_ErrorCt + 1;
			}
		}
		a_CursorPos = a_CursorPos + 1;
	}

	a_ContentStartPos = a_CursorPos;
	a_ContentEndPos = a_TextEndPos;
	while(a_CursorPos < a_ContentEndPos){
		if((*a_pCallback)(a_pBuf[a_CursorPos],a_bIsOpenParen,a_bIsCloseParen)){
			if(a_bIsOpenParen){
				a_ParenDepth++;
			}else if (a_bIsCloseParen){
				a_ParenDepth--;
				if (0 == a_ParenDepth){
					a_ContentEndPos = a_CursorPos;
				}
			}else{
				a_ErrorCt = a_ErrorCt + 1;
			}
		}
		a_CursorPos = a_CursorPos + 1;
	}// while

	a_RemnantStartPos = a_CursorPos;
	a_RemnantEndPos = a_TextEndPos;

	if (0 < a_ParenDepth){
		a_ErrorCt = a_ErrorCt + a_ParenDepth;
	}else if (0 > a_ParenDepth){		
		a_ErrorCt = a_ErrorCt - a_ParenDepth;
	}

	if ( out_pCmdWord ){
		if ( a_CmdWordStartPos < a_CmdWordEndPos ){
			*out_pCmdWord = in_Pps.substr(a_CmdWordStartPos, a_CmdWordEndPos - a_CmdWordStartPos);
		}else{
			*out_pCmdWord = "";
		}
	}

	if ( out_pContent ){
		if(a_ContentStartPos < a_ContentEndPos){
			*out_pContent = in_Pps.substr(a_ContentStartPos, a_ContentEndPos - a_ContentStartPos);
		}else{
			*out_pContent = "";
		}
	}

	if ( out_pRemnant ){
		if(a_RemnantStartPos < a_RemnantEndPos){
			*out_pRemnant = in_Pps.substr(a_RemnantStartPos, a_RemnantEndPos - a_RemnantStartPos);
		}else{
			*out_pRemnant = "";
		}
	}

	if (a_ErrorCt <= 0){
		return true;
	}else{
		return false;
	}
}// ParsePps