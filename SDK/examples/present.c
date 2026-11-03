// C program to test PRESENT CIPHER
// Incoming data from serial terminal 
//FPGA tested

#include<stdint.h>
#include "delay.h"
#include "printf.h"

//*** ENCRYPTION ADDRESS RANGE ***//
// GPIO unit to control  ENCRYPTION
#define ENC_DONE_ADDR 0x12000000
#define ENC_DONE *((volatile unsigned int *)(ENC_DONE_ADDR ))
#define ENC_ENABLE_ADDR 0x15000000
#define ENC_ENABLE *((volatile unsigned int *)(ENC_ENABLE_ADDR ))
#define ENC_DATA_IN_LOWER_ADDR 0x16000000
#define ENC_DATA_IN_LOWER *((volatile unsigned int *)(ENC_DATA_IN_LOWER_ADDR ))
#define ENC_DATA_IN_UPPER_ADDR 0x17000000
#define ENC_DATA_IN_UPPER *((volatile unsigned int *)(ENC_DATA_IN_UPPER_ADDR ))
#define ENC_DATA_OUT_LOWER_ADDR 0x13000000
#define ENC_DATA_OUT_LOWER *((volatile unsigned int *)(ENC_DATA_OUT_LOWER_ADDR ))
#define ENC_DATA_OUT_UPPER_ADDR 0x14000000
#define ENC_DATA_OUT_UPPER *((volatile unsigned int *)(ENC_DATA_OUT_UPPER_ADDR ))

/*
//*** DECRYPTION ADDRESS RANGE***
//AES GPIO unit to control AES DECRYPTION
#define DEC_DATA_IN_ADDR 0x07000000
#define DEC_DATA_IN *((volatile unsigned int *)(DEC_DATA_IN_ADDR ))
#define DEC_VALID_IN_ADDR 0x08000000
#define DEC_VALID_IN *((volatile unsigned int *)(DEC_VALID_IN_ADDR ))
#define DEC_KEY1_ADDR  0x09000000
#define DEC_KEY1 *((volatile unsigned int *)(DEC_KEY1_ADDR ))
#define DEC_KEY2_ADDR 0x0a000000
#define DEC_KEY2 *((volatile unsigned int *)(DEC_KEY2_ADDR ))
#define DEC_VALID_OUT_ADDR 0x0b000000
#define DEC_VALID_OUT *((volatile unsigned int *)(DEC_VALID_OUT_ADDR ))
#define DEC_DATA_OUT_ADDR 0x0c000000
#define DEC_DATA_OUT *((volatile unsigned int *)(DEC_DATA_OUT_ADDR ))
*/

//encryption function
void present_encrypt(volatile uint32_t plaintxt_low, volatile uint32_t plaintxt_high,  volatile uint32_t* result_low , volatile uint32_t* result_high)
{   
    ENC_DATA_IN_LOWER = plaintxt_low;
    ENC_DATA_IN_UPPER = plaintxt_high;
    ENC_ENABLE = 1; //trigger encryption process to start
   
    //while(ENC_DONE==0) ;//wait until conversion finished
    delay(100);// a small delay to finish conversion
     //STORE RESULT INTO MEM LOCATION USING POINTER
    *result_low = ENC_DATA_OUT_LOWER;
    delay(100);
    *result_high=ENC_DATA_OUT_UPPER;
     ENC_ENABLE = 0;
   
}
/*
//decryption function
volatile uint32_t aes_decrypt(uint32_t my_char, uint32_t key1, uint32_t key2)
{
    DEC_KEY1 = key1;
    DEC_KEY2 = key2;
    DEC_DATA_IN = my_char; //data to be decrypted
    DEC_VALID_IN = 1; //trigger encryption process to start
    delay(100);
   // while(DEC_VALID_OUT==0) ;//wait until conversion finished
   DEC_VALID_IN = 0;
    return DEC_DATA_OUT;
   
}
*/

//Driver program to test above function
int main(void)
{
   
  //uart_init(UART1);
 printf("***** RISC-V based PRESENT Lightweight Cipher IP Core Demo ****\n\r");
 
  volatile uint32_t ciphertxt_lower,ciphertxt_upper, plaintxt_lower, plaintxt_upper;
  // data input for encryption
  plaintxt_lower = 0xffffffff;
  plaintxt_upper = 0xffffffff;
  //sending data for encryption
   printf("***Test cases for Encryption, Plaintext: 0xFFFFFFFFFFFFFFFF***\n\r");
   printf("Cipher Text is : ");
   //uart_sendline("\n\r");
   present_encrypt(plaintxt_lower, plaintxt_upper, &ciphertxt_lower, &ciphertxt_upper);
   //ciphertxt_lower=present_encrypt(plaintxt_lower, plaintxt_upper);
   //print_hex(ciphertxt_upper);//print lower 32 bit cipher text data in uart
   //print_hex(ciphertxt_lower); //print upper 32 bit
   printf("ciphertxt1=%08x, ciphertxt2=%08x\r\n", ciphertxt_lower,ciphertxt_upper);
   printf("\n\r");
   printf("conversion completed....\n\r");
   plaintxt_lower = 0x00000000;
   plaintxt_upper = 0x00000000;
   printf("***Test cases for Encryption, Plaintext: 0x0000000000000000***\n\r");
   printf("Cipher Text is : ");
   //uart_sendline("\n\r");
   present_encrypt(plaintxt_lower, plaintxt_upper, &ciphertxt_lower, &ciphertxt_upper);
   //ciphertxt_lower=present_encrypt(plaintxt_lower, plaintxt_upper);
   //print_hex(ciphertxt_upper);//print lower 32 bit cipher text data in uart
   //print_hex(ciphertxt_lower); //print upper 32 bit
   printf("ciphertxt1=%08x, ciphertxt2=%08x\r\n", ciphertxt_lower,ciphertxt_upper);
   printf("\n\r");
   printf("conversion completed....\n\r");
   //uart result should be 64'h3333DCD3213210D2
  while(1){
    //do nothing
  }
	

	return 0;
}