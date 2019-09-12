#include <EEPROM.h>

TaskHandle_t Task1;

//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <mini-gmp.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
#define EEPROM_SIZE 313 //this is the size of the id and password +1
BluetoothSerial SerialBT;
//vars for current sensor


/////////////////////////////////////////////////
float vpc = .0048828125;  // 5 V/ 4096 points
float vpc12 = 0.001220703125;// 5/4096
float sens = .025;
int sample, out, Vref;
float Voltage, Current;
long lastTime = 0;
long sampleSum = 0;
int sampleCount;
float mean,rmsSample;

//////////////////////////////////

String encryptedPassword = "35052111338673026690212423937053328511880760811579981620642802346685810623109850235943049080973386241113784040794704193978215378499765413083646438784740952306932534945195080183861574225226218879827232453912820596886440377536082465681750074417459151485407445862511023472235560823053497791518928820272257787786";
String DecryptedPW = "1976620216402300889624482718775150";
String PasswordStatus;
bool msgVar;
long TIME = 0;
int val = 0;
int ind = 100;
#define LED 2
String strr = "m1 on";
String message;
String str ;
String strbt;
char strbtt;
String readString;
int ID;
String SavedPW;
int shour, month, day, year, minute, second;
String mm[] = {"Jan", "Feb","Mar","Apr","May","June","July","Aug","Sep","Oct","Nov","Dec"};
bool MachineStatus; //This determines if the machine is allowed to turn on or off 
bool MachineState; // This determines if the machaine is actually on or off
void setup() {
  Serial.begin(115200);
  EEPROM.begin(EEPROM_SIZE);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  pinMode(32, INPUT);
  pinMode(35, INPUT);
  pinMode(5, OUTPUT);
  pinMode(34, INPUT);
//  pinMode(15, INPUT);
//  pinMode(2, INPUT);
  Serial.println("The device started, now you can pair it with bluetooth!");
  pinMode(LED, OUTPUT);

  //reads ID on startup
  ID = EEPROMreadString(0,4).toInt();
  SavedPW = EEPROMreadString(5,308);
    const char* m = SavedPW.c_str();
    SavedPW=((char*)DECRYPT(m));
    if(SavedPW == DecryptedPW)
      {
        PasswordStatus = "Verified";
      }
      else
      {
        PasswordStatus = "Failed";
      }

   
}

