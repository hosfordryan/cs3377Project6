/*
 * Usage of CDK Matrix
 *
 * File:   program6.cc
 * Author: Ryan Hosford
 * Email:  rph160030@utdallas.edu
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
#include <inttypes.h>
#include "cdk.h"


#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 20
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

class BinaryFileHeader{
	public:
		uint32_t magicNumber;
		uint32_t versionNumber;
		uint64_t numRecords;
};


const int maxRecordStringLength = 25;

class BinaryFileRecord{
	public:
		uint8_t strLength;
		char stringBuffer[maxRecordStringLength];
};

int main()
{

  WINDOW	*window;
  CDKSCREEN	*cdkscreen;
  CDKMATRIX     *myMatrix;           // CDK Screen Matrix

  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char 		*rowTitles[] = {"R0", "R1", "R2", "R3", "R4", "R5"};
  const char 		*columnTitles[] = {"C0", "C1", "C2", "C3", "C4", "C5"};
  int		boxWidths[] = {BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH};
  int		boxTypes[] = {vMIXED, vMIXED, vMIXED, vMIXED,  vMIXED,  vMIXED};

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr();
  cdkscreen = initCDKScreen(window);

  /* Start CDK Colors */
  initCDKColor();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
  */
  myMatrix = newCDKMatrix(cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_HEIGHT, MATRIX_WIDTH,
			  MATRIX_NAME_STRING, (char **) rowTitles, (char **) columnTitles, boxWidths,
				     boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix ==NULL)
    {
      printf("Error creating Matrix\n");
      _exit(1);
    }

  /* Display the Matrix */
  drawCDKMatrix(myMatrix, true);


  //Read the file
  BinaryFileHeader *myHeader = new BinaryFileHeader();
  ifstream binInFile("cs3377.bin" , ios::in | ios::binary);

  binInFile.read((char *) myHeader, sizeof(BinaryFileHeader));
  //Display header...
  char textToWrite[80];
  strcpy(textToWrite,"Magic: 0x");
  sprintf(textToWrite + strlen(textToWrite), "%X", myHeader->magicNumber);

  setCDKMatrixCell(myMatrix, 1,1,textToWrite);

  strcpy(textToWrite, "Version: ");
 sprintf(textToWrite + strlen(textToWrite), "%u", myHeader->versionNumber);
  setCDKMatrixCell(myMatrix, 1,2,textToWrite); 

 
  strcpy(textToWrite, "NumRecords: ");
 sprintf(textToWrite + strlen(textToWrite), "%lu", myHeader->numRecords);
  setCDKMatrixCell(myMatrix, 1,3,textToWrite); 
  //Now for the records
  //
  BinaryFileRecord *myRecord = new BinaryFileRecord();


  for(uint64_t i = 0; i < myHeader->numRecords; i++){
	   binInFile.read((char *) myRecord, sizeof(BinaryFileRecord));
	  char textToWrite[80];
	   strcpy(textToWrite, "strlen: ");
	   sprintf(textToWrite + strlen(textToWrite), "%" PRId8,myRecord->strLength);
	   setCDKMatrixCell(myMatrix, 2+i,1,textToWrite); 

	   sprintf(textToWrite, "%s", myRecord->stringBuffer);
	   setCDKMatrixCell(myMatrix, 2+i, 2, textToWrite);
  }

  /*
   * Dipslay a message
   */
  drawCDKMatrix(myMatrix, true);    /* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;

  // Cleanup screen
  endCDK();
}
