/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int letras[26];
int numeroDisplays = 5;

// Textos completos con espacios al final para el barrido continuo
char nombreReposo[] = "     ";
char nombreGerman[]   = "    GERMAN ALBERTO HIDALGO ALVARADO ";
char nombreJuan[]  = "    JUAN MANUEL BUITRAGO LANCHEROS ";
char nombreHarold[]  = "    HAROLD MAURICIO GONZALES MUNEVAR ";

char* nombreActual = nombreReposo;

int longitud = 5;
int token = 0;
int estadoNombre = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void mostrarLetra(int codigo);
void definirLetras(void);
int getLetra(char letra);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
    int displayActual = 0;
    int aux = 0;
    int estadoBotonAnterior = 1; // 1 equivale a GPIO_PIN_SET
    /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */
  	  definirLetras();
      nombreActual = nombreReposo; // Arranca en reposo (pantallas apagadas)
      longitud = strlen(nombreActual);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */

	        // 1. LECTURA DEL BOTÓN (Sin pausar el código)
	        int estadoBotonActual = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);

	        // Si el botón se acaba de presionar (pasó de 1 a 0)
	        if(estadoBotonActual == GPIO_PIN_RESET && estadoBotonAnterior == GPIO_PIN_SET){

	            estadoNombre++;
	            if(estadoNombre > 3) estadoNombre = 0; // Pasa de 3 (Profe) de nuevo a 0 (Reposo)

	            if(estadoNombre == 0)      nombreActual = nombreReposo;
	            else if(estadoNombre == 1) nombreActual = nombreGerman;
	            else if(estadoNombre == 2) nombreActual = nombreJuan;
	            else if(estadoNombre == 3) nombreActual = nombreHarold;

	            longitud = strlen(nombreActual);
	            token = 0;
	            aux = 0;
	        }
	        estadoBotonAnterior = estadoBotonActual; // Guarda el estado para el siguiente ciclo

	        // 2. MULTIPLEXACIÓN LIMPIA
	        // Apaga los transistores (PB5 a PB9)
	        GPIOB->BSRR = (GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9) << 16;
	        mostrarLetra(0);

	        int index = (token + displayActual) % longitud;
	        mostrarLetra(getLetra(nombreActual[index]));

	        // Enciende el transistor de la pantalla actual (PB5 es display 0, PB9 es display 4)
	        GPIOB->BSRR = (1 << (displayActual + 5));

	        HAL_Delay(1);

	        // 3. CONTROL DE DESPLAZAMIENTO FLUIDO
	        displayActual++;
	        if(displayActual >= numeroDisplays) {
	            displayActual = 0;
	            aux++;

	            // Subimos a 150 para que no pase demasiado rápido
	            if(aux >= 35){
	                aux = 0;
	                if(estadoNombre != 0){
	                    token++;
	                    if(token >= longitud) token = 0;
	                }
	            }
	        }
	      }
	    /* USER CODE END 3 */
      }
/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // --- LIBERAR JTAG Y MANTENER SWD (ST-LINK) ACTIVO ---
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level para Segmentos A (PA0 a PA10) */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level para Segmentos B y Transistores (PB5-PB9) */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /* Configuración de Puertos A */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configuración de Puertos B (Salidas) */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Configuración Botón en PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void mostrarLetra(int codigo){
    // 1. APAGAR TODOS los pines de segmentos en GPIOA y GPIOB
    GPIOA->BSRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                   GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                   GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10) << 16;

    GPIOB->BSRR = (GPIO_PIN_0 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15) << 16;

    if(codigo == 0) return; // Si es espacio, apagado

    // 2. ENCENDER los pines según los bits del código binario
    if (codigo & (1 << 0))  GPIOA->BSRR = GPIO_PIN_0;   // Bit 0  -> Pin 1  (PA0)
    if (codigo & (1 << 1))  GPIOA->BSRR = GPIO_PIN_1;   // Bit 1  -> Pin 2  (PA1)
    if (codigo & (1 << 2))  GPIOA->BSRR = GPIO_PIN_2;   // Bit 2  -> Pin 3  (PA2)
    if (codigo & (1 << 3))  GPIOA->BSRR = GPIO_PIN_3;   // Bit 3  -> Pin 4  (PA3)
    if (codigo & (1 << 4))  GPIOA->BSRR = GPIO_PIN_4;   // Bit 4  -> Pin 5  (PA4)
    if (codigo & (1 << 5))  GPIOA->BSRR = GPIO_PIN_5;   // Bit 5  -> Pin 6  (PA5)
    if (codigo & (1 << 6))  GPIOA->BSRR = GPIO_PIN_6;   // Bit 6  -> Pin 7  (PA6)
    if (codigo & (1 << 7))  GPIOA->BSRR = GPIO_PIN_7;   // Bit 7  -> Pin 8  (PA7)
    if (codigo & (1 << 8))  GPIOB->BSRR = GPIO_PIN_0;   // Bit 8  -> Pin 9  (PB0)
    if (codigo & (1 << 9))  GPIOA->BSRR = GPIO_PIN_10;  // Bit 9  -> Pin 10 (PA10)
    if (codigo & (1 << 10)) GPIOA->BSRR = GPIO_PIN_9;   // Bit 10 -> Pin 11 (PA9)
    if (codigo & (1 << 11)) GPIOA->BSRR = GPIO_PIN_8;   // Bit 11 -> Pin 12 (PA8)
    if (codigo & (1 << 12)) GPIOB->BSRR = GPIO_PIN_15;  // Bit 12 -> Pin 13 (PB15)
    if (codigo & (1 << 13)) GPIOB->BSRR = GPIO_PIN_14;  // Bit 13 -> Pin 14 (PB14)
    if (codigo & (1 << 14)) GPIOB->BSRR = GPIO_PIN_13;  // Bit 14 -> Pin 15 (PB13)
    if (codigo & (1 << 15)) GPIOB->BSRR = GPIO_PIN_12;  // Bit 15 -> Pin 16 (PB12)
}

void definirLetras(){
    for(int i = 0; i < 26; i++) letras[i] = 0;

    letras['A'-65] = 0b1011010010011001;
    letras['B'-65] = 0b1011011100100011;
    letras['C'-65] = 0b1000001110001001;
    letras['D'-65] = 0b1010011100100011;
    letras['E'-65] = 0b1000001110011001;
    letras['G'-65] = 0b1001011110001001;
    letras['H'-65] = 0b0011010010011000;
    letras['I'-65] = 0b1000001100100011;
    letras['J'-65] = 0b1010011110000000;
    letras['L'-65] = 0b0000001110001000;
    letras['M'-65] = 0b0110010010001100;
    letras['N'-65] = 0b0010110010001100;
    letras['O'-65] = 0b1010011110001001;
    letras['R'-65] = 0b1011100010011001;
    letras['S'-65] = 0b1001011100011001;
    letras['T'-65] = 0b1000000000100011;
    letras['U'-65] = 0b0010011110001000;
    letras['V'-65] = 0b0100000011001000;
    letras['Z'-65] = 0b1100001101000001;
}

int getLetra(char letra){
    if(letra == ' ') return 0;

    // Convertimos a mayúscula por si acaso para evitar desbordes
    if(letra >= 'a' && letra <= 'z') letra -= 32;

    return letras[(int)(letra - 65)];
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
