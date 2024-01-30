
#include <stdio.h>
#include <stdint.h>

#define BMP_NAME_SIZE 255 //the maximum size of the bitmaps' filename

int main(void) {
	FILE *file; //the file for the bitmaps' name
	FILE *bmp;  //the bitmap file
	FILE *cArr; //the output (a header file)

	char BmpName[BMP_NAME_SIZE] = {0};  //used to store the name of the bitmap
	fpos_t NameLength = 0;              //used to rewrite the bitmaps' name for further use

	uint32_t BmpSize = 0;
	uint32_t BmpPixels = 0;
	/*
	 * this variable determines the length of the array used
	 * to transfer the data from the bitmap to the header file
	 */

	file = fopen("file.txt","r");       //opens the file.txt in reading mode
	fgets(BmpName,BMP_NAME_SIZE,file);  //reads the bitmaps' name
	fgetpos(file,&NameLength);          //reads the length of the bitmaps' name by checking how far the cursor has moved
	fclose(file);

	bmp = fopen(BmpName,"r");  //opens the bitmap using the name obtained from the file.txt
	fseek(bmp,0x2,SEEK_SET);   //moves the cursor to the address 0x2 to read the size of the bitmap
	fread(&BmpSize,4,1,bmp);   //reads 4 bytes
	fseek(bmp,0xA,SEEK_SET);   //moves the cursor to the address 0xA to read the starting address of the pixeldata in the array
	fread(&BmpPixels,4,1,bmp); //reads 4 bytes

		printf("Filename = %s\n", BmpName); //outputs some data
		printf("FileSize = %u\n", BmpSize);
		printf("PixelAddr = %u\n", BmpPixels);

	uint8_t BMP[BmpSize];       //creates an array to store all the data of the bitmap
	fseek(bmp,0,SEEK_SET);      //moves the cursor back to the start within the bitmap
	fread(BMP,1, BmpSize,bmp);  //reads the whole bitmap and puts it into the array
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

	BmpName[NameLength-4] = 0;                  //rewrites the dot with a NULL character
	fprintf(cArr,"const unsigned char ");       //prints the first few words into the header file
	fprintf(cArr,BmpName);                      //prints the modified name and stops when it encounters a NULL character
	fprintf(cArr,"[] = {\n  0x%.2x",BMP[0]);    //prints the first byte

	//sets the alpha value (transparency) of every white pixel to 0
	for(uint32_t i = BmpPixels; i < BmpSize; i += 4) {
		if((BMP[i+1] == 0xFF) && (BMP[i+2] == 0xFF) && (BMP[i+3] == 0xFF)) {
			BMP[i] = 0x00;
		}
	}

	//prints the rest of the data and starts a new line after every 12 bytes
	for(uint32_t i = 1; i < BmpSize; i++) {
		fprintf(cArr,", ");
		if(!(i % 12)) fprintf(cArr,"\n  ");
		fprintf(cArr,"0x%.2x",BMP[i]);
	}
	fprintf(cArr,"}");
}










