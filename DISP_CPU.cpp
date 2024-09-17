#include "DISP_CPU.h"

// ************************
// *     CONTROL UNIT     *
// ************************

DISP_CONTROL::DISP_CONTROL()
{
  // Setting up TFT display:
  Serial.print("Setting up TFT ... ");
  tft = new TFT_eSPI();
  tft->init(); // Initialise the TFT screen
  tft->setRotation(0); // Set the rotation
  tft->fillScreen(TFT_BLACK); // Clear the screen
  // tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN,
  // 				BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_GREEN); // Draw keypad background
  Serial.println("done.");
  // Setting touch screen calibration:
  tft->setTouch(calData);
  // // Bit memory init:
  // Serial.print("Initializing memory ... ");
  // for (int n=0; n<MEM_COLS; n++)
  // 	mem[n] = 155;
  Serial.println("done.");
  Serial.print("Setting up bit display ... ");
  show_mem();
  Serial.println("done.");
}

void DISP_CONTROL::show_row(unsigned int row)
{
  unsigned int bgcolor;
  tft->setFreeFont(BIT_FONT);
  tft->setTextSize(1);
  if (row==active_row)
    {
      tft->setTextColor(TFT_BLACK);
      bgcolor = TFT_WHITE;
    }
    else
    {
      tft->setTextColor(TFT_WHITE);
      bgcolor = bg[row];
    }
    tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN+row*BIT_AREA_DY,
                  BIT_AREA_XMAX-BIT_AREA_XMIN, BIT_AREA_DY,
                  bgcolor);
    for (int col=0; col<MEM_COLS; col++)
      tft->drawNumber(bitRead(mem[row],MEM_COLS-col-1),
                      BIT_TEXT_XMIN+col*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
}

void DISP_CONTROL::show_mem()
{

  tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN,
         BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_BLACK); // Draw keypad background
  for (int row=0; row<MEM_ROWS; row++)
    show_row(row);
}

void DISP_CONTROL::check()
{
  uint16_t tx = 0, ty = 0; // To store the touch coordinates
  // Get current touch state and coordinates
  bool pressed = tft->getTouch(&tx, &ty);

  if (pressed)
  {
    unsigned int col = int((tx - BIT_TEXT_XMIN) / BIT_AREA_DX);
    unsigned int row = int((ty - BIT_TEXT_YMIN) / BIT_AREA_DY);
    toggleBit(row, col);
    Serial.printf("row = %d   col = %d\n", row, col);
  }

}

void DISP_CONTROL::setrow(unsigned int row)
{
  if (row==active_row)
    return;
  unsigned int old_row = active_row;
  active_row = row;
  show_row(old_row);
  show_row(active_row);
}

unsigned int DISP_CONTROL::reverse6bit(unsigned int x)
{
	unsigned int xout = 0;
	for (int n=0; n<6; n++)
		xout += (bitRead(x,n)<<(5-n));
	return xout;
}

unsigned int DISP_CONTROL::get_ctrl(unsigned int row)
{
  return reverse6bit((mem[row] & 0b1111110000) >> 4);
}

unsigned int DISP_CONTROL::get_cset(unsigned int row)
{
  return ((mem[row] & 0b0000000100) >> 2);
}

unsigned int DISP_CONTROL::get_creset(unsigned int row)
{
  return ((mem[row] & 0b0000001000) >> 3);
}

unsigned int DISP_CONTROL::get_pcount(unsigned int row)
{
  return ((mem[row] & 0b0000000010) >> 1);
}

unsigned int DISP_CONTROL::get_pset(unsigned int row)
{
  return ((mem[row] & 0b0000000001));
}

void DISP_CONTROL::toggleBit(unsigned int row, unsigned int col)
{
  // bitWrite(mem[col], row, !bitRead(mem[col], row));
  // show_mem();
  setrow(row);
}

// unsigned char LED_CONTROL::reverse4bit(unsigned char x)
// {
// 	unsigned char xout = 0;
// 	for (int n=0; n<4; n++)
// 		xout += (bitRead(x,n)<<(3-n));
// 	return xout;
// }

// void LED_CONTROL::update(unsigned char bus, unsigned char ctrl, unsigned char counter,
// 			unsigned char ccount, unsigned char cset, unsigned char creset,
// 			unsigned char pcount, unsigned char pset)
// {
// 	unsigned short leddata1 = ((ccount & 1)<<3)
// 	                        + ((counter & 0x0F)<<4)
// 	                        + ((creset & 1)<<10)
// 	                        + ((cset & 1)<<11)
// 	                        + ((reverse4bit(bus) & 0x0F)<<12);
// 	unsigned short leddata2 = (bitRead(ctrl, CTRL_BIT_G1)<<1)
// 	                        + (bitRead(ctrl, CTRL_BIT_RB)<<2)
// 	                        + (bitRead(ctrl, CTRL_BIT_RA)<<3)
// 	                        + (bitRead(ctrl, CTRL_BIT_PM)<<4)
// 	                        + (bitRead(ctrl, CTRL_BIT_RC)<<5)
// 	                        + (bitRead(ctrl, CTRL_BIT_G2)<<6)
// 	                        + ((pcount & 1)<<7)
// 	                        + ((pset & 1)<<8);
// 	leds1->set_leds(leddata1);
// 	leds2->set_leds(leddata2);
// }
