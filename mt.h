#include <cstdint>

class mt {
    private:
        uint32_t state[624];
        short index;

    public:
        mt(uint32_t seed) {
            this->state[0] = seed;
            this->index = 624;

            for (short i = 1; i < 624; i++) {
                seed = 0x6C078965 * (seed ^ (seed >> 30)) + i;
                this->state[i] = seed;
            }
        }

        void shuffle() {
            for (int i = 0; i < 227; i++) {
                uint32_t y = (this->state[i] & 0x80000000) | (this->state[i + 1] & 0x7FFFFFFF);
                this->state[i] = this->state[i + 397] ^ (y >> 1) ^ (0x9908B0DF * (y & 1));
            }

            for (int i = 227; i < 623; i++) {
                uint32_t y = (this->state[i] & 0x80000000) | (this->state[i + 1] & 0x7FFFFFFF);
                this->state[i] = this->state[i - 227] ^ (y >> 1) ^ (0x9908B0DF * (y & 1));
            }

            uint32_t y = (this->state[623] & 0x80000000) | (this->state[0] & 0x7FFFFFFF);
            this->state[623] ^= (y >> 1) ^ (0x9908B0DF * (y & 1));
        }

        uint32_t tamperSeed(uint32_t seed) {
            uint32_t y = seed ^ (seed >> 11);
            y ^= (y << 7) & 0x9D2C5680;
            y ^= (y << 15) & 0xEFC60000;
            y ^= y >> 18;

            return y;
        }

        uint32_t advance(unsigned long advances) {
            unsigned long advance = advances + this->index;

            while (advance >= 624)
            {
                shuffle();
                advance -= 624;
            }

            this->index = advance;

            return tamperSeed(this->state[this->index]);
        }
};