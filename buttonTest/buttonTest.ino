#include<Wire.h>

#define MCP23017_IO_DIR_A 0x00
#define MCP23017_IO_DIR_B 0x01
#define MCP23017_GP_PU_A 0x0C
#define MCP23017_GP_PU_B 0x0D
#define MCP23017_GP_IO_A 0x12
#define MCP23017_GP_IO_B 0x13
#define MCP23017_OLAT_A 0x14
#define MCP23017_OLAT_B 0x15
#define MCP23017_BANK_A MCP23017_IO_DIR_A
#define MCP23017_BANK_B MCP23017_IO_DIR_B

#define MCP23017_BUTTON_ADDR 0x20

#define ELEMENTCNT(x) (sizeof(x)/sizeof(x[0]))

#define SET_BIT(p,n) (p |=  (1 << n))
#define CLR_BIT(p,n) (p &= ~(1 << n))
#define GET_BIT(p,n) ((p & (1 << n)) != 0)

void setup() {
  // Initialize the serial bus
  Serial.begin (115200);

  // Enable the pull-up resistors on the MCP23017 I2C device
  writeI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_PU_A, 0xFF);
  writeI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_PU_B, 0xFF);

}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.print("Addr: ");
    Serial.print(MCP23017_BUTTON_ADDR,HEX);
    int val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_IO_DIR_A);
    Serial.print(" MCP23017_IO_DIR_A:");
    Serial.print(val,HEX);
    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_IO_DIR_B);
    Serial.print(" MCP23017_IO_DIR_B:");
    Serial.print(val,HEX);
    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_IO_A);
    Serial.print(" MCP23017_GP_IO_A:");
    Serial.print(val,HEX);
    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_IO_B);
    Serial.print(" MCP23017_GP_IO_B:");
    Serial.print(val,HEX);
    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_PU_A);
    Serial.print(" MCP23017_GP_PU_A:");
    Serial.print(val,HEX);
    val = readI2c(MCP23017_BUTTON_ADDR, MCP23017_GP_PU_B);
    Serial.print(" MCP23017_GP_PU_B:");
    Serial.println(val,HEX);

delay(500);
}



static bool i2cWokeUp = false;
void writeI2c(uint8_t i2cAddr, uint8_t regOffset, uint8_t value)
{
  if (!i2cWokeUp)
  {
    Wire.begin(); // wake up I2C bus
    i2cWokeUp = true;
  }
  Wire.beginTransmission(i2cAddr);
  Wire.write(regOffset); // register
  Wire.write(value);
  Wire.endTransmission();
}

uint8_t readI2c(uint8_t i2cAddr, uint8_t regOffset)
{
    if (!i2cWokeUp)
  {
    Wire.begin(); // wake up I2C bus
    i2cWokeUp = true;
  }
  byte readValue;
  Wire.beginTransmission(i2cAddr);
  Wire.write(regOffset);
  Wire.endTransmission();
  Wire.requestFrom((int)i2cAddr, 1); // request one byte of data from MCP20317
  readValue = Wire.read(); // store the incoming byte into "inputs"
  return readValue;
}
