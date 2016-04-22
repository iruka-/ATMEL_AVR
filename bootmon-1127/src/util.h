
void	memdump(void *ptr,int len,int off);
void 	UsbBench(int cnt,int psize);
void	UsbDump(int adr,int arena,int size);
void 	UsbPoke(int adr,int arena,int data,int mask);
void 	UsbPoke_b(int adr,int arena,int bit,int mask);
int 	UsbPeek(int adr,int arena);
int UsbSetPoll_slow (int adr, int arena);
int UsbPoll_slow (void);
int UsbSetPoll (int adr, int arena);
int UsbPoll (void);
void UsbCheckPollCmd (void);
int 	UsbRead(int adr,int arena,uchar *buf,int size);
int 	UsbBootTarget(int adr,int boot);
int 	UsbEraseTargetROM(int adr,int size);
int		UsbInit(int verbose,int enable_bulk,char *serial);	
int		UsbExit(void);

