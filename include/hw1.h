#ifndef HW_H
#define HW_H

#define U_OPTION 0x4000000000000000
#define D_OPTION 0x2000000000000000
#define C_OPTION 0x1000000000000000
#define H_OPTION 0x8000000000000000
#define P_OPTION 0x0800000000000000

#include "audio.h"
#include "const.h"
#include "myrand.h"

#endif
extern unsigned int num_zeros_to_append;
/*@brief checks if factor is valid or not.
 * @details This function will check if the factor is valid or not
 * returning 1 if validation succeeds and 0 if validation fails.
 * @param key The key value passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 */
int check_factor_valid(char* factor);

/*@brief checks if key is valid or not.
 * @details This function will check if the key is valid or not
 * returning 1 if validation succeeds and 0 if validation fails.
 * @param key The key value passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 */
int check_key_valid(char* key);

/*@brief checks converts a given string to an unsigned long.
 * @details This function will convert the given string into an unsigned long and returns it.
 * @param str The string passed to the program from the CLI.
 * @return an unsigned number.
 */
unsigned string_to_unsigned_long(char* str);

/*@brief Converts Endianness.
 * @details This function will convert the endianness of given unsigned int  and returns it.
 * @param num The num  that needs to be converted.
 * @return an unsigned int number with changed endianness.
 */
unsigned int convertEndianess(unsigned int num);

/*@brief Gets the factor which is set in global_options.
 * @details This function will get the factor recorded in bits(57-48) of global_options
 * and returns it.
 * @return Returns the factor.
 */
unsigned int getFactor();

/*@brief Fills up the output_annotation buffer.
 * @details This function will fill the output_annotation buffer, if annotation size is 0,
 * it terminates the buffer with '\n' and '\0'  and if the annotation_size is not 0, it adds '\n'
 * and appends input_annotation buffer to output_annotation buffer and returns the total number of
 * characters that are prepended to output_annotation buffer.
 * @param argv is the command line argument passed to the program from CLI.
 * @param annotation_size is the size calculated in recode function to prepend those many number of
 * characters
 * @return Returns the total number of characters that are prepended to output_annotation buffer.
 */
unsigned int prepend_annotation(char** argv,unsigned int annotation_size);

/*@brief Gets the key which is set in global_options.
 * @details This function will get the factor recorded in bits(31-0) of global_options
 * and returns it.
 * @return Returns the key.
 */
unsigned int getKey();

/*@brief takes a hex string and convert it to a 32 bit number (max 8 hex digits).
 * @details This function will take a hex string and converts it to a 32 bit number (max 8 hex digits)
 * and returns it.
 * @param hex is the string passed that needs to be converted into a 32 bit number.
 * @return Returns a 32 bit number.
 */
unsigned int hex2int(char *hex);

/**
 * @brief Read, from the standard input, a single sample of audio data having
 * a specified number of bytes per sample.
 * @details  This function takes a pointer 'fp' to a buffer having sufficient
 * space to hold 'sample' value of type 'int', it reads
 * 'bytes_per_sample' data bytes from the standard input,
 * interpreting each sample in the big-endian representation of a signed integer
 * sample value, and it stores the decoded sample value into the specified buffer.
 *
 * @param  fp  A pointer to the buffer that is to receive the decoded sample
 * value.
 * @param  bytes_per_sample  The number of bytes per sample.
 * @return  1 if a complete frame was read without error; otherwise 0.
 */
int read_sample(int *fp,int bytes_per_sample);

/**
 * @brief  Write, to the standard output, a single sample of audio data having
 * a specified number of bytes per sample.
 * @details  This function takes a pointer 'fp' to a buffer that contains
 * 'sample' value of type 'int', and it writes these data value to the
 * standard output using big-endian byte order, resulting in a total of
 * 'bytes_per_sample' data bytes written.
 * @param  fp  A pointer to the buffer that contains the sample value to
 * be written.
 * @param  bytes_per_sample  The number of bytes per sample.
 * @return  1 if the complete frame was written without error; otherwise 0.
 */
int write_sample(int *fp, int bytes_per_sample);
