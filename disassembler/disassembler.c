/*
 * Program to disassemble Intel 8080 programs into assembly language.
 */

#include <stdio.h>
#include <stdlib.h>

int load_rom(char *file_name, unsigned char *buffer[]);

int disassemble_8080(unsigned char *buffer, int pc, FILE *asm_file);

int main(int argc, char *argv[])
{
    FILE *asm_file;
    unsigned char *rom_buffer;
    int rom_size, pc = 0;

    /* Check input arguments */
    if (argc != 3) {
        printf("Usage: ./a.out <rom file> <assembly file>\n");
        exit(EXIT_FAILURE);
    }

    /* Load the rom into a buffer */
    rom_size = load_rom(argv[1], &rom_buffer);

    /* Open file for asssembly code */
    asm_file = fopen(argv[2], "w");
    if (asm_file == NULL) {
        perror("Error opening asssembly file.");
        exit(EXIT_FAILURE);
    }

    /* While pc hasnt reached the end of the buffer */
    while (pc < rom_size) {
        /* Write opcode positon at start of line */
        fprintf(asm_file, "%0.4x\t", pc);

        /* Decode opcode and write to assembly file */
        pc = disassemble_8080(rom_buffer, pc, asm_file);
    }

    fclose(asm_file);
}

int load_rom(char *file_name, unsigned char *buffer[])
{
    FILE *rom_file;
    int rom_size;

    /* Open rom file */
    rom_file = fopen(file_name, "rb");
    if (rom_file == NULL) {
        perror("Error opening rom.");
        exit(EXIT_FAILURE);
    }

    /* Get size of rom */
    if (fseek(rom_file, 0, SEEK_END) == 0) {
        rom_size = ftell(rom_file);
        rewind(rom_file);
    } else {
        perror("Error getting size of rom.");
        exit(EXIT_FAILURE);
    }

    /* Create buffer to contain rom */
    *buffer = calloc(rom_size, sizeof(char));
    if (*buffer == NULL) {
        perror("Error creating buffer.");
        exit(EXIT_FAILURE);
    }

    /* Read rom into buffer */
    if (fread(*buffer, sizeof(char), rom_size, rom_file) != rom_size) {
        perror("Error reading rom to buffer.");
        exit(EXIT_FAILURE);
    }

    fclose(rom_file);
    return rom_size;
}

