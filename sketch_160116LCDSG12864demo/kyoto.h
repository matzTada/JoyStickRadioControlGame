// kyoto.h

#ifndef kyoto_h
#define kyoto_h

PROGMEM const uint8_t BMP_kyoto[] = {
	43, // width
	29, // height
	0x00,0x00,0x00,0x00,0x00,0x20,0x20,0xb0,0xf8,0xf8,0xfc,0xff,0xfc,0xf8,0xf8,0xb0,
	0x20,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0xe8,0x28,0x28,
	0x28,0x2e,0x28,0x28,0x28,0xe8,0x08,0x00,0x00,0x00,0x00, // page0
	0x00,0x00,0x00,0x20,0x22,0x32,0xb3,0xfb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfb,
	0xb3,0x32,0x22,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x11,0x09,0x05,
	0x21,0x3f,0x01,0x05,0x09,0x11,0x20,0x20,0x00,0x00,0x00, // page1
	0x00,0x20,0x22,0x22,0x33,0x33,0xfb,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xfb,0x33,0x33,0x22,0x22,0x20,0x00,0x00,0x00,0x00,0x00,0x90,0x50,0xf2,0x5f,0x5a,
	0xd6,0x10,0xfe,0x22,0x32,0x4e,0x80,0x00,0x00,0x00,0x00, // page2
	0x00,0x00,0x00,0x00,0x1c,0x1c,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,
	0x1f,0x1c,0x1c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f,0x09,0x09,
	0x0f,0x00,0x1f,0x02,0x02,0x02,0x01,0x00,0x00,0x00,0x00  // page3
};
#endif
