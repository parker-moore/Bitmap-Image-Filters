#include <iostream>
#include "bitmap.h"

/* cellShade
 * Parameter: Bitmap& b,
 * Description:The ﬁrst ﬁlter is cell shading. The idea here is really simple. For a 24 or 32 bit image each component can be 0-255. 
 * We want to restrict this to 3 possible value (0, 128, 255). Round each component to the nearest value. So a pixel with 23 red, 130 green, 
 * and 200 blue will become (0,128,255).
 */
void cellShade(Bitmap& b) {
	//loop through each pixel
	for(int w=0;w<b.getWidth();w++)
		for(int h=0;h<b.getHeight();h++){
			
			//grab R,G,B,A components of pixel and bitshift to make number between 0-255
			unsigned int red = (b.pixel[w][h] & b.mask1)>>SHIFT3BYTES;
			unsigned int green = (b.pixel[w][h] & b.mask2)>>SHIFT2BYTES;
			unsigned int blue = (b.pixel[w][h]&b.mask3)>>SHIFT1BYTE;
			unsigned int alpha = b.pixel[w][h]&255;

			//find the # (0,128,255)that the value of R,G or B is closest to
			red = findClosest(red);
			green = findClosest(green);
			blue = findClosest(blue);
			alpha = findClosest(alpha);	

			//bitshift colors back to original position 	
			red = red<<SHIFT3BYTES;
			green = green<<SHIFT2BYTES;
			blue = blue<<SHIFT1BYTE;	

			//set b.pixel[w][h] = to newly compiled color
			b.pixel[w][h] = red | green | blue | alpha;
		}
}
//findClosest
//Parameter: integer n, 
//Description:	takes in R,G,B,A between 0-255 and finds 
//		closest # of 0,128, and 255
int findClosest(int n){
	int t=n;
	abs(t-128)<t && abs(t-128) < abs(t-255)? n=128:n=0;
	abs(t-255)>=t && abs(t-255) >= abs(t-128)?:n=255;
	return n;
}

/* grayscale
 * Parameter: Bitmap& b
 * Description: The second ﬁlter is gray-scale. We get a gray color when red, green and blue all have the same value. 
 * To convert an image to gray-scale we average the red, green and blue component for each pixel
 */
void grayscale(Bitmap& b) {
	//loop through each pixel
	for(int w=0;w<b.getWidth();w++)
		for(int h=0;h<b.getHeight();h++){
			//grab R,G,B,A component of each pixel
			unsigned int red = (b.pixel[w][h] & b.mask1)>>SHIFT3BYTES;
			unsigned int green = (b.pixel[w][h] & b.mask2)>>SHIFT2BYTES;
			unsigned int blue = (b.pixel[w][h] & b.mask3)>>SHIFT1BYTE;
			unsigned int alpha = b.pixel[w][h] & 255;

			//average R, G, B
			unsigned int avg = (red + green + blue) / 3;

			//bitshift colors back to original position;
			red = avg<<SHIFT3BYTES;
			green = avg<<SHIFT2BYTES;
			blue = avg<<SHIFT1BYTE;

			//set pixel back to nwly compiled color
			b.pixel[w][h] = red | green | blue | alpha;
		}
}
/* pixelate
 * Parameter: Bitmap& b,
 * Description: The third ﬁlter is pixelate. Divide your image into 16x16 blocks. 
 * For each block compute the average color, and make the entire block that color.
 */
void pixelate(Bitmap& b) {
	//loop through each pixel
	for(int w =0;w<b.getWidth();w+=16){
		for(int h=0;h<b.getHeight();h+=16){

			unsigned int avgR = 0;
			unsigned int avgG = 0;
			unsigned int avgB = 0;
			unsigned int alpha = b.pixel[w][h] & 255;

			for(int i = w;i < w + 16;i++)
				for(int j = h;j< h+16;j++){
					avgR += (b.pixel[i][j] & b.mask1)>>SHIFT3BYTES;
					avgG += (b.pixel[i][j] & b.mask2)>>SHIFT2BYTES;
					avgB += (b.pixel[i][j] & b.mask3)>>SHIFT1BYTE;

				}
			avgR = (avgR/255)<<SHIFT3BYTES;
			avgG = (avgG/255)<<SHIFT2BYTES;
			avgB = (avgB/255)<<SHIFT1BYTE;

			for(int i=w;i<w+16;i++)
				for(int j=h;j<h+16;j++)
					b.pixel[i][j] = avgR | avgG | avgB | alpha;
		}
	}
}

/* blur
 * Parameter: Bitmap& b,
 * Description: The ﬁnal ﬁlter is Gaussian blurring. This is a little more involved. For each pixel take the 5x5 block surrounding it. 
 * This block will contain 25 pixels. For each component color, multiply all of the values by Gaussian matrix and add the values together.
 */
