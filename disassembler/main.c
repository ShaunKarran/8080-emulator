/*
 * Program to disassemble Intel 8080 programs into assembly language.
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	FILE *rom_file, *asm_file;
	int rom_size, i = 0;
	unsigned char *rom_buffer;

	/* Check input arguments */
	if (argc != 3) {
		printf("Usage: ./a.out <input rom> <output assembly>\n");
		exit(EXIT_FAILURE);
	}

	/* Open rom file */
	rom_file = fopen(argv[1], "rb");
	if (rom_file == NULL) {
		perror("Error opening rom.");
		exit(EXIT_FAILURE);
	}

	/* Open file for asssembly code. */
	asm_file = fopen(argv[2], "w");
	if (asm_file == NULL) {
		perror("Error opening asssembly file.");
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
	rom_buffer = calloc(rom_size, sizeof(char));
	if (rom_buffer == NULL) {
		perror("Error creating buffer.");
		exit(EXIT_FAILURE);
	}

	/* Read rom into buffer */
	if (fread(rom_buffer, sizeof(char), rom_size, rom_file) != rom_size) {
		perror("Error reading rom to buffer.");
		exit(EXIT_FAILURE);
	}

	// point pc to start of buffer
	// while *pc is valid
	//		decode opcode
	// 		write assembly line to file.

	fclose(rom_file);
	fclose(asm_file);
}
