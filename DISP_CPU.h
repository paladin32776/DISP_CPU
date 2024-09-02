#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

#define CTRL_BIT_G1 0
#define CTRL_BIT_RB 1
#define CTRL_BIT_RA 2
#define CTRL_BIT_PM 3
#define CTRL_BIT_RC 4
#define CTRL_BIT_G2 5

#define CONTROL_MEM_COLS 16
#define CONTROL_MEM_ROWS 10

#define PROGRAM_MEM_COLS 4
#define PROGRAM_MEM_ROWS 16

#define BIT_AREA_XMIN 0
#define BIT_AREA_XMAX 240
#define BIT_AREA_YMIN 80
#define BIT_AREA_YMAX 320
#define BIT_AREA_DX 15
#define BIT_AREA_DY 24
#define BIT_TEXT_XMIN 2
#define BIT_TEXT_YMIN 85

#define BIT_TEXT_SIZE 1   // Font size multiplier
#define BIT_FONT &FreeSansBold9pt7b    // Key label font 2

#define TFT_DARKYELLOW 0x9CC0

// LED connections on ALU board:
// =============================
// led1_0..led1_3 : bus 0..3
// led1_4..led1_7 : ra 0..3
// led1_8..led1_11 : rb 3..0
// led1_12 : ePM
// led1_13 : eRA
// led1_14 : eRB
// led1_15 : eG1

// led2_0..led2_3 : rc 3..0
// led2_4 : eRC
// led2_5 : neg
// led2_6 : overflow
// led2_7 : eG2


class DISP_CONTROL
{
	private:
		TFT_eSPI *tft;
		TFT_eSPI_Button *membits[CONTROL_MEM_COLS][CONTROL_MEM_ROWS];
		unsigned int mem[CONTROL_MEM_COLS] = {0b1};
		// Touch screen calibration data:
		uint16_t calData[5] = {203, 3657, 350, 3491, 4};
		unsigned int bg[16] = {	TFT_RED, TFT_RED, 
							   	TFT_DARKYELLOW, TFT_DARKYELLOW, TFT_DARKYELLOW, 
							   	TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, 
							   	TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, 
							   	TFT_PURPLE};
		unsigned int active_col = 0;
		void show_col(unsigned int col);
		void show_mem();
	public:
		DISP_CONTROL();
		void check();
		void setcol(unsigned int col);
		unsigned int get_ctrl(unsigned int col);
		unsigned int get_cset(unsigned int col);
		unsigned int get_creset(unsigned int col);
		unsigned int get_pcount(unsigned int col);
		unsigned int get_pset(unsigned int col);
		void toggleBit(unsigned int col, unsigned int row);
		// void update(unsigned char bus, unsigned char ctrl, unsigned char counter,
		// 	unsigned char ccount, unsigned char cset, unsigned char creset,
		// 	unsigned char pcount, unsigned char pset);
};

class DISP_PROGRAM
{
	private:
		TFT_eSPI *tft;
		TFT_eSPI_Button *membits[PROGRAM_MEM_COLS][PROGRAM_MEM_ROWS];
		// Touch screen calibration data:
		uint16_t calData[5] = {203, 3657, 350, 3491, 4};
		unsigned int bg[16] = {	TFT_RED, TFT_RED, 
							   	TFT_DARKYELLOW, TFT_DARKYELLOW, TFT_DARKYELLOW, 
							   	TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, 
							   	TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, TFT_BLUE, 
							   	TFT_PURPLE};

		String smem[PROGRAM_MEM_ROWS] = {"LDA","4","ADD","7","SUB","2","JMP","10","ADD","3","SUB","5","HLT"};
		unsigned char mem[PROGRAM_MEM_ROWS] = {1,4,4,7,9,2,14,10,4,3,9,5,0};
		unsigned int active_row = 0;
		void show_row(unsigned int row);
		void show_mem();
	public:
		DISP_PROGRAM();
		void check();
		void setrow(unsigned int row);
		unsigned int get_data(unsigned int row);
};





