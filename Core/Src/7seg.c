/*
 * 7seg.c
 *  
 *      Author: Mitesh Parikh
 */
#include "7seg.h"

// Bit masks for displaying each digit
const uint8_t num[10] 	= {0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

// Mask for selecting each digit
const uint8_t seg[4]	= {0xF1,0xF2,0xF4,0xF8};

uint16_t seg_disp;
uint16_t digit[4];

// Initialization
void initSeg()
{
	// Starting out at zero before first poll of customer queue
	multiplexSegment(0); 
}

// Displays each segment then loops to the next
void displaySegment(void)
{	
	static uint8_t seg=0;

	LatchCLK(LOW);
	for(int8_t i=15;i>-1;i--) // Shift in digit then segment mask
	{	ShiftCLK(LOW);
		if(digit[seg] & (1<<i)){
			dataS(HIGH);
		}else{
			dataS(LOW);
		}
		ShiftCLK(HIGH); // Moves bit over by one
	}
	LatchCLK(HIGH); // Update parallel output

	if(++seg>3) seg=0;
}

// Breaks down the provided number into its 4 segments
void multiplexSegment(uint16_t num4digit)
{
    __disable_irq();      // lock out timer interrupt
    for (uint8_t i=0; i<4; i++)
    {
        uint16_t seg_disp;
        switch(i)
        {
            case 0: seg_disp = num4digit / 1000; break;
            case 1: seg_disp = (num4digit % 1000) / 100; break;
            case 2: seg_disp = (num4digit % 100) / 10; break;
            case 3: seg_disp = num4digit % 10; break;
        }
        digit[i] = (num[seg_disp] << 8) | seg[i];
    }
    __enable_irq();       // release timer interrupt
}
