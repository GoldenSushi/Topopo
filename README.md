# Topopo

Allows the user to organize traverse survey data. Calculates azimuth automagically if reference azimuth (the azimuth from the first point to the second) is provided.

## Usage

The data must be put in order, and the provided reference azimuth must be measured in the clockwise direction. The user may provide as many measurements he or she whishes from the same point, but when moving to the next "Ponto Visado" (the point being measured) of the polygon, the word "Vante" (forward in Portuguese) must be written at the Point column so the program changes the reference azimuth accordingly for all values after that point (yeah, pretty annoying. Should be easier to use in the future).

## Known issues / Features to be implemented in the future

   * In ll fields that receive angle values, for angles to be properly computed there must be no extra spaces. Any exceeding space will result in the program not recognizing the the values as angles.

  * ~~Program still doesn't support data persistence. That means there is no way to save the work as of now (yeah, that means the program is pretty useless... oh well ¯\\_(ツ)_/¯ ). Should be implemented soon, I promise.~~
  
  * Rows can't be moved, only deleted and edited, which can be really annoying sometimes.
 
  * All text is hardcoded in Portuguese, so nothing in english for now. I might as well learn how to use string resourses on Qt framework. Should be done eventually.
 
 ## Changelog
 
  * 02/06/2018
  > Program is working. Probably with some bugs, and yet to be implemented features. But well, it runs smoothly (I hope so).
  * 03/06/2018
  > Reference azimuth input bug fixed;
  >
  > Angle calculation fixed;
  >
  > Fixed issue where program was correcting first row's azimuth and it shouldn't;
  >
  > Single zero inputs on table now automatically formats text to 0° 0' 0";
  >
  > At new point window, if Station column is filled focus will jump to the next field;
  >
  > Now table is refreshed after a new point is added.
  * 18/06/2018
  > Save and load functions are now implemented, yeaaah! Although a very primitive and basic system, table data can now be stored in a csv file by clicking in the save button. If data is organized accordingly (in the same column order it was saved in, and with the header included) it is also possible to load the file (not thoroughly tested though :v).
  >
  >New table added. The new table is responsible for the error calculations. For now it is only partially functional.
  * 24//06/2018
  > Corrected issue where loading would cause the program to crash.
  >
  > Error table only calculates corrections for polygon entries (rows where the word "Vante" is found) as it should be.
  >
  > Most fields now update automatically.
  >
  > Just attempted to make my Topography project using it. Seems to be working correctly (or else I'm screwed).