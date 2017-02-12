#include "StdInc.h"
#include "DataSource.h"
#include "DataTarget.h"

//aggregate method required source1 source2 ... sourceN target

int main(int argc, char *argv[])
{
   assert(argc >= 6);

   uint8_t method = 0;
   if(strcmp(argv[1], "avg") == 0)
   {
      method = METHOD_AVG;
   }
   else if(strcmp(argv[1], "min") == 0)
   {
      method = METHOD_MIN;
   }
   else if(strcmp(argv[1], "max") == 0)
   {
      method = METHOD_MAX;
   }
   assert(method != 0);
   
   uint16_t required = atoi(argv[2]);
   assert(required > 0);

   uint8_t numSources = argc - 4;
   uint16_t period = 0;
   uint32_t tsBegin = 0;
   uint32_t tsEnd = UINT32_MAX;
   DataSource **ds = (DataSource **) malloc(numSources * sizeof(DataSource *));
   for(uint8_t i = 0; i < numSources; ++i)
   {
      ds[i] = new DataSource(argv[3 + i]);
      assert(ds[i]->isPeriodic());
      if(period == 0)
      {
         period = ds[i]->getPeriod();
      }
      else
      {
         assert(period == ds[i]->getPeriod());
      }
      tsBegin = MAX(tsBegin, ds[i]->getFirstTimestamp());
      tsEnd = MIN(tsEnd, ds[i]->getLastTimestamp());
   }
   
   //printf("open target\n");
   DataTarget dt(argv[argc - 1], period);
   if(dt.getNextTimestamp() != 0)
   {
      tsBegin = dt.getNextTimestamp();
   }
   
   //printf("begin aggregation\n");
   for(uint32_t ts = tsBegin; ts <= tsEnd; ts += period)
   {
      uint16_t samples = 0;
      float aggr = (method == METHOD_MIN) ? INFINITY : (method == METHOD_MAX) ? -INFINITY : 0.0;
      //printf("%u:\n", ts);
      for(uint8_t i = 0; i < numSources; ++i)
      {
         ds[i]->seekTimestamp(ts);
         if(ts == ds[i]->getCurrentTimestamp())
         {
            float value = ds[i]->getCurrentValue();
            //printf("   s%i: %lf\n", i, value);
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
         }
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
      //printf("   agr: %lf\n\n", aggr);
      dt.append(ts, aggr);
   }

   return 0;
}

