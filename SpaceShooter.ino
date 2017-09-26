#include <SPI.h>
#include <ILI9341_due_config.h>
#include <ILI9341_due.h>
#include "fonts\Arial_bold_14.h"
#include <URTouch.h>
#include <Scheduler.h>
#include "fighter2.h"

// For the Adafruit shield, these are the default.
#define TFT_RST 46
#define TFT_DC 44
#define TFT_CS 48

// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);

URTouch  myTouch(34, 32, 30, 28, 26);

int x, y; //Touch Finger Location Variables
int z = 0; //Score Variable
int i = 0; //Targets loops variable
int targets[8] = {0,0,0,0,0,0,0,0}; //Targets X location array
int targetHits[8] = {1,1,1,1,1,1,1,1}; //Which Target was hit array
int targetGone[8] = {1,1,1,1,1,1,1,1}; //Target disappeared after hit array
int lives = 3; //Number of lives available
boolean crash = false; //Spaceship crash state
int levelSpeed = 80;
int location = 0; //Spaceship X location
int targetfall = 0; //Targets Y location
char strBuff[10]; //Text buffer array for LCD
int buzzer = 13; //Buzzer pin for sounds


/*************************
**   Custom functions   **
*************************/

/***********************
 * Start menu function *
 **********************/
void drawStart()
{
  crash = false; // Reset crash variable
  z = 0;  // Reset score
  lives = 3; // Reset lives
  levelSpeed = 80; // Reset level
  tft.fillScreen(ILI9341_BLACK);
  for (int i = 0; i <=50; i++)
  {        
    tft.drawPixel(random(0,320), random(0, 240), ILI9341_CYAN);
    tft.drawPixel(random(0,320), random(0, 240) , ILI9341_GRAY);    
  }
  tft.fillRoundRect(40, 105, 240, 60, 6, ILI9341_BLUE);
  tft.setTextScale(2);
  tft.setTextColor(ILI9341_CYAN, ILI9341_BLACK);
  tft.printAlignedOffseted(".: SPACE FIGHTER :.", gTextAlignTopCenter, 0, 20);
  tft.drawImage(fighter2, 135, 60 , 55, 33);
  tft.setTextColor(ILI9341_LIME, ILI9341_BLUE);
  tft.printAlignedOffseted("START GAME", gTextAlignTopCenter, 0, 120);
  while (true)
  {
    myTouch.read();
    x = myTouch.getX();
    y = myTouch.getY();
    if ( (x >40 && x <260) && (y>100 && y<160) )
    {
      tft.setTextColor(ILI9341_BLUE, ILI9341_LIME);
      tft.fillRoundRect(40, 105, 240, 60, 6, ILI9341_LIME);
      tft.printAlignedOffseted("START GAME", gTextAlignTopCenter, 0, 120);
      analogWrite(buzzer, 230);
      delay(10);
      analogWrite(buzzer, 0);
      delay(500);
      drawGame();
      break;
    }
  }        
}


/********************
*Game start function *
 ********************/
void drawGame()
{
  tft.setTextScale(1);
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_LIME, ILI9341_BLACK);
  tft.printAlignedOffseted("Score", gTextAlignTopCenter, 120, 8);
  tft.printAlignedOffseted("Fighters", gTextAlignTopCenter, -120, 8);
  //tft.setSPIClockDivider(2); // lower the frequency if the image is distorted
  tft.drawImage(fighter2, 135, 140 , 55, 33);
  tft.fillCircle(160, 220, 20, ILI9341_RED);
  tft.fillTriangle(0, 220, 40, 200, 40, 240, ILI9341_RED);
  tft.fillTriangle(320, 220, 280, 200, 280, 240, ILI9341_RED);
  tft.fillRect(0, 198, 320, 3, ILI9341_RED);
}


/*****************************
 * Space Background Function *
 ****************************/
void drawSpace()
{
  int ss;
  tft.setTextScale(1);
  tft.fillRect(0, 0, 320, 190, ILI9341_BLACK);
  tft.printAlignedOffseted("Score", gTextAlignTopCenter, 120, 8);
  tft.printAlignedOffseted("Fighters", gTextAlignTopCenter, -120, 8);
  sprintf(strBuff, "%d", z); 
  tft.printAlignedOffseted(strBuff, gTextAlignTopCenter, 120, 20);
  sprintf(strBuff, "%d", lives);
  tft.printAlignedOffseted(strBuff, gTextAlignTopCenter, -120, 20); 
  tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
  
  for (ss = 0; ss <=20; ss++)
  {        
    tft.drawPixel(random(0,320), random(0, 190) , ILI9341_GRAY);    
  }
  delay(80);
}


/********************
 * Targets Function *
 *******************/