void loop() {
  //read message from endergia if it says a message then send a message to both devices endergia should blink from it
  
  // reading message from bluetooth if it says hi blink twice
  if (SerialBT.available()>0) 
  {
      strbt=SerialBT.readString();
      if(strbt.charAt(0)=='D' )
      { 
        //this reads the ID sent sets the device ID to it and reads ID back
       message = strbt.substring(1,5);
       ID = message.toInt();
       EEPROMwriteString(0,message);
       SerialBT.print("DID set as: " + message); 
//       Serial.print(message);
      }
      else if (strbt.charAt(0) == 'T')
      { 
        // A date to compare current date with will be processed and sent to energia from here
        Serial.print(strbt);

      }
      else if(strbt.charAt(0) == 'P')
      {
        message = strbt.substring(1);
        //this sends the encrypted code to energia so that it may be retrieved after this device is turned off and on
        const char* m = message.c_str();
        SavedPW=((char*)DECRYPT(m));// calls the function for decryption
        if(SavedPW == DecryptedPW)
        {
         SerialBT.print("PVerified");
         EEPROMwriteString(5,message);
         PasswordStatus = "Verified";
        } 
      else
        {
        SerialBT.print("PFailed");
        PasswordStatus = "Unverified";
        }
     }
      else if(strbt.charAt(0) == 'L')
        {
        // request all information from energia
        // send all information that is said and done
        // safety hall sensor and time
        // current sensor and time
        // power supply hall sensor and time 
        // current time coming from rtc
        // password validation
        // machine status
        Serial.print('L');
      }
  }
// logged data sending
if(Serial.available()>0)
{
  str = Serial.readString();
  if(str.charAt(0)== 'L')
  {
    SavedPW = EEPROMreadString(5,308);
    const char* m = SavedPW.c_str();
    SavedPW=((char*)DECRYPT(m));
    if(SavedPW == DecryptedPW)
      {
        PasswordStatus = "Verified";
      }
      else
      {
        PasswordStatus = "Failed";
      }
SerialBT.print(str + " ID is: " + String(ID) + " PW status:" + PasswordStatus +  " Machine Status:" + MachineStatus );  
  }
}
//CurrentSensor


// this is where the machine turns on
MachineStatus = digitalRead(34);
if(MachineStatus == true && PasswordStatus=="Verified")
{
  if(digitalRead(32)== 1 && digitalRead(35)==0)// this is where the green/on button is connected
  { 
    MachineState = true;
    digitalWrite(5,HIGH);
  }
  else if(digitalRead(32) ==1 && digitalRead(35)==1) // if both buttons get pressed the machine should turn off
  {
    MachineState = false;
    digitalWrite(5, LOW);
  }
  else if(digitalRead(32)== 0 && digitalRead(35)==1)// this is where the red/off button is connected
  { 
    MachineState = false;
    digitalWrite(5,LOW);
  }
//  if(MachineState == false && TrueMachineState == 1)
//  {
//    fraudTracker++;
//  }
}
else
{
  digitalWrite(5, LOW);
  MachineState=false;
}



//
//if(millis()> lastTime+8)
//{
//  sampleSum += sq(analogRead(15)/4 -512); //squares and adds the sample
////  sampleSum += sq(analogRead(15)/4 - analogRead(2)/4); //squares and adds the sample
//Serial.println(analogRead(25));
//  sampleCount++;
//  lastTime = millis();
//}
//
//if(sampleCount>=125)
//{
//  //Averaging stuff.
// mean = sampleSum/sampleCount;
// rmsSample = sqrt(mean); // take the square root of the mean
//// Serial.println("The final RRMSValue is: " + String(rmsSample));
// Voltage = rmsSample*vpc;
//// Serial.println("The final Voltage is: " + String(Voltage));
// Current = Voltage/sens;
////Serial.println("The final RMS Amperage is: " + String(Current));
// sampleSum = 0;
// sampleCount = 0;
//}



} // end of void loop[



//writes a string to the storage
void EEPROMwriteString(int startingStorageSpace, String storedString)
{
  int strLength = storedString.length();
  int pos = 0;
  int storagePos;
  while(pos<strLength)
  {
    storagePos = pos+startingStorageSpace;
    EEPROM.write(storagePos, storedString.charAt(pos));
    pos++;
  }
  EEPROM.commit();
}
//reads a string from the storage
String EEPROMreadString(int startReadPos,int Size)
{
  byte byteArray[Size];
  byteArray[Size] = 0;
  int pos = 0;
  int storagePos;
  while(pos<Size)
  {
    storagePos = pos+startReadPos;
    byteArray[pos]=EEPROM.read(storagePos);
    pos++;
  }

  String myString = String((char *)byteArray);
  return myString;
}


















/* psuedo code
 * every ten seconds energia will send esp a message
 * the esp will then blink a light once and send a message to both the app and endergia\
 * any time the esp recieves hi from bluetooth it will blink twice
 */




 ///////////////////////////////////////////////////////////////////////
 //RSA decryption



 /* Tranform string from char array to decimal string
 * */



 int decrypt_message(mpz_t message_enc, mpz_t d, mpz_t n, mpz_t message_dec){
  //c^d * mod n = m
  //m is message_dec

  mpz_powm(message_dec, message_enc, d, n);//Set message_dec to message_enc ^ d * mod n
  
  return 0;
}

