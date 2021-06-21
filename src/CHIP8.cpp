#include "CHIP8.h"

void CHIP8::reset()
{
    m_addressI = 0;
    m_PC = 0x200; // Programs are loaded into memory at 0x200.
    memset(m_registers, 0, sizeof(m_registers));

    FILE* file;
    file = fopen("TETRIS", "rb");
    fread(&m_memory[0x200], 0xFFF, 1, file);
    fclose(file);
}

WORD CHIP8::next_opcode()
{
    WORD result;
    result = m_memory[m_PC];
    result = result << 8;
    result = result | m_memory[m_PC + 1];

    m_PC += 2; // Opcode is two bytes long.
    return result;
}

void CHIP8::decode_opcode(WORD opcode)
{
    BYTE vx = opcode & 0x0F00;
    BYTE vy = opcode & 0x00F0;
    int sum;
    switch (opcode & 0xF000)
    {
        case 0x0000: {
            switch (opcode)
            {
                case 0x00E0:
                    clear_screen();
                    break; // CLEAR SCREEN
                case 0x0EE: // RETURN SUBROUTINE
                    m_stack.pop_back();
                    break;
            }
            break;
        }
        case 0x1000: {// JUMP
            m_PC = opcode & 0x0FFF;
            break;
        }
        case 0x2000: { // CALL
            m_stack.push_back(m_PC);
            m_PC = opcode & 0x0FFF;
            break;
        }
        case 0x3000: { // SKIP IF 0x3XYZ: Skip if regX == YZ
            if (m_registers[opcode & 0x0F00] == (opcode & 0x00FF))
                m_PC += 2;
            break;
        }
        case 0x4000: { // SKIP IF NOT
            if (m_registers[opcode & 0x0F00] != (opcode & 0x00FF))
                m_PC += 2;
            break;
        }
        case 0x5000: { // SKIP IF REGISTERS == 
            if (m_registers[opcode & 0x0F00] == m_registers[opcode & 0x00F0])
                m_PC = 2;
            break;
        }
        case 0x6000: { // LD Vx, byte
            m_registers[opcode & 0x0F00] = (opcode & 0x00FF);
            break;
        }
        case 0x7000: { // ADD Vx, byte
            m_registers[opcode & 0x0F00] = vx + (opcode & 0x00FF);
            break;
        }
        case 0x8000: {
            switch (opcode & 0x000F)
            {
                case 0: { // LOAD
                    m_registers[opcode & 0x0F00] = m_registers[opcode & 0x00F0];
                    break;
                }
                case 1: { // OR
                    m_registers[opcode & 0x0F00] = vx | vy;
                    break;
                }
                case 2: { // AND
                    m_registers[opcode & 0x0F00] = vx & vy;
                    break;
                }
                case 3: { // XOR
                    m_registers[opcode & 0x0F00] = vx ^ vy;
                    break;
                }
                case 4: { // ADD
                    sum = m_registers[opcode & 0x0F00] + m_registers[opcode & 0x00F0];

                    m_registers[0xF] = 0; // Set carry to 0
                    if (sum > 0xFF) m_registers[0xF] = 1;

                    m_registers[opcode & 0x0F00] = sum;
                    break;
                }
                case 5: { // SUB
                    m_registers[0xF] = 0;
                    if (vx > vy) m_registers[0xF] = 1;

                    sum = vx - vy;
                    m_registers[opcode & 0x0F00] = sum;
                    break;
                }
                case 6: { // SHR
                    m_registers[0xF] = m_registers[opcode & 0x0F00] & 0x1;
                    m_registers[opcode & 0x0F00] >>= 1;
                    break;
                }
                case 7: { // SUBN Vx, Vy
                    m_registers[0xF] = 0;
                    if (vy > vx) m_registers[0xF] = 1;

                    sum = vy - vy;
                    m_registers[opcode & 0x0F00] = sum;
                    break;
                }
                case 0xE: {
                    m_registers[0xF] = m_registers[opcode & 0x0F00] & 0x80;
                    m_registers[opcode & 0x0F00] <<= 1;
                    break;
                }
            }
            break;
        }
        case 0x9000: {
            if (vx != vy) m_PC += 2;
            break;
        }
            
        case 0xA000: {
            m_addressI = (opcode & 0x0FFF);
            break;
        }

        case 0xB000: {
            m_PC = (opcode & 0x0FFF) + m_registers[0];
            break;
        }

        case 0xC000: {
            int random_byte = rand() % 255;
            m_registers[opcode & 0x0F00] = random_byte & (opcode & 0x00FF);
            break;
        }

        case 0xD000: { // DRAW CALLS
            BYTE n = (opcode & 0x000F);
            int width = 8;
            int height = (opcode & 0xF);
            
            m_registers[0xF] = 0;

            for (unsigned int y = 0; y < height; y++)
            {
                BYTE sprite = m_memory[m_addressI + y];
                for (unsigned int x = 0; x < width; x++)
                {
                    if ((sprite & 0x80) > 0)
                    {
                        if (set_pixel(m_registers[vx] + x, m_registers[vy] + y))
                            m_registers[0xF] = 1;
                    }
                    sprite <<= 1;
                }
            }
            break;
        }
        
        case 0xE000: {
            switch (opcode & 0x00FF)
            {
                case 0x009E:  { // IF KEY IS PRESSED m_PC += 2;
                    // FIXME: Implement...
                    break;
                }
                case 0x00A1: { // IF KEY IS NOT PRESSED m_PC += 2;
                    break;
                }
            }
            break;
        }
        case 0xF000: {
            switch (opcode & 0x00FF)
            {
                case 0x0007: {
                    m_registers[vx] = m_delay_timer;
                    break;
                }

                case 0x000A: {
                    m_running = false;
                    m_waiting_for_key_press = true;
                    break;
                }

                case 0x0015: {
                    m_delay_timer = m_registers[vx];
                    break;
                }

                case 0x0018: {
                    m_sound_timer = m_registers[vy];
                    break;
                }

                case 0x001E: {
                    m_addressI += vx;
                    break;
                }

                case 0x0029: { // SET I = Address of Hexadecimal font for x
                    m_addressI = m_registers[vx] * 5;
                    break;
                }

                case 0x0033: {
                    BYTE value = (opcode & 0x0F00);
                    m_memory[m_addressI] = value / 100;
                    m_memory[m_addressI + 1] = (value % 100) / 10;
                    m_memory[m_addressI + 2] = value % 10;
                    break;
                }

                case 0x0055: {
                    BYTE register_index = (opcode & 0x0F00);
                    for (unsigned int i = 0; i <= register_index; i++) {
                        m_memory[m_addressI + i] = m_registers[i];
                    }
                }

                case 0x0065: {
                    BYTE register_index = (opcode & 0x0F00);
                    for (unsigned int i = 0; i <= register_index; i++) {
                        m_registers[i] = m_memory[m_addressI + i];
                    }
                }
            } 
            break;
        }
    }
}

