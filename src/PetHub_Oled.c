
#include "PetHub_Oled.h"


// I2C initialization function
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

    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC5 = 0;
    LATCbits.LATC3 = 0;
    LATCbits.LATC5 = 0;
    TRISCbits.RC3 = 0;           
    TRISCbits.RC5 = 0; 

    OpenSPI(SPI_FOSC_4, MODE_00, SMPEND);
}

void SPIWrite(unsigned char data) 
{
    LATDbits.LATD6 = 0;           // Select the OLED
    WriteSPI(data);           // Send data
    while (!SSP1STATbits.BF);    // Wait until the data is sent
    LATDbits.LATD6 = 1; 
}

// OLED command function
void oledCommand(unsigned char command) 
{
    LATDbits.LATD5 = 0; // Command mode
    SPIWrite(command);
}


// Function to send data to OLED
void oledData(unsigned char data) 
{
    LATDbits.LATD5 = 1; // Data mode
    SPIWrite(data);
}


// OLED initialization function
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
void oleddisplayOff(void) {
    oledCommand(0xAE); // Display Off
}
void oleddisplayOn(void) {
    oledCommand(0xAF); // Display Off
}

// Function to set cursor position on OLED
void oledSetCursor(unsigned char column, unsigned char page) {
    oledCommand(0xB0 + page); // Set page address
    oledCommand((((column+2)&0xF0)>>4)|0x10);
    oledCommand((column+2)&0x0F);
}


// Function to print a single character on OLED
void oledPrintChar(char column, char page, char c) {
    char i;
	oledSetCursor(column*6,page );
    if (c < 32 || c > 127) c = ' '; // Replace non-printable characters with space
    for (i = 0; i < 6; i++) {
        oledData(font6x8[c - 32][i]);
    }
}


void displayClear(){
	char i,n,j;
    for(j=0;j<PAGE_SIZE;j++){
        for(n=0;n<22;n++){
		oledPrintChar(n, j, 0);     
        }
    }
}

void lineClear(char i){
	char n;
        for(n=0;n<22;n++)
		{
			oledPrintChar(n, i, 0);     
        }
}

void oledPrintSpecialChar(char column, char page, char c) {
    char i;
	oledSetCursor(column*6,page );
	for (i = 0; i < 6; i++) {
        oledData(special_chars[c][i]);
    }
}

// Function to print a string on OLED
void oledPrintString(char x, char y,char *msg){
	while(*msg){  
        oledPrintChar(x,y,*msg++);
        x++;
    }
 
}

void oledPrintLogo() {
    unsigned char c,i;
	for (c = 0; c < 8; c++) 
	{
		oledSetCursor(0,c);
		for (i = 0; i < 128; i++) 
		{
	        oledData(logo[c][i]);
	    }
	}
}

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

