#include "video.h"
#include "gui.h"
#include "stm32f7xx_hal.h"
#include "variables.h"
#include "core.h"
#include "ltdc.h"
#include "image888.h"
#include "ff.h"

volatile DMA2D_Status PLC_DMA2D_Status = {1};
volatile u8 LayerOfView = 0;
const u32 ProjectionLayerAddress[2]={SDRAM_BANK_ADDR + LAYER_1_OFFSET, SDRAM_BANK_ADDR + LAYER_2_OFFSET}; // Were we fill out our objects?


u8 _HW_DrawLine( s16 x1, s16 y1, s16 x2, s16 y2, u32 c)
{

   if ( y1 == y2 )
   {
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
  HAL_DMA2D_Init(&hdma2d);
 if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
  if(HAL_DMA2D_Start_IT(&hdma2d, 
                        c, /* Color value in Register to Memory DMA2D mode */
                        ProjectionLayerAddress[LayerOfView] + 4 * ((u32)y1 * DisplayWIDTH + (u32)x1),  /* DMA2D output buffer */
                        x2-x1+1, /* width of buffer in pixels */
                        1) /* height of buffer in lines */ 
     != HAL_OK)
    {
    M_pull()(); 
     }  
    }
   }

   else if ( x1 == x2 )
   {
 // hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_ARGB8888;
  hdma2d.Init.OutputOffset = DisplayWIDTH-1;
  hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
  HAL_DMA2D_Init(&hdma2d);
  if(PLC_DMA2D_Status.Ready != 0){ 
    PLC_DMA2D_Status.Ready = 0;
   if(HAL_DMA2D_Start_IT(&hdma2d, 
                        c, /* Color value in Register to Memory DMA2D mode */
                        ProjectionLayerAddress[LayerOfView] + 4 * ((u32)y1 * DisplayWIDTH + (u32)x1),  /* DMA2D output buffer */
                        1 , /* width of buffer in pixels */
                        y2-y1+1) /* height of buffer in lines */ 
     != HAL_OK)
  {
    M_pull()(); 
  } 
 }
 
 }
 else
   {
     return 1;
   }
     return 0;
}





void Fill_Background(void){


}

void _HW_Fill_Finite_Color(u32 StartAddress, u32 color){
 // hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_R2M;
  hdma2d.Init.ColorMode = DMA2D_ARGB8888;
   hdma2d.Init.OutputOffset       = 0;
  hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
  HAL_DMA2D_Init(&hdma2d);
  
 if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
 if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  if(HAL_DMA2D_Start_IT(&hdma2d, 
                        color, /* Color value in Register to Memory DMA2D mode */
                        StartAddress,  /* DMA2D output buffer */
                        DisplayWIDTH, /* width of buffer in pixels */
                        DisplayHEIGHT) /* height of buffer in lines */ 
     != HAL_OK)
    {
    M_pull()(); 
     }  
    }
}  

void _HW_Fill_Display_From_Mem(u32 SourceAddress, u32 DstAddress){
  
 hdma2d.Init.Mode               = DMA2D_M2M;
 hdma2d.Init.ColorMode          = DMA2D_ARGB8888;
 hdma2d.Init.OutputOffset       = 0;
 hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
  if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
 if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  if(HAL_DMA2D_Start_IT(&hdma2d, 
                        SourceAddress, /* Color value in Register to Memory DMA2D mode */
                        DstAddress,  /* DMA2D output buffer */
                        DisplayWIDTH, /* width of buffer in pixels */
                        DisplayHEIGHT) /* height of buffer in lines */ 
     != HAL_OK)
    {
    M_pull()(); 
     }  
    }
 
}

void _HW_Fill_Region(u32 DstAddress, uint32_t xSize, uint32_t ySize, uint32_t OffLine, u32 color) 
{
  /* Register to memory mode with ARGB8888 as color Mode */ 
   hdma2d.Init.Mode         = DMA2D_R2M;
   hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
   hdma2d.Init.OutputOffset = OffLine;      
   hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
   
  /* DMA2D Initialization */

 if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
      if (HAL_DMA2D_Start_IT(&hdma2d, color, DstAddress, xSize, ySize) == HAL_OK)
      {
    
    M_pull()(); 
       
   }
  }
}

void _HW_Fill_Image(u32 SrcAddress, u32 DstAddress, uint32_t xSize, uint32_t ySize) 
{
  /* Register to memory mode with ARGB8888 as color Mode */ 
  
while((DMA2D->CR & DMA2D_CR_START)) 
                         RoutineMedium();  

   hdma2d.Init.Mode         = DMA2D_M2M;
   hdma2d.Init.ColorMode    = DMA2D_ARGB8888;
   hdma2d.Init.OutputOffset = DisplayWIDTH - xSize;      
   hdma2d.XferCpltCallback = Transfer_DMA2D_Completed;
   
  /* DMA2D Initialization */

 if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
      if (HAL_DMA2D_Start_IT(&hdma2d, SrcAddress, DstAddress, xSize, ySize) == HAL_OK)
      {
   while((DMA2D->CR) & DMA2D_CR_START) 
                         RoutineMedium(); 
   }
  }
}

