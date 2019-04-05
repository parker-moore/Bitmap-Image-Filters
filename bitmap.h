#include <iostream>
#include <string.h>

using namespace std;
const int SHIFT3BYTES = 24;
const int SHIFT2BYTES = 16;
const int SHIFT1BYTE = 8;

class Bitmap
{
private:
    friend istream& operator>>(istream& in, Bitmap& b);
    friend ostream& operator<<(ostream& out, const Bitmap& b);
    char tag[2];
    uint32_t size;
    uint64_t offset;
    uint32_t width;
    uint32_t height;

    uint32_t h2[31];

public:
    Bitmap();
    //Bitmap(const Bitmap&);
    //Bitmap operator=(const Bitmap&);
    //Bitmap(Bitmap&&);
    //~Bitmap();
    void setWidth(uint32_t w){width=w;};
    uint32_t getWidth()const{return width;};

    void setHeight(uint32_t h){height=h;};
    uint32_t getHeight()const{return height;};

    uint32_t** pixel;

    uint32_t mask1;
    uint32_t mask2;
    uint32_t mask3;
    uint32_t mask4;
};

void cellShade(Bitmap& b);
int findClosest(int n);
void grayscale(Bitmap& b);
void pixelate(Bitmap& b);
void blur(Bitmap& b);
void rot90(Bitmap& b);
void rot180(Bitmap& b);
void rot270(Bitmap& b);
void flipv(Bitmap& b);
void fliph(Bitmap& b);
void flipd1(Bitmap& b);
void flipd2(Bitmap& b);
void scaleUp(Bitmap& b);
void scaleDown(Bitmap& b);