void drawTargets(int y)
{
  if (y == 0) // if targets reached bottom of screen, generate new random targets in x axis
  {
    targets[0] = random(20, 260);
    targets[1] = random(20, 260);
    targets[2] = random(20, 260);
    targets[3] = random(20, 260); 
    targets[4] = random(20, 260);
    targets[5] = random(20, 260);
    targets[6] = random(20, 260); 
    targets[7] = random(20, 260); 
  }

// Check if a target was hit
  for (i=0; i<8; ++i)
  {
    if (targetHits[i] == 0 && targetGone[i] == 1)
    { 
      targetGone[i] = 0;    
      tft.fillCircle(targets[i], 4*(y), 12, ILI9341_YELLOW);
      delay(20);
      tft.drawCircle(targets[i], 4*(y), 15, ILI9341_YELLOW);
      delay(20);
      tft.fillCircle(targets[i], 4*(y), 12, ILI9341_BLACK);
      analogWrite(buzzer, 10);
      delay(15);
      analogWrite(buzzer, 0);   
      z +=10;
      //break;
    }
  }
  for (i=0; i<8; ++i)
  {
    tft.fillCircle(targets[i], 4*(y-1), 6*targetGone[i], ILI9341_BLACK);
  }
  for (i=0; i<8; ++i)
  {
    tft.fillCircle(targets[i], 4*(y), 6*targetGone[i], ILI9341_BLUE);
  }
   
  delay(levelSpeed);
  
  //Check if any of falling targets hit the ship bounderies, then call the ship crash function.
  if (targetfall >= 38 && targetfall <= 41)
  {
    for (i=0; i<8; ++i)
    {
      if ( ((targetGone[i]*(targets[i]+6)) > (135+8*location+2)) && ((targetGone[i]*(targets[i]+6)) < (135+(8*location)+53)) )
      {
        for (i=0; i<8; ++i)
        {
          targetHits[i] = 1;
          targetGone[i] = 1;
        }
        shipCrash();
        targetfall = 0;
      }
    }
  }
  
  if (targetfall >= 47) //Targets reached bottom of the screen
  {
    if (z >= 500)  //Level 2 after score of 500
    {
      levelSpeed = 60;
    }
    if (z >= 1000) //Level 3 after score of 1000
    {
      levelSpeed = 50;
    }
    if (z >= 2000) //Level 4 after score of 2000
    {
      levelSpeed = 30;
    }
    if (z >= 3000) //Level 5 after score of 3000
    {
      levelSpeed = 20;    
    }
    
    //reset targets hit and gone arrays to generate 8 new targets from top
    for (i=0; i<8; ++i)
    {
      targetHits[i] = 1;
      targetGone[i] = 1;
      tft.fillCircle(targets[i], 4*(y), 6, ILI9341_BLACK);
    }      
  }
}

/******************
 * Crash Function *
 *****************/
void shipCrash()
{
  crash = true;
  lives--;
  analogWrite(buzzer, 250);
  tft.fillCircle(155+8*location, 155, 6, ILI9341_RED);
  delay(100);
  tft.fillCircle(155+8*location, 155, 10, ILI9341_RED);
  analogWrite(buzzer, 0);
  delay(100);
  analogWrite(buzzer, 250);
  tft.fillCircle(155+8*location, 155, 15, ILI9341_RED);
  delay(100);
  tft.drawCircle(155+8*location, 155, 20, ILI9341_RED);
  analogWrite(buzzer, 0);
  delay(200);
  analogWrite(buzzer, 250); 
  tft.drawCircle(155+8*location, 155, 30, ILI9341_RED);
  delay(200);
  analogWrite(buzzer, 0);
  tft.drawCircle(155+8*location, 155, 35, ILI9341_RED);
  delay(500);
  analogWrite(buzzer, 250);
  tft.fillCircle(155+8*location, 155, 5, ILI9341_YELLOW);
  delay(100);
  analogWrite(buzzer, 250);
  tft.fillCircle(155+8*location, 155, 12, ILI9341_YELLOW);
  delay(100);
  analogWrite(buzzer, 250);
  tft.fillCircle(155+8*location, 155, 17, ILI9341_YELLOW);
  delay(100);
  analogWrite(buzzer, 250);
  tft.drawCircle(155+8*location, 155, 22, ILI9341_YELLOW);
  delay(200); 
  tft.drawCircle(155+8*location, 155, 32, ILI9341_YELLOW);
  delay(200);
  analogWrite(buzzer, 0);
  tft.drawCircle(155+8*location, 155, 37, ILI9341_YELLOW);
  delay(3000);
  tft.fillCircle(155+8*location, 155, 37, ILI9341_BLACK); 
  delay(500);
  if (lives == -1)
  {
    gameOver();
  }
  else
  {
    crash = false;
    drawGame();
  }
}

/**********************
 * Game Over Function *
 *********************/
