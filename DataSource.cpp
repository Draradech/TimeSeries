#include "StdInc.h"
#include "DataSource.h"

DataSource::DataSource(char* fname)
{
   void *addr;
   int ret;
   struct stat sb;

   fd = open(fname, O_RDONLY);
   assert(fd != -1);

   ret = fstat(fd, &sb);
   assert(ret != -1);
   length = sb.st_size / sizeof(TRecord);
   assert(length * sizeof(TRecord) == (uint32_t)sb.st_size);
   assert(length > 10);

   addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   assert(addr != MAP_FAILED);

   content = (TRecord *)addr;
   index = 0;
   firstTimestamp = content[0].timestamp;
   lastTimestamp = content[length - 1].timestamp;
   
   periodic = true;
   period = content[1].timestamp - content[0].timestamp;
   
   if (  (content[length - 1].timestamp != content[0].timestamp + (length - 1) * period)
      || (content[length / 2].timestamp != content[0].timestamp + (length / 2) * period)
      )
   {
      periodic = false;
      period = 0;
   }
}

DataSource::~DataSource()
{
   munmap(content, length * sizeof(TRecord));
   close(fd);
}

bool DataSource::isPeriodic()
{
   return periodic;
}

uint32_t DataSource::getPeriod()
{
   return period;
}

uint32_t DataSource::getFirstTimestamp()
{
   return firstTimestamp;
}

uint32_t DataSource::getLastTimestamp()
{
   return lastTimestamp;
}

uint32_t DataSource::getLength()
{
   return length;
}

uint32_t DataSource::getCurrentTimestamp()
{
   return content[index].timestamp;
}

uint32_t DataSource::getCurrentIndex()
{
   return index;
}

float DataSource::getCurrentValue()
{
   return content[index].value;
}

void DataSource::seekIndex(uint32_t index)
{
   this->index = MIN(index, length - 1);
}

void DataSource::seekTimestamp(uint32_t timestamp)
{
   if(content[index].timestamp == timestamp) return;
   next();
   if(content[index].timestamp == timestamp) return;
   
   if(periodic)
   {
      if(timestamp <= firstTimestamp)
      {
         index = 0;
      }
      else if(timestamp >= lastTimestamp)
      {
         index = length - 1;
      }
      else
      {
         index = (timestamp - firstTimestamp) / period;
      }
   }
   else
   {
      int32_t l = 0;
      int32_t r = length - 1;
      int32_t m;
      for(;;)
      {
         m = (l + r) >> 1; // NOT divide by two. div by two rounds towards zero, this rounds down (on most compilers and architectures)
         if(l > r)
         {
            // now pointing to the too small timestamp (instead of the too big one)
            break; 
         }
         if(content[m].timestamp < timestamp)
         {
            l = m + 1;
         }
         else if(content[m].timestamp > timestamp)
         {
            r = m - 1;
         }
         else
         {
            break; // found exactly
         }
      }
      index = MIN((uint32_t)m, length - 1);
   }
}

void DataSource::next()
{
   index = MIN(index + 1, length - 1);
}

