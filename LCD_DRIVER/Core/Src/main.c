/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* USER CODE BEGIN PD */
#define RESET(c)    LL_GPIO_ResetOutputPin(c##_GPIO_Port, c##_Pin)
#define SET(c)      LL_GPIO_SetOutputPin(c##_GPIO_Port, c##_Pin)
#define DIR_OUT(c)  LL_GPIO_SetPinMode(c##_GPIO_Port, c##_Pin, LL_GPIO_MODE_OUTPUT)
#define DIR_IN(c)   LL_GPIO_SetPinMode(c##_GPIO_Port, c##_Pin, LL_GPIO_MODE_INPUT)
#define READ(c)     LL_GPIO_IsInputPinSet(c##_GPIO_Port, c##_Pin)
/* USER CODE END PD */

/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* USER CODE BEGIN PV */
/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
void Delay_uS(int uSTim) {
    for (int i = 0; i < uSTim * 8; i++) { __NOP(); }
}

void setLcdDataPort(uint8_t portDato) {
    if (portDato & 0x1) { SET(DISP_DB4); } else { RESET(DISP_DB4); }
    if (portDato & 0x2) { SET(DISP_DB5); } else { RESET(DISP_DB5); }
    if (portDato & 0x4) { SET(DISP_DB6); } else { RESET(DISP_DB6); }
    if (portDato & 0x8) { SET(DISP_DB7); } else { RESET(DISP_DB7); }
}

int lcdCheckBusy(void) {
    uint8_t tmpLcdData;
    int delay2 = 0;
    DIR_IN(DISP_DB4); DIR_IN(DISP_DB5);
    DIR_IN(DISP_DB6); DIR_IN(DISP_DB7);
    RESET(DISP_RS);
    SET(DISP_RW);
    do {
        Delay_uS(1);
        SET(DISP_EN);
        Delay_uS(1);
        tmpLcdData = READ(DISP_DB7);
        RESET(DISP_EN);
        Delay_uS(1);
        SET(DISP_EN);
        Delay_uS(1);
        RESET(DISP_EN);
        if (tmpLcdData == 0) break;
    } while (delay2++ < 200);
    RESET(DISP_RW);
    DIR_OUT(DISP_DB4); DIR_OUT(DISP_DB5);
    DIR_OUT(DISP_DB6); DIR_OUT(DISP_DB7);
    return 0;
}

void lcdSendCmd(char cmd) {
    lcdCheckBusy();
    RESET(DISP_RS);
    RESET(DISP_RW);
    setLcdDataPort((cmd >> 4) & 0x0F);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    setLcdDataPort(cmd & 0x0F);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
}

void lcdSendChar(char data) {
    lcdCheckBusy();
    SET(DISP_RS);
    RESET(DISP_RW);
    setLcdDataPort((data >> 4) & 0x0F);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    setLcdDataPort(data & 0x0F);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
}

void lcdInit(void) {
    RESET(DISP_EN); RESET(DISP_RS); RESET(DISP_RW);
    HAL_Delay(150);
    setLcdDataPort(0x3);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    HAL_Delay(5);
    setLcdDataPort(0x3);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    Delay_uS(150);
    setLcdDataPort(0x3);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    Delay_uS(150);
    setLcdDataPort(0x2);
    Delay_uS(1); SET(DISP_EN); Delay_uS(1); RESET(DISP_EN);
    Delay_uS(150);
    lcdSendCmd(0x28);
    lcdSendCmd(0x0C);
    lcdSendCmd(0x01);
    HAL_Delay(2);
    lcdSendCmd(0x06);
}
/* USER CODE END 0 */

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  lcdInit();
  char nome[] = "YASSINE SAKHOUNE ";
  for (int i = 0; nome[i] != '\0'; i++) {
      lcdSendChar(nome[i]);
  }
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE BEGIN 3 */
    /* USER CODE END 3 */
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_FLASH_SET_LATENCY(FLASH_LATENCY_1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  LL_GPIO_ResetOutputPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin);
  LL_GPIO_ResetOutputPin(USER_LED_GPIO_Port, USER_LED_Pin);
  LL_GPIO_ResetOutputPin(DISP_DB4_GPIO_Port, DISP_DB4_Pin);
  LL_GPIO_ResetOutputPin(DISP_EN_GPIO_Port, DISP_EN_Pin);
  LL_GPIO_ResetOutputPin(DISP_DB7_GPIO_Port, DISP_DB7_Pin);
  LL_GPIO_ResetOutputPin(DISP_DB5_GPIO_Port, DISP_DB5_Pin);
  LL_GPIO_ResetOutputPin(DISP_DB6_GPIO_Port, DISP_DB6_Pin);
  LL_GPIO_ResetOutputPin(DISP_RW_GPIO_Port, DISP_RW_Pin);
  LL_GPIO_ResetOutputPin(DISP_RS_GPIO_Port, DISP_RS_Pin);

  GPIO_InitStruct.Pin = USER_BUTTON_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(USER_BUTTON_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VCP_USART2_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(VCP_USART2_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = VCP_USART2_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(VCP_USART2_RX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_DB4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_DB4_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_EN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_EN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_DB7_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_DB7_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_DB5_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_DB5_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_DB6_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_DB6_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_RW_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_RW_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = DISP_RS_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(DISP_RS_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {}
#endif
