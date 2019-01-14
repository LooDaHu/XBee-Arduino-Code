
#define ID_REQUEST_HEADER 0x0A
//#define ID_RESPONSE_HEADER 0x0B
#define MSG_HEADER 0x0C
#define ACCEPT_REQUEST_HEADER 0xEF
#define REFUSE_REQUEST_HEADER 0xFF
#define MAX_CAPACITY 2

const uint8_t ID_IDENTIFIER = 'A';//NEED TO BE CHANGE FOR EVERY NODE
const uint32_t SH = 0x0013A200;  //High 32 bit of MAC address
const uint32_t SL = 0x41876FFE; //Low 32 bit of MAC address 

uint8_t level = 0;
uint8_t current_capacity = MAX_CAPACITY;
uint8_t temp_check_identifier = 'A';
uint8_t discounted_count = 0;
uint8_t no_recieve_count = 0;
//uint8_t SL_seg1 = SL >> 24;
//uint8_t SL_seg2 = SL >> 16;
//uint8_t SL_seg3 = SL >> 8;
//uint8_t SL_seg4 = SL;
uint8_t SL_seg1 = 0x41;
uint8_t SL_seg2 = 0x87;
uint8_t SL_seg3 = 0x6F;
uint8_t SL_seg4 = 0xFE;
uint8_t level2[2] = { 0x00,0x02 };
uint8_t level1[2] = { 0x00,0x01 };
uint8_t the_buffer[3];

//class Test
//{
//  public:
//  void test();  
//};
