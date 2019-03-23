/*
 * do_xor_crc.h
 *
 *  Created on: Mar 23, 2019
 *      Author: mark
 */

char *do_xor_crc(char *BitString){

	static char Res[4];                                 // CRC Result
	   char CRC[3];
	   int  i;
	   char DoInvert;

	   for (i=0; i<3; ++i)  CRC[i] = 0;                    // Init before calculation

	   for (i=0; i<strlen(BitString); ++i)
	      {
	      DoInvert = ('1'==BitString[i]) ^ CRC[2];         // XOR required?

	      CRC[2] = CRC[1];
	      CRC[1] = CRC[0];
	      CRC[0] = DoInvert;
	      }

	   for (i=0; i<3; ++i)  Res[2-i] = CRC[i] ? '1' : '0'; // Convert binary to ASCII
	   Res[3] = 0;                                         // Set string terminator

	   return(Res);
	   }

	// A simple test driver:

	#include <stdio.h>

	int main()
	   {
	   char *Data, *Result;                                       // Declare two strings

	   Data = "1101000101000111";
	   Result = MakeCRC(Data);                                    // Calculate CRC

	   printf("CRC of [%s] is [%s] with P=[1001]\n", Data, Result);

	   return(0);


}
