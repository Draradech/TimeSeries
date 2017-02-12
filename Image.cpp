#include "StdInc.h"
#include "Image.h"
#include "Font.h"

Image::Image(uint16_t w, uint16_t h)
{
   int length;

   width = w;
   height = h;
   length = width * height * sizeof(Color);
   data = (Color *)malloc(length);
   memset(data, 0, length);
   c.r = c.g = c.b = 0;
}

Image::Image(char* fname)
{
   FILE* f = fopen((fname), "rb");
   char buffer[16];
   int index = 0;
   int field = 0;
   int isP5 = 0;
   int length;
   unsigned char c = ' ';
   while(field < 4)
   {
      do
      {
         if(c == '#') while(c = fgetc(f), c != '\n');
      } while(c = fgetc(f), isspace(c) || c == '#');
      index = 0;
      do
      {
         buffer[index++] = c;
      } while(c = fgetc(f), !isspace(c) && c != '#' && index < 15);
      buffer[index] = 0;
      switch(field)
      {
         case 0:
            if (strcmp(buffer, "P5") == 0) isP5 = 1;
            else if (strcmp(buffer, "P6") == 0) isP5 = 0;
            else fprintf(stderr, "image format \"%s\" unsupported (not P5 or P6)\n", buffer), exit(1);
            break;
         case 1:
            width = atoi(buffer);
            break;
         case 2:
            height = atoi(buffer);
            break;
         case 3:
            index = atoi(buffer);
            if (index != 255) fprintf(stderr, "image format unsupported (not 255 values per channel)\n"), exit(1);
            break;
      }
      field++;
   }

   length = width * height * sizeof(Color);
   data = (Color *)malloc(length);
   memset(data, 0, length);
   this->c.r = this->c.g = this->c.b = 0;
   
   if (isP5)
   {
      int length = width * height;
      for(index = 0; index < length; ++index)
      {
         data[index].r = data[index].g = data[index].b = fgetc(f);
      }
   }
   else
   {
      fread(data, sizeof(Color), width * height, f);
   }
   fclose(f);
}


Image::~Image()
{
   free(data);
}

void Image::setPixel(uint16_t x, uint16_t y)
{
   data[x + y * width] = c;
}

void Image::setPixel(uint16_t x, uint16_t y, Color c)
{
   data[x + y * width] = c;
}

void Image::setPixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b)
{
   data[x + y * width].r = r;
   data[x + y * width].g = g;
   data[x + y * width].b = b;
}

Color Image::getPixel(uint16_t x, uint16_t y)
{
   return data[x + y * width];
}

void Image::setColor(uint8_t r, uint8_t g, uint8_t b)
{
   c.r = r; c.g = g; c.b = b;
}

void Image::drawLine(uint16_t x, uint16_t y, uint16_t xx, uint16_t yy)
{
   int x0 = x, y0 = y, x1 = xx, y1 = yy;
   int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
   int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
   int err = dx + dy, e2 = 0;
   
   for(;;)
   {
      setPixel(x0, y0);
      if (x0 == x1 && y0 == y1) break;
      e2 = 2 * err;
      if (e2 >= dy) {err += dy; x0 += sx;}
      if (e2 <= dx) {err += dx; y0 += sy;}
   }
}

void Image::print(uint16_t x, uint16_t y, char* str, uint8_t align)
{
   char c;

   if(align == ALIGN_R) x -= 6 * strlen(str);
   if(align == ALIGN_C) x -= 3 * strlen(str) - 1;

   while((c = *(str++)))
   {
      int i = c - 32;
      if(i >= 0 && i < 96)
      {
         for(int dx = 0; dx < 5; ++dx)
         {
            for(int dy = 0; dy < 8; ++dy)
            {
               if(font[i][dx] & (1 << dy))
               {
                  setPixel(x + dx, y + dy - 6);
               }
            }
         }
      }
      x += 6;
   }
}

void Image::save(char* fname)
{
   FILE* f = fopen((fname), "wb");
   fprintf(f, "P6 %d %d 255\n", width, height);
   fwrite(data, sizeof(Color), width * height, f);
   fclose(f);
}

