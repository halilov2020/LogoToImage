#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "gd.h"
int entity(gdImagePtr pLogo, char nameInFoto[50], char nameOutFoto[50], char pathIn[50], char pathOut[50], float borderPer, float logoScalePer) {
	char pathIN[50];
	strcpy(pathIN, pathIn);
	char pathOUT[50];
	strcpy(pathOUT, pathOut);

	FILE* inFoto;
	strcat(pathIN, nameInFoto);
	printf("%s\n", pathIN);
	inFoto = fopen(pathIN, "rb");
	FILE* out;
	strcat(pathOUT, nameOutFoto);
	printf("%s\n", pathOUT);
	out = fopen(pathOUT, "wb");
	gdImagePtr pFoto = gdImageCreateFromJpegEx(inFoto, GD_TRUE);
	gdImagePtr pOut = gdImageClone(pFoto);
	gdImageDestroy(pFoto);
	float logoX = gdImageSX(pLogo);
	float logoY = gdImageSY(pLogo);
	float outX = gdImageSX(pOut);
	float outY = gdImageSY(pOut);

	float border = (outX * borderPer / 100 + outY * borderPer / 100) / 2;
	float logoScale = min(outX, outY) * logoScalePer / 100;
	float x;
	if (outY < outX) {
		x = logoScale / logoY;
	}
	else {
		x = logoScale / logoX;
	}
	float dstX = border;
	float dstY = outY - border - logoY * x;
	float dstW = logoX * x;
	float dstH = logoY * x;
	gdImageCopyResized(pOut, pLogo, dstX, dstY, 0, 0, dstW, dstH, logoX, logoY);

	gdImageJpeg(pOut, out, -1);
	gdImageDestroy(pOut);
	return 0;
}
int main() {

	char pathin[100];
	char pathout[100];
	char logoPath[100];
	char format[10];
	float borderPer;
	float logoScalePer;
	printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!!\n");
	printf("!!!!ALL PATHS SHOULD USE SLASH - / not BACKSLASH - \\!!!!\n");
	printf("Full path to the folder with input files(ended with / ): ");
	scanf("%s", &pathin);
	printf("Format of input files(.jpg, .JPG): ");
	scanf("%s", &format);
	printf("Full path to the folder with output files (ended with / ): ");
	scanf("%s", &pathout);
	printf("Full path to the logo file(ended with .png): ");
	scanf("%s", &logoPath);
	printf("Borders of logo in final foto (in percent): ");
	scanf("%f", &borderPer);
	printf("Logo scale relative to final foto (in percent): ");
	scanf("%f", &logoScalePer);
	FILE* inLogo;
	inLogo = fopen(logoPath, "rb");

	gdImagePtr pLogo = gdImageCreateFromPng(inLogo);
	int numOfFiles = 0;
	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(pathin)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strstr(ent->d_name, format)) {
				printf("%i.)%s\n", numOfFiles + 1, ent->d_name);
				entity(pLogo, ent->d_name, ent->d_name, pathin, pathout, borderPer, logoScalePer);
				numOfFiles++;
			}
		}
		closedir(dir);
	}
	else {
		perror("We can't open the directory!");
		return EXIT_FAILURE;
	}
	printf("Finished!!!Processed %i images!", numOfFiles);
}