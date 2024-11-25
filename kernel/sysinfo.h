struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes)
  uint64 nproc;     // number of process
};

extern uint64 cal_freemem(void);
extern uint64 cal_nproc(void);