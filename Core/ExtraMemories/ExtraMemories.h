#pragma once

// E.g. int SPI_FLASH_DATA g_Initialized = 1;
#ifdef FLASH_EXTERNAL_MEMORY
#define SPI_FLASH_DATA __attribute__((section(".spi_flash_data")))
#else
#define SPI_FLASH_DATA __attribute__(())
#endif
