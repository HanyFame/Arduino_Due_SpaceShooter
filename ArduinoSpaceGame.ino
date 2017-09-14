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

int x, y; //Touch Location Variables
int z = 0; //Score Variable
int target1, target2, target3; //Tagets X location
int location = 0; //Space Ship X location
int targetfall = 0; //Targets Y location
char strBuff[10];

/*************************
**   Custom functions   **
*************************/

void drawGame()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.printAlignedOffseted("Score", gTextAlignTopCenter, 120, 8);
  //tft.setSPIClockDivider(2); // lower the frequency if the image is distorted
  tft.drawImage(fighter2, 135, 140 , 55, 33);
  tft.fillCircle(160, 220, 20, ILI9341_RED);
  tft.fillTriangle(0, 220, 40, 200, 40, 240, ILI9341_RED);
  tft.fillTriangle(320, 220, 280, 200, 280, 240, ILI9341_RED);
  tft.fillRect(0, 198, 320, 3, ILI9341_RED);
}

/*********************
 * Space BG Function *
 ********************/
void drawSpace()
{
  int ss;
  tft.fillRect(0, 0, 320, 190, ILI9341_BLACK);
  tft.printAlignedOffseted("Score", gTextAlignTopCenter, 120, 8);
  sprintf(strBuff, "%d", z); 
  tft.printAlignedOffseted(strBuff, gTextAlignTopCenter, 120, 20); 
  tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
  
  for (ss = 0; ss <=20; ss++)
  {        
    //tft.drawPixel(random(0,320), random(0, 190), ILI9341_CYAN);
    tft.drawPixel(random(0,320), random(0, 190), ILI9341_GRAY);    
  }
  delay(75);
}

/*******************
 * Targets Function *
 ***************** */
void drawTargets(int y)
{
  if (y == 0)
  {
    target1 = random(20, 260);
    target2 = random(20, 260);
    target3 = random(20, 260);   
  }
  tft.fillCircle(target1, 4*(y-1), 4, ILI9341_BLACK);
  tft.fillCircle(target2, 4*(y-1), 4, ILI9341_BLACK);
  tft.fillCircle(target3, 4*(y-1), 4, ILI9341_BLACK);
  tft.fillCircle(target1, 4*(y), 4, ILI9341_BLUE);
  tft.fillCircle(target2, 4*(y), 4, ILI9341_BLUE);
  tft.fillCircle(target3, 4*(y), 4, ILI9341_BLUE);
  if (targetfall >= 47)
  {
    tft.fillCircle(target1, 4*(y), 4, ILI9341_BLACK);
    tft.fillCircle(target2, 4*(y), 4, ILI9341_BLACK);
    tft.fillCircle(target3, 4*(y), 4, ILI9341_BLACK);      
  }
  delay(50);
}


/************************
**  Required function  **
************************/

void setup()
{
	Serial.begin(9600);
	// Initial setup
	tft.begin();
	tft.setRotation(iliRotation270);	// landscape
	tft.fillScreen(ILI9341_BLACK);

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	tft.setFont(Arial_bold_14);
	drawGame();
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
  drawSpace();
  yield();
}


/*************
 * Main Loop *
 ************/
void loop()
{
	while (true)
	{
		if (myTouch.dataAvailable())
		{
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();

			if ((y >= 200) && (x <= 40))  // left
			{
        z++;
        //drawSpace();

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

			if ((y >= 160) && (x >=130) && (190 >= x))  // Shoot Laser
			{
        z++;

        tft.drawImage(fighter2, 135+8*location, 140, 55, 33);
        
        tft.fillCircle(160, 220, 20, ILI9341_BLUE);
        delay(5);
        tft.fillCircle(160, 220, 20, ILI9341_RED);

        /// Shoot Fighter's Laser
        tft.drawLine(135+(8*location)+26, 140, 135+(8*location)+26, 0, ILI9341_CYAN);
        tft.drawLine(135+(8*location)+27, 140, 135+(8*location)+27, 0, ILI9341_CYAN);
        tft.drawLine(135+(8*location)+28, 140, 135+(8*location)+28, 0, ILI9341_CYAN);
        delay(20);
        tft.drawLine(135+(8*location)+26, 140, 135+(8*location)+26, 0, ILI9341_BLACK);
        tft.drawLine(135+(8*location)+27, 140, 135+(8*location)+27, 0, ILI9341_BLACK);  
        tft.drawLine(135+(8*location)+28, 140, 135+(8*location)+28, 0, ILI9341_BLACK);    
   
			}

			if ((y >= 160) && (x >= 280))  // right
			{
        z++;
        
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
