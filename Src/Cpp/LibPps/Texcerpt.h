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
#ifndef HDRSEEN_Texcerpt
#define HDRSEEN_Texcerpt


#include <string>
#include <iostream>


///////////////////////////////
// Texcerpt
///////////////////////////////

struct Texcerpt{
	enum Syntax{
		Syntax_NoConstraint = 0,
		Syntax_UnsignedInt,
		Syntax_SignedInt,
		Syntax_UnsignedIntInequality,
		Syntax_SignedIntAsPctFrac,
		Syntax_UnsignedIntInequalityAsPctFrac,
	};

	const char	*m_pBuf;
	int			 m_nLen;

	inline		 Texcerpt()
		: m_pBuf(NULL)
		, m_nLen(0)
	{
	}

	inline		 Texcerpt(const Texcerpt &in_Texcerpt)
		: m_pBuf(in_Texcerpt.m_pBuf)
		, m_nLen(in_Texcerpt.m_nLen)
	{
	}

	inline		 Texcerpt(const char * in_pTagoBuf, int in_nTagoLen)
		: m_pBuf(in_pTagoBuf)
		, m_nLen(in_nTagoLen)
	{
	}

	inline		 Texcerpt(const char * in_pszTago)
		: m_pBuf(in_pszTago)
		, m_nLen((int) strlen(in_pszTago))
	{
	}

	inline		 Texcerpt(const std::string& in_Tago)
		: m_pBuf(in_Tago.c_str())
		, m_nLen((int) in_Tago.length())
	{
	}

	inline void PointTo(const Texcerpt &in_Texcerpt){
		m_pBuf = in_Texcerpt.m_pBuf;
		m_nLen = in_Texcerpt.m_nLen;
	}

	inline void PointTo(const char * in_pTagoBuf, int in_nTagoLen){
		m_pBuf = in_pTagoBuf;
		m_nLen = in_nTagoLen;
	}

	inline void PointTo(const char * in_pszTago){
		m_pBuf = in_pszTago;
		m_nLen = (int) strlen(in_pszTago);
	}

	inline void PointTo(const std::string& in_Tago){
		m_pBuf = in_Tago.c_str();
		m_nLen = (int) in_Tago.length();
	}

	inline void EmptyAtBuf(const Texcerpt &in_Texcerpt){
		PointTo(in_Texcerpt.m_pBuf, 0);
	}
	inline void EmptyAtBuf(const char * in_pszTago){
		PointTo(in_pszTago, 0);
	}
	inline void EmptyAtBuf(const std::string& in_Tago){
		PointTo(in_Tago.c_str(), 0);
	}

	inline const char *GetBuf() const {
		return m_pBuf;
	}

	bool	 StrMatch(const Texcerpt &in_Texcerpt);
	bool	 StrMatch(const char *in_pszStr);
	void	 StdOut(void) const;
	void	 TextFileAppend(const char *in_pszFilePath) const;

	bool	 QualifySyntax(
		Syntax	 in_Syntax,
		float	*out_pVal = NULL) const;
	float	 GetFloatVal(void) const;
	int		 GetIntVal(void) const;
	float	 GetIntAsPctFrac(void) const;

	void	 ClearToNull(void){
		m_pBuf = NULL;
		m_nLen = 0;
	}
};


#endif // HDRSEEN_Texcerpt