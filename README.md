# Arduino Driver for Sharp LS018B7DH02 Memory LCD

## Introduction

This repository contains a modified version of the Adafruit Memory LCD driver, specifically for the Sharp LS018B7DH02.

## LS018B7DH02

<img src="images/lcd_image.jpg" width="500">
(image taken from nameless AliExpress Store)


Size:
* width: 31mm
* height: 41.46mm
* diagonal: 1.8inch

Resolution: 230 x 303

See website and [datasheet](datasheets/LS018B7DH02_31Oct23_Spec_LD-2023X08.pdf) for more information:

https://www.sharpsde.com/products/displays/model/ls018b7dh02/

## Driver Info

### Commnication Protocol

The main reason why existing memory LCD drivers don't work with the LS018B7DH02 is that it uses 9 bits for line numbers in the SPI communication format. 
The reason for this is that the LS018B7DH02 has 303 rows, so you need 9 bits to refer to a specific line row. The other smaller, more common, memory displays have a smaller resolution (e.g. the one sold by Adafruit), and their communication protocol uses 8 bits to refer to a line number. 

This is further complicated by the fact that the display needs this 9 bit number split into 2 bytes, in LSB order. 

From the LS018B7DH02 datasheet:

<img src="images/datasheet_data_row.jpg" width="600">

As you can see, the data to update a single row is:
* 2 bytes containing "command bits" and the row number to update 
* 30 bytes of image data, one bit per pixel
  * note that a row is only 230 bits, but it requires another 10 bits of "padding"
* 2 empty bytes 

To update more than one row, wait to send the final empty bytes until the last row:
* 2 bytes containing "command bits" and the row number to update
* 30 bytes of image data, one bit per pixel
* ...
* 2 bytes containing "command bits" and the row number to update
* 30 bytes of image data, one bit per pixel
* 2 empty bytes

### Implementation

I chose to modify the existing Adafruit Memory LCD driver, instead of starting from scratch.
This also makes it compatible with the Adafruit GFX library, with relatively little effort.

### How to Use

See `src/example.ino` for an example. 

## Speeding up the display refresh

To maintain a broad compatibility, I kept the use of slower Adafruit_SPIDevice for SPI transfers. 
However, depending on your board, you might achieve much faster refreshes by using board-specific hardware SPI.

To achieve this, simply replace the relevant SPI calls in the code, especially in the refresh function.

Currently the function looks something like this:
```c++
void Adafruit_SharpMem::refresh(void) {
    ...
    spidev->beginTransaction();
    ...
    spidev->transfer(...);
    ...
    spidev->endTransaction();
    ...
}
```

Simply replace these calls with whatever native flavour of SPI you want to use. For example, for the Seeed XIAO nrf52840 it looks like this:
```c++
#include <SPI.h>

SPI.begin(); // call this during for instance setup()
  
void Adafruit_SharpMem::refresh(void) {
    ...
    SPI.beginTransaction(SPISettings(4000000, LSBFIRST, SPI_MODE0));
    ...
    SPI.transfer(...);
    ...
    SPI.endTransaction();
    ...
}
```

As the support for hardware SPI, and the way to use it, differs from board to board, you have to make this change yourself if a higher refresh speed is required.

## Sources

* [https://www.sharpsde.com/products/displays/model/ls018b7dh02/](https://www.sharpsde.com/products/displays/model/ls018b7dh02/)
* [LS018B7DH02 Datasheet](datasheets/LS018B7DH02_31Oct23_Spec_LD-2023X08.pdf)
* [Sharp Application Notes: Programming Sharp’s Memory LCDs](datasheets/2016_SDE_App_Note_for_Memory_LCD_programming_V1.3.pdf)
* [https://github.com/jeffc/nrf52-watch-firmware/tree/master/lib/memory_lcd_spi](https://github.com/jeffc/nrf52-watch-firmware/tree/master/lib/memory_lcd_spi)
* [https://github.com/adafruit/Adafruit_SHARP_Memory_Display](https://github.com/adafruit/Adafruit_SHARP_Memory_Display)
* [https://www.azumotech.com/products/1-8-monochrome-display-12616-03/](https://www.azumotech.com/products/1-8-monochrome-display-12616-03/)