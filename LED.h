#define LDaddrs1 (0x70)  //LEDドライバーHT16K33　アドレス

extern void LED_setup();
extern void LED_Driver_Setup(uint8_t LD_addrs, uint8_t on_off);
extern void LED_Driver_Blink(uint8_t LD_addrs, uint8_t on_off, uint8_t blink_Hz);
extern void LED_Driver_Brightness(uint8_t LD_addrs, uint8_t brightness);
extern void LED_Driver_DisplayInt(uint8_t LD_addrs);
extern void LED_Ada88_BiColor_cnv(uint8_t* Bdot1, uint8_t* Bdot2);
extern void LED_SHIFT(uint8_t* Bdot1, uint8_t* Bdot2);
extern void LED_Driver_DisplayOutput(uint8_t LD_addrs, uint8_t* DotB1, uint8_t* DotB2);

extern void disp_temp(float ftmp);    //温度の表示
extern void disp_humid(float fhumi);  //湿度の表示

extern uint8_t swhumid;   //0:温度計 1:湿度計
extern short brightness;  //ブライトネスの値