void blur(Bitmap& b) {
	int gMatrix[5][5] = {{1,4,6,4,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,4,1}};
	uint32_t temp[b.getWidth()][b.getHeight()];

	for(int w = 0; w < b.getWidth(); w++)
		for(int h=0; h < b.getHeight(); h++){
			//current pixel is at gMatrix[2][2]
			//multiply by gmatrix[i][j]/256 and make sure value is 0-4 inclusive (use %)
			//take w subtract 2, take h subtract 2
			//now start going through matrix at normal as long as current "w" is > 0 AND current "h" is > 0 AND "w" < 1280 AND "h" < 720
			unsigned int sumR = 0;
			unsigned int sumG = 0;
			unsigned int sumB = 0;
			unsigned int alpha = b.pixel[w][h] & 255;

			for(int w2=w-2;w2<w+3;w2++)
				for(int h2 = h-2; h2 < h + 3; h2++){
					if(w2 >= 0 && h2 >= 0 && w2 < 1280 && h2 < 720){
						unsigned int red = (b.pixel[w2][h2] & b.mask1)>>SHIFT3BYTES;
						unsigned int green = (b.pixel[w2][h2] & b.mask2) >>SHIFT2BYTES;
						unsigned int blue = (b.pixel[w2][h2]& b.mask3)>>SHIFT1BYTE;

						red = (red * gMatrix[(w2+2)-w][(h2+2)-h]);
						green = (green * gMatrix[(w2+2)-w][(h2+2)-h]);
						blue = (blue * gMatrix[(w2+2)-w][(h2+2)-h]);

						sumR += red;
						sumG += green;
						sumB += blue;
					}	
				}
			sumR/=256;
			sumG/=256;
			sumB/=256;

			sumR = sumR<<SHIFT3BYTES;
			sumG = sumG<<SHIFT2BYTES;
			sumB = sumB<<SHIFT1BYTE;

			temp[w][h] = sumR | sumG | sumB | alpha;
		}
	for(int w = 0; w < b.getWidth(); w++)
		for(int h = 0; h < b.getHeight(); h++)
			b.pixel[w][h] = temp[w][h];
}
void rot90(Bitmap& b) {/* unimplemented */}
/* rot180
 * Parameter: Bitmap& b,
 * Description: rotate bitmap transform around 'Y' axis by 180 degrees
 */
void rot180(Bitmap& b) {
	uint32_t pixelFlip[b.getWidth()][b.getHeight()];
	                                                 
	for(int w = 0; w < b.getWidth(); w++)
		for(int h = 0; h < b.getHeight(); h++){
			pixelFlip[b.getWidth()-1-w][b.getHeight()-1-h] = b.pixel[w][h];
		}
	for(int w = 0; w < b.getWidth(); w++)
		for(int h = 0; h < b.getHeight(); h++)
			b.pixel[w][h] = pixelFlip[w][h];
	
}
void rot270(Bitmap& b) {/* unimplemented */}
void flipv(Bitmap& b) {/* unimplemented */}
void fliph(Bitmap& b) {/* unimplemented */}
void flipd1(Bitmap& b) {
}
void flipd2(Bitmap& b) {/* unimplemented */}
void scaleUp(Bitmap& b) {/* unimplemented */}
void scaleDown(Bitmap& b) {/* unimplemented */}

istream& operator>>(istream& in, Bitmap& b) {

	//Type of bitmap	
	in >> b.tag[0] >> b.tag[1];

	if(b.tag[0]!='B' && b.tag[1] != 'M')
		throw; 
	//Size of BMP file
	in.read((char*)&b.size,4);

	//Garbage/Offset
	in.read((char*)&b.offset,8);

	//Read in second header
	for(int i = 0; i<31;i++)
		in.read((char*)&b.h2[i],4);

	//set width
	b.setWidth(b.h2[1]);

	//set height
	b.setHeight(b.h2[2]);

	b.mask1 = b.h2[10];
	b.mask2 = b.h2[11];
	b.mask3 = b.h2[12];
	b.mask4 = b.h2[13];

	cout<<b.mask1<<" "<<b.mask2<<" "<<b.mask3<<" "<<b.mask4;

	//dynamically allocate 2D-pixel array to width and height (in pixels) of .bmp file
	b.pixel = new uint32_t*[b.getWidth()];
	for(int i = 0; i < b.getWidth(); i++)
		b.pixel[i] = new uint32_t[b.getHeight()];

	//read in pixels
	for(int h=0;h<b.getHeight();h++)
		for(int w=0;w<b.getWidth();w++)
			in.read((char*)&b.pixel[w][h],4);

	return in;
}

ostream& operator<<(ostream& out, const Bitmap& b) {
	//Type of bitmap	
	out<<b.tag[0]<<b.tag[1];
	//size of bitmap
	out.write((char*)&b.size,4);
	//fill next 8 up with garbage
	out.write((char*)&b.offset,8);

	//write second header
	for(int i = 0;i < 31; i++)
		out.write((char*)&b.h2[i],4);

	//write pixels
	for(int h = 0;h<b.getHeight();h++)
		for(int w=0;w<b.getWidth();w++)
			out.write((char*)&b.pixel[w][h],4);

	return out;
}

Bitmap::Bitmap() {/* unimplemented */}
