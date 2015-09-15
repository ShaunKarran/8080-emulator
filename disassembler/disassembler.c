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
		fprintf(asm_file, "0x%0.4x\t", pc);

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
		case 0x01: /* LXI B,#$data16 */
			fprintf(asm_file, "LXI\t\tB,#$%0.2x%0.2x\n", opcode[2], opcode[1]);
			pc += 3;
			break;
		case 0x02:
			fprintf(asm_file, "STAX\tB\n");
			pc += 1;
			break;
		case 0x03:
			fprintf(asm_file, "INX\t\tB\n");
			pc += 1;
			break;
		case 0xc3:
			fprintf(asm_file, "JMP\t\t$%0.2x%0.2x\n", opcode[2], opcode[1]);
			pc += 3;
			break;
		default:
			fprintf(asm_file, "Not handled yet\n");
			pc += 1;
	}

	return pc;
}
