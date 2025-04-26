#include "mik32_hal_usart.h"
#include "mik32_hal_spi.h"
#include "timer32.h"
#include "xprintf.h"

#include "utils/pins.h"
#include "utils/delays.h"
#include "libs/IRreciever.h"
#include "libs/tone.h"


#define SYSTEM_FREQ_HZ 32000000UL
#define PWM_FREQ_HZ (100)
#define IR_PIN 2
#define PWM_PERIOD_TICKS (SYSTEM_FREQ_HZ / PWM_FREQ_HZ)
#define PWM_DUTY_CYCLE_PERCENT (50)
#define PWM_DUTY_CYCLE_TICKS ((PWM_PERIOD_TICKS / 100) * PWM_DUTY_CYCLE_PERCENT)


static void SystemClock_Config();
static void USART_Init();
static void GPIO_Init();
static void SPI_Init();
static void TMR_PWM_Init();
static void TMR_Init();

USART_HandleTypeDef husart0;
SPI_HandleTypeDef spi;

typedef struct a {
  float freq;
  float dur;
} note;

uint8_t i = 0;


int main() {
  SystemClock_Config();
  GPIO_Init();
  USART_Init();
  SPI_Init();
  TMR_PWM_Init();
  TMR_Init();

  // ir_set_pin(2);

  note notes[] = {
    {.freq = 466.16, .dur = 1000},
    {.freq = 100, .dur = 1000}
  };

  tone_init(3, TIMER32_1, TIMER32_0);
  set_position_change(&i);
  while (1) {
    tone(notes[i].freq, notes[i].dur);
    if (i >= 2) {
      i = 0;
    }
    char x = i + '0';
    HAL_USART_Print(&husart0, &x, USART_TIMEOUT_DEFAULT);
    delay(1000);
  }
  
}

void TMR_PWM_Init() {
  // PAD_CONFIG->PORT_0_CFG |= 0b10;
  // Включение тактирования TIMER32_1
  PM->CLK_APB_P_SET = PM_CLOCK_APB_P_TIMER32_1_M;
  TIMER32_1->ENABLE = 0;
  TIMER32_1->CONTROL =
      TIMER32_CONTROL_MODE_UP_M | TIMER32_CONTROL_CLOCK_PRESCALER_M;
  TIMER32_1->INT_CLEAR = 0xFFFFFFFF;

  TIMER32_1->CHANNELS[0].CNTRL =
      TIMER32_CH_CNTRL_MODE_PWM_M | TIMER32_CH_CNTRL_ENABLE_M;
      
  TIMER32_1->CHANNELS[0].OCR = 0;
  TIMER32_1->ENABLE = 1;
}

void TMR_Init() {
  // Включение тактирования TIMER32_0
  PM->CLK_APB_M_SET = PM_CLOCK_APB_M_TIMER32_0_M;
  TIMER32_0->ENABLE = 0;
  TIMER32_0->PRESCALER = 0;
  TIMER32_0->CONTROL =
      TIMER32_CONTROL_MODE_UP_M | TIMER32_CONTROL_CLOCK_PRESCALER_M;
  TIMER32_0->INT_MASK = 0;
  TIMER32_0->INT_CLEAR = 0xFFFFFFFF;
  // TIMER32_0->ENABLE = 1;
  // Включение прерывания по переполнению
  // TIMER32_0->INT_MASK = TIMER32_INT_OVERFLOW_M;
}