void _HW_Fill_ImageToRAM(u32 SrcAddress, u32 DstAddress, uint32_t xSize, uint32_t  ySize){
 while((DMA2D->CR & DMA2D_CR_START)) 
                         RoutineMedium();  

   hdma2d.Init.Mode             = DMA2D_M2M;
   hdma2d.Init.ColorMode        = DMA2D_ARGB8888;
   hdma2d.Init.OutputOffset     = 0;      
   hdma2d.XferCpltCallback      = Transfer_DMA2D_Completed;
   
 if(HAL_DMA2D_Init(&hdma2d) == HAL_OK) 
  if(PLC_DMA2D_Status.Ready != 0){
  PLC_DMA2D_Status.Ready = 0;
      if (HAL_DMA2D_Start_IT(&hdma2d, SrcAddress, DstAddress, xSize, ySize) == HAL_OK)
      {
   while((DMA2D->CR) & DMA2D_CR_START) 
                         RoutineMedium(); 
   }
  }

}

void LCD_Layers_Init(void){
  _HW_Fill_Finite_Color(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, 0xFFFFFFFF);
  while(!PLC_DMA2D_Status.Ready)RoutineMedium(); 
  //fill the first layer  
  _HW_Fill_Display_From_Mem(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, SDRAM_BANK_ADDR + LAYER_1_OFFSET);
  while(!PLC_DMA2D_Status.Ready)RoutineMedium(); 
  //fill the second layer
   _HW_Fill_Display_From_Mem(SDRAM_BANK_ADDR + LAYER_BACK_OFFSET, SDRAM_BANK_ADDR + LAYER_2_OFFSET);
   while(!PLC_DMA2D_Status.Ready)RoutineMedium(); 
}

  
void LoadBitmapFromSD(uint8_t *NameOfFile, uint32_t AddressOfImage)//
{
  uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
 // uint32_t address;
  uint32_t input_color_mode = 0;
  uint8_t pbmp[DisplayWIDTH*4];
  u8 res; //variable for return values
  FATFS fs; //fat object FATFS
  FIL OurFile; // this is our file here
  UINT br; //just counter
  DWORD PositionOfFile, LineBytesSize;
  
  res = f_mount(&fs,"0:",1);
if (res == FR_OK){
  //open the file
  res = f_open(&OurFile,(char const*)NameOfFile,FA_READ);
//  f_lseek(&OurFile,0); //pointer to the first byte
  f_read(&OurFile, pbmp, 30, &br);
 // f_write(&OurFile,buff, NumberOfBytes, &br); //write the file

 }
 else{
   //�� ������� ������������ ����
 }


  /* Get bitmap data address offset */
  index = *(__IO uint16_t *) (pbmp + 10);
  index |= (*(__IO uint16_t *) (pbmp + 12)) << 16;
  
  /* Read bitmap width */
  width = *(uint16_t *) (pbmp + 18);
  width |= (*(uint16_t *) (pbmp + 20)) << 16;
  
  /* Read bitmap height */
  height = *(uint16_t *) (pbmp + 22);
  height |= (*(uint16_t *) (pbmp + 24)) << 16; 
  
  /* Read bit/pixel */
  bit_pixel = *(uint16_t *) (pbmp + 28);   
  
  /* Set the address */
  //address = AddressOfImage;
  
  /* Get the layer pixel format */    
  if ((bit_pixel/8) == 4)
  {
    input_color_mode = CM_ARGB8888;
  }
  else if ((bit_pixel/8) == 2)
  {
    input_color_mode = CM_RGB565;   
  }
  else 
  {
    input_color_mode = CM_RGB888;
  }
  //// here we can read
  /* Bypass the bitmap header */
 // pbmp += (index + (width * (height - 1) * (bit_pixel/8))); 
  PositionOfFile = index + (width * (height - 1) * (bit_pixel/8));
  LineBytesSize = (bit_pixel/8)*DisplayWIDTH;
  res = f_lseek(&OurFile, PositionOfFile); //pointer to the last line of bitmap
  res = f_read(&OurFile, &pbmp[0], LineBytesSize, &br);

  /* Convert picture to ARGB8888 pixel format */
  for(index=0; index < height; index++)
  {
    /* Pixel format conversion */
    LL_ConvertLineToARGB8888(pbmp, (void *)AddressOfImage, (uint32_t) width,(uint32_t) input_color_mode);
    
    /* Increment the source and destination buffers */
    AddressOfImage +=  (DisplayWIDTH*4);
    PositionOfFile -= width*(bit_pixel/8);
  f_lseek(&OurFile, PositionOfFile); //pointer to the last line of bitmap
  f_read(&OurFile, pbmp, LineBytesSize, &br);
  } 
  f_close(&OurFile);//close the file
  f_mount(NULL, "0:", 0);//unmount the drive
  
}  
   
void Transfer_DMA2D_Completed(DMA2D_HandleTypeDef *hdma2d){
  
  PLC_DMA2D_Status.Ready = 1;
//  RCC->PLLSAICFGR = 0x44003FC0;
}
