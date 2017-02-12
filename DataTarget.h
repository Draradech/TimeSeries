class DataTarget
{
public:
   DataTarget(char *fname, uint32_t period);
   ~DataTarget();
   
   uint32_t getPeriod();
   uint32_t getNextTimestamp();
   void     append(uint32_t timestamp, float value);
   
private:
   uint32_t period;
   uint32_t nextTimestamp;
   int      fd;
};

