#define FIRSTBIT 0x8000000000000000

#define ADD_BIT(block, from, position_from, position_to) \
if(((from << (position_from)) & FIRSTBIT) != 0) \
{ \
	block += (FIRSTBIT >> position_to); \
}

uint64_t key;
uint64_t data;
uint64_t a_key[16], next_key;

uint8_t const PC_1[56] =
{
	57, 49, 41, 33, 25, 17,  9,
	 1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27,
	19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	 7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29,
	21, 13,  5, 28, 20, 12,  4
};

uint8_t const PC_2[48] =
{
	14, 17, 11, 24,  1,  5,
	 3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

uint8_t const InitialPermutation[64] =
{
	58, 50, 42, 34, 26, 18, 10,  2,
	60, 52, 44, 36, 28, 20, 12,  4,
	62, 54, 46, 38, 30, 22, 14,  6,
	64, 56, 48, 40, 32, 24, 16,  8,
	57, 49, 41, 33, 25, 17,  9,  1,
	59, 51, 43, 35, 27, 19, 11,  3,
	61, 53, 45, 37, 29, 21, 13,  5,
	63, 55, 47, 39, 31, 23, 15,  7
};

uint8_t const FinalPermutation[64] =
{
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

uint8_t const DesExpansion[48] =
{
	32,  1,  2,  3,  4,  5,  4,  5,
	 6,  7,  8,  9,  8,  9, 10, 11,
	12, 13, 12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21, 20, 21,
	22, 23, 24, 25, 24, 25, 26, 27,
	28, 29, 28, 29, 30, 31, 32,  1
};

uint8_t const Rotations[16] =
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

uint8_t const DesSbox[8][4][16] =
{
	{
		{14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7},
		{ 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8},
		{ 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0},
		{15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},
	},
	{
		{15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10},
		{ 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5},
		{ 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15},
		{13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},
	},
	{
		{10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8},
		{13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1},
		{13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7},
		{ 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},
	},
	{
		{ 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15},
		{13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9},
		{10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4},
		{ 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},
	},
	{
		{ 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9},
		{14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6},
		{ 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14},
		{11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},
	},
	{
		{12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11},
		{10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8},
		{ 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6},
		{ 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},
	},
	{
		{ 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1},
		{13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6},
		{ 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2},
		{ 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},
	},
	{
		{13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7},
		{ 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2},
		{ 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8},
		{ 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11},
	},
};

uint8_t const Pbox[32] =
{
	16,  7, 20, 21, 29, 12, 28, 17,
	 1, 15, 23, 26,  5, 18, 31, 10,
	 2,  8, 24, 14, 32, 27,  3,  9,
	19, 13, 30,  6, 22, 11,  4, 25
};

void Permutation(bool initial)
{
	uint64_t data_temp;
	data_temp = 0;

	for(int ii = 0; ii < 64; ii++)
	{
		if(initial)
		{
			ADD_BIT(data_temp, data, InitialPermutation[ii] - 1, ii)
		}
		else
		{
			ADD_BIT(data_temp, data, FinalPermutation[ii] - 1, ii)
		}
	}
	data = data_temp;
}

void key_schedule(int round)
{
	uint64_t key_left = 0;
	uint64_t key_right = 0;
	uint64_t key_left_temp = 0;
	uint64_t key_right_temp = 0;

	next_key = 0;

	if(round == 0)
	{
		for(int ii = 0; ii < 56; ii++)
		{
			PORTB ^= 0x10;
			if(ii < 28)
			{
				ADD_BIT(key_left, a_key[round], PC_1[ii] - 1, ii)
			}
			else
			{
				ADD_BIT(key_right, a_key[round], PC_1[ii] - 1, ii % 28)
			}
		}
	}
	else
	{
		for(int ii = 0; ii < 56; ii++)
		{
			if(ii < 28)
			{
				ADD_BIT(key_left, a_key[round], ii, ii)
			}
			else
			{
				ADD_BIT(key_right, a_key[round], ii, ii % 28)
			}
		}
	}

	key_left_temp = Rotations[round] == 1 ? FIRSTBIT : 0xC000000000000000;
	key_right_temp = Rotations[round] == 1 ? FIRSTBIT : 0xC000000000000000;
	key_left_temp = (key_left & key_left_temp) >> (28 - Rotations[round]);
	key_right_temp = (key_right & key_right_temp) >> (28 - Rotations[round]);
	key_left_temp += (key_left << Rotations[round]);
	key_right_temp += (key_right << Rotations[round]);

	for(int ii = 0; ii < 56; ii++)
	{
		if(ii < 28)
		{
			ADD_BIT(next_key, key_left_temp, ii, ii)
		}
		else
		{
			ADD_BIT(next_key, key_right_temp, ii % 28, ii)
		}
	}

	a_key[round] = 0;
	for(int ii = 0; ii < 48; ii++)
	{
		ADD_BIT(a_key[round], next_key, PC_2[ii] - 1, ii)
	}
}

void genkey(void)
{
	int parity_bit;
	parity_bit = 0;

	for(int ii = 0; ii < 64; ii++)
	{
		if(ii % 8 == 7)
		{
			if(parity_bit == 1)
			{
				key += (FIRSTBIT >> ii);
			}
			parity_bit = 0;
		}
		else
		{
			if(random(0, 2) % 2 == 1)
			{
				key += (FIRSTBIT >> ii);
				parity_bit = parity_bit == 0 ? 1 : 0;
			}
		}
	}

	a_key[0] = key;
	for(int ii = 0; ii < 16; ii++)
	{
		key_schedule(ii);
		if(ii != 15)
		{
			a_key[ii + 1] = next_key;
		}
	}

	bool weak;
	weak = false;
	for(int ii = 0; ii < 15; ii++)
	{
		for(int jj = ii + 1; jj < 16; jj++)
		{
			if(a_key[ii] == a_key[jj])
			{
				weak = true;
			}
		}
	}
	if(weak)
	{
		genkey();
	}
}

void rounds(uint64_t key)
{
	uint64_t right_block = 0;
	uint64_t right_block_temp = 0;

	for(int ii = 0; ii < 48; ii++)
	{
		ADD_BIT(right_block, data, DesExpansion[ii] + 31, ii)
	}

	right_block = right_block ^ key;

	int coordx, coordy;
	uint64_t substitued;
	for(int ii = 0; ii < 8; ii++)
	{
		coordx = ((right_block << 6 * ii) & FIRSTBIT) == FIRSTBIT ? 2 : 0;
		if(((right_block << (6 * ii + 5)) & FIRSTBIT) == FIRSTBIT)
		{
			coordx++;
		}
		coordy = 0;
		for(int jj = 1; jj < 5; jj++)
		{
			if(((right_block << (6 * ii + jj)) & FIRSTBIT) == FIRSTBIT)
			{
				coordy += 2 ^ (4 - jj);
			}
		}
		substitued = DesSbox[ii][coordx][coordy];
		substitued = substitued << (60 - 4 * ii);
		right_block_temp += substitued;
	}

	right_block = right_block_temp;

	right_block_temp = 0;
	for(int ii = 0; ii < 32; ii++)
	{
		ADD_BIT(right_block_temp, right_block, Pbox[ii] - 1, ii)
	}
	right_block = right_block_temp;

	right_block = right_block ^ data;

	data = (data << 32) + (right_block >> 32);
}

void setup()
{
	Serial.begin(9600);
	pinMode(12, OUTPUT);
	SREG &= ~0x80;
}

void loop()
{
	key = 0;
	genkey();

	/*if(!key_parity_verify(key))
	{
		while(1)
		{
		}
	}*/

	data = 0x92B5AC985B409D04;

	a_key[0] = key;
	for(int ii = 0; ii < 16; ii++)
	{
		key_schedule(ii);
		if(ii != 15)
		{
			a_key[ii + 1] = next_key;
		}
	}

	Permutation(true);
	for(int ii = 0; ii < 16; ii++)
	{
		rounds(a_key[ii]);
	}
	Permutation(false);

	Permutation(true);
	data = (data << 32) + (data >> 32);
	for(int ii = 15; ii >= 0; ii--)
	{
		rounds(a_key[ii]);
	}
	data = (data << 32) + (data >> 32);
	Permutation(false);
}