//we just need to verify that this decrypted code can be compared with
char* DECRYPT(const char* encryptedCode)
{
  mpz_t p; //p
  mpz_t q; //q
  //p != q
  mpz_t n; //n = p * q
  mpz_t totient_n;
  mpz_init(totient_n);
  mpz_t e; //e
  mpz_init_set_str(e, "65537",10);
  mpz_t d; //d = gcd (e,totient_n)
  mpz_init(d);
  mpz_init(n);
  mpz_t message_enc;
  mpz_init(message_enc);
  mpz_t message;
  //message is "attack at dawn"
  mpz_init(message);
  
  mpz_init_set_str(message_enc, encryptedCode,10);

  mpz_init_set_str(p, "12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803541",10);
    mpz_init_set_str(q, "12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415233",10);
    mpz_init_set_str(n, "145906768007583323230186939349070635292401872375357164399581871019873438799005358938369571402670149802121818086292467422828157022922076746906543401224889672472407926969987100581290103199317858753663710862357656510507883714297115637342788911463535102712032765166518411726859837988672111837205085526346618740053",10);
    mpz_init_set_str(totient_n, "145906768007583323230186939349070635292401872375357164399581871019873438799005358938369571402670149802121818086292467422828157022922076746906543401224889648313811232279966317301397777852365301547848273478871297222058587457152891606459269718119268971163555070802643999529549644116811947516513938184296683521280",10);
    mpz_init_set_str(d, "89489425009274444368228545921773093919669586065884257445497854456487674839629818390934941973262879616797970608917283679875499331574161113854088813275488110588247193077582527278437906504015680623423550067240042466665654232383502922215493623289472138866445818789127946123407807725702626644091036502372545139713",10);
  //Hardcode to test if it works
  //mpz_init_set_str(p, "12131072439211271897323671531612440428472427633701410925634549312301964373042085619324197365322416866541017057361365214171711713797974299334871062829803540",10);
  //mpz_init_set_str(q, "12027524255478748885956220793734512128733387803682075433653899983955179850988797899869146900809131611153346817050832096022160146366346391812470987105415232",10);

  //start point
//  Serial.println("\n\n");
  //mpz_add_ui(q, p, op_100k);//set q = p + 100k

  // q = p + 100_000
  int comp = mpz_cmp(p, q);
  if(comp != 0){


    //string_to_decimal(str ,message);
//    Serial.println("\nMessage_enc:\n");
    //mpz_out_str(stdout ,10 ,message_enc);//output message to stdout return 0 if error
//    Serial.println("\n\n");
//    Serial.println("\n\n\n\nPRINTIN GNEW SRRINGS \n\n");
        int size = mpz_sizeinbase(message_enc,10);
        size = size + 2;
        char t[size];
        char * a = mpz_get_str(t,10, message_enc);
//        Serial.println(t);
//        Serial.println("\n\n\n\nDONE PRINTIN GNEW SRRINGS \n\n");
    decrypt_message(message_enc, d, n, message);
//    Serial.println("\nMessage:\n");
//    Serial.println("\n\n");
//    Serial.println("\n\n\n\nPRINTIN GNEW SRRINGS2 \n\n");
        int size1 = mpz_sizeinbase(message,10);
        size1 = size1 + 2;
        char x[size1];
        char * b = mpz_get_str(x,10, message);
//        Serial.print("P");
//        Serial.println(x);////////////////////////////////////////////////////////////////
        return b;
//        Serial.println("\n\n\n\nDONE PRINTIN GNEW SRRINGS2 \n\n");
    //decimal_to_string(message, str);

  }

  mpz_clear(message_enc);
  mpz_clear(p);
  mpz_clear(q);
  mpz_clear(e);
  mpz_clear(d);
  mpz_clear(n);
  mpz_clear(totient_n);
  mpz_clear(message);

  if(comp == 0 ){
    Serial.println("ERROR: p == q");

  }
}
//"35052111338673026690212423937053328511880760811579981620642802346685810623109850235943049080973386241113784040794704193978215378499765413083646438784740952306932534945195080183861574225226218879827232453912820596886440377536082465681750074417459151485407445862511023472235560823053497791518928820272257787786"


