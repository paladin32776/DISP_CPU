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
                BIT_AREA_XMAX-BIT_AREA_XMIN, BIT_AREA_DY,
                bgcolor);
  for (int col=0; col<PROGRAM_MEM_COLS; col++)
    tft->drawNumber(bitRead(mem[row],PROGRAM_MEM_COLS-col-1),
                    BIT_TEXT_XMIN+col*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
  String str;
  if (row%2==0) // Show opname and opcode in parenthesis, like so: ADD(4)
    str = opnames[find_opcode_index(mem[row])] + "(" + String(mem[row]) + ")";
  else
    str = String(mem[row]);
  tft->drawString(str , BIT_TEXT_XMIN+PROGRAM_MEM_COLS*BIT_AREA_DX, BIT_TEXT_YMIN+row*BIT_AREA_DY);
}

void DISP_PROGRAM::show_mem()
{

  tft->fillRect(BIT_AREA_XMIN, BIT_AREA_YMIN, BIT_AREA_XMAX-BIT_AREA_XMIN,
         BIT_AREA_YMAX-BIT_AREA_YMIN, TFT_BLACK); // Draw keypad background
  for (int row=0; row<PROGRAM_MEM_ROWS; row++)
    show_row(row);
}

int DISP_PROGRAM::next_opcode_index(unsigned char opcode)
{
  int opcode_index = find_opcode_index(opcode);
  if (opcode_index >= 0)
    return (opcode_index+1)%5; 
  else
    return -1;
}

int DISP_PROGRAM::find_opcode_index(unsigned char opcode)
{
  for (int n=0; n<5; n++)
    if (opcode==opcodes[n])
      return n;
  return -1;
}

void DISP_PROGRAM::check()
{
  uint16_t tx = 0, ty = 0; // To store the touch coordinates
  // Get current touch state and coordinates
  bool pressed = tft->getTouch(&tx, &ty);

  delay(100);

  if (pressed)
  {
    unsigned int col = int((tx - BIT_TEXT_XMIN) / BIT_AREA_DX);
    unsigned int row = int((ty - BIT_TEXT_YMIN) / BIT_AREA_DY);
    // toggleBit(col, row);
    Serial.printf("row = %d   col = %d\n", row, col);
    if (row%2==0)
    {
      unsigned int opcode = opcodes[next_opcode_index(mem[row])];
      mem[row] = opcode;
      show_row(row);
    }
    else
    {
      unsigned int data = mem[row];
      data = (data+1)%16;
      mem[row] = data;
      show_row(row);
    }
  }

}

void DISP_PROGRAM::setrow(unsigned int row)
{
  unsigned int old_row = active_row;
  active_row = row;
  show_row(old_row);
  show_row(active_row);
}

unsigned int DISP_PROGRAM::get_data(unsigned int row)
{
  return mem[row];
}