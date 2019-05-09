#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         10           // Configurable, see typical pin layout above
#define SS_PIN          A0          // Configurable, see typical pin layout above

#define NAME_RFID_BLOCK 8
#define DATA_RFID_BLOCK 12

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.


void rfidSetup()
{
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init();
  //mfrc522.PCD_SetAntennaGain(mfrc522.RxGain_33dB);  //RxGain_33dB RxGain_38dB RxGain_43dB RxGain_48dB
}

static boolean rfidDetected(byte uid[], byte playerName[], byte playerData[], int incByteLoc) {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent())
    return false;

  // make sure nuid has been read
  if ( ! mfrc522.PICC_ReadCardSerial())
    return false;
  
  for (int i = 0; i < mfrc522.uid.size; i++) {
    uid[i] = mfrc522.uid.uidByte[i];
  }
  dump_byte_array(uid, mfrc522.uid.size);
  
  boolean status;
  status = authRfid(); Serial.println("auth executed");
  if (status != true) {return status;}  
  status = readRfid(playerName, NAME_RFID_BLOCK, 16); Serial.println("read name executed");
  if (status != true) {return status;}
  status = readRfid(playerData, DATA_RFID_BLOCK, 16); Serial.println("read data executed");
  if (status != true) {return status;}
  if (incByteLoc != 16) {  //dummy default value for not incrementing tries/complete bytes
    status = incrementBlockRfid(incByteLoc); Serial.println("inc byte executed");
    if (status != true) {return status;}
  }  
  return true;
  //dump_byte_array(playerData, 16);
}

boolean authRfid() {
  MFRC522::StatusCode status;
  byte PSWBuff[] = {0xFF, 0xFF, 0xFF, 0xFF}; //32 bit PassWord default FFFFFFFF
  byte pACK[] = {0, 0}; //16 bit PassWord ACK returned by the NFCtag
  
  status = (MFRC522::StatusCode) mfrc522.PCD_NTAG216_AUTH(&PSWBuff[0], pACK);
  if (status != MFRC522::STATUS_OK) {return false;}
  return true;
}

boolean writeRfid(byte *dataBlock, byte blockAddr, byte bufferSize) {

  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, bufferSize);
  if (status != MFRC522::STATUS_OK) {return false;}
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr+1, &dataBlock[4], bufferSize);
  if (status != MFRC522::STATUS_OK) {return false;}
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr+2, &dataBlock[8], bufferSize);
  if (status != MFRC522::STATUS_OK) {return false;}
  status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr+3, &dataBlock[12], bufferSize);
  if (status != MFRC522::STATUS_OK) {return false;}
  return true;
}

boolean readRfid(byte *dataBlock, byte blockAddr, byte bufferSize) {

  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {return false;}
  
  memcpy(dataBlock, buffer, 16);
  return true;
}

boolean incrementBlockRfid(byte dataBlockByte) {
  boolean status;
  byte rfidData[16];
  status = readRfid(rfidData,DATA_RFID_BLOCK,16);
  if (status != true) {return status;}
  rfidData[dataBlockByte]++;
  status = writeRfid(rfidData,DATA_RFID_BLOCK,16);
  return status;
}

void closeRfid() {
  // Halt PICC
  mfrc522.PICC_HaltA();
  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}


void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

boolean timeOut() {
  return false;
}