void SystemClock_Config(void) {
  PCC_InitTypeDef PCC_OscInit = {0};

  PCC_OscInit.OscillatorEnable = PCC_OSCILLATORTYPE_ALL;
  PCC_OscInit.FreqMon.OscillatorSystem = PCC_OSCILLATORTYPE_OSC32M;
  PCC_OscInit.FreqMon.ForceOscSys = PCC_FORCE_OSC_SYS_UNFIXED;
  PCC_OscInit.FreqMon.Force32KClk = PCC_FREQ_MONITOR_SOURCE_OSC32K;
  PCC_OscInit.AHBDivider = 0;
  PCC_OscInit.APBMDivider = 0;
  PCC_OscInit.APBPDivider = 0;
  PCC_OscInit.HSI32MCalibrationValue = 128;
  PCC_OscInit.LSI32KCalibrationValue = 8;
  PCC_OscInit.RTCClockSelection = PCC_RTC_CLOCK_SOURCE_AUTO;
  PCC_OscInit.RTCClockCPUSelection = PCC_CPU_RTC_CLOCK_SOURCE_OSC32K;
  HAL_PCC_Config(&PCC_OscInit);
}

void GPIO_Init() {
  /**< Включить  тактирование GPIO_0 */
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_GPIO_0_M;

  /**< Включить  тактирование GPIO_1 */
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_GPIO_1_M;

  /**< Включить  тактирование GPIO_2 */
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_GPIO_2_M;

  /**< Включить  тактирование схемы формирования прерываний GPIO */
  PM->CLK_APB_P_SET |= PM_CLOCK_APB_P_GPIO_IRQ_M;

  GPIO_InitTypeDef gpio_sclk = {
    .Pin  = GPIO_PIN_2,
    .Mode = HAL_GPIO_MODE_SERIAL,
    .Pull = HAL_GPIO_PULL_NONE,
    .DS   = HAL_GPIO_DS_2MA
  };

  HAL_GPIO_Init(GPIO_1, &gpio_sclk);

    GPIO_InitTypeDef gpio_mosi = {
        .Pin  = GPIO_PIN_1,
        .Mode = HAL_GPIO_MODE_SERIAL,
        .Pull = HAL_GPIO_PULL_NONE,
        .DS   = HAL_GPIO_DS_2MA
    };

    HAL_GPIO_Init(GPIO_1, &gpio_mosi);

    GPIO_InitTypeDef gpio_res = {
        .Pin  = GPIO_PIN_12,
        .Mode = HAL_GPIO_MODE_GPIO_OUTPUT,
        .Pull = HAL_GPIO_PULL_NONE,
        .DS   = HAL_GPIO_DS_2MA
    };

    HAL_GPIO_Init(GPIO_1, &gpio_res);

    GPIO_InitTypeDef gpio_dc = {
        .Pin  = GPIO_PIN_13,
        .Mode = HAL_GPIO_MODE_GPIO_OUTPUT,
        .Pull = HAL_GPIO_PULL_NONE,
        .DS   = HAL_GPIO_DS_2MA
    };

    HAL_GPIO_Init(GPIO_1, &gpio_dc);

    GPIO_InitTypeDef gpio_cs = {
        .Pin  = GPIO_PIN_5,
        .Mode = HAL_GPIO_MODE_SERIAL,
        .Pull = HAL_GPIO_PULL_NONE,
        .DS   = HAL_GPIO_DS_2MA
    };

    HAL_GPIO_Init(GPIO_1, &gpio_cs);

  // первая функция (порт общего назначения);
//   PAD_CONFIG->PORT_0_CFG |= 0 << (LED_PIN_NUM * 2);

  // нагрузочная способность 2 мА;
//   PAD_CONFIG->PORT_0_DS |= 0 << (LED_PIN_NUM * 2);

  // резисторы подтяжки отключены
//   PAD_CONFIG->PORT_0_PUPD |= 0 << (LED_PIN_NUM * 2);

  // Установка направления выводов как выход.
//   GPIO_2->DIRECTION_OUT = 1 << LED_PIN_NUM;

}

