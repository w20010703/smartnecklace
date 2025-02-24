__IO double		SysTimer;
__IO double		PreSysTimer;

bool ADC1_completed;
bool ADC2_completed;
bool ADC3_completed;

uint16_t ADC1Value[CAPTURE_BUFFER_SIZE];
uint16_t ADC2Value[CAPTURE_BUFFER_SIZE];
uint16_t ADC3Value[CAPTURE_BUFFER_SIZE];

uint16_t ADC1Value_stored[CAPTURE_BUFFER_SIZE];
uint16_t ADC2Value_stored[CAPTURE_BUFFER_SIZE];
uint16_t ADC3Value_stored[CAPTURE_BUFFER_SIZE];

uint32_t capture_index;

bool first_run;
bool adc_capture_completed_flag;

FATFS	FatFs;
FIL		fil;