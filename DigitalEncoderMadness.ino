// Copyright Taylor Oliver, 2019
// This will get you up and running with the KY040 Encoder.
// 
#define ENCODER_USE_INTERRUPTS
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Bounce2.h>
///////////////////////////////////////////////////////////////////
// Pin Assignments
///////////////////////////////////////////////////////////////////

// LED pins
int LEDs = 4;
int wLED = 23;
int yLED = 22; 
int gLED = 21;
int bLED = 20;

// Encoder 1 pins

const int enc1pinA = 5;
const int enc1pinB = 6;

// Encoder 2 pins

const int enc2pinA = 2;
const int enc2pinB = 3;

// Encoder 1 Button pin

const int enc1pinC = 7;

// Encoder 2 Button pin

const int enc2pinC = 4;

// Power On/Off pin
const byte powerPin = 34;

// Time Varialble
unsigned long currentTime = millis();

// Input/Output strings
String inputString = "";
String outputString = "";

///////////////////////////////////////////////////////////////////

// Encoder Declarations

int indexLimit = 3;
int bufferLimit = 5;
int encArray[8] = { 0,0,0,0,0,0,0,0 };
unsigned long timeBuff1 = 0;
unsigned long timeBuff2 = 0;
unsigned long timeBuff3 = 0;
unsigned long timeBuff4 = 0;

Encoder enc1(enc1pinA, enc1pinB);
long oldPosition1 = -999;
unsigned long enc1move = 1;

Encoder enc2(enc2pinA, enc2pinB);
long oldPosition2 = -999;
unsigned long enc2move = 1;

///////////////////////////////////////////////////////////////////

// Button Declarations
const int buttonOnepin = enc1pinC;
Bounce buttonOne = Bounce();

const int buttonTwopin = enc2pinC;
Bounce buttonTwo = Bounce();

///////////////////////////////////////////////////////////////////
// Button action declarations
///////////////////////////////////////////////////////////////////

int debounceInterval = 5;
unsigned long holdValue = 500;
unsigned long wasPressed1 = 0;
unsigned long wasReleased1 = 0;
unsigned long wasPressed2 = 0;
unsigned long wasReleased2 = 0;
unsigned long longPress1 = 0;
unsigned long longPress2 = 0;
boolean buttonHold1 = false;
boolean buttonHold2 = false;
boolean doubleHold = false;
boolean bholdPress1 = false;
boolean bholdPress2 = false;

///////////////////////////////////////////////////////////////////
// Menu declarations
///////////////////////////////////////////////////////////////////

// Total number of menus
int totalMenus = 5;

// Initial setting for menu 1
unsigned int menuSelection1 = 0;

// Initial setting for menu 2
unsigned int menuSelection2 = 0;

// Default setting for menu 3 ( acts similar to a 3 position momentary switch )
unsigned int bholdPress = 0;

// Menu 1 size
const int tnSize = 3;
// Menu 1 names
String tabNav[tnSize] = { "tab1" , "tab2", "tab3" };

// Menu 2 size
const int mnSize = 5;
// Menu 2 names
String modeNav[mnSize] = { "mode1" , "mode2" , "mode3" , "mode4" , "mode5" }; 

// Menu 3 size
const int smSize = 3;
// Menu 3 names
String submenuNav[smSize] = { "submenuUp" , "submenuDown" , "submenuSelect" ,};

//// Menu 4 size
//cmSize = 4;
//// Menu 4 names
//cursorMove[cmSize] = { "up" , "down" , "left" , "right" };

//// Menu 5 size
//lmSize = 2;
////Menu 5 names
//listMove[lmSize] = { "cursorup" , "cursordown" };

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// dynamicMenu function ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

// Max setting for menu 1
unsigned int menuMax1 = tnSize;

// Max setting for menu 2
unsigned int menuMax2 = mnSize;

// Max setting for menu 3
unsigned int bholdPressMax = smSize;

// Minimum menu settings are all 0
unsigned int bholdPressMin = 0;
unsigned int menuMin1 = 0;
unsigned int menuMin2 = 0;

struct menu { String header, footer, leftMenu; } tab, mode, sub, sel, old;

menu menuChange = old;