void USART_Init() {
  husart0.Instance = UART_0;
  husart0.transmitting = Enable;
  husart0.receiving = Enable;
  husart0.frame = Frame_8bit;
  husart0.parity_bit = Disable;
  husart0.parity_bit_inversion = Disable;
  husart0.bit_direction = LSB_First;
  husart0.data_inversion = Disable;
  husart0.tx_inversion = Disable;
  husart0.rx_inversion = Disable;
  husart0.swap = Disable;
  husart0.lbm = Disable;
  husart0.stop_bit = StopBit_1;
  husart0.mode = Asynchronous_Mode;
  husart0.xck_mode = XCK_Mode3;
  husart0.last_byte_clock = Disable;
  husart0.overwrite = Disable;
  husart0.rts_mode = AlwaysEnable_mode;
  husart0.channel_mode = Duplex_Mode;
  husart0.tx_break_mode = Disable;
  husart0.Interrupt.ctsie = Disable;
  husart0.Interrupt.eie = Disable;
  husart0.Interrupt.idleie = Disable;
  husart0.Interrupt.lbdie = Disable;
  husart0.Interrupt.peie = Disable;
  husart0.Interrupt.rxneie = Disable;
  husart0.Interrupt.tcie = Disable;
  husart0.Interrupt.txeie = Disable;
  husart0.Modem.rts = Disable; //out
  husart0.Modem.cts = Disable; //in
  husart0.Modem.dtr = Disable; //out
  husart0.Modem.dcd = Disable; //in
  husart0.Modem.dsr = Disable; //in
  husart0.Modem.ri = Disable;  //in
  husart0.Modem.ddis = Disable;//out
  husart0.baudrate = 115200;

  husart0.dma_tx_request = Enable;
  husart0.dma_rx_request = Disable;

  HAL_USART_Init(&husart0);
}

void SPI_Init() {
  spi.Instance = SPI_1;
  spi.Init.SPI_Mode = HAL_SPI_MODE_MASTER;

  spi.Init.BaudRateDiv = SPI_BAUDRATE_DIV64;
  spi.Init.ManualCS = SPI_MANUALCS_OFF;

  spi.Init.CLKPhase = SPI_PHASE_OFF;
  spi.Init.CLKPolarity = SPI_POLARITY_LOW;

  spi.Init.Decoder = SPI_DECODER_NONE;
  
  spi.Init.ChipSelect = SPI_CS_1;

  if (HAL_SPI_Init(&spi) == HAL_OK)
      HAL_USART_Print(&husart0, "SPI0 init OK\r\n", USART_TIMEOUT_DEFAULT);
  HAL_SPI_Enable(&spi);
}

// временная функция
void oled_temp() {
  GPIO_1->DIRECTION_OUT |= (1 << 12) | (1 << 13); // RST и D/C#
  GPIO_1->CLEAR |= (1 << 12) | (1 << 13);         // сброс
  delay(10);
  GPIO_1->OUTPUT |= (1 << 12);                    // RST = 1 (отпустить сброс)
  delay(10);

  // Передача команд
  GPIO_1->CLEAR |= (1 << 13); // D/C# = 0

  uint8_t init[] = {
    0xC8,         // COM Output Scan Direction: remapped mode
    0x8D, 0x14,   // Enable charge pump
    0xAF          // Display ON
  };

  uint8_t rx[1];
  HAL_SPI_Exchange(&spi, init, rx, sizeof(init), 3000);

  uint8_t white[128] = { [0 ... 127] = 0x0 }; // строка из нулей (все пиксели включены)
  for (int page = 0; page < 8; page++) {
      uint8_t set_page[] = {0xB0 + page, 0x00, 0x10};
      GPIO_1->CLEAR |= (1 << 13); // команда
      HAL_SPI_Exchange(&spi, set_page, rx, 3, -1);

      GPIO_1->OUTPUT |= (1 << 13); // данные
      HAL_SPI_Exchange(&spi, white, rx, sizeof(white), -1);
  }

  uint8_t set_page[] = {0xB0 + 5, 0x00, 0x10};
  GPIO_1->CLEAR |= (1 << 13); // команда
  HAL_SPI_Exchange(&spi, set_page, rx, 3, -1);
  uint8_t white2[128] = { [0 ... 127] = 0xFF };

  GPIO_1->OUTPUT |= (1 << 13); // данные
    HAL_SPI_Exchange(&spi, white2, rx, sizeof(white), -1);
}