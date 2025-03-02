//****************************** */
// Develop SD Handler on this project
// When it works, push to HAL submodule
// Include wrappers for options/customization when initializing SD (same as what is in HAL library already)
//****************************** */
#include <SDHandler.h>
// For examples, go to SDHandler.h > SD.h > from SD.h ../examples/Files/Files.ino



File myFile;

// change this to match your SD shield or module;
// Arduino Ethernet shield: pin 4
// Adafruit SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
// Wiz820+SD board: pin 4
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
const int chipSelect = BUILTIN_SDCARD;
const int flashModule;
int savecount = 0;

void InitSD()
{
 //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
 //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
 //SPI.setSCK(14);  // Audio shield has SCK on pin 14
  
  Serial.println("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
// Open serial communications and wait for port to open:
  // Serial.begin(9600);
  //  while (!Serial) {
  //   ; // wait for serial port to connect.
  // }
  Serial.println("SD card initialized.");


}

void createExampleTxt() {
  

  if (SD.exists("FlightData.fdat")) {
    Serial.println("FlightData.fdat exists.");
  }
  else {
    Serial.println("FlightData.fdat doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating FlightData.fdat...");
  myFile = SD.open("FlightData.fdat", FILE_WRITE);
  myFile.close();

}

void deleteExampleTxt(){
  // Check to see if the file exists: 
  if (SD.exists("FlightData.fdat")) {
    Serial.println("FlightData.fdat exists.");
  }
  else {
    Serial.println("FlightData.fdat doesn't exist.");  
  }

  // delete the file:
  Serial.println("Removing FlightData.fdat...");
  SD.remove("FlightData.fdat");

  if (SD.exists("FlightData.fdat")){ 
    Serial.println("FlightData.fdat exists.");
  }
  else {
    Serial.println("FlightData.fdat doesn't exist.");  
  }
  
}

void WriteTest(){
  //SD.remove("FlightData.fdat");
  File f = SD.open("FlightData.fdat", FILE_WRITE);
  std::string buf = "Jello World";
  for (int i = 0; i < 10; i++)
  {
    /* code */
    //f.write(buf,12);
    f.println(buf.c_str());
  }
  f.close();

  f = SD.open("FlightData.fdat",FILE_READ);
  char b[12];

  for (int i = 0; i < 30; i++)
  {
    /* code */
    f.readBytes(b,12);
    Serial.print(b);
  }
  f.close();
}


void StoreStringLineToCSV(std::string stringline){
  File f; 
  if (!SD.exists("FlightData.csv")) {
    f = SD.open("FlightData.csv", FILE_WRITE);
    f.println("BarVal,Thermoval,accel_x,accell_y,accell_z,Gyro_x,Gyro_y,Gyro_z,timestamp");
  }else{
    f = SD.open("FlightData.csv", FILE_WRITE);
  }
  f.println(stringline.c_str());
  f.close();

}

// void loop()
// {
//   // Write data to SD continuously
// }



