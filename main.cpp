
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BMP_NAME_SIZE 255 //the maximum size of the BMPs filename

int main(void) {
	FILE *file;
	FILE *bmp;
	FILE *cArr;

	char BmpName[BMP_NAME_SIZE] = {0};
	fpos_t NameLength = 0;
	uint32_t BmpWidth = 0;
	uint32_t BmpHeight = 0;

	file = fopen("file.txt","r");
	fgets(BmpName,BMP_NAME_SIZE,file);
	fgetpos(file,&NameLength);
	fclose(file);

	bmp = fopen(BmpName,"r");
	fseek(bmp,0x12,SEEK_SET);
	fread(&BmpWidth,4,1,bmp);
	fread(&BmpHeight,4,1,bmp);

	uint8_t BMP[BmpHeight*BmpWidth*4];
	fseek(bmp,0,SEEK_SET);
	fread(BMP,1,BmpHeight*BmpWidth*4,bmp);
	fclose(bmp);

	BmpName[NameLength-3] = 'h';
	BmpName[NameLength-2] = 0;
	//BmpName[NameLength-1] = 0;
	cArr = fopen(BmpName,"w");

	BmpName[NameLength-4] = 0;
	//BmpName[NameLength-3] = 0;
	fprintf(cArr,"const unsigned char ");
	fprintf(cArr,BmpName);
	fprintf(cArr,"[] = {\n  0x%.2x",BMP[0]);

	for(uint32_t i = 1; i < BmpHeight*BmpWidth*4; i++) {
		fputs(", ",cArr);
		if(!(i % 12)) fprintf(cArr,"\n  ");
		fprintf(cArr,"0x%.2x",BMP[i]);
	}
	fputs("}",cArr);
}










