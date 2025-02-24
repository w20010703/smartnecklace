#define SAMPLE_FREQ				3000
#define ADC_PRESCALE	        10
#define ADC_CNT					(int)((84000000/ADC_PRESCALE)/SAMPLE_FREQ)

#define CAPTURE_BUFFER_SIZE		3000	// < 9000
#define CAPTURE_PERIOD_SECS		10
#define CAPTURE_POINTS			(uint32_t)(CAPTURE_PERIOD_SECS*SAMPLE_FREQ)

#define DONT_DISCARD __attribute__((used))