#include "Example1.h"
#include "../LibPps/Texcerpt.h"
#include "../LibPps/PpsCore.h"


void NewlineIndent(int in_Indentation)
{
	int		a_Idx;

	std::cout << "\n";
	for (a_Idx = 0; a_Idx < in_Indentation; a_Idx++){
		std::cout << "  ";
	}
}// NewlineIndent


bool ParseAndPrint(const Texcerpt& in_Pps, 	int in_Indentation)
{
	Texcerpt	a_OuterText;
	Texcerpt	a_InnerText;
	Texcerpt	a_Remnant;

	if (! ParsePps(in_Pps, &a_OuterText, &a_InnerText, &a_Remnant)){
		return false;
	}

	NewlineIndent(in_Indentation);
	a_OuterText.StdOut();

	if (a_InnerText.m_nLen > 0){
		if (! ParseAndPrint(a_InnerText, in_Indentation+1)){
			return false;
		}
	}

	if (a_Remnant.m_nLen > 0){
		if (! ParseAndPrint(a_Remnant, in_Indentation)){
			return false;
		}
	}

	return true;
}// ParseAndPrint


int main(int argc, char* argv[])
{
	Texcerpt	a_TextInput("A1(B1(C1)B2())");

	// Simple test string initialized above is overridden if argv provides an alternative.
	if (argc > 1){
		a_TextInput.PointTo(argv[1]);
	}

	std::cout << "\nStarting text: ";
	a_TextInput.StdOut();

	if (ParseAndPrint(a_TextInput, 0)){
		std::cout << "\n\nParse successfully completed.";
	}else{
		std::cout << "\n\nParse ended with unmatched delimeters.";
	}

	return 0;
}// main

