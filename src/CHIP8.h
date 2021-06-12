#pragma once

#include <chrono>
#include <cstring>
#include <cstdio>
#include <iomanip>
#include <thread>
#include <vector>

#include "Logger.h"

typedef unsigned char BYTE;
typedef unsigned short int WORD;

class CHIP8
{
public:
    void reset();
    void run();
private:
    BYTE m_memory[0xFFF];
    BYTE m_registers[16];
    WORD m_addressI;
    WORD m_PC;
    std::vector<WORD> m_stack;
    
    BYTE m_display[64][32];

    bool m_running { false };
    bool m_waiting_for_key_press { false };

    // FIXME: Should these be BYTES, WORDS, INTS? Who knows...
    BYTE m_delay_timer { 0 };
    BYTE m_sound_timer { 0 };

    WORD next_opcode(); 
    void decode_opcode(WORD);
};