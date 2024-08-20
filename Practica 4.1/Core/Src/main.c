/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    BUTTON_UP, BUTTON_FALLING, BUTTON_DOWN, BUTTON_RAISING,
} debounceState_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* Hago overflow a 0 cuando la variable llega al límite dado */
#define INC_SATURADO(variable, limite) do { \
    (variable)++; \
    if ((variable) >= (limite)) { \
        (variable) = 0; \
    } \
} while (0)

/* Devuelvo el tamaño de un array */
#define DIM(x) (sizeof(x)/sizeof(x[0]))

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

static const tick_t DEBOUNCING_TICKS = 40;
static debounceState_t machine_state = BUTTON_UP;
static delay_t debouncing_delay;

static const tick_t LED_TICKS = 500;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// debe encender el LED
void buttonPressed()
{
    BSP_LED_On(LED1);
    BSP_LED_Toggle(LED3);
}

// debe apagar el LED
void buttonReleased()
{
    BSP_LED_Off(LED1);
}

/* Carga el estado inicial */
void debounceFSM_init()
{
    machine_state = BUTTON_UP;
    delayInit(&debouncing_delay, DEBOUNCING_TICKS);
}

/* debe leer las entradas, resolver la lógica de
 transición de estados y actualizar las salidas */
void debounceFSM_update()
{
    bool button_state = BSP_PB_GetState(BUTTON_USER);
    switch (machine_state) {
    case BUTTON_UP:
        if (button_state) {
            machine_state = BUTTON_FALLING;
            delayInit(&debouncing_delay, DEBOUNCING_TICKS);
        }
        break;
    case BUTTON_FALLING:
        if (delayRead(&debouncing_delay)) {
            if (button_state) {
                machine_state = BUTTON_DOWN;
                buttonPressed();
            } else {
                machine_state = BUTTON_UP;
            }
        }
        break;
    case BUTTON_DOWN:
        if (!button_state) {
            machine_state = BUTTON_RAISING;
            delayInit(&debouncing_delay, DEBOUNCING_TICKS);
        }
        break;
    case BUTTON_RAISING:
        if (delayRead(&debouncing_delay)) {
            if (!button_state) {
                machine_state = BUTTON_UP;
                buttonReleased();
            } else {
                machine_state = BUTTON_DOWN;
            }
        }
        break;
    default:
        machine_state = BUTTON_UP;
    }
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval Doesn't return. Return type shouldn't be int here but the compiler complains wrongly
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    /* USER CODE BEGIN 2 */

    /* Init code */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
    BSP_PB_Init(BUTTON_USER, BUTTON_MODE_GPIO);

    debounceFSM_init();

    delay_t led_delay;
    delayInit(&led_delay, LED_TICKS);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        debounceFSM_update();

        if (delayRead(&led_delay)) {
            BSP_LED_Toggle(LED2);
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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                    | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

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
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
