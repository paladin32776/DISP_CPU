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
  for (int n=0; n<CONTROL_MEM_COLS; n++)
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
    for (int row=0; row<CONTROL_MEM_ROWS; row++)
      tft->drawNumber(bitRead(mem[col],row), 
                      BIT_TEXT_XMIN+col*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
}

void DISP_CONTROL::show_mem()
{

  tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN, 
         BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_BLACK); // Draw keypad background
  for (int col=0; col<CONTROL_MEM_COLS; col++)
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


// ************************
// *    PROGRAM MEMORY    *
// ************************

DISP_PROGRAM::DISP_PROGRAM()
{
  // Setting up TFT display:
  Serial.print("Setting up TFT ... ");
  tft = new TFT_eSPI();
  tft->init(); // Initialise the TFT screen
  tft->setRotation(0); // Set the rotation
  tft->fillScreen(TFT_BLACK); // Clear the screen
  // tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN, 
  //        BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_GREEN); // Draw keypad background
  Serial.println("done.");
  // Setting touch screen calibration:
  tft->setTouch(calData);
  Serial.print("Setting up bit display ... ");
  show_mem();
  Serial.println("done.");
}

void DISP_PROGRAM::show_row(unsigned int row)
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
                  BIT_AREA_DX, BIT_AREA_YMAX-BIT_AREA_YMIN,
                  bgcolor);
    for (int col=0; col<PROGRAM_MEM_COLS; col++)
      tft->drawNumber(bitRead(mem[row],col), 
                      BIT_TEXT_XMIN+col*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
}

void DISP_PROGRAM::show_mem()
{

  tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN, 
         BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_BLACK); // Draw keypad background
  for (int row=0; row<CONTROL_MEM_ROWS; row++)
    show_row(row);
}

void DISP_PROGRAM::check()
{
  uint16_t tx = 0, ty = 0; // To store the touch coordinates
  // Get current touch state and coordinates
  bool pressed = tft->getTouch(&tx, &ty);

  if (pressed)
  {
    unsigned int col = int((tx - BIT_TEXT_XMIN) / BIT_AREA_DX);
    unsigned int row = int((ty - BIT_TEXT_YMIN) / BIT_AREA_DY);
    // toggleBit(col, row);
    Serial.printf("row = %d   col = %d\n", row, col);
  }

}

void DISP_PROGRAM::setrow(unsigned int row)
{
  unsigned int old_row = active_row;
  active_row = row;
  show_row(old_row);
  show_row(active_row);
}

// unsigned int DISP_CONTROL::get_ctrl(unsigned int col)
// {
//   return ((mem[col] & 0b0011111100) >> 2);
// }

// unsigned int DISP_CONTROL::get_cset(unsigned int col)
// {
//   return ((mem[col] & 0b0000000010) >> 1);
// }

// unsigned int DISP_CONTROL::get_creset(unsigned int col)
// {
//   return ((mem[col] & 0b0000000001));
// }

// unsigned int DISP_CONTROL::get_pcount(unsigned int col)
// {
//   return ((mem[col] & 0b1000000000) >> 9);
// }

// unsigned int DISP_CONTROL::get_pset(unsigned int col)
// {
//   return ((mem[col] & 0b0100000000) >> 8);
// }

// void DISP_CONTROL::toggleBit(unsigned int col, unsigned int row)
// {
//   // bitWrite(mem[col], row, !bitRead(mem[col], row));
//   // show_mem();
//   setcol(col);
// }


