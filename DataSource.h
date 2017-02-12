typedef struct
{
   uint32_t timestamp;
   float    value;
} TRecord;

class DataSource
{
public:
   DataSource(char *fname);
   ~DataSource();
   
   bool     isPeriodic();
   uint32_t getPeriod();
   uint32_t getFirstTimestamp();
   uint32_t getLastTimestamp();
   uint32_t getLength();
   uint32_t getCurrentTimestamp();
   uint32_t getCurrentIndex();
   float    getCurrentValue();
   void     seekIndex(uint32_t index);
   void     seekTimestamp(uint32_t timestamp);
   void     next();
   
private:
   bool     periodic;
   uint32_t period;
   uint32_t firstTimestamp;
   uint32_t lastTimestamp;
   uint32_t length;
   uint32_t index;
   TRecord  *content;
   int      fd;
};

