#include <cstdint>

class lcrng {
    private:
        uint32_t state;

    public:
        lcrng(uint32_t seed) {
            this->state = seed;
        }

        uint32_t next(unsigned long count = 1) {
            for (unsigned long i = 0; i < count; i++) {
                this->state = 0x6C078965 * this->state + 0x1; // ARNG
            }

            return this->state;
        }
};