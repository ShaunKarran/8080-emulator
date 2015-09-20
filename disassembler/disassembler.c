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

        case 0xe3: /* XTHL */
            fprintf(asm_file, "XTHL\n");
            pc += 1;
            break;
        case 0xeb: /* XCHG */
            fprintf(asm_file, "XCHG\n");
            pc += 1;
            break;

        case 0x76:
            fprintf(asm_file, "HLT\n");
            pc += 1;
            break;

        case 0xd3: /* OUT data8 */
            fprintf(asm_file, "OUT\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;

        case 0x07: /* RLC */
            fprintf(asm_file, "RLC\n");
            pc += 1;
            break;
        case 0x0f: /* RRC */
            fprintf(asm_file, "RRC\n");
            pc += 1;
            break;
        case 0x17: /* RAL */
            fprintf(asm_file, "RAL\n");
            pc += 1;
            break;
        case 0x1f: /* RAR */
            fprintf(asm_file, "RAR\n");
            pc += 1;
            break;

        case 0xc7: /* RST 0 */
            fprintf(asm_file, "RST\t\t0\n");
            pc += 1;
            break;
        case 0xcf: /* RST 1 */
            fprintf(asm_file, "RST\t\t1\n");
            pc += 1;
            break;
        case 0xd7: /* RST 2 */
            fprintf(asm_file, "RST\t\t2\n");
            pc += 1;
            break;
        case 0xdf: /* RST 3 */
            fprintf(asm_file, "RST\t\t3\n");
            pc += 1;
            break;
        case 0xe7: /* RST 4 */
            fprintf(asm_file, "RST\t\t4\n");
            pc += 1;
            break;
        case 0xef: /* RST 5 */
            fprintf(asm_file, "RST\t\t5\n");
            pc += 1;
            break;
        case 0xf7: /* RST 6 */
            fprintf(asm_file, "RST\t\t6\n");
            pc += 1;
            break;
        case 0xff: /* RST 7 */
            fprintf(asm_file, "RST\t\t7\n");
            pc += 1;
            break;

        case 0x2f: /* CMA */
            fprintf(asm_file, "CMA\n");
            pc += 1;
            break;

        case 0x37: /* STC */
            fprintf(asm_file, "STC\n");
            pc += 1;
            break;
        case 0x3f: /* CMC */
            fprintf(asm_file, "CMC\n");
            pc += 1;
            break;

        case 0x02: /* STAX B */
            fprintf(asm_file, "STAX\t\tB\n");
            pc += 1;
            break;
        case 0x12: /* STAX D */
            fprintf(asm_file, "STAX\t\tD\n");
            pc += 1;
            break;
        case 0x0a: /* LDAX B */
            fprintf(asm_file, "LDAX\t\tB\n");
            pc += 1;
            break;
        case 0x1a: /* LDAX D */
            fprintf(asm_file, "LDAX\t\tD\n");
            pc += 1;
            break;

        case 0xc6: /* ADI data8 */
            fprintf(asm_file, "ADI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xce: /* ACI data8 */
            fprintf(asm_file, "ACI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xd6: /* SUI data8 */
            fprintf(asm_file, "SUI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xde: /* SBI data8 */
            fprintf(asm_file, "SBI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xe6: /* ANI data8 */
            fprintf(asm_file, "ANI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xee: /* XRI data8 */
            fprintf(asm_file, "XRI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xf6: /* ORI data8 */
            fprintf(asm_file, "ORI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0xfe: /* CPI data8 */
            fprintf(asm_file, "CPI\t\t$%0.2x\n", opcode[1]);
            pc += 2;
            break;

        case 0xc0: /* RNZ */
            fprintf(asm_file, "RNZ\n");
            pc += 1;
            break;
        case 0xc8: /* RZ */
            fprintf(asm_file, "RZ\n");
            pc += 1;
            break;
        case 0xc9: /* RET */
            fprintf(asm_file, "RET\n");
            pc += 1;
            break;
        case 0xd0: /* RNC */
            fprintf(asm_file, "RNC\n");
            pc += 1;
            break;
        case 0xd8: /* RC */
            fprintf(asm_file, "RC\n");
            pc += 1;
            break;
        case 0xe0: /* RPO */
            fprintf(asm_file, "RPO\n");
            pc += 1;
            break;
        case 0xe8: /* RPE */
            fprintf(asm_file, "RPE\n");
            pc += 1;
            break;
        case 0xf0: /* RP */
            fprintf(asm_file, "RP\n");
            pc += 1;
            break;
        case 0xf8: /* RM */
            fprintf(asm_file, "RM\n");
            pc += 1;
            break;

        case 0xc4: /* CNZ addr */
            fprintf(asm_file, "CNZ\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xcc: /* CZ addr */
            fprintf(asm_file, "CZ\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xcd: /* CALL addr */
            fprintf(asm_file, "CALL\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xd4: /* CNC addr */
            fprintf(asm_file, "CNC\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xdc: /* CC addr */
            fprintf(asm_file, "CC\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xe4: /* CPO addr */
            fprintf(asm_file, "CPO\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xec: /* CPE addr */
            fprintf(asm_file, "CPE\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xf4: /* CP addr */
            fprintf(asm_file, "CP\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xfc: /* CM addr */
            fprintf(asm_file, "CM\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0x09: /* DAD B */
            fprintf(asm_file, "DAD\t\tB\n");
            pc += 1;
            break;
        case 0x19: /* DAD D */
            fprintf(asm_file, "DAD\t\tD\n");
            pc += 1;
            break;
        case 0x29: /* DAD H */
            fprintf(asm_file, "DAD\t\tH\n");
            pc += 1;
            break;
        case 0x39: /* DAD SP */
            fprintf(asm_file, "DAD\t\tSP\n");
            pc += 1;
            break;

        case 0x04: /* INR B */
            fprintf(asm_file, "INR\t\tB\n");
            pc += 1;
            break;
        case 0x0c: /* INR C */
            fprintf(asm_file, "INR\t\tC\n");
            pc += 1;
            break;
        case 0x14: /* INR D */
            fprintf(asm_file, "INR\t\tD\n");
            pc += 1;
            break;
        case 0x1c: /* INR E */
            fprintf(asm_file, "INR\t\tE\n");
            pc += 1;
            break;
        case 0x24: /* INR H */
            fprintf(asm_file, "INR\t\tH\n");
            pc += 1;
            break;
        case 0x2c: /* INR L */
            fprintf(asm_file, "INR\t\tL\n");
            pc += 1;
            break;
        case 0x34: /* INR M */
            fprintf(asm_file, "INR\t\tM\n");
            pc += 1;
            break;
        case 0x3c: /* INR A */
            fprintf(asm_file, "INR\t\tA\n");
            pc += 1;
            break;

        case 0x0b: /* DCX B */
            fprintf(asm_file, "DCX\t\tB\n");
            pc += 1;
            break;
        case 0x1b: /* DCX D */
            fprintf(asm_file, "DCX\t\tD\n");
            pc += 1;
            break;
        case 0x2b: /* DCX H */
            fprintf(asm_file, "DCX\t\tH\n");
            pc += 1;
            break;
        case 0x3b: /* DCX SP */
            fprintf(asm_file, "DCX\t\tSP\n");
            pc += 1;
            break;

        case 0x22: /* SHLD addr */
            fprintf(asm_file, "SHLD\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x2a: /* LHLD addr */
            fprintf(asm_file, "LHLD\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x32: /* STA addr */
            fprintf(asm_file, "STA\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x3a: /* LDA addr */
            fprintf(asm_file, "LDA\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0x03: /* INX B */
            fprintf(asm_file, "INX\t\tB\n");
            pc += 1;
            break;
        case 0x13: /* INX D */
            fprintf(asm_file, "INX\t\tD\n");
            pc += 1;
            break;
        case 0x23: /* INX H */
            fprintf(asm_file, "INX\t\tH\n");
            pc += 1;
            break;
        case 0x33: /* INX SP */
            fprintf(asm_file, "INX\t\tSP\n");
            pc += 1;
            break;

        case 0xfb: /* EI */
            fprintf(asm_file, "EI\n");
            pc += 1;
            break;

        case 0xc1: /* POP B */
            fprintf(asm_file, "POP\t\tB\n");
            pc += 1;
            break;
        case 0xd1: /* POP D */
            fprintf(asm_file, "POP\t\tD\n");
            pc += 1;
            break;
        case 0xe1: /* POP H */
            fprintf(asm_file, "POP\t\tH\n");
            pc += 1;
            break;
        case 0xf1: /* POP PSW */
            fprintf(asm_file, "POP\t\tPSW\n");
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

        case 0x27: /* DAA */
            fprintf(asm_file, "DAA\n");
            pc += 1;
            break;

        case 0x01: /* LXI B,data16 */
            fprintf(asm_file, "LXI\t\tB, $%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x11: /* LXI D,data16 */
            fprintf(asm_file, "LXI\t\tD, $%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x21: /* LXI H,data16 */
            fprintf(asm_file, "LXI\t\tH, $%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0x31: /* LXI SP,data16 */
            fprintf(asm_file, "LXI\t\tSP, $%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0x80: /* ADD B */
            fprintf(asm_file, "ADD\t\tB\n");
            pc += 1;
            break;
        case 0x81: /* ADD C */
            fprintf(asm_file, "ADD\t\tC\n");
            pc += 1;
            break;
        case 0x82: /* ADD D */
            fprintf(asm_file, "ADD\t\tD\n");
            pc += 1;
            break;
        case 0x83: /* ADD E */
            fprintf(asm_file, "ADD\t\tE\n");
            pc += 1;
            break;
        case 0x84: /* ADD H */
            fprintf(asm_file, "ADD\t\tH\n");
            pc += 1;
            break;
        case 0x85: /* ADD L */
            fprintf(asm_file, "ADD\t\tL\n");
            pc += 1;
            break;
        case 0x86: /* ADD M */
            fprintf(asm_file, "ADD\t\tM\n");
            pc += 1;
            break;
        case 0x87: /* ADD A */
            fprintf(asm_file, "ADD\t\tA\n");
            pc += 1;
            break;

        case 0x88: /* ADC B */
            fprintf(asm_file, "ADC\t\tB\n");
            pc += 1;
            break;
        case 0x89: /* ADC C */
            fprintf(asm_file, "ADC\t\tC\n");
            pc += 1;
            break;
        case 0x8a: /* ADC D */
            fprintf(asm_file, "ADC\t\tD\n");
            pc += 1;
            break;
        case 0x8b: /* ADC E */
            fprintf(asm_file, "ADC\t\tE\n");
            pc += 1;
            break;
        case 0x8c: /* ADC H */
            fprintf(asm_file, "ADC\t\tH\n");
            pc += 1;
            break;
        case 0x8d: /* ADC L */
            fprintf(asm_file, "ADC\t\tL\n");
            pc += 1;
            break;
        case 0x8e: /* ADC M */
            fprintf(asm_file, "ADC\t\tM\n");
            pc += 1;
            break;
        case 0x8f: /* ADC A */
            fprintf(asm_file, "ADC\t\tA\n");
            pc += 1;
            break;

        case 0x90: /* SUB B */
            fprintf(asm_file, "SUB\t\tB\n");
            pc += 1;
            break;
        case 0x91: /* SUB C */
            fprintf(asm_file, "SUB\t\tC\n");
            pc += 1;
            break;
        case 0x92: /* SUB D */
            fprintf(asm_file, "SUB\t\tD\n");
            pc += 1;
            break;
        case 0x93: /* SUB E */
            fprintf(asm_file, "SUB\t\tE\n");
            pc += 1;
            break;
        case 0x94: /* SUB H */
            fprintf(asm_file, "SUB\t\tH\n");
            pc += 1;
            break;
        case 0x95: /* SUB L */
            fprintf(asm_file, "SUB\t\tL\n");
            pc += 1;
            break;
        case 0x96: /* SUB M */
            fprintf(asm_file, "SUB\t\tM\n");
            pc += 1;
            break;
        case 0x97: /* SUB A */
            fprintf(asm_file, "SUB\t\tA\n");
            pc += 1;
            break;

        case 0x98: /* SBB B */
            fprintf(asm_file, "SBB\t\tB\n");
            pc += 1;
            break;
        case 0x99: /* SBB C */
            fprintf(asm_file, "SBB\t\tC\n");
            pc += 1;
            break;
        case 0x9a: /* SBB D */
            fprintf(asm_file, "SBB\t\tD\n");
            pc += 1;
            break;
        case 0x9b: /* SBB E */
            fprintf(asm_file, "SBB\t\tE\n");
            pc += 1;
            break;
        case 0x9c: /* SBB H */
            fprintf(asm_file, "SBB\t\tH\n");
            pc += 1;
            break;
        case 0x9d: /* SBB L */
            fprintf(asm_file, "SBB\t\tL\n");
            pc += 1;
            break;
        case 0x9e: /* SBB M */
            fprintf(asm_file, "SBB\t\tM\n");
            pc += 1;
            break;
        case 0x9f: /* SBB A */
            fprintf(asm_file, "SBB\t\tA\n");
            pc += 1;
            break;

        case 0xa0: /* ANA B */
            fprintf(asm_file, "ANA\t\tB\n");
            pc += 1;
            break;
        case 0xa1: /* ANA C */
            fprintf(asm_file, "ANA\t\tC\n");
            pc += 1;
            break;
        case 0xa2: /* ANA D */
            fprintf(asm_file, "ANA\t\tD\n");
            pc += 1;
            break;
        case 0xa3: /* ANA E */
            fprintf(asm_file, "ANA\t\tE\n");
            pc += 1;
            break;
        case 0xa4: /* ANA H */
            fprintf(asm_file, "ANA\t\tH\n");
            pc += 1;
            break;
        case 0xa5: /* ANA L */
            fprintf(asm_file, "ANA\t\tL\n");
            pc += 1;
            break;
        case 0xa6: /* ANA M */
            fprintf(asm_file, "ANA\t\tM\n");
            pc += 1;
            break;
        case 0xa7: /* ANA A */
            fprintf(asm_file, "ANA\t\tA\n");
            pc += 1;
            break;

        case 0xa8: /* XRA B */
            fprintf(asm_file, "XRA\t\tB\n");
            pc += 1;
            break;
        case 0xa9: /* XRA C */
            fprintf(asm_file, "XRA\t\tC\n");
            pc += 1;
            break;
        case 0xaa: /* XRA D */
            fprintf(asm_file, "XRA\t\tD\n");
            pc += 1;
            break;
        case 0xab: /* XRA E */
            fprintf(asm_file, "XRA\t\tE\n");
            pc += 1;
            break;
        case 0xac: /* XRA H */
            fprintf(asm_file, "XRA\t\tH\n");
            pc += 1;
            break;
        case 0xad: /* XRA L */
            fprintf(asm_file, "XRA\t\tL\n");
            pc += 1;
            break;
        case 0xae: /* XRA M */
            fprintf(asm_file, "XRA\t\tM\n");
            pc += 1;
            break;
        case 0xaf: /* XRA A */
            fprintf(asm_file, "XRA\t\tA\n");
            pc += 1;
            break;

        case 0xb0: /* ORA B */
            fprintf(asm_file, "ORA\t\tB\n");
            pc += 1;
            break;
        case 0xb1: /* ORA C */
            fprintf(asm_file, "ORA\t\tC\n");
            pc += 1;
            break;
        case 0xb2: /* ORA D */
            fprintf(asm_file, "ORA\t\tD\n");
            pc += 1;
            break;
        case 0xb3: /* ORA E */
            fprintf(asm_file, "ORA\t\tE\n");
            pc += 1;
            break;
        case 0xb4: /* ORA H */
            fprintf(asm_file, "ORA\t\tH\n");
            pc += 1;
            break;
        case 0xb5: /* ORA L */
            fprintf(asm_file, "ORA\t\tL\n");
            pc += 1;
            break;
        case 0xb6: /* ORA M */
            fprintf(asm_file, "ORA\t\tM\n");
            pc += 1;
            break;
        case 0xb7: /* ORA A */
            fprintf(asm_file, "ORA\t\tA\n");
            pc += 1;
            break;

        case 0xb8: /* CMP B */
            fprintf(asm_file, "CMP\t\tB\n");
            pc += 1;
            break;
        case 0xb9: /* CMP C */
            fprintf(asm_file, "CMP\t\tC\n");
            pc += 1;
            break;
        case 0xba: /* CMP D */
            fprintf(asm_file, "CMP\t\tD\n");
            pc += 1;
            break;
        case 0xbb: /* CMP E */
            fprintf(asm_file, "CMP\t\tE\n");
            pc += 1;
            break;
        case 0xbc: /* CMP H */
            fprintf(asm_file, "CMP\t\tH\n");
            pc += 1;
            break;
        case 0xbd: /* CMP L */
            fprintf(asm_file, "CMP\t\tL\n");
            pc += 1;
            break;
        case 0xbe: /* CMP M */
            fprintf(asm_file, "CMP\t\tM\n");
            pc += 1;
            break;
        case 0xbf: /* CMP A */
            fprintf(asm_file, "CMP\t\tA\n");
            pc += 1;
            break;

        case 0x05: /* DCR B */
            fprintf(asm_file, "DCR\t\tB\n");
            pc += 1;
            break;
        case 0x0d: /* DCR C */
            fprintf(asm_file, "DCR\t\tC\n");
            pc += 1;
            break;
        case 0x15: /* DCR D */
            fprintf(asm_file, "DCR\t\tD\n");
            pc += 1;
            break;
        case 0x1d: /* DCR E */
            fprintf(asm_file, "DCR\t\tE\n");
            pc += 1;
            break;
        case 0x25: /* DCR H */
            fprintf(asm_file, "DCR\t\tH\n");
            pc += 1;
            break;
        case 0x2d: /* DCR L */
            fprintf(asm_file, "DCR\t\tL\n");
            pc += 1;
            break;
        case 0x35: /* DCR M */
            fprintf(asm_file, "DCR\t\tM\n");
            pc += 1;
            break;
        case 0x3d: /* DCR A */
            fprintf(asm_file, "DCR\t\tA\n");
            pc += 1;
            break;

        case 0x06: /* MVI B,data8 */
            fprintf(asm_file, "MVI\t\tB, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x0e: /* MVI C,data8 */
            fprintf(asm_file, "MVI\t\tC, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x16: /* MVI D,data8 */
            fprintf(asm_file, "MVI\t\tD, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x1e: /* MVI E,data8 */
            fprintf(asm_file, "MVI\t\tE, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x26: /* MVI H,data8 */
            fprintf(asm_file, "MVI\t\tH, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x2e: /* MVI L,data8 */
            fprintf(asm_file, "MVI\t\tL, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x36: /* MVI (HL),data8 */
            fprintf(asm_file, "MVI\t\tM, $%0.2x\n", opcode[1]);
            pc += 2;
            break;
        case 0x3e: /* MVI A,data8 */
            fprintf(asm_file, "MVI\t\tA, $%0.2x\n", opcode[1]);
            pc += 2;
            break;

        case 0xe9: /* PCHL */
            fprintf(asm_file, "PCHL\n");
            pc += 1;
            break;
        case 0xc2: /* JNZ addr */
            fprintf(asm_file, "JNZ\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xc3: /* JMP addr */
            fprintf(asm_file, "JMP\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xca: /* JZ addr */
            fprintf(asm_file, "JZ\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xd2: /* JNC addr */
            fprintf(asm_file, "JNC\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xda: /* JC addr */
            fprintf(asm_file, "JC\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xe2: /* JPO addr */
            fprintf(asm_file, "JPO\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xea: /* JPE addr */
            fprintf(asm_file, "JPE\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xf2: /* JP addr */
            fprintf(asm_file, "JP\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;
        case 0xfa: /* JM addr */
            fprintf(asm_file, "JM\t\t$%0.4x\n", (opcode[2] << 8) | opcode[1]);
            pc += 3;
            break;

        case 0xdb: /* IN port */
            fprintf(asm_file, "IN\t\t$%0.2x\n", opcode[1]);
            pc += 2;
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

        case 0x08:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x10:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x18:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x20: /* RIM according to site */
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x28:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x30:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
        case 0x38:
            fprintf(asm_file, "NOCODE\n");
            pc += 1;
            break;
            
        default:
            fprintf(asm_file, "N/A\t\t0x%0.2x\n", opcode[0]);
            pc += 1;
    }

    return pc;
}
