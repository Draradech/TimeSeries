#include "StdInc.h"
#include "DataSource.h"
#include "DataTarget.h"

//resample samplerate method required source target

int main(int argc, char *argv[])
{
   assert(argc == 6);

   uint32_t samplerate = atoi(argv[1]);
   switch(argv[1][strlen(argv[1])-1])
   {
      case 'm':
         samplerate *= 60;
         break;
      case 'h':
         samplerate *= 60 * 60;
         break;
      case 'd':
         samplerate *= 24 * 60 * 60;
         break;
      case 'w':
         samplerate *= 7 * 24 * 60 * 60;
         break;
   }
   assert(samplerate != 0);

   uint8_t method = 0;
   if(strcmp(argv[2], "avg") == 0)
   {
      method = METHOD_AVG;
   }
   else if(strcmp(argv[2], "min") == 0)
   {
      method = METHOD_MIN;
   }
   else if(strcmp(argv[2], "max") == 0)
   {
      method = METHOD_MAX;
   }
   assert(method != 0);

   uint16_t required = atoi(argv[3]);
   assert(required > 0);

   if(access(argv[4], F_OK) != 0) return -1;
   DataSource ds(argv[4]);
   if((ds.getLastTimestamp() - ds.getCurrentTimestamp()) / samplerate <= 10) return -1;

   DataTarget dt(argv[5], samplerate);

   uint32_t nextEntry = dt.getNextTimestamp();
   if(nextEntry == 0)
   {
      nextEntry = ds.getCurrentTimestamp();
      nextEntry /= samplerate;
      nextEntry *= samplerate;
   }
   else
   {
      ds.seekTimestamp(nextEntry);
      if(ds.getCurrentTimestamp() < nextEntry) ds.next();
   }

   // Now ds is at first sample for nextEntrys time period (if there are samples), dt is prepared to recieve samples
   while(ds.getLastTimestamp() >= nextEntry + samplerate)
   {
      uint16_t samples = 0;
      float aggr = (method == METHOD_MIN) ? INFINITY : (method == METHOD_MAX) ? -INFINITY : 0.0;
      while(ds.getCurrentTimestamp() < nextEntry + samplerate)
      {
         float value = ds.getCurrentValue();
         //printf("%u: %lf\n", ds.getCurrentTimestamp(), value);
         if(std::isfinite(value))
         {
            switch(method)
            {
               case METHOD_AVG:
                  aggr += value;
                  break;
               case METHOD_MIN:
                  aggr = MIN(aggr, value);
                  break;
               case METHOD_MAX:
                  aggr = MAX(aggr, value);
                  break;
            }
            samples++;
         }
         ds.next();
      }
      if(samples >= required)
      {
         if(method == METHOD_AVG)
         {
            aggr /= samples;
         }
      }
      else
      {
         aggr = NAN;
      }
      //printf("aggr%u: %lf\n", nextEntry, aggr);
      dt.append(nextEntry, aggr);
      nextEntry += samplerate;
   }
}
