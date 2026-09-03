/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : 5 displays SH8103AS - desplazamiento de nombres
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdint.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/*
 * ============================================================
 * CONEXIONES STM32F103C8T6 -> SH8103AS
 * ============================================================
 *
 * PA0  -> pin 1  -> segmento A
 * PA1  -> pin 18 -> segmento B
 * PA2  -> pin 16 -> segmento C
 * PA3  -> pin 13 -> segmento D
 * PA4  -> pin 10 -> segmento E
 * PA5  -> pin 9  -> segmento F
 * PA6  -> pin 8  -> segmento G
 * PA7  -> pin 4  -> segmento H
 * PA8  -> pin 3  -> segmento K
 * PA9  -> pin 2  -> segmento M
 * PA10 -> pin 17 -> segmento N
 * PA11 -> pin 15 -> segmento P
 * PC14 -> pin 14 -> segmento R
 * PA12 -> pin 6  -> segmento S
 * PC15 -> pin 7  -> segmento T
 * PA15 -> pin 5  -> segmento U
 *
 * PIN 11 DEL SH8103AS:
 *   COMUN del display -> transistor correspondiente.
 *
 * PIN 12 DEL SH8103AS:
 *   NO SE USA. DEJAR SIN CONECTAR.
 *
 * IMPORTANTE:
 * PA12 DEL STM32 NO ES el pin 12 del display.
 * PA12 DEL STM32 va al PIN 6 del SH8103AS (segmento S).
 */

/* Bits internos para los 16 segmentos */
#define SEG_A   (1U << 0)
#define SEG_B   (1U << 1)
#define SEG_C   (1U << 2)
#define SEG_D   (1U << 3)
#define SEG_E   (1U << 4)
#define SEG_F   (1U << 5)
#define SEG_G   (1U << 6)
#define SEG_H   (1U << 7)
#define SEG_K   (1U << 8)
#define SEG_M   (1U << 9)
#define SEG_N   (1U << 10)
#define SEG_P   (1U << 11)
#define SEG_R   (1U << 12)
#define SEG_S   (1U << 13)
#define SEG_T   (1U << 14)
#define SEG_U   (1U << 15)

/*
 * Seleccion de los 5 displays:
 *
 * Display 1 -> PB4
 * Display 2 -> PB3
 * Display 3 -> PB9
 * Display 4 -> PB1
 * Display 5 -> PB0
 */
#define DISPLAY_MASK (GPIO_PIN_4 | GPIO_PIN_3 | GPIO_PIN_9 | GPIO_PIN_1 | GPIO_PIN_0)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint16_t letras[26];

const int numeroDisplays = 5;

/*
 * Se agregan 5 espacios al inicio y al final para que
 * cada nombre entre completamente por Display 1,
 * avance hasta Display 5 y salga completamente.
 */
char nombreReposo[] = "     ";
char nombreJuan[]   = "     JUAN MANUEL BUITRAGO     ";
char nombreGerman[] = "     GERMAN HIDALGO     ";
char nombreHarold[] = "     HAROLD GONZALEZ     ";

char *nombreActual = nombreReposo;

int longitud = 5;
int token = 0;

/*
 * Estados del pulsador:
 * 0 = reposo
 * 1 = JUAN MANUEL BUITRAGO
 * 2 = GERMAN HIDALGO
 * 3 = HAROLD GONZALEZ
 */
int estadoNombre = 0;

/*
 * Pines de los transistores en el mismo orden de los displays.
 */