void dynamicMenu(unsigned int, unsigned int, unsigned int) {
  // Declare pointers to appropriate address spaces
  menu *xtab = &tab;
  menu *xmode = &mode;
  menu *xsub = &sub;
  menu *xsel = &sel;
  // Associate the appropriate menu selector with the respective pointer
  xtab->header = tabNav[menuSelection1];
  xmode->footer = modeNav[menuSelection2];  
  xsub->leftMenu = submenuNav[bholdPress];
  xsel->header = xtab->header;
  xsel->footer = xmode->footer;
  xsel->leftMenu = xsub->leftMenu;
  // Pointer to the pointers
  if ( sel.header != old.header ) {
    Serial.println(sel.header);
    old.header = sel.header;
  }
  if ( sel.footer != old.footer ) {
    Serial.println(sel.footer);
    old.footer = sel.footer;
  }
  if ( sel.leftMenu != old.leftMenu ) {
    Serial.println(sel.leftMenu);
    old.leftMenu = sel.leftMenu;
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// buttonHandler function /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

unsigned int buttonHandler( Bounce, Bounce ) {

  if ( buttonOne.fell() == true ) {
      currentTime  =  millis();
      wasPressed1  =  currentTime;
      buttonHold1  =  true;
  }
  if ( buttonTwo.fell() == true ) {
      currentTime  =  millis();
      wasPressed2  =  currentTime;
      buttonHold2  =  true;
  }
  if ( buttonOne.rose() == true ) {
      currentTime  =  millis();
      wasReleased1 =  currentTime;
      buttonHold1  =  false;
  }
  if ( buttonTwo.rose() == true) {
      currentTime  =  millis();
      wasReleased2 =  currentTime;
      buttonHold2  =  false;
  }
    // If the time that the buttons are held is greater than the hold value, save the value.
//  if ( wasReleased1 - wasPressed1 > holdValue ) {
//    longPress1 = wasReleased1 - wasPressed1;
//  } 
//  // Compare time button 1 was held to the hold value.
//  if ( wasReleased2 - wasPressed2 > holdValue ) {
//    longPress2 = wasReleased2 - wasPressed2;
//  }
  // If a button is being held and another button is pressed.
  if ( buttonHold1 == true && buttonTwo.fell() == true ) {
    bholdPress = 1;
  } else if ( buttonHold2 == true && buttonOne.fell() == true ) {
    bholdPress = 0;
  } else {
    bholdPress = 2;
  }
//  if ( bholdPress1 == true ) {
//    bholdPress = 2;
////    Serial.print( "bholdPress: " );
////    Serial.println( bholdPress );
//    bholdPress1 = false;
//  }
//  if ( bholdPress2 == true ) {
//    bholdPress = 1;
////    Serial.print( "bholdPress: " );
////    Serial.println( bholdPress );
//    bholdPress2 = false;
//  }
//  if ( longPress1 > holdValue ) {
//    bholdPress = 0;
//    longPress1 = 0;
//  }
  return bholdPress;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// menuOne function ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

unsigned int menuOne( Encoder ) {
  // Check for encoder 1 movement
  long newPosition1 = enc1.read();
  
  if ( newPosition1 != oldPosition1 ) {
    setStandby(false); 
    timeBuff1 = currentTime;
    if ( timeBuff1 - timeBuff2 > 500 ) {
      for ( int i = 0; i < 8; i++ ) {
        if ( encArray[i] != 0 ) {
         encArray[i] = 0;
        }
      }
      timeBuff2 = timeBuff1 - 80;
    }
    if ( timeBuff1 > timeBuff2 + 20) {
      if ( newPosition1 == oldPosition1 + 1 ) {
        if ( encArray[2] != 0 && encArray[3] != 0 ) {
          for ( int i = 2; i < 4; i++ ) {
                encArray[i] = 0;
          }
        }
        if ( encArray[0] <= indexLimit ) {
          encArray[0]++;
          encArray[1]++;
        }
        if ( encArray[0] == indexLimit ) {
          if ( encArray[1] < bufferLimit ) {
            encArray[1]++;
            if ( encArray[1] == bufferLimit ) {
              for ( int i = 0; i < 4; i++ ) {
                encArray[i] = 0;
              }
            }
          }
        }
      }
      if ( newPosition1 == oldPosition1 - 1 ) {
        if ( encArray[0] != 0 && encArray[1] != 0 ) {
          for ( int i = 0; i < 2; i++ ) {
                encArray[i] = 0;
          }
        }
        if ( encArray[2] <= indexLimit ) {
          encArray[2]++;
          encArray[3]++;
        }
        if ( encArray[2] == indexLimit ) {
          if ( encArray[3] < bufferLimit ) {
            encArray[3]++;
            if ( encArray[3] == bufferLimit ) {
              for ( int i = 0; i < 4; i++ ) {
                encArray[i] = 0;
              }
            }
          }
        }
      }
    }
    if ( encArray[0] + encArray[1] >= bufferLimit ) { 
      timeBuff2 = currentTime;
      if ( enc1move == menuMax1 ) {
        enc1move = menuMax1;
      }
      if ( enc1move < menuMax1 - 1) {
        enc1move++;
      }
      menuSelection1 = enc1move;
      for ( int i = 0; i < 4; i++ ) {
        encArray[i] = 0;
      }
    }
    if ( encArray[2] + encArray[3] >= bufferLimit) {
      timeBuff2 = currentTime;
      if ( enc1move == menuMin1 ) {
        enc1move = menuMin1;
      }
      if ( enc1move > menuMin1 ) {
        enc1move--;
      }
      menuSelection1 = enc1move;
      for ( int i = 0; i < 4; i++ ) {
        encArray[i] = 0;
      }
    }
    oldPosition1 = newPosition1;
  }
  return menuSelection1;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// menuTwo function ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

unsigned int menuTwo( Encoder ) {
  
  // Check for encoder 2 movement
  long newPosition2 = enc2.read();
  
  if ( newPosition2 != oldPosition2 ) {
    setStandby(false); 
    timeBuff3 = currentTime;
    if ( timeBuff1 - timeBuff2 > 500 ) {
      for ( int i = 4; i < 8; i++ ) {
        if ( encArray[i] != 0 ) {
         encArray[i] = 0;
        }
      }
      timeBuff3 = timeBuff4 - 80;
    }
    if ( timeBuff3 > timeBuff4 + 20) {
      if ( newPosition2 == oldPosition2 + 1 ) {
        if ( encArray[6] != 0 && encArray[7] != 0 ) {
          for ( int i = 6; i < 8; i++ ) {
                encArray[i] = 0;
          }
        }
        if ( encArray[4] <= indexLimit ) {
          encArray[4]++;
          encArray[5]++;
        }
        if ( encArray[4] == indexLimit ) {
          if ( encArray[5] < bufferLimit ) {
            encArray[5]++;
            if ( encArray[5] == bufferLimit ) {
              for ( int i = 4; i < 8; i++ ) {
                encArray[i] = 0;
              }
            }
          }
        }
      }
      if ( newPosition2 == oldPosition2 - 1 ) {
        if ( encArray[4] != 0 && encArray[5] != 0 ) {
          for ( int i = 4; i < 6; i++ ) {
                encArray[i] = 0;
          }
        }
        if ( encArray[6] <= indexLimit ) {
          encArray[6]++;
          encArray[7]++;
        }
        if ( encArray[6] == indexLimit ) {
          if ( encArray[7] < bufferLimit ) {
            encArray[7]++;
            if ( encArray[7] == bufferLimit ) {
              for ( int i = 0; i < 4; i++ ) {
                encArray[i] = 0;
              }
            }
          }
        }
      }
    }
    if ( encArray[4] + encArray[5] >= bufferLimit ) { 
      timeBuff4 = currentTime;
      if ( enc2move == menuMax2 ) {
        enc1move = menuMax2;
      }
      if ( enc2move < menuMax2 - 1 ) {
        enc2move++;
      }
      menuSelection2 = enc2move;
      for ( int i = 4; i < 8; i++ ) {
        encArray[i] = 0;
      }
    }
    if ( encArray[6] + encArray[7] >= bufferLimit) {
      timeBuff4 = currentTime;
      if ( enc2move == menuMin2 ) {
        enc2move = menuMin2;
      }
      if ( enc2move > menuMin2 ) {
        enc2move--;
      }
      menuSelection2 = enc2move;
      for ( int i = 4; i < 8; i++ ) {
        encArray[i] = 0;
      }
    }
    oldPosition2 = newPosition2;
  }
  return menuSelection2;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// Standby function ///////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

boolean standby = false;
int modeNum = 0;

void setStandby (boolean sleep) {
  sleep = ( sleep );
  if ( sleep != standby ) {
    standby = sleep;
    if ( standby ) {
      Serial.println( "STANDING BY" );
      digitalWrite( powerPin, LOW );
//      digitalWrite(ledPins[tabNum], HIGH);
    } else {
      Serial.println( "WAKING UP" );
      digitalWrite( powerPin, HIGH );
//      digitalWrite(ledPins[tabNum], LOW);
    }
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// serialCommunication function ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void serialCommunicator() {
while ( Serial.available() ) {
    char inChar = ( char )Serial.read();
    if ( inChar == '\n' ) {
      if ( inputString == "sleep" ) {
        setStandby( true );
      }
      if ( inputString == "wake" ) {
        setStandby( false );
      }
      //  This could be better but it works just fine like this.
      //  Change identify/IDENTITY as necessary baseed on use case. 
      if ( inputString == "identify" ) {
        Serial.println( "IDENTITY" );
      }
//      Serial.print( inChar );
      inputString = "";
    }
    else {
      inputString += inChar;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void setup : START /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

void setup() {

  Serial.begin(38400);
   
//Set initial encoder position

  oldPosition1 = enc1.read();

// Set up buttonOne

  pinMode(buttonOnepin, INPUT_PULLUP);
  buttonOne.attach(buttonOnepin);
  buttonOne.interval(debounceInterval);

// Set up buttonTwo

  pinMode(buttonTwopin, INPUT_PULLUP);
  buttonTwo.attach(buttonTwopin);
  buttonTwo.interval(debounceInterval);
  
// Set up LED's as outputs and turn them all off

  int LEDpins[LEDs] = { wLED, yLED, gLED, bLED };

  for (int i = 0; i <= LEDs; i++) {
  
    pinMode(LEDpins[i], OUTPUT);
    digitalWrite(LEDpins[i], LOW);  
    
  }

// Set intitial menu states

  menuSelection1 = 0;
  menuSelection2 = 0;

// bholdPress acts like a 3 position momentary switch.  
// If one button is pressed, it turns to 2, then back to 1.
// If the other button is pressed, it turns to 0, then back to 1.
// The default setting is 1 but this can be changed to 0 or 2 (or whatever unsigned int you want).
// I'd like to keep it as an unsigned int for standardization purposes at the moment.  If a need
// arises to do otherwise later, I want it done in a more application specific fashion since
// I'm going to throw this whole ugly and chaotic code into a more organized class/struct/header/etc.

}

void loop() {

  // Reset the press & release times.
  wasReleased1 = wasPressed1;
  wasReleased2 = wasPressed2;
  
  // Set currentTime
  currentTime = millis();

  // Update button 1
  buttonOne.update();
  
  // Update button 2
  buttonTwo.update();
  
  // "Serial" communicator function returns whatever variables are requested
  serialCommunicator();
  
// The dynamicMenu function takes the encoder and button interactions into account to set the menuString variables

  dynamicMenu( menuOne( enc1 ) , menuTwo( enc2 ) , buttonHandler( buttonOne , buttonTwo ) );

//
//// Menu struct variables can be accessed via the following:
////
//// sel.header
//// sel.footer
//// sel.leftmenu
////
//// Example:
////
////if ( sel.header == "something" ) {
////  blah blah blah;
////}
////if ( sel.footer != "something" ) {
////  blah blah blah;  
////}
////if ( sel.leftMenu == 1 ) {
////  blah blah blah;
////}
//

//

//

//

//

//

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////// DEBUGGING: START ////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//if ( buttonOne.fell() == true ) {
//  Serial.print( "menuString: " );
//  Serial.println( sel.header );
//  Serial.println( sel.footer );
//  Serial.println( sel.leftMenu );
//}
//  Serial.print("menuSelection1: ");
//  Serial.println(menuSelection1);
//  Serial.print("enc1move: ");
//  Serial.println(enc1move);
//  Serial.print("menuSelection2: ");
//  Serial.println(menuSelection2);
//  Serial.print("enc2move: ");
//  Serial.println(enc2move);
//  Serial.print(" encArray[ ");
//  Serial.print(encArray[0]);
//  Serial.print(" , ");
//  Serial.print(encArray[1]);
//  Serial.print(" , ");
//  Serial.print(encArray[2]);
//  Serial.print(" , ");
//  Serial.print(encArray[3]);
//  Serial.print(" , ");
//  Serial.print(encArray[4]);
//  Serial.print(" , ");
//  Serial.print(encArray[5]);
//  Serial.print(" , ");
//  Serial.print(encArray[6]);
//  Serial.print(" , ");
//  Serial.print(encArray[7]);
//  Serial.print(" ] ");
//  currentTime = millis();
//  wasPressed1 = currentTime;
//  Serial.print("wasPressed1: ");
//  Serial.println(wasPressed1);
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// DEBUGGING: END /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//////// void loop : END ////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