void CHIP8::run()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        64 * 8, 32 * 8, 0);
    
    if (window == NULL) {
        Logger::the()->log(LogLevel::FATAL, "Could not init SDL or Window!");
        return;
    }

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    SDL_Event event;

    while (true)
    {
        m_running = true;
        if (m_running)
        {   
            WORD opcode = next_opcode();
            printf("Next instruction: %X\n", opcode);
            decode_opcode(opcode);

            if (m_delay_timer > 0) m_delay_timer--;
            if (m_sound_timer > 0) m_sound_timer--;

            // FIXME: Temporary
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyWindow(window);
            }
        }

        for (int x = 0; x < 64; x++)
        {
            for (int y = 0; y < 32; y++)
            {
                const SDL_Rect rect { x * 8, y * 8, 8, 8 };
                if (m_display[x][y] != 0)
                    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 255, 255));
                else
                    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 0));
            }
        }
    }
}

bool CHIP8::set_pixel(unsigned int x, unsigned int y)
{
    if (x >= 64) x = x % 64;
    if (x < 0) x = 64 - (x % 64);
    if (y >= 32) y = y % 32;
    if (y < 0) y = 32 - (y % 32);

    Logger::the()->log("Set pixel at: " + std::to_string(x) + ", " + std::to_string(y));

    m_display[x][y] ^= 1;
    return m_display[x][y] != 1;
}

void CHIP8::clear_screen() {
    for (int x = 0; x < 64; x++) {
        for (int y = 0; y < 32; y++) {
            m_display[x][y] = 0;
        }
    }
}
