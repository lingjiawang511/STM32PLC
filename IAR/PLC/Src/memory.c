#include "memory.h"
#include "fmc.h"

void SDRAM_free(void){
 u32 i;
   for(i = 0;i < IS42S16160G_SIZE/2; i++){
     my_array_in_SDRAM[i] = 0x0000;
//     while(my_array_in_SDRAM[i] != 0x0000)my_array_in_SDRAM[i] = 0x0000;
   }
   
}  

void NAND_free(void){
 u32 i, a; 
 my_array_in_NAND[0] = 0xFF;
 for(i = 0; i < NAND_512_SIZE; i++){
    a = 0;
    my_array_in_NAND[0] = 0xFF;
    a =  my_array_in_NAND[i];
 }
}

void NAND_readId(void)
{
	NAND_IDTypeDef NAND_ID;
	char buf[100];

	if (HAL_NAND_Read_ID(&hnand1, &NAND_ID) == HAL_OK)
	{
          
		sprintf(buf, "Nand Flash ID = %02X,%02X,%02X,%02X\n",NAND_ID.Maker_Id, NAND_ID.Device_Id, NAND_ID.Third_Id, NAND_ID.Fourth_Id);
	}
}

#define NAND_CMD_WR0 ((uint8_t)0x80)
#define NAND_CMD_WR1 ((uint8_t)0x10)
  
uint8_t NAND_WriteNByte(uint8_t* Buffer, uint16_t BlockNumber, uint8_t PageNumber, uint16_t ByteNumber, uint16_t N)
{

uint16_t i=0;
uint8_t status;

return 0;
}