void gameOver()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextScale(2);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.printAlignedOffseted("GAME OVER!", gTextAlignTopCenter, 0, 100);
  tft.setTextColor(ILI9341_LIME, ILI9341_BLACK); 
  tft.setTextScale(1);
  tft.printAlignedOffseted("Score:", gTextAlignTopCenter, -20, 160);
  sprintf(strBuff, "%d", z);
  tft.printAlignedOffseted(strBuff, gTextAlignTopCenter, 30, 160);
  analogWrite(buzzer, 250);
  delay(300);
  analogWrite(buzzer, 0);
  delay(1000);
  tft.setTextScale(2);
  tft.setTextColor(ILI9341_YELLOW, ILI9341_BLACK);
  tft.printAlignedOffseted("GAME OVER!", gTextAlignTopCenter, 0, 100);
  analogWrite(buzzer, 250); 
  delay(1000);
  analogWrite(buzzer, 0);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.printAlignedOffseted("GAME OVER!", gTextAlignTopCenter, 0, 100);
  delay(5000);
  drawStart(); 
}


/************************
**  Required function  **
************************/

void setup()
{
  Serial.begin(9600);
  // Initial setup
  pinMode (buzzer, OUTPUT);
  tft.begin();
  tft.setRotation(iliRotation270);  // landscape
  tft.fillScreen(ILI9341_BLACK);

  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  tft.setFont(Arial_bold_14);
  drawStart();
  tft.setTextColor(ILI9341_LIME, ILI9341_BLACK);
  Scheduler.startLoop(loop2);
  Scheduler.startLoop(loop3);
}


/***************************
 * Targets Function Thread *
 **************************/
  void loop2()
{
  drawTargets(targetfall);
  targetfall++;
  if (targetfall >= 48)
  {
    targetfall=0;  
  }
  yield();
}


/**************************
 * Space Bacground Thread *
 *************************/
void loop3()
{
  yield();
  while (crash == false)
  {
    drawSpace();
  }
}


/*************
 * Main Loop *
 ************/
void loop()
{
  yield();
  while (true && crash == false)
  {
    if (myTouch.dataAvailable())
    {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();

      if ((y >= 200) && (x <= 40))  // Move space ship left
      {
        tft.fillTriangle(0, 220, 40, 200, 40, 240, ILI9341_BLUE);
        delay(5);        
        tft.fillTriangle(0, 220, 40, 200, 40, 240, ILI9341_RED);

        if ((135+8*(location+1)) >= 265)
        {
          tft.fillRect(265, 140, 55, 33, ILI9341_BLACK);              
        }
        else
        {
          tft.fillRect(135+8*location, 140, 55, 33, ILI9341_BLACK);       
        }
        location--;
        
        if ((135+8*location) <= 0)
        {
         tft.drawImage(fighter2, 0, 140 , 55, 33);
         delay(30);
         location++; 
        }
        else
        {
          tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
        delay(30);
        }
      }

      if ((y >= 160) && (x >=130) && (190 >= x))  // Shoot Laser and checks if it hits a falling target
      {
        tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
        
        tft.fillCircle(160, 220, 20, ILI9341_BLUE);
        delay(5);
        tft.fillCircle(160, 220, 20, ILI9341_RED);

        /// Shoot Fighter's Laser
        tft.drawLine(135+(8*location)+26, 140, 135+(8*location)+26, 0, ILI9341_ORANGE);
        tft.drawLine(135+(8*location)+27, 140, 135+(8*location)+27, 0, ILI9341_ORANGE);
        tft.drawLine(135+(8*location)+28, 140, 135+(8*location)+28, 0, ILI9341_ORANGE);
        delay(20);
        tft.drawLine(135+(8*location)+26, 140, 135+(8*location)+26, 0, ILI9341_ORANGE);
        tft.drawLine(135+(8*location)+27, 140, 135+(8*location)+27, 0, ILI9341_ORANGE);  
        tft.drawLine(135+(8*location)+28, 140, 135+(8*location)+28, 0, ILI9341_ORANGE);
        analogWrite(buzzer, 150);
        delay(5);
        analogWrite(buzzer, 0);
        
        //Check if laser x axis hit a target
        for (i = 0; i<8 ; ++i)
        {
          if (  (((targets[i]-6) <= (135+(8*location)+26)) && ((135+(8*location)+26) <= (targets[i]+6)))  || ((((135+(8*location)+28) >= (targets[i]-6))) && ((135+(8*location)+28) <= (targets[i]-1)))  )
          {
            targetHits[i] = 0;
          }
        } 
      }

      if ((y >= 160) && (x >= 280))  // Move space ship right
      {      
        tft.fillTriangle(320, 220, 280, 200, 280, 240, ILI9341_BLUE);
        delay(5);
        tft.fillTriangle(320, 220, 280, 200, 280, 240, ILI9341_RED);

        if ((135+8*(location-1)) <= 0)
        {
          tft.fillRect(0, 140, 55, 33, ILI9341_BLACK );
        }
        else
        {
          tft.fillRect(135+8*location, 140, 55, 33, ILI9341_BLACK );
        }
        
        location++;
        
        if ((135+8*location) >= 265)
        {
          tft.drawImage(fighter2, 265, 140, 55, 33);
          delay(30);
          location--; 
        }
        else
        {
          tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
          delay(30);              
        }
      }
      else
      {
        yield();
      }
    }
    else
    {
      yield();
    }
  }
}
