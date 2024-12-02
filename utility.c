/**
 * @file utility.c
 * @brief Utility file for additional functions
 *
 * @author Alejandro Barranco-Leyte
 */

// Function to convert decimal to 7-segment value
unsigned int bcd2sevenSegmentDecoder(int digit) {
	switch (digit) {
	case 0:
		return 0x3F; // 00111111 -> Display '0'
	case 1:
		return 0x06; // 00000110 -> Display '1'
	case 2:
		return 0x5B; // 01011011 -> Display '2'
	case 3:
		return 0x4F; // 01001111 -> Display '3'
	case 4:
		return 0x66; // 01100110 -> Display '4'
	case 5:
		return 0x6D; // 01101101 -> Display '5'
	case 6:
		return 0x7D; // 01111101 -> Display '6'
	case 7:
		return 0x07; // 00000111 -> Display '7'
	case 8:
		return 0x7F; // 01111111 -> Display '8'
	case 9:
		return 0x6F; // 01101111 -> Display '9'
	default:
		return 0x00; // Blank or off
	}
}
/**
 * Utility function to check if all the possible digits are a valid one
 */
int isValidHex(unsigned int val) {
	return (val == 0x3F || val == 0x06 || val == 0x5B || val == 0x4F
			|| val == 0x66 || val == 0x6D || val == 0x7D || val == 0x07
			|| val == 0x7F || val == 0x6F || val == 0x00);
}

// Function to get the hex code for a letter
unsigned char getHexForLetter(char letter) {
    switch (letter) {
        case 'E': return 0x79; // E
        case 'R': return 0x50; // R
        case 'O': return 0x3F; // O
        case 'P': return 0x73; // P
        case 'L': return 0x38; // L
        case 'A': return 0x77; // A
        case 'S': return 0x6D; // S
        case 'T': return 0x78; // T
        default: return 0x00; // Blank
    }
}
