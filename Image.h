#define ALIGN_L 0
#define ALIGN_R 1
#define ALIGN_C 2

typedef struct
{
   uint8_t r;
   uint8_t g;
   uint8_t b;
} Color;

class Image
{
public:
   Image(uint16_t w, uint16_t h);
   Image(char* fname);
   ~Image();

   void setPixel(uint16_t x, uint16_t y);
   void setPixel(uint16_t x, uint16_t y, Color c);
   void setPixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b);

   Color getPixel(uint16_t x, uint16_t y);

   void setColor(uint8_t r, uint8_t g, uint8_t b);
   void drawLine(uint16_t x, uint16_t y, uint16_t xx, uint16_t yy);
   void print(uint16_t x, uint16_t y, char* str, uint8_t align = ALIGN_L);

   void save(char* fname);
   
private:
   Color* data;
   int width;
   int height;
   Color c;
};

