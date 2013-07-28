/**
  ******************************************************************************
  * @file    IO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"

#include "arm_math.h"
#include "math_helper.h"

const float32_t A_f32[16] =
{
        /* Const,       numTaps,        blockSize,      numTaps*blockSize */
        1.0,            32.0,           4.0,            128.0,
        1.0,            32.0,           64.0,           2048.0,
        1.0,            16.0,           4.0,            64.0,
        1.0,            16.0,           64.0,           1024.0,
};

const float32_t B_f32[16] =
{
        /* Const,       numTaps,        blockSize,      numTaps*blockSize */
        1.0,            32.0,           4.0,            128.0,
        1.0,            32.0,           64.0,           2048.0,
        1.0,            16.0,           4.0,            64.0,
        1.0,            16.0,           64.0,           1024.0,
};

/* ---------------------------------------------------------------------- 
 * * Temporary buffers  for storing intermediate values 
 * * ------------------------------------------------------------------- */
/* Output X Buffer */
float32_t X_f32[16];


/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
// какие регистры нужно включать описано
// One item I noticed is that you inadvertently commented out the lines following line 79 of your startup code by omitting a '/' at the end of the block comment. These few lines enable the FPU (see 7.3.1 of Cortex-M4 TRM) and are rather important.
//http://www.nabiltewolde.com/2011/10/programming-stm32f4discovery-board-with.html

    SCB->CPACR |= ((3UL << 10*2) |                 /* set CP10 Full Access */
                   (3UL << 11*2)  );               /* set CP11 Full Access */

    uint32_t i;

    arm_matrix_instance_f32 A;              /* Matrix A Instance */
    arm_matrix_instance_f32 B;              /* Matrix B instance */
    arm_matrix_instance_f32 X;              /* Matrix X(Unknown Matrix) instance */

    uint32_t srcRows, srcColumns;   /* Temporary variables */
    arm_status status;

   /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

   /* GPIOD Periph clock enable */
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

   /* Configure PD13 in output pushpull mode */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;

   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

   GPIO_SetBits(GPIOD, GPIO_Pin_13);

   int lamp = 0;
   while(1)
   { 
      for(i=0; i<1000000; i++)
      { 
         /* Initialise A Matrix Instance with numRows, numCols and data array(A_f32) */
         srcRows = 4;
         srcColumns = 4;
         arm_mat_init_f32(&A, srcRows, srcColumns, (float32_t *)A_f32);

         /* Initialise B Matrix Instance with numRows, numCols and data array(B_f32) */
         srcRows = 4;
         srcColumns = 4;
         arm_mat_init_f32(&B, srcRows, srcColumns, (float32_t *)B_f32);

         /* Initialise X Matrix Instance with numRows, numCols and data array(X_f32) */
         srcRows = 4;
         srcColumns = 4;
         arm_mat_init_f32(&X, srcRows, srcColumns, X_f32);

         /* calculation of B Multiply with A */
         status = arm_mat_mult_f32(&B, &A, &X);
      }
      lamp%2 == 0 ? GPIO_ResetBits(GPIOD, GPIO_Pin_13) : GPIO_SetBits(GPIOD, GPIO_Pin_13);
      lamp++;
   } 

}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