int disassemble_8080(unsigned char *buffer, int pc, FILE *asm_file)
{
    unsigned char *opcode;

    opcode = &buffer[pc];

    switch (*opcode) {
        case 0x00:
            fprintf(asm_file, "NOP\n");
            pc += 1;
            break;

        case 0x01:    /* LXI B,data16 */
            fprintf(asm_file, "LXI\t\tB, %d\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x11:    /* LXI D,data16 */
            fprintf(asm_file, "LXI\t\tD, %d\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x21:    /* LXI H,data16 */
            fprintf(asm_file, "LXI\t\tH, %d\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x31:    /* LXI SP,data16 */
            fprintf(asm_file, "LXI\t\tSP, %d\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0x02:    /* STAX B */
            fprintf(asm_file, "STAX\tB\n");
            pc += 1;
            break;
        case 0x03:    /* INX B */
            fprintf(asm_file, "INX\t\tB\n");
            pc += 1;
            break;
        case 0x32: /* STA addr */
            fprintf(asm_file, "STA\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0x05: /* DCR B */
            fprintf(asm_file, "DCR\t\tB\n");
            pc += 1;
            break;
        case 0x0c: /* DCR C */
            fprintf(asm_file, "DCR\t\tC\n");
            pc += 1;
            break;
        case 0x15: /* DCR D */
            fprintf(asm_file, "DCR\t\tD\n");
            pc += 1;
            break;
        case 0x1c: /* DCR E */
            fprintf(asm_file, "DCR\t\tE\n");
            pc += 1;
            break;
        case 0x25: /* DCR H */
            fprintf(asm_file, "DCR\t\tH\n");
            pc += 1;
            break;
        case 0x2c: /* DCR L */
            fprintf(asm_file, "DCR\t\tL\n");
            pc += 1;
            break;
        case 0x35: /* DCR (HL) */
            fprintf(asm_file, "DCR\t\tM\n");
            pc += 1;
            break;
        case 0x3c: /* DCR A */
            fprintf(asm_file, "DCR\t\tA\n");
            pc += 1;
            break;

        case 0x06: /* MVI B,data8 */
            fprintf(asm_file, "MVI\t\tB, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x0e: /* MVI C,data8 */
            fprintf(asm_file, "MVI\t\tC, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x16: /* MVI D,data8 */
            fprintf(asm_file, "MVI\t\tD, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x1e: /* MVI E,data8 */
            fprintf(asm_file, "MVI\t\tE, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x26: /* MVI H,data8 */
            fprintf(asm_file, "MVI\t\tH, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x2e: /* MVI L,data8 */
            fprintf(asm_file, "MVI\t\tL, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x36: /* MVI (HL),data8 */
            fprintf(asm_file, "MVI\t\tM, %d\n", opcode[1]);
            pc += 2;
            break;
        case 0x3e: /* MVI A,data8 */
            fprintf(asm_file, "MVI\t\tA, %d\n", opcode[1]);
            pc += 2;
            break;

        case 0xc3:    /* JMP addr */
            fprintf(asm_file, "JMP\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0xcd: /* CALL addr */
            fprintf(asm_file, "CALL\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0xdb: /* IN port */
            fprintf(asm_file, "IN\t\t%d\n", opcode[1]);
            pc += 2;
            break;

        case 0x0f: /* RRC */
            fprintf(asm_file, "RRC\n");
            pc += 1;
            break;

        case 0x40: /* MOV B, B */
            fprintf(asm_file, "MOV\t\tB, B\n");
            pc += 1;
            break;
        case 0x41: /* MOV B, C */
            fprintf(asm_file, "MOV\t\tB, C\n");
            pc += 1;
            break;
        case 0x42: /* MOV B, D */
            fprintf(asm_file, "MOV\t\tB, D\n");
            pc += 1;
            break;
        case 0x43: /* MOV B, E */
            fprintf(asm_file, "MOV\t\tB, E\n");
            pc += 1;
            break;
        case 0x44: /* MOV B, H */
            fprintf(asm_file, "MOV\t\tB, H\n");
            pc += 1;
            break;
        case 0x45: /* MOV B, L */
            fprintf(asm_file, "MOV\t\tB, L\n");
            pc += 1;
            break;
        case 0x46: /* MOV B, M */
            fprintf(asm_file, "MOV\t\tB, M\n");
            pc += 1;
            break;
        case 0x47: /* MOV B, A */
            fprintf(asm_file, "MOV\t\tB, A\n");
            pc += 1;
            break;
        case 0x48: /* MOV C, B */
            fprintf(asm_file, "MOV\t\tC, B\n");
            pc += 1;
            break;
        case 0x49: /* MOV C, C */
            fprintf(asm_file, "MOV\t\tC, C\n");
            pc += 1;
            break;
        case 0x4a: /* MOV C, D */
            fprintf(asm_file, "MOV\t\tC, D\n");
            pc += 1;
            break;
        case 0x4b: /* MOV C, E */
            fprintf(asm_file, "MOV\t\tC, E\n");
            pc += 1;
            break;
        case 0x4c: /* MOV C, H */
            fprintf(asm_file, "MOV\t\tC, H\n");
            pc += 1;
            break;
        case 0x4d: /* MOV C, L */
            fprintf(asm_file, "MOV\t\tC, L\n");
            pc += 1;
            break;
        case 0x4e: /* MOV C, M */
            fprintf(asm_file, "MOV\t\tC, M\n");
            pc += 1;
            break;
        case 0x4f: /* MOV C, A */
            fprintf(asm_file, "MOV\t\tC, A\n");
            pc += 1;
            break;
        case 0x50: /* MOV D, B */
            fprintf(asm_file, "MOV\t\tD, B\n");
            pc += 1;
            break;
        case 0x51: /* MOV D, C */
            fprintf(asm_file, "MOV\t\tD, C\n");
            pc += 1;
            break;
        case 0x52: /* MOV D, D */
            fprintf(asm_file, "MOV\t\tD, D\n");
            pc += 1;
            break;
        case 0x53: /* MOV D, E */
            fprintf(asm_file, "MOV\t\tD, E\n");
            pc += 1;
            break;
        case 0x54: /* MOV D, H */
            fprintf(asm_file, "MOV\t\tD, H\n");
            pc += 1;
            break;
        case 0x55: /* MOV D, L */
            fprintf(asm_file, "MOV\t\tD, L\n");
            pc += 1;
            break;
        case 0x56: /* MOV D, M */
            fprintf(asm_file, "MOV\t\tD, M\n");
            pc += 1;
            break;
        case 0x57: /* MOV D, A */
            fprintf(asm_file, "MOV\t\tD, A\n");
            pc += 1;
            break;
        case 0x58: /* MOV E, B */
            fprintf(asm_file, "MOV\t\tE, B\n");
            pc += 1;
            break;
        case 0x59: /* MOV E, C */
            fprintf(asm_file, "MOV\t\tE, C\n");
            pc += 1;
            break;
        case 0x5a: /* MOV E, D */
            fprintf(asm_file, "MOV\t\tE, D\n");
            pc += 1;
            break;
        case 0x5b: /* MOV E, E */
            fprintf(asm_file, "MOV\t\tE, E\n");
            pc += 1;
            break;
        case 0x5c: /* MOV E, H */
            fprintf(asm_file, "MOV\t\tE, H\n");
            pc += 1;
            break;
        case 0x5d: /* MOV E, L */
            fprintf(asm_file, "MOV\t\tE, L\n");
            pc += 1;
            break;
        case 0x5e: /* MOV E, M */
            fprintf(asm_file, "MOV\t\tE, M\n");
            pc += 1;
            break;
        case 0x5f: /* MOV E, A */
            fprintf(asm_file, "MOV\t\tE, A\n");
            pc += 1;
            break;
        case 0x60: /* MOV H, B */
            fprintf(asm_file, "MOV\t\tH, B\n");
            pc += 1;
            break;
        case 0x61: /* MOV H, C */
            fprintf(asm_file, "MOV\t\tH, C\n");
            pc += 1;
            break;
        case 0x62: /* MOV H, D */
            fprintf(asm_file, "MOV\t\tH, D\n");
            pc += 1;
            break;
        case 0x63: /* MOV H, E */
            fprintf(asm_file, "MOV\t\tH, E\n");
            pc += 1;
            break;
        case 0x64: /* MOV H, H */
            fprintf(asm_file, "MOV\t\tH, H\n");
            pc += 1;
            break;
        case 0x65: /* MOV H, L */
            fprintf(asm_file, "MOV\t\tH, L\n");
            pc += 1;
            break;
        case 0x66: /* MOV H, M */
            fprintf(asm_file, "MOV\t\tH, M\n");
            pc += 1;
            break;
        case 0x67: /* MOV H, A */
            fprintf(asm_file, "MOV\t\tH, A\n");
            pc += 1;
            break;
        case 0x68: /* MOV L, B */
            fprintf(asm_file, "MOV\t\tL, B\n");
            pc += 1;
            break;
        case 0x69: /* MOV L, C */
            fprintf(asm_file, "MOV\t\tL, C\n");
            pc += 1;
            break;
        case 0x6a: /* MOV L, D */
            fprintf(asm_file, "MOV\t\tL, D\n");
            pc += 1;
            break;
        case 0x6b: /* MOV L, E */
            fprintf(asm_file, "MOV\t\tL, E\n");
            pc += 1;
            break;
        case 0x6c: /* MOV L, H */
            fprintf(asm_file, "MOV\t\tL, H\n");
            pc += 1;
            break;
        case 0x6d: /* MOV L, L */
            fprintf(asm_file, "MOV\t\tL, L\n");
            pc += 1;
            break;
        case 0x6e: /* MOV L, M */
            fprintf(asm_file, "MOV\t\tL, M\n");
            pc += 1;
            break;
        case 0x6f: /* MOV L, A */
            fprintf(asm_file, "MOV\t\tL, A\n");
            pc += 1;
            break;
        case 0x70: /* MOV M, B */
            fprintf(asm_file, "MOV\t\tM, B\n");
            pc += 1;
            break;
        case 0x71: /* MOV M, C */
            fprintf(asm_file, "MOV\t\tM, C\n");
            pc += 1;
            break;
        case 0x72: /* MOV M, D */
            fprintf(asm_file, "MOV\t\tM, D\n");
            pc += 1;
            break;
        case 0x73: /* MOV M, E */
            fprintf(asm_file, "MOV\t\tM, E\n");
            pc += 1;
            break;
        case 0x74: /* MOV M, H */
            fprintf(asm_file, "MOV\t\tM, H\n");
            pc += 1;
            break;
        case 0x75: /* MOV M, L */
            fprintf(asm_file, "MOV\t\tM, L\n");
            pc += 1;
            break;
        case 0x77: /* MOV M, A */
            fprintf(asm_file, "MOV\t\tM, A\n");
            pc += 1;
            break;
        case 0x78: /* MOV A, B */
            fprintf(asm_file, "MOV\t\tA, B\n");
            pc += 1;
            break;
        case 0x79: /* MOV A, C */
            fprintf(asm_file, "MOV\t\tA, C\n");
            pc += 1;
            break;
        case 0x7a: /* MOV A, D */
            fprintf(asm_file, "MOV\t\tA, D\n");
            pc += 1;
            break;
        case 0x7b: /* MOV A, E */
            fprintf(asm_file, "MOV\t\tA, E\n");
            pc += 1;
            break;
        case 0x7c: /* MOV A, H */
            fprintf(asm_file, "MOV\t\tA, H\n");
            pc += 1;
            break;
        case 0x7d: /* MOV A, L */
            fprintf(asm_file, "MOV\t\tA, L\n");
            pc += 1;
            break;
        case 0x7e: /* MOV A, M */
            fprintf(asm_file, "MOV\t\tA, M\n");
            pc += 1;
            break;
        case 0x7f: /* MOV A, A */
            fprintf(asm_file, "MOV\t\tA, A\n");
            pc += 1;
            break;

        case 0xc5: /* PUSH B */
            fprintf(asm_file, "PUSH\tB\n");
            pc += 1;
            break;
        case 0xd5: /* PUSH D */
            fprintf(asm_file, "PUSH\tD\n");
            pc += 1;
            break;
        case 0xe5: /* PUSH H */
            fprintf(asm_file, "PUSH\tH\n");
            pc += 1;
            break;
        case 0xf5: /* PUSH PSW */
            fprintf(asm_file, "PUSH\tPSW\n");
            pc += 1;
            break;
            
        default:
            fprintf(asm_file, "N/A\n");
            pc += 1;
    }

    return pc;
}
