#include "Arduino.h"
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library

#define CTRL_BIT_G1 0
#define CTRL_BIT_RB 1
#define CTRL_BIT_RA 2
#define CTRL_BIT_PM 3
#define CTRL_BIT_RC 4
#define CTRL_BIT_G2 5

#define MEM_COLS 10
#define MEM_ROWS 16

#define BIT_AREA_XMIN 0
#define BIT_AREA_XMAX 240
#define BIT_AREA_YMIN 0
#define BIT_AREA_YMAX 320
#define BIT_AREA_DX 24
#define BIT_AREA_DY 20
#define BIT_TEXT_XMIN 2
#define BIT_TEXT_YMIN 5

#define BIT_TEXT_SIZE 1   // Font size multiplier
#define BIT_FONT &FreeSansBold9pt7b    // Key label font 2

#define TFT_DARKYELLOW 0x9CC0


class DISP_CONTROL
{
	private:
		TFT_eSPI *tft;
		TFT_eSPI_Button *membits[MEM_ROWS][MEM_COLS];
		uint16_t mem[MEM_ROWS] = {0b0000000100, 0b1010000010, 0b1000000000,
															0b0000001010, 0b1101000010, 0b1001100000,
														  0b0001010000, 0b0011010000, 0b0001011010,
														  0b1100000010, 0b1000100000, 0b0000010000,
														  0b0010010000, 0b0000011010, 0b0100000010,
														  0b0000001001};
		// Touch screen calibration data:
		uint16_t calData[5] = {203, 3657, 350, 3491, 4};
		unsigned int bg[16] = {	TFT_PURPLE,
									TFT_BLUE, TFT_BLUE, TFT_BLUE,
									TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN, TFT_DARKGREEN,
							   	TFT_DARKYELLOW, TFT_DARKYELLOW, TFT_DARKYELLOW, TFT_DARKYELLOW, TFT_DARKYELLOW,
							   	TFT_RED, TFT_RED};
		unsigned int active_row = 0;
		void show_row(unsigned int row);
		void show_mem();
		unsigned int reverse6bit(unsigned int x);
	public:
		DISP_CONTROL();
		void check();
		void setrow(unsigned int row);
		unsigned int get_ctrl(unsigned int row);
		unsigned int get_cset(unsigned int row);
		unsigned int get_creset(unsigned int row);
		unsigned int get_pcount(unsigned int row);
		unsigned int get_pset(unsigned int row);
		void toggleBit(unsigned int row, unsigned int col);
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
