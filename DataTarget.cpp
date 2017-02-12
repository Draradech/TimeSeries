#include "StdInc.h"
#include "DataSource.h"
#include "DataTarget.h"

DataTarget::DataTarget(char *fname, uint32_t period)
{
   if(access(fname, F_OK) == 0)
   {
      DataSource ds(fname);
      assert(ds.isPeriodic());
      assert(ds.getPeriod() == period);
      nextTimestamp = ds.getLastTimestamp() + period;
   }
   else
   {
      nextTimestamp = 0;
   }
   this->period = period;
   fd = open(fname, O_WRONLY | O_APPEND | O_CREAT, 0660);
   assert(fd != -1);
}

DataTarget::~DataTarget()
{
   close(fd);
}

uint32_t DataTarget::getPeriod()
{
   return period;
}

uint32_t DataTarget::getNextTimestamp()
{
   return nextTimestamp;
}

void DataTarget::append(uint32_t timestamp, float value)
{
   if(nextTimestamp != 0)
   {
      assert(timestamp == nextTimestamp);
   }
   nextTimestamp = timestamp + period;
   TRecord record;
   record.timestamp = timestamp;
   record.value = value;
   ssize_t ret = write(fd, &record, sizeof(TRecord));
   assert(ret != -1);
}

