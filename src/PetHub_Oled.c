
#include "PetHub_Oled.h"


// I2C initialization function
void I2C_Init(void) {
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC4 = 0;
    LATCbits.LATC3 = 0;
    LATCbits.LATC4 = 0;
    TRISCbits.RC3 = 1;           
    TRISCbits.RC4 = 1; 
    OpenI2C1(MASTER, SLEW_OFF); // Initialize I2C as Master with slew rate control off
    SSP1ADD = 0x09; // Set I2C clock frequency to 100kHz
}

// I2C communication functions
void I2C_Start(void) {
    StartI2C1(); // Initiate start condition
}

void I2C_Stop(void) {
    StopI2C1(); // Initiate stop condition
}

void I2C_Write(unsigned char data) {
    WriteI2C1(data); // Write data to I2C bus
}

// OLED command function
void oledCommand(unsigned char command) {
    I2C_Start();
    I2C_Write(SH1106_I2C_ADDRESS << 1); // Send address with write bit
    I2C_Write(0x00); // Co = 0, D/C# = 0
    I2C_Write(command);
    I2C_Stop();
}

// OLED initialization function
void oledInit(void) {
    Delay10KTCYx(10); // Wait for the OLED to power up

    oledCommand(0xAE); // Display off
    oledCommand(0xD5); // Set display clock divide ratio/oscillator frequency
    oledCommand(0x80); // Default setting for display clock divide ratio/oscillator frequency
    oledCommand(0xA8); // Set multiplex ratio
    oledCommand(0x3F); // 1/64 duty
    oledCommand(0xD3); // Set display offset
    oledCommand(0x00); // No offset
    oledCommand(0x40); // Set start line address
    oledCommand(0x8D); // Enable charge pump
    oledCommand(0x14); // Enable charge pump
    oledCommand(0x20); // Set Memory Addressing Mode
    oledCommand(0x00); // Horizontal addressing mode
    oledCommand(0xA1); // Set segment re-map 0 to 127
    oledCommand(0xC8); // Set COM Output Scan Direction
    oledCommand(0xDA); // Set COM Pins hardware configuration
    oledCommand(0x12); // Alternative COM pin configuration, disable COM left/right remap
    oledCommand(0x81); // Set contrast control
    oledCommand(0xCF); // Set contrast value
    oledCommand(0xD9); // Set pre-charge period
    oledCommand(0xF1); // Set pre-charge period (default is 0x22)
    oledCommand(0xDB); // Set VCOMH deselect level
    oledCommand(0x40); // Set VCOMH deselect level
    oledCommand(0xA4); // Enable display outputs according to the GDDRAM contents
    oledCommand(0xA6); // Set normal display mode (not inverted)
    oledCommand(0x2E); // Deactivate scroll
    oledCommand(0xAF); // Display ON in normal mode
}

// Function to send data to OLED
void oledData(unsigned char data) {
    I2C_Start();
    I2C_Write(SH1106_I2C_ADDRESS << 1); // Send address with write bit
    I2C_Write(0x40); // Co = 0, D/C# = 1
    I2C_Write(data);
    I2C_Stop();
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
