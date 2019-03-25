/*
 * do_crc.h
 *
 *  Created on: Mar 25, 2019
 *      Author: mark
 */

#ifndef DO_CRC_H_
#define DO_CRC_H_



#endif /* DO_CRC_H_ */
 int do_crc(char buff[], int buffer_len){

	 printf("\n%02x%02x%02x%02x%02x%02x%02x\n",buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]);
	 printf("%d\n",buffer_len);


	 int i=1;
	 unsigned int sum;
	 buffer_len = buffer_len - 2;


	        while ( i < buffer_len)
	        {
	           sum ^= buff[i];
	           printf("%02x,%02x\n",sum,buff[i]);
	           i++;
	        }
	        printf("%d\n",buffer_len);
printf("the check sum = %02x\n",sum);
exit(0);







	// return(crc_val);
}
