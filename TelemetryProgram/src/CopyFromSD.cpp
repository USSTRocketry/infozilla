/**
 * @file CopyFromSD.cpp
 * @author USST Avionics
 * @brief This file provides test functions for the SD
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <SerialFlash.h>
#include <SD.h>
#include <SPI.h>

const int SDchipSelect = 4;    // Audio Shield has SD card CS on pin 10
const int FlashChipSelect = 6; // digital pin for flash chip CS pin
//const int FlashChipSelect = 21; // Arduino 101 built-in SPI Flash


/**
 * @brief test function that copies all files from the SD card to the SPI Flash memory chip.
 *
 * This function initializes serial communication and sets up the SD and SPI Flash.
 * It loops through all files in the root directory of the SD card. For each file:
 *  - If the file already exists on the flash chip, it compares the file sizes.
 *  - If the sizes are different (or comparison code is enabled and the content differs),
 *    the existing file on the flash chip is deleted and replaced.
 *  - If the file does not exist or was removed, the file is created and copied over.
 *
 * Dependencies:
 *  - Requires `SD` and `SerialFlash` libraries.
 *  - Assumes `SDchipSelect` and `FlashChipSelect` are defined globally.
 *
 * Notes:
 *  - Uncomment SPI pin settings if using Teensy Audio shield or other SPI devices.
 *  - Serial must be open (waits up to 10 seconds) before file operations start.
 *  - Optional file content comparison code is included but commented out.
 *
 * Output:
 *  - Logs file transfer status via Serial Monitor.
 */
void copySDToSPIFlash() {
  // Teensy Audio Shield users may need to uncomment these:
  // SPI.setSCK(14);
  // SPI.setMOSI(7);

  // If using other SPI devices:
  // pinMode(4, INPUT_PULLUP);
  // pinMode(10, INPUT_PULLUP);

  Serial.begin(9600);

  // Wait for Serial Monitor to open (max 10 seconds)
  unsigned long startMillis = millis();
  while (!Serial && (millis() - startMillis < 10000));
  delay(100);

  Serial.println(F("Copy all files from SD Card to SPI Flash"));

  if (!SD.begin(SDchipSelect)) {
    Serial.println(F("Error: Unable to access SD card"));
    return;
  }

  if (!SerialFlash.begin(FlashChipSelect)) {
    Serial.println(F("Error: Unable to access SPI Flash chip"));
    return;
  }

  File rootdir = SD.open("/");
  while (1) {
    Serial.println();
    File f = rootdir.openNextFile();
    if (!f) break;

    const char *filename = f.name();
    unsigned long length = f.size();
    Serial.print(filename);
    Serial.print(F("    "));
    Serial.println(length);

    if (SerialFlash.exists(filename)) {
      Serial.println(F("  already exists on the Flash chip"));
      SerialFlashFile ff = SerialFlash.open(filename);
      if (ff && ff.size() == f.size()) {
        Serial.println(F("  size is the same, comparing data..."));
        // Optional content comparison:
        // if (compareFiles(f, ff)) {
        //   Serial.println(F("  files are identical :)"));
        //   f.close();
        //   ff.close();
        //   continue;
        // } else {
        //   Serial.println(F("  files are different"));
        // }
      } else {
        Serial.print(F("  size is different, "));
        Serial.print(ff.size());
        Serial.println(F(" bytes"));
      }

      Serial.println(F("  delete file from Flash chip"));
      SerialFlash.remove(filename);
    }

    // Create and write file on flash
    if (SerialFlash.create(filename, length)) {
      SerialFlashFile ff = SerialFlash.open(filename);
      if (ff) {
        Serial.print(F("  copying"));
        unsigned long count = 0;
        unsigned char dotcount = 9;
        while (count < length) {
          char buf[256];
          unsigned int n = f.read(buf, 256);
          ff.write(buf, n);
          count += n;
          Serial.print(".");
          if (++dotcount > 100) {
            Serial.println();
            dotcount = 0;
          }
        }
        ff.close();
        if (dotcount > 0) Serial.println();
      } else {
        Serial.println(F("  error opening freshly created file!"));
      }
    } else {
      Serial.println(F("  unable to create file"));
    }

    f.close();
  }

  rootdir.close();
  delay(10);
  Serial.println(F("Finished All Files"));
}

/**
 * @brief This function compares if a file and serial flash file have the same data
 * 
 * @param file  File 
 * @param ffile SerialFlashFile
 * @return true 
 * @return false 
 */
bool compareFiles(File &file, SerialFlashFile &ffile) {
  file.seek(0);
  ffile.seek(0);
  unsigned long count = file.size();
  while (count > 0) {
    char buf1[128], buf2[128];
    unsigned long n = count;
    if (n > 128) n = 128;
    file.read(buf1, n);
    ffile.read(buf2, n);
    if (memcmp(buf1, buf2, n) != 0) return false; // differ
    count = count - n;
  }
  return true;  // all data identical
}

void error(const char *message) {
  while (1) {
    Serial.println(message);
    delay(2500);
  }
}
