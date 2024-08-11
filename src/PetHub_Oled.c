/*=============================================================================
	File Name:	PetHub.c  
	Author:		Edwin Poulose
	Date:		28/05/2024
	Modified:	None
	� Fanshawe College, 2024

	Description: Driver for SSD1309 OLED Display
=============================================================================*/
#include "PetHub_Oled.h"


/*>>> SPIInit: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		SPI initialization function for OLED
Input: 		None
Returns:	None
 ============================================================================*/
void SPIInit(void) 
{
    ANSELDbits.ANSD6 = 0;
    ANSELDbits.ANSD5 = 0;
    ANSELDbits.ANSD4 = 0;

    LATDbits.LATD6 = 1; // Chip Select pin 
    LATDbits.LATD5 = 0; // Data/Command pin
    LATDbits.LATD4 = 1; // Reset pin 

    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD4 = 0;

    // SPI CLK and DATA Pins
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC5 = 0;
    LATCbits.LATC3 = 0;
    LATCbits.LATC5 = 0;
    TRISCbits.RC3 = 0;           
    TRISCbits.RC5 = 0; 

    OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);
}

/*>>> SPIWrite: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		write data to oled
Input: 		None
Returns:	None
 ============================================================================*/
void SPIWrite(unsigned char data) 
{
    LATDbits.LATD6 = 0;           // Select the OLED
    WriteSPI(data);           // Send data
    while (!SSP1STATbits.BF);    // Wait until the data is sent
    LATDbits.LATD6 = 1; 
}

/*>>> oledCommand: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		OLED command function
Input: 		None
Returns:	None
 ============================================================================*/
void oledCommand(unsigned char command) 
{
    LATDbits.LATD5 = 0; // Command mode
    SPIWrite(command);
}

/*>>> oledData: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to send data to OLED
Input: 		None
Returns:	None
 ============================================================================*/
void oledData(unsigned char data) 
{
    LATDbits.LATD5 = 1; // Data mode
    SPIWrite(data);
}


/*>>> oledInit: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		OLED initialization function
Input: 		None
Returns:	None
 ============================================================================*/
void oledInit(void) {

    // Reset the display
    LATDbits.LATD4 = 0;
    Delay10KTCYx(1);
    LATDbits.LATD4 = 1;

    // Initialization sequence for SSD1309
	// Settings are optional
    oledCommand(0xAE); // Display Off
    oledCommand(0xA8); // Set Multiplex Ratio
    oledCommand(0x3F); // 1/64 duty
    oledCommand(0xD3); // Set Display Offset
    oledCommand(0x00); // 0 offset
    oledCommand(0x40); // Set Display Start Line
    oledCommand(0xA1); // Set Segment Re-map
    oledCommand(0xC8); // Set COM Output Scan Direction
    oledCommand(0xDA); // Set COM Pins hardware configuration
    oledCommand(0x12); // Alternative COM pin configuration, disable COM left/right remap
    oledCommand(0x81); // Set Contrast Control
    oledCommand(0x7F); // 0x00-0xFF, Set HIGH brightness
    oledCommand(0xA4); // Disable Entire Display On
    oledCommand(0xA6); // Set Normal Display
    oledCommand(0xD5); // Set Oscillator Frequency
    oledCommand(0x80); // Default frequency
    oledCommand(0x8D); // Enable charge pump regulator
    oledCommand(0x14); // Enable
    oledCommand(0xAF); // Display On
}

/*>>> oleddisplayOff: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		OLED off wrapper funcion
Input: 		None
Returns:	None
 ============================================================================*/
void oleddisplayOff(void) {
    oledCommand(0xAE); // Display Off
}

/*>>> oleddisplayOn: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		OLED on wrapper function
Input: 		None
Returns:	None
 ============================================================================*/
void oleddisplayOn(void) {
    oledCommand(0xAF); // Display Off
}

/*>>> oledSetCursor: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to set cursor position on OLED
Input: 		None
Returns:	None
 ============================================================================*/
void oledSetCursor(unsigned char column, unsigned char page) {
    oledCommand(0xB0 + page); // Set page address
    oledCommand((((column+2)&0xF0)>>4)|0x10);
    oledCommand((column+2)&0x0F);
}

/*>>> oledPrintChar: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to print a single character on OLED
Input: 		None
Returns:	None
 ============================================================================*/
void oledPrintChar(char column, char page, char c) {
    char i;
	oledSetCursor(column*6,page );
    if (c < 32 || c > 127) c = ' '; // Replace non-printable characters with space
    for (i = 0; i < 6; i++) {
        oledData(font6x8[c - 32][i]);
    }
}

/*>>> displayClear: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to clear display, prints all screen null
Input: 		None
Returns:	None
 ============================================================================*/
void displayClear(){
	char i,n,j;
    for(j=0;j<PAGE_SIZE;j++){
        for(n=0;n<22;n++){
		oledPrintChar(n, j, 0);     
        }
    }
}

/*>>> lineClear: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to clear display, prints all screen null
Input: 		None
Returns:	None
 ============================================================================*/
void lineClear(char i){
	char n;
        for(n=0;n<22;n++)
		{
			oledPrintChar(n, i, 0);     
        }
}

/*>>> oledPrintSpecialChar: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to print special charecters and symbols
Input: 		None
Returns:	None
 ============================================================================*/
void oledPrintSpecialChar(char column, char page, char c) {
    char i;
	oledSetCursor(column*6,page );
	for (i = 0; i < 6; i++) {
        oledData(special_chars[c][i]);
    }
}

/*>>> oledPrintString: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to print a string on OLED
Input: 		None
Returns:	None
 ============================================================================*/
void oledPrintString(char x, char y,char *msg){
	while(*msg){  
        oledPrintChar(x,y,*msg++);
        x++;
    }
 
}

/*>>> oledPrintLogo: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to print splashscreen logo
Input: 		None
Returns:	None
 ============================================================================*/
void oledPrintLogo() {
    unsigned char page,line;
    // cycles through each page, 8 page make the full display
	for (page = 0; page < 8; page++) 
	{
		oledSetCursor(0,page);
        // cycles through each line, 128 lines make the whole page
		for (line = 0; line < 128; line++) 
		{
	        oledData(logo[page][line]);
	    }
	}
}


/*>>> drawProgressBar: ===========================================================
Author:		Edwin Poulose
Date:		04/07/2024
Modified:	None
Desc:		Function to print a progress bar
Input: 		None
Returns:	None
 ============================================================================*/
void drawProgressBar(char startPage, char startColumn, char filledSegments) 
{
    char j, i;
    char totalSegments = 4;
    char segmentWidth = 8; // Width of each segment

    oledSetCursor(startColumn, startPage);
    oledData(0x7F); // Border segment
    
    // Draw filled segments
    for (i = 0; i < filledSegments; i++) {
        oledSetCursor(startColumn + 1 + (i * segmentWidth), startPage);
        for (j = 0; j < segmentWidth; j++) {
            oledData(0x7F); // Fill the segment
        }
    }

    // Draw empty segments
    for (i = filledSegments; i < totalSegments; i++) {
        oledSetCursor(startColumn + 1 + (i * segmentWidth), startPage);
        for (j = 0; j < segmentWidth; j++) {
            oledData(0x41); // Empty segment
        }
    }
    oledData(0x7F); // Border segment
}

