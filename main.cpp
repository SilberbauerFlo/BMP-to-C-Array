
#include <stdio.h>
#include <stdint.h>

#define BMP_NAME_SIZE 255 //the maximum size of the bitmaps' filename

int main(void) {
	FILE *file; //the file for the bitmaps' name
	FILE *bmp;  //the bitmap file
	FILE *cArr; //the output (a header file)

	char BmpName[BMP_NAME_SIZE] = {0};  //used to store the name of the bitmap
	fpos_t NameLength = 0;              //used to rewrite the bitmaps' name for further use

	uint32_t BmpWidth = 0;
	uint32_t BmpHeight = 0;

	/*
	 * these variables determine the length of the array used to transfer the data
	 * from the bitmap to the header file
	 */

	file = fopen("file.txt","r");       //opens the file.txt in reading mode
	fgets(BmpName,BMP_NAME_SIZE,file);  //reads the bitmaps' name
	fgetpos(file,&NameLength);          //reads the length of the bitmaps' name by checking how far the cursor has moved
	fclose(file);

	bmp = fopen(BmpName,"r");  //opens the bitmap using the name obtained from the file.txt
	fseek(bmp,0x12,SEEK_SET);  //moves the cursor to the address 0x12 (the first byte of the width of a bitmap)
	fread(&BmpWidth,4,1,bmp);  //reads 4 bytes (the size of the width of a bitmap)
	fread(&BmpHeight,4,1,bmp); //reads the next 4 bytes (the size of the height of a bitmap)

	uint8_t BMP[BmpHeight*BmpWidth*4];      //creates an array to store all the data of the bitmap
	fseek(bmp,0,SEEK_SET);                  //moves the cursor back to the start within the bitmap
	fread(BMP,1,BmpHeight*BmpWidth*4,bmp);  //reads the whole bitmap and puts it into the array
	fclose(bmp);

	BmpName[NameLength-3] = 'h';
	BmpName[NameLength-2] = 0;

	/*
	 * E|x|a|m|p|l|e|.|b|m|p
	 * 0|1|2|3|4|5|6|7|8|9|10 -> NameLength = 11
	 *                 ^ first rewrites the b with h
	 *                   ^ then rewrites the m with a NULL character
	 */

	cArr = fopen(BmpName,"w"); //creates a new file using the modified name in writing mode

	BmpName[NameLength-4] = 0;                  //rewrites the dot with NULL
	fprintf(cArr,"const unsigned char ");       //prints the first few words into the header file
	fprintf(cArr,BmpName);                      //prints the modified name and stops when it encounters a NULL character
	fprintf(cArr,"[] = {\n  0x%.2x",BMP[0]);    //prints the first byte

	for(uint32_t i = 1; i < BmpHeight*BmpWidth*4; i++) { //prints the rest of the data and starts a new line after every 12 bytes
		fprintf(cArr,", ");
		if(!(i % 12)) fprintf(cArr,"\n  ");
		fprintf(cArr,"0x%.2x",BMP[i]);
	}
	fprintf(cArr,"}");
}










