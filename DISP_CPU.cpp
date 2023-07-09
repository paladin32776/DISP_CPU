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
  // Bit memory init:
  Serial.print("Initializing memory ... ");
  for (int n=0; n<MEM_COLS; n++)
  	mem[n] = 155;
  Serial.println("done.");
  Serial.print("Setting up bit display ... ");
  show_mem();
  Serial.println("done.");
}

void DISP_CONTROL::show_col(unsigned int col)
{
  unsigned int bgcolor;
  tft->setFreeFont(BIT_FONT);
  tft->setTextSize(1);
  if (col==active_col)
    {
      tft->setTextColor(TFT_BLACK);
      bgcolor = TFT_WHITE;
    }
    else
    {
      tft->setTextColor(TFT_WHITE);
      bgcolor = bg[col];
    }
    tft->fillRect(BIT_AREA_XMIN+col*BIT_AREA_DX, BIT_AREA_YMIN,
                  BIT_AREA_DX, BIT_AREA_YMAX-BIT_AREA_YMIN,
                  bgcolor);
    for (int row=0; row<MEM_ROWS; row++)
      tft->drawNumber(bitRead(mem[col],row), 
                      BIT_TEXT_XMIN+col*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
}

void DISP_CONTROL::show_mem()
{

  tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN, 
         BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_BLACK); // Draw keypad background
  for (int col=0; col<MEM_COLS; col++)
    show_col(col);
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
    toggleBit(col, row);
    Serial.printf("row = %d   col = %d\n", row, col);
  }

}

void DISP_CONTROL::setcol(unsigned int col)
{
  unsigned int old_col = active_col;
  active_col = col;
  show_col(old_col);
  show_col(active_col);
}

unsigned int DISP_CONTROL::get_ctrl(unsigned int col)
{
  return ((mem[col] & 0b0011111100) >> 2);
}

unsigned int DISP_CONTROL::get_cset(unsigned int col)
{
  return ((mem[col] & 0b0000000010) >> 1);
}

unsigned int DISP_CONTROL::get_creset(unsigned int col)
{
  return ((mem[col] & 0b0000000001));
}

unsigned int DISP_CONTROL::get_pcount(unsigned int col)
{
  return ((mem[col] & 0b1000000000) >> 9);
}

unsigned int DISP_CONTROL::get_pset(unsigned int col)
{
  return ((mem[col] & 0b0100000000) >> 8);
}

void DISP_CONTROL::toggleBit(unsigned int col, unsigned int row)
{
  // bitWrite(mem[col], row, !bitRead(mem[col], row));
  // show_mem();
  setcol(col);
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
