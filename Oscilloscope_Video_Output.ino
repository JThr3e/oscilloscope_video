/*
 *  Simple program to output video on an old oscilloscope
 *  using a couple of mcp4725 DACs
 */
 
#include "Wire.h"
// Pixels of resolution in horizontal/vertical
#define H_RES 25
#define V_RES 20

// Voltage range of horizontal/vertical (max of 4095)
#define H_MAX 3000
#define V_MAX 2400

#define MCP4725_CMD_WRITEDAC (0x40)    ///< Writes data to the DAC

//  LOOKUP TABLES
uint16_t htable[H_RES];
uint16_t vtable[V_RES];

int old_address = -1;

inline void setValue(uint16_t value){
  uint8_t packet[3];
  packet[0] = MCP4725_CMD_WRITEDAC;
  packet[1] = value >> 4;        // Upper data bits (D11.D10.D9.D8.D7.D6.D5.D4)
  packet[2] = (value & 0xf) << 4; // Lower data bits (D3.D2.D1.D0.x.x.x.x)
  Wire.write(packet,3);
}

void setup()
{
  //  fill luts
  for (int i = 0; i < H_RES; i++)
  {
    htable[i] = (uint16_t)(i * (H_MAX / H_RES));
  }
  for (int i = 0; i < V_RES; i++)
  {
    vtable[i] = (uint16_t)(i * (V_MAX / V_RES));
  }
  
  Wire.begin();
  Wire.setClock(860000); //this is the highest the mcp and 32u4 could go without dying
}


void loop()
{
   for(int i = 0; i < V_RES; i++){
       Wire.beginTransmission(0x60);
       setValue(vtable[i]);
       Wire.endTransmission();
       for(int j = 0; j < H_RES; j++){
           Wire.beginTransmission(0x61);
           setValue(htable[j]);
           Wire.endTransmission();
       }
   }
}
