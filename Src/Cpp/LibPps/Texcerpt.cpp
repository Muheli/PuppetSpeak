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


#include "Texcerpt.h"
#include <assert.h>


bool Texcerpt::StrMatch(const Texcerpt &in_Texcerpt){
	bool	a_bDifferent = false;

	if(m_nLen == in_Texcerpt.m_nLen){
		int		a_Idx;
		
		for(a_Idx = 0; a_Idx < m_nLen; a_Idx++){
			if( m_pBuf[a_Idx] != in_Texcerpt.m_pBuf[a_Idx] ){
				a_bDifferent = true;
				break;
			}
		}
	}else{
		a_bDifferent = true;
	}

	return (! a_bDifferent);
}


bool Texcerpt::StrMatch(const char *in_pszStr){
	Texcerpt a_Texcerpt(in_pszStr);
	return StrMatch( a_Texcerpt );
}


void Texcerpt::StdOut(void) const{
	std::string	 a_Msg( m_pBuf, m_nLen );
	std::cout << a_Msg;
}


void Texcerpt::TextFileAppend(const char *in_pszFilePath) const{
	size_t		 nBytesAppended = 0;
	FILE		*pFile;
	errno_t		 a_ErrCode;
	
	a_ErrCode = fopen_s(&pFile, in_pszFilePath, "a+");
	if (pFile && (a_ErrCode == 0)){
		nBytesAppended = fwrite(m_pBuf, sizeof(char), m_nLen, pFile);
		fclose(pFile);
		assert(nBytesAppended == m_nLen);
	}
}


bool Texcerpt::QualifySyntax(Syntax in_Syntax, float *out_pVal) const {
	bool	 a_Confirmed = false;
	float	 a_Val = 0.0f;

	if (m_pBuf) switch (in_Syntax){
		case Syntax_NoConstraint:
			a_Confirmed = true;
		case Syntax_UnsignedInt:
			{
				int	 a_Idx;
				char a_Char;	
				for (a_Idx = 0; a_Idx < m_nLen; a_Idx++){
					a_Char = m_pBuf[a_Idx];
					if ((a_Char < '0') || ('9' < a_Char)){
						break;
					}
				}
				if (a_Idx > 0){
					assert(a_Idx <= m_nLen);
					if (out_pVal){
						// Return int rep as float.
						a_Val = GetFloatVal();
					}
					a_Confirmed = true;
				}
			}
			break;
		case Syntax_SignedInt:
			{
				Texcerpt		a_AfterSign(m_pBuf, m_nLen );
				bool	a_Negate = false;

				if ((m_nLen >= 2) && (m_pBuf[0] == '-')){
					a_AfterSign.m_pBuf = &m_pBuf[1];
					a_AfterSign.m_nLen = m_nLen -1;
					a_Negate = true;
				}
				a_Confirmed = a_AfterSign.QualifySyntax(
					Syntax_UnsignedInt, &a_Val);
				if (a_Negate){
					a_Val = -a_Val;
				}
			}
			break;
		case Syntax_UnsignedIntInequality:
			{
				Texcerpt		a_AfterSymbol(m_pBuf, m_nLen );
				float	a_DirSignFactor = 1.0f;
				float	a_InequalityEpsilon = 0.005f; // one half pct.

				if (	(m_nLen >= 2)
					&&	(	(m_pBuf[0] == '<')
						||	(m_pBuf[0] == '>')
				)){
					if (m_pBuf[0] == '<'){
						a_DirSignFactor = -1.0f;
					}
					a_AfterSymbol.m_pBuf = &m_pBuf[1];
					a_AfterSymbol.m_nLen = m_nLen -1;
				}
				a_Confirmed = a_AfterSymbol.QualifySyntax(
					Syntax_UnsignedInt, &a_Val);
				a_Val *= a_DirSignFactor;
			}
			break;
		case Syntax_SignedIntAsPctFrac:
			{
				a_Confirmed = QualifySyntax(
					Syntax_SignedInt, &a_Val);
				a_Val /= 100.0f;
			}
			break;
		case Syntax_UnsignedIntInequalityAsPctFrac:
			{
				a_Confirmed = QualifySyntax(
					Syntax_UnsignedIntInequality, &a_Val);
				a_Val /= 100.0f;
			}
			break;
		default:
			break;
	}
	if (out_pVal){
		*out_pVal = a_Val;
	}
	return a_Confirmed;
}// Texcerpt::QualifySyntax


float Texcerpt::GetFloatVal(void) const {
	std::string	 a_Msg( m_pBuf, m_nLen );
	return( (float) atof(a_Msg.c_str()) );
}


int Texcerpt::GetIntVal(void) const {
	std::string	 a_Msg( m_pBuf, m_nLen );
	return( atoi(a_Msg.c_str()) );
}


float Texcerpt::GetIntAsPctFrac(void) const {
	std::string	 a_Msg( m_pBuf, m_nLen );
	return (((float) atoi(a_Msg.c_str())) / 100.0f);
}
