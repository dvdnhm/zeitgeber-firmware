#define C2_S1_WIDTH	16
#define C2_S1_HEIGHT	23
#define C2_S1_SIZE	(C2_S1_WIDTH * C2_S1_HEIGHT * 2)
uint16 __eds__ c2_s1_bytes[736] __attribute__((space(prog))) = {
	0x0000,0x0000,0x0000,0x3186,0x528a,0x8410,0x8410,0x8410,0x8410,0x738e,0x3186,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x738e,0xffff,0xffff,0xffff,0xffff,0xc618,0xc618,0xd69a,0xffff,0xffff,0xe71c,0x4208,0x0000,0x0000,0x0000,
	0x0000,0x528a,0x738e,0x4208,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x528a,0xe71c,0xffff,0x4208,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xf79e,0xc618,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x9492,0xffff,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x528a,0xffff,0x2104,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x8410,0xffff,0x2104,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xb596,0xffff,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xf79e,0xd69a,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x9492,0xffff,0x630c,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x3186,0xffff,0xc618,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x3186,0xf79e,0xf79e,0x1082,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xd69a,0xf79e,0x3186,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xd69a,0xf79e,0x3186,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1082,0xd69a,0xffff,0x4208,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xd69a,0xffff,0x630c,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0xa514,0xffff,0x630c,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0x0000,0xa514,0xffff,0x630c,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0x0000,0xa514,0xffff,0x630c,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x0000,0xa514,0xffff,0x9492,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
	0x0000,0x630c,0xffff,0xd69a,0x4208,0x4208,0x4208,0x4208,0x4208,0x4208,0x4208,0x4208,0x4208,0x4208,0x3186,0x0000,
	0x0000,0xc618,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xc618,0x0000,
	0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
};
const image_t img_c2_s1 = {c2_s1_bytes, C2_S1_WIDTH, C2_S1_HEIGHT};