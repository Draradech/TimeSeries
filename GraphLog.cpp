#include "StdInc.h"
#include "DataSource.h"
#include "Image.h"

//graphlog width xmax xres xminor xmajor height ymin ymax source1 name color ... sourceN name color target
//graphlog 1440 $(date +%s) 1m 30m 2h 500 100.0 20000.0 power.1m.log Leistung ff0000 temp.1d.pnm

#define minorgrid 60, 60, 60
#define majorgrid 90, 90, 90

uint32_t readTime(char* str)
{
   uint32_t time = atoi(str);
   switch(str[strlen(str)-1])
   {
      case 'm':
         time *= 60;
         break;
      case 'h':
         time *= 60 * 60;
         break;
      case 'd':
         time *= 24 * 60 * 60;
         break;
      case 'w':
         time *= 7 * 24 * 60 * 60;
         break;
   }
   return time;
}

int main(int argc, char *argv[])
{
   assert(argc >= 13);
   assert((argc + 2) % 3 == 0);

   uint16_t ml = 35;
   uint16_t mr = 10;
   uint16_t mt = 20;
   uint16_t mb = 20;

   uint16_t width = atoi(argv[1]);
   uint32_t xmax = atoi(argv[2]);
   uint32_t xres = readTime(argv[3]);
   uint32_t xminor = atoi(argv[4]);
   char xminormode = argv[4][strlen(argv[4])-1];
   uint32_t xmajor = atoi(argv[5]);
   char xmajormode = argv[5][strlen(argv[5])-1];
   uint16_t height = atoi(argv[6]);
   double ymin = atof(argv[7]);
   double ymax = atof(argv[8]);

   uint8_t numSources = (argc - 10) / 3;
   uint16_t legendLen = 0;
   for(uint8_t i = 0; i < numSources; ++i)
   {
      uint8_t len = strlen(argv[10 + 3 * i]);
      if(len > 0) legendLen += len + 2;
   }
   legendLen -= 2;

   Image img(ml + width + mr, mt + height + mb);

   // adjust xmax to match resolution
   xmax -= xmax % xres;

   ////////////////////////////
   // Draw grid and numbers  //
   ////////////////////////////
   char buf[16];
   double lymin = log10(ymin);
   double lymax = log10(ymax);
   double lyres = height/(lymax - lymin);
   for(double y = pow(10.0,(int)lymin); y <= ymax; y *= 10.0)
   {
      for(double y2 = 2.0 * y; y2 < 10.0 * y; y2 += y)
      {
         double ly = log10(y2);
         double py = ((ly - lymin) * lyres);
         if(py >= 0.0 && py <= height)
         {
            img.setColor(minorgrid);
            img.drawLine(ml, mt + height - py, ml + width, mt + height - py);
         }
      }
   }
   int xminorcurrent = -1;
   int xmajorcurrent = -1;
   for(int16_t x = -1; x <= width; ++x)
   {
      bool printxmin = false;
      bool printxmaj = false;
      uint32_t tx = xmax + (- width + x) * xres;
      time_t txt = tx;
      struct tm *tm = localtime(&txt);
      switch(xminormode)
      {
         case 'm':
         {
            if(tm->tm_min % xminor == 0 && tm->tm_min != xminorcurrent)
            {
               xminorcurrent = tm->tm_min;
               printxmin = true;
            }
            break;
         }
         case 'h':
         {
            if(tm->tm_hour % xminor == 0 && tm->tm_hour != xminorcurrent)
            {
               xminorcurrent = tm->tm_hour;
               printxmin = true;
            }
            break;
         }
         case 'd':
         {
            if(((tm->tm_mday % xminor == 0 && xminor != 7) || (tm->tm_wday == 1 && xminor == 7)) && tm->tm_mday != xminorcurrent)
            {
               xminorcurrent = tm->tm_mday;
               printxmin = true;
            }
            break;
         }
      }
      switch(xmajormode)
      {
         case 'h':
         {
            if(tm->tm_hour % xmajor == 0 && tm->tm_hour != xmajorcurrent)
            {
               xmajorcurrent = tm->tm_hour;
               printxmaj = true;
            }
            break;
         }
         case 'd':
         {
            if(((tm->tm_mday % xmajor == 0 && xmajor != 7) || (tm->tm_wday == 1 && xmajor == 7)) && tm->tm_mday != xmajorcurrent)
            {
               xmajorcurrent = tm->tm_mday;
               printxmaj = true;
            }
            break;
         }
         case 'M':
         {
            if((tm->tm_mon + 1) % xmajor == 0 && (tm->tm_mon + 1) != xmajorcurrent)
            {
               xmajorcurrent = (tm->tm_mon + 1);
               printxmaj = true;
            }
            break;
         }
      }
      if(x >= 0)
      {
         if(printxmaj)
         {
            img.setColor(majorgrid);
            img.drawLine(ml + x, mt, ml + x, mt + height);
            sprintf(buf, "%02d", xmajorcurrent);
            img.print(ml + x, mt + height + 10, buf, ALIGN_C);
         }
         else if(printxmin)
         {
            img.setColor(minorgrid);
            img.drawLine(ml + x, mt, ml + x, mt + height);
         }
      }
   }
   for(double y = pow(10.0,(int)lymin); y <= ymax; y *= 10.0)
   {
      double ly = log10(y);
      double py = ((ly - lymin) * lyres);
      if(py >= 0.0 && py <= height)
      {
         img.setColor(majorgrid);
         img.drawLine(ml, mt + height - py, ml + width, mt + height - py);
         sprintf(buf, "%.0lf", y);
         img.print(ml - 3, mt + height - py + 3, buf, ALIGN_R);
      }
   }

   ////////////////////////////
   // Draw graphs and legend //
   ////////////////////////////

   uint16_t legendPos = width / 2 - legendLen / 2 * 6;
   for(uint8_t i = 0; i < numSources; ++i)
   {
      DataSource ds(argv[9 + 3 * i]);
      uint32_t c = strtol(argv[11 + 3 * i], NULL, 16);
      uint8_t r = (c & 0xff0000) >> 16;
      uint8_t g = (c & 0xff00) >> 8;
      uint8_t b = (c & 0xff);
      img.setColor(r, g, b);
      uint8_t len = strlen(argv[10 + 3 * i]);
      if(len > 0)
      {
         img.print(legendPos + ml, mt - 4, argv[10 + 3 * i]);
         legendPos += (len + 2) * 6;
      }
      uint32_t txOld = 0;
      double valOld = 0;
      for(uint16_t x = 0; x <= width; ++x)
      {
         uint32_t tx = xmax + (- width + x) * xres;
         ds.seekTimestamp(tx);
         double val = ds.getCurrentValue();
         if (  (std::isfinite(val))
            && (val >= ymin)
            && (val <= ymax)
            && (  (tx - ds.getCurrentTimestamp() < xres)
               || (  (ds.isPeriodic())
                  && (tx - ds.getCurrentTimestamp() < ds.getPeriod())
                  )
               )
            )
         {
            //printf("%u: %lf\n", ds.getCurrentTimestamp(), val);
            double plval = (log10(val) - lymin) * lyres;
            double plvalOld = (log10(valOld) - lymin) * lyres;
            if(tx - txOld < 2 * xres)
            {
               img.drawLine(ml + x, mt + height - plvalOld, ml + x, mt + height - plval);
            }
            else
            {
               img.setPixel(ml + x, mt + height - plval);
            }
            txOld = tx;
            valOld = val;
         }
      }
   }

   img.save(argv[argc - 1]);
   return 0;
}