static const uint16_t displayPins[5] =
{
    GPIO_PIN_4,   /* Display 1 -> PB4 */
    GPIO_PIN_3,   /* Display 2 -> PB3 */
    GPIO_PIN_9,   /* Display 3 -> PB9 */
    GPIO_PIN_1,   /* Display 4 -> PB1 */
    GPIO_PIN_0    /* Display 5 -> PB0 */
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
void mostrarLetra(uint16_t codigo);
void definirLetras(void);
uint16_t getLetra(char letra);
void apagarDisplays(void);
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

  int displayActual = 0;
  int aux = 0;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */

  definirLetras();

  nombreActual = nombreReposo;
  longitud = strlen(nombreActual);

  apagarDisplays();
  mostrarLetra(0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    /*
     * ============================================================
     * 1. PULSADOR EN PC13
     * ============================================================
     *
     * PC13 tiene PULL-UP interno:
     * sin pulsar = HIGH
     * pulsado    = LOW
     */
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
    {
        HAL_Delay(50);   /* anti-rebote */

        if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
        {
            estadoNombre++;

            if (estadoNombre > 3)
            {
                estadoNombre = 0;
            }

            switch (estadoNombre)
            {
                case 0:
                    nombreActual = nombreReposo;
                    break;

                case 1:
                    nombreActual = nombreJuan;
                    break;

                case 2:
                    nombreActual = nombreGerman;
                    break;

                case 3:
                    nombreActual = nombreHarold;
                    break;

                default:
                    nombreActual = nombreReposo;
                    estadoNombre = 0;
                    break;
            }

            longitud = strlen(nombreActual);

            /* Reinicia el desplazamiento desde el principio */
            token = 0;
            aux = 0;
            displayActual = 0;

            /* Esperar hasta que se suelte el pulsador */
            while (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET)
            {
                HAL_Delay(10);
            }

            HAL_Delay(50);   /* anti-rebote al soltar */
        }
    }

    /*
     * ============================================================
     * 2. MULTIPLEXACION DE LOS 5 DISPLAYS
     * ============================================================
     */

    /* Apagar todos los comunes antes de cambiar segmentos */
    apagarDisplays();

    /* Apagar todos los segmentos */
    mostrarLetra(0);

    /*
     * Esta formula hace que el texto:
     *
     * Display 1 -> Display 2 -> Display 3 -> Display 4 -> Display 5
     *
     * Es decir, entra por el Display 1 y sale por el Display 5.
     */
    int index = (token + (numeroDisplays - 1 - displayActual)) % longitud;

    /* Cargar la letra correspondiente */
    mostrarLetra(getLetra(nombreActual[index]));

    /* Encender solamente el transistor del display actual */
    GPIOB->BSRR = displayPins[displayActual];

    /* Tiempo de encendido de cada display */
    HAL_Delay(1);

    /*
     * ============================================================
     * 3. CONTROL DEL DESPLAZAMIENTO
     * ============================================================
     */
    displayActual++;

    if (displayActual >= numeroDisplays)
    {
        displayActual = 0;
        aux++;

        /*
         * Velocidad de desplazamiento.
         *
         * Mayor numero = mas lento.
         * Menor numero = mas rapido.
         *
         * 30 es un valor inicial razonable.
         */
        if (aux >= 30)
        {
            aux = 0;

            if (estadoNombre != 0)
            {
                token++;

                if (token >= longitud)
                {
                    token = 0;
                }
            }
        }
    }

    /* USER CODE END 3 */
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType =
      RCC_CLOCKTYPE_HCLK |
      RCC_CLOCKTYPE_SYSCLK |
      RCC_CLOCKTYPE_PCLK1 |
      RCC_CLOCKTYPE_PCLK2;

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

  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /*
   * PA15, PB3 y PB4 normalmente pertenecen a JTAG.
   *
   * Desactivamos JTAG pero mantenemos SWD activo,
   * por lo que ST-LINK sigue funcionando por SWDIO/SWCLK.
   */
  __HAL_RCC_AFIO_CLK_ENABLE();
  __HAL_AFIO_REMAP_SWJ_NOJTAG();

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  /*
   * ============================================================
   * ESTADO INICIAL DE LOS SEGMENTOS
   * ============================================================
   */

  HAL_GPIO_WritePin(
      GPIOA,
      GPIO_PIN_0  |
      GPIO_PIN_1  |
      GPIO_PIN_2  |
      GPIO_PIN_3  |
      GPIO_PIN_4  |
      GPIO_PIN_5  |
      GPIO_PIN_6  |
      GPIO_PIN_7  |
      GPIO_PIN_8  |
      GPIO_PIN_9  |
      GPIO_PIN_10 |
      GPIO_PIN_11 |
      GPIO_PIN_12 |
      GPIO_PIN_15,
      GPIO_PIN_RESET
  );

  HAL_GPIO_WritePin(
      GPIOC,
      GPIO_PIN_14 | GPIO_PIN_15,
      GPIO_PIN_RESET
  );

  /*
   * Los cinco transistores empiezan apagados.
   */
  HAL_GPIO_WritePin(
      GPIOB,
      DISPLAY_MASK,
      GPIO_PIN_RESET
  );

  /*
   * ============================================================
   * GPIOA - SEGMENTOS
   * ============================================================
   *
   * PA0-PA12 y PA15 son salidas.
   *
   * PA12 del STM32 se conecta al PIN 6 del display.
   * El PIN 12 fisico del SH8103AS NO se conecta.
   */
  GPIO_InitStruct.Pin =
      GPIO_PIN_0  |
      GPIO_PIN_1  |
      GPIO_PIN_2  |
      GPIO_PIN_3  |
      GPIO_PIN_4  |
      GPIO_PIN_5  |
      GPIO_PIN_6  |
      GPIO_PIN_7  |
      GPIO_PIN_8  |
      GPIO_PIN_9  |
      GPIO_PIN_10 |
      GPIO_PIN_11 |
      GPIO_PIN_12 |
      GPIO_PIN_15;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*
   * ============================================================
   * PC14 Y PC15 - SEGMENTOS R Y T
   * ============================================================
   */
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*
   * ============================================================
   * PC13 - PULSADOR
   * ============================================================
   */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*
   * ============================================================
   * TRANSISTORES DE LOS 5 DISPLAYS
   * ============================================================
   *
   * PB4 -> Display 1
   * PB3 -> Display 2
   * PB9 -> Display 3
   * PB1 -> Display 4
   * PB0 -> Display 5
   */
  GPIO_InitStruct.Pin = DISPLAY_MASK;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief Apaga los cinco transistores de seleccion.
  */
void apagarDisplays(void)
{
    GPIOB->BSRR = ((uint32_t)DISPLAY_MASK << 16U);
}

/**
  * @brief Envia el patron de una letra a los 16 segmentos.
  *
  * SH8103AS de catodo comun:
  * segmento HIGH = segmento encendido,
  * siempre que el transistor del display correspondiente este activo.
  */
void mostrarLetra(uint16_t codigo)
{
    uint32_t mascaraA =
        GPIO_PIN_0  |
        GPIO_PIN_1  |
        GPIO_PIN_2  |
        GPIO_PIN_3  |
        GPIO_PIN_4  |
        GPIO_PIN_5  |
        GPIO_PIN_6  |
        GPIO_PIN_7  |
        GPIO_PIN_8  |
        GPIO_PIN_9  |
        GPIO_PIN_10 |
        GPIO_PIN_11 |
        GPIO_PIN_12 |
        GPIO_PIN_15;

    /*
     * Apagar todos los segmentos conectados a GPIOA.
     */
    GPIOA->BSRR = (mascaraA << 16U);

    /*
     * Apagar PC14 y PC15.
     */
    GPIOC->BSRR =
        ((uint32_t)(GPIO_PIN_14 | GPIO_PIN_15) << 16U);

    /*
     * codigo = 0 corresponde a espacio.
     */
    if (codigo == 0)
    {
        return;
    }

    /*
     * ============================================================
     * BIT -> STM32 -> PIN SH8103AS
     * ============================================================
     */

    if (codigo & SEG_A) GPIOA->BSRR = GPIO_PIN_0;   /* A -> PA0  -> pin 1  */
    if (codigo & SEG_B) GPIOA->BSRR = GPIO_PIN_1;   /* B -> PA1  -> pin 18 */
    if (codigo & SEG_C) GPIOA->BSRR = GPIO_PIN_2;   /* C -> PA2  -> pin 16 */
    if (codigo & SEG_D) GPIOA->BSRR = GPIO_PIN_3;   /* D -> PA3  -> pin 13 */
    if (codigo & SEG_E) GPIOA->BSRR = GPIO_PIN_4;   /* E -> PA4  -> pin 10 */
    if (codigo & SEG_F) GPIOA->BSRR = GPIO_PIN_5;   /* F -> PA5  -> pin 9  */
    if (codigo & SEG_G) GPIOA->BSRR = GPIO_PIN_6;   /* G -> PA6  -> pin 8  */
    if (codigo & SEG_H) GPIOA->BSRR = GPIO_PIN_7;   /* H -> PA7  -> pin 4  */
    if (codigo & SEG_K) GPIOA->BSRR = GPIO_PIN_8;   /* K -> PA8  -> pin 3  */
    if (codigo & SEG_M) GPIOA->BSRR = GPIO_PIN_9;   /* M -> PA9  -> pin 2  */
    if (codigo & SEG_N) GPIOA->BSRR = GPIO_PIN_10;  /* N -> PA10 -> pin 17 */
    if (codigo & SEG_P) GPIOA->BSRR = GPIO_PIN_11;  /* P -> PA11 -> pin 15 */

    if (codigo & SEG_R) GPIOC->BSRR = GPIO_PIN_14;  /* R -> PC14 -> pin 14 */

    /*
     * MUY IMPORTANTE:
     * PA12 DEL STM32 VA AL PIN 6 DEL SH8103AS.
     * NO VA AL PIN 12 DEL DISPLAY.
     */
    if (codigo & SEG_S) GPIOA->BSRR = GPIO_PIN_12;  /* S -> PA12 -> pin 6  */

    if (codigo & SEG_T) GPIOC->BSRR = GPIO_PIN_15;  /* T -> PC15 -> pin 7  */
    if (codigo & SEG_U) GPIOA->BSRR = GPIO_PIN_15;  /* U -> PA15 -> pin 5  */
}

/**
  * @brief Tabla completa A-Z para el display de 16 segmentos.
  *
  * Orden de bits:
  * A B C D E F G H K M N P R S T U
  *
  * Los patrones corresponden al esquema de segmentos
  * utilizado por este tipo de display alfanumerico.
  */
void definirLetras(void)
{
    for (int i = 0; i < 26; i++)
    {
        letras[i] = 0;
    }

    /* A */
    letras['A' - 'A'] =
        SEG_A | SEG_B |
        SEG_C | SEG_D |
        SEG_G | SEG_H |
        SEG_T | SEG_U;

    /* B */
    letras['B' - 'A'] =
        SEG_A | SEG_B |
        SEG_C | SEG_D |
        SEG_E | SEG_F |
        SEG_G | SEG_H |
        SEG_T | SEG_U;

    /* D */
    letras['D' - 'A'] =
        SEG_A | SEG_B |
        SEG_C | SEG_D |
        SEG_E | SEG_F |
        SEG_G | SEG_H;

    /* E */
    letras['E' - 'A'] =
        SEG_A | SEG_B |
        SEG_E | SEG_F |
        SEG_G | SEG_H |
        SEG_T | SEG_U;

    /* G */
    letras['G' - 'A'] =
        SEG_A | SEG_B |
        SEG_D |
        SEG_E | SEG_F |
        SEG_G | SEG_H |
        SEG_U;

    /* H */
    letras['H' - 'A'] =
        SEG_C | SEG_D |
        SEG_G | SEG_H |
        SEG_T | SEG_U;

    /* I */
    letras['I' - 'A'] =
        SEG_A | SEG_B |
        SEG_E | SEG_F |
        SEG_M | SEG_R;

    /* J */
    letras['J' - 'A'] =
        SEG_C | SEG_D |
        SEG_E | SEG_F |
        SEG_G;

    /* L */
    letras['L' - 'A'] =
        SEG_E | SEG_F |
        SEG_G | SEG_H;

    /* M */
    letras['M' - 'A'] =
        SEG_C | SEG_D |
        SEG_G | SEG_H |
        SEG_K | SEG_N;

    /* N */
    letras['N' - 'A'] =
        SEG_C | SEG_D |
        SEG_G | SEG_H |
        SEG_K | SEG_P;

    /* O */
    letras['O' - 'A'] =
        SEG_A | SEG_B |
        SEG_C | SEG_D |
        SEG_E | SEG_F |
        SEG_G | SEG_H;

    /* R */
    letras['R' - 'A'] =
        SEG_A | SEG_B |
        SEG_C |
        SEG_G | SEG_H |
        SEG_T | SEG_U |
        SEG_P;

    /* T */
    letras['T' - 'A'] =
        SEG_A | SEG_B |
        SEG_M | SEG_R;

    /* U */
    letras['U' - 'A'] =
        SEG_C | SEG_D |
        SEG_E | SEG_F |
        SEG_G | SEG_H;

    /* Z */
    letras['Z' - 'A'] =
        SEG_A | SEG_B |
        SEG_E | SEG_F |
        SEG_N | SEG_S;
}
/**
  * @brief Convierte una letra ASCII a su patron de segmentos.
  */
uint16_t getLetra(char letra)
{
    if (letra == ' ')
    {
        return 0;
    }

    /*
     * Permite tambien minusculas por seguridad.
     */
    if ((letra >= 'a') && (letra <= 'z'))
    {
        letra = (char)(letra - 'a' + 'A');
    }

    if ((letra < 'A') || (letra > 'Z'))
    {
        return 0;
    }

    return letras[letra - 'A'];
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */

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
  * @param  line: source line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */

  (void)file;
  (void)line;

  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
