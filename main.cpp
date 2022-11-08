#include <iostream>
#include "Bitmap.h"

int main(void)
{
	Bitmap img(1024, 768);
	img.Fill(BitmapColor(1.0f, 0.5f, 0.25f));
	img.SaveFile("raytracy.bmp");

	return 0;
}