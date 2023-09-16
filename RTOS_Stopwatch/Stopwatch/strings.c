#include "strings.h"


enum Result eHexStringToUInt(char pcStr[],unsigned int *puiValue)
{
	unsigned char ucCharacterCounter;
	unsigned char ucCurrentCharacter;
	*puiValue=0;
	if((pcStr[0]!='0') || (pcStr[1]!='x') || (pcStr[2]==NULL))
	{
		return ERROR;
	}
	for(ucCharacterCounter=2; pcStr[ucCharacterCounter]!=NULL; ucCharacterCounter++)
	{
		ucCurrentCharacter = pcStr[ucCharacterCounter];
		if(ucCharacterCounter==6)
		{
			return ERROR;
		}
		*puiValue = *puiValue << 4;
		if((ucCurrentCharacter <= '9') && (ucCurrentCharacter >= '0'))
		{
			*puiValue = (*puiValue) | (ucCurrentCharacter-'0');			
		}
		else if((ucCurrentCharacter) <= 'F' && (ucCurrentCharacter >= 'A'))
		{
			*puiValue= (*puiValue) | (ucCurrentCharacter-'A'+10);
		}
		else
		{
			return ERROR;
		}
	}
	return OK;
	
}

enum CompResult eCompareString(char pcStr1[], char pcStr2[])
{ 
	unsigned char ucCharcounter; 
	for(ucCharcounter = 0; pcStr1[ucCharcounter]!=  '\0'; ucCharcounter++)
	{ 
		if( pcStr1[ucCharcounter] != pcStr2[ucCharcounter] )
		{ 
			return DIFFERENT; 
		} 
	} 
	if( pcStr1[ucCharcounter] != pcStr2[ucCharcounter] )
	{ 
		return DIFFERENT; 
	} 
	return EQUAL; 
}	

void ReplaceCharactersInString(char pcString[],char cOldChar,char cNewChar)
{ 
	unsigned char ucArrayelement; 
	for(ucArrayelement = 0; pcString[ucArrayelement] != '\0'; ucArrayelement++)
	{ 
		if(pcString[ucArrayelement] == cOldChar)
		{		 
			pcString[ucArrayelement] = cNewChar; 
		} 
	}
}


