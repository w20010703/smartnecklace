extern __IO double		SysTimer;
extern __IO double		PreSysTimer;

extern bool ADC1_completed;
extern uint16_t ADC1Value[CAPTURE_BUFFER_SIZE];
extern uint16_t ADC1Value_stored[CAPTURE_BUFFER_SIZE];
extern uint32_t capture_index;

extern bool first_run;
extern bool adc_capture_completed_flag;

extern FATFS	FatFs;
extern FIL		fil;