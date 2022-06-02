#include <stdlib.h>
#include <string.h>

//First, a bunch of codes to help with the conversions

int convert(int n, int b, char str[], int i) {
	if (n/b > 0)
		i = convert(n/b, b, str, i);
	str[i++] = "0123456789ABCDEF"[n%b];
	return i;
}

void my_itoa(int n, int b, char str[]) {
	int i = convert(n, b, str, 0);
	str[i] = '\0';
}





char bitconv(int bit)	//converts an int into a char bit
{
	if(bit == 0)
		return('0');
	else
		return('1');
}

int bitval(char c)      //converts a character 0 or 1 into an int
{
	if(c == '1')
		return(1);
	else
		return(0);
}



void dec_to_bin_str(const unsigned int n, char str[])
{
	//uses simple bitwise conversion by Fred Swartz  - 2001-09-04
	//to convert to binary string
	for (int i=15; i>=0; i--) 
	{
		int bit = ((n >> i) & 1);
		str[i] = bitconv(bit);
	}
	str[16] = '\0';
}


void dec_to_gray(const unsigned int enc, char str[])
{
	unsigned int g;
	int codelen;
	char padding_buffer[33];
	int j, rpos;
	
	g=enc;
	g = g^(g>>1);
	my_itoa(g, 2, str);
	
	//Check that the string length is 32 long
	codelen=strlen(str);		
	if(codelen < 32)
	{
		//Too short - pad front with zeros
		strcpy(padding_buffer, str);
		for(j=0; j<(32-codelen); j++)
		{
			str[j]='0';
		}
		rpos=0;
		do
		{
			str[j]=padding_buffer[rpos];
			j++;
			rpos++;
		}
		while(j<32);
	}
	
}

unsigned int gray_to_dec(char str[])
{
	//From Wikipedia
	//Converts gray code back into binary.
	
	int g=0;			//gray code int
	unsigned int n;		//binary int
	
	//First convert string to gray code int
	g=strtol(str, NULL, 2);
	
	
	//Convert g to binary n using binary bits
	g ^= g >> 1;
	g ^= g >> 2;
	g ^= g >> 4;
	g ^= g >> 8;
	n= g ^ (g >> 16);
	return(n);
}

