#include "StdInc.h"
#include "DataSource.h"

//dump source

int main(int argc, char *argv[])
{
   assert(argc == 2);

   DataSource ds(argv[1]);

   for(;;)
   {
      time_t tim = ds.getCurrentTimestamp();
      printf("%s%f\n\n", asctime(gmtime(&tim)), ds.getCurrentValue());
      if(ds.getCurrentTimestamp() == ds.getLastTimestamp()) break;
      ds.next();
   }
}

