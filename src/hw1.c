#include <stdlib.h>
#include "debug.h"
#include "hw1.h"

#ifdef _STRING_H
#error "Do not #include <string.h>. You will get a ZERO."
#endif

#ifdef _STRINGS_H
#error "Do not #include <strings.h>. You will get a ZERO."
#endif

#ifdef _CTYPE_H
#error "Do not #include <ctype.h>. You will get a ZERO."
#endif
unsigned int num_zeros_to_append=0;
unsigned int check_data_size=0;
/*
 * You may modify this file and/or move the functions contained here
 * to other source files (except for main.c) as you wish.
 *
 * IMPORTANT: You MAY NOT use any array brackets (i.e. [ and ]) and
 * you MAY NOT declare any arrays or allocate any storage with malloc().
 * The purpose of this restriction is to force you to use pointers.
 * Variables to hold the content of three frames of audio data and
 * two annotation fields have been pre-declared for you in const.h.
 * You must use those variables, rather than declaring your own.
 * IF YOU VIOLATE THIS RESTRICTION, YOU WILL GET A ZERO!
 *
 * IMPORTANT: You MAY NOT use floating point arithmetic or declare
 * any "float" or "double" variables.  IF YOU VIOLATE THIS RESTRICTION,
 * YOU WILL GET A ZERO!
 */

/**
 * @brief Validates command line arguments passed to the program.
 * @details This function will validate all the arguments passed to the
 * program, returning 1 if validation succeeds and 0 if validation fails.
 * Upon successful return, the selected program options will be set in the
 * global variables "global_options", where they will be accessible
 * elsewhere in the program.
 *
 * @param argc The number of arguments passed to the program from the CLI.
 * @param argv The argument strings passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 * Refer to the homework document for the effects of this function on
 * global variables.
 * @modifies global variable "global_options" to contain a bitmap representing
 * the selected options.
 */
int validargs(int argc, char **argv)
{
    if(argc==1)// No flags provided
        return 0;
    else{
        unsigned long key;
        unsigned long fact;
        if(*(*(argv+1)+0)=='-' && *(*(argv+1)+1)=='h'){//-h option provided
            global_options=H_OPTION;
            return 1;
        }
        else if(*(*(argv+1)+0)=='-' && *(*(argv+1)+1)=='u'){//-u option provided
            global_options=U_OPTION;//only -u option
            if(!*(argv+2))// No other flags after -u
                return 1;
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='f'){//-u -f
                if(!*(argv+3))// No factor provided
                    return 0;
                else{
                    if(check_factor_valid(*(argv+3))==1){// Check Factor provided
                        fact=string_to_unsigned_long(*(argv+3));
                        fact=((fact-1))<<48;
                        global_options|=fact;
                        if(!*(argv+4))// Check if -p option provided
                            return 1;
                        else{
                            if(*(*(argv+4)+0)=='-' && *(*(argv+4)+1)=='p'){
                                global_options|=P_OPTION;
                                return 1;
                            }
                        }
                    }
                    else{
                        return 0;
                    }
                }
            }
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='p'){//-p
                global_options|=P_OPTION;
                if(!*(argv+3))
                    return 1;
                else{
                    if(*(*(argv+3)+0)=='-' && *(*(argv+3)+1)=='f'){// -p -f
                        if(!*(argv+4))
                            return 0;
                        else{
                            if(check_factor_valid(*(argv+4))==1){//Check if factor valid
                                fact=string_to_unsigned_long(*(argv+4));
                                fact=(fact-1)<<48;
                                global_options|=fact;
                                return 1;
                            }
                            else
                                return 0;
                        }
                    }
                }
            }
        }
        else if(*(*(argv+1)+0)=='-' && *(*(argv+1)+1)=='d'){// -d provided
            global_options=D_OPTION;
            if(!*(argv+2)){
                return 1;
            }
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='f'){// -d -f
                if(!*(argv+3))// No factor provided
                    return 0;
                else{
                    if(check_factor_valid(*(argv+3))==1){
                        unsigned long fact;
                        fact=string_to_unsigned_long(*(argv+3));
                        fact=(fact-1)<<48;
                        global_options|=fact;
                        if(!*(argv+4))
                            return 1;
                        else{
                            if(*(*(argv+4)+0)=='-' && *(*(argv+4)+1)=='p'){
                                global_options|=P_OPTION;//-p option
                                return 1;
                            }
                        }
                    }
                    else{
                        return 0;
                    }
                }
            }
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='p'){//-p first
                global_options|=P_OPTION;
                if(!*(argv+3))
                    return 1;
                else if(*(*(argv+3)+0)=='-' && *(*(argv+3)+1)=='f'){// -p -f
                    if(!*(argv+4))
                        return 0;
                    else{
                        if(check_factor_valid(*(argv+4))==1){
                        fact=string_to_unsigned_long(*(argv+4));
                        fact=(fact-1)<<48;
                        global_options|=fact;
                        return 1;
                        }
                        else
                            return 0;
                    }
                }
            }
        }
        else if(*(*(argv+1)+0)=='-' && *(*(argv+1)+1)=='c'){//-c
            global_options=C_OPTION;
            if(!*(argv+2))
                return 0;
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='k'){// -c -k
                if(!*(argv+3))// No factor provided
                    return 0;
                else{
                    if(check_key_valid(*(argv+3))==1){
                        key=hex2int(*(argv+3));
                        global_options|=key;
                    }
                    else{
                        return 0;
                    }
                    if(!*(argv+4)){
                        return 1;
                    }
                    else{
                        if(*(*(argv+4)+0)=='-' && *(*(argv+4)+1)=='p'){
                        global_options|=P_OPTION;
                        return 1;
                        }
                    }
                }
            }
            else if(*(*(argv+2)+0)=='-' && *(*(argv+2)+1)=='p'){
                global_options|=P_OPTION;
                if(!*(argv+3))
                    return 0;
                else if(*(*(argv+3)+0)=='-' && *(*(argv+3)+1)=='k'){
                    if(!*(argv+4))
                        return 0;//no key provided
                    else{
                        if(check_key_valid(*(argv+4))==1){
                        key=hex2int(*(argv+4));
                        global_options|=key;
                        return 1;
                    }
                    else
                        return 0;
                    }
                }
            }
        }
    }
    debug("returned 0");
    return 0;
}

/**
 * @brief  Recodes a Sun audio (.au) format audio stream, reading the stream
 * from standard input and writing the recoded stream to standard output.
 * @details  This function reads a sequence of bytes from the standard
 * input and interprets it as digital audio according to the Sun audio
 * (.au) format.  A selected transformation (determined by the global variable
 * "global_options") is applied to the audio stream and the transformed stream
 * is written to the standard output, again according to Sun audio format.
 *
 * @param  argv  Command-line arguments, for constructing modified annotation.
 * @return 1 if the recoding completed successfully, 0 otherwise.
 */
int recode(char **argv) {
    //Read Header
    AUDIO_HEADER hp;
    if(read_header(&hp)==0){
        return 0;
    }
    check_data_size=hp.data_size;// storing data_size in temp variable to check
                                    // for data size after reading input data
    unsigned int calc_data_size=0;// to calculate number of size of data read.
    unsigned int annotation_size=hp.data_offset-sizeof(AUDIO_HEADER);
    //Check if annotation size is valid or not
    if(annotation_size>ANNOTATION_MAX){
        return 0;
    }
    int frame_size=(hp.encoding-1)*(hp.channels);
    unsigned int num_data_frames=(hp.data_size)/(frame_size);

    if(hp.data_size % frame_size!=0){//Check if datasize is a multiple of frame_size
        return 0;
    }
    if(global_options & U_OPTION){//-u set calculate num_data_frames and data size of the output
        num_data_frames=(num_data_frames+(getFactor()-1))/(getFactor());
        hp.data_size=num_data_frames * frame_size;
    }
    else if(global_options & D_OPTION){//-d set calculate the data size of the output
        hp.data_size+=(num_data_frames-1)*(getFactor()-1)* frame_size;

    }
    if(annotation_size!=0){
        if(read_annotation(input_annotation,annotation_size)==0)
            return 0;
        //check if no transformation for annotations
        if(!(global_options & P_OPTION)){//-p not set
            //Modify annotation
            annotation_size=prepend_annotation(argv,annotation_size);
            hp.data_offset+=annotation_size;
            unsigned int num_zeros_to_append=(((sizeof(AUDIO_HEADER)+annotation_size) % 8));
            if(num_zeros_to_append!=0){
                num_zeros_to_append=8-((sizeof(AUDIO_HEADER)+annotation_size) % 8);
            }
            hp.data_offset+=num_zeros_to_append;
            if(write_header(&hp)==0)
                return 0;
            if(write_annotation(output_annotation,annotation_size)==0)
                return 0;
            while(num_zeros_to_append>0){
                if(putchar('\0')==EOF)
                    return 0;
                num_zeros_to_append--;
            }

        }
        else{
            if(write_header(&hp)==0)
                return 0;
            if(write_annotation(input_annotation,annotation_size)==0)
                return 0;
            while(num_zeros_to_append>0){
                if(putchar('\0')==EOF)
                    return 0;
                num_zeros_to_append--;
            }
        }
    }
    else if(annotation_size==0){
        if(!(global_options & P_OPTION)){// annotation size 0 and -p not set
            annotation_size=prepend_annotation(argv,annotation_size);
            hp.data_offset+=annotation_size;
            num_zeros_to_append=(((sizeof(AUDIO_HEADER)+annotation_size) % 8));
            if(num_zeros_to_append!=0)
                num_zeros_to_append=8-((sizeof(AUDIO_HEADER)+annotation_size) % 8);
            hp.data_offset+=num_zeros_to_append;
            if(write_header(&hp)==0)
                return 0;
            if(write_annotation(output_annotation,annotation_size)==0)
                return 0;
            while(num_zeros_to_append>0){
                if(putchar('\0')==EOF)
                    return 0;
                num_zeros_to_append--;
            }
        }
        else{//annotation size 0 and -p set
            if(write_header(&hp)==0)// No annotation to be written just write the header
                return 0;
        }
    }
    //Check if datasize is valid
    unsigned int factor=getFactor();
    if(global_options & U_OPTION){//-u option set
        //Do Decimation(Speed up)
        int* ip=(int*)&(input_frame);
        if(read_frame(ip, hp.channels, hp.encoding-1)==0)
            return 0;
        calc_data_size+=frame_size;
        if(write_frame(ip, hp.channels, hp.encoding-1)==0)
            return 0;
        for(int i=1;i<num_data_frames;i++){
            int j=0;
            for(;j<factor;j++){
                if(read_frame(ip, hp.channels, hp.encoding-1)==0)
                    return 0;
                calc_data_size+=frame_size;
            }
            if(write_frame(ip, hp.channels, hp.encoding-1)==0)
                return 0;
        }
    }
    else if(global_options & C_OPTION){//-c option set
        frame_size=hp.channels *(hp.encoding-1);
        unsigned int seed=getKey();
        int* ip=(int*)&input_frame;
        int* op=(int*)output_frame;
        mysrand(seed);
        for(int i=0;i<num_data_frames;i++){
            if(read_frame(ip, hp.channels, hp.encoding-1)==0)
                return 0;
            calc_data_size+=frame_size;
            for(int j=0;j<hp.channels;j++){
                int sequence=myrand32();
                *(op+j)=convertEndianess(*(op+j));
                *(op+j) = *(ip+j)^ sequence;
            }
            if(write_frame(op, hp.channels, hp.encoding-1)==0)
                return 0;
        }
    }

    else if(global_options & D_OPTION){//-d option set
        int *pp=(int*)&previous_frame;
        int *ip=(int*)&input_frame;
        int *op=(int*)&output_frame;
        if(read_frame(pp, hp.channels, hp.encoding-1)==0)
            return 0;
        calc_data_size+=frame_size;
        if(write_frame(pp, hp.channels,hp.encoding-1)==0)
            return 0;
        for(int i=1;i<num_data_frames;i++){
            if(read_frame(ip, hp.channels, hp.encoding-1)==0)
                return 0;
            calc_data_size+=frame_size;
            for(int j=1;j<factor;j++){
                for (int k=0;k<hp.channels;k++){
                    int sum;
                    if(*(ip+k)-*(pp+k) <0){
                        sum=-(*(ip+k)-*(pp+k))*j/factor;
                        sum=-sum;
                    }
                    else{
                    sum =(*(ip+k)-*(pp+k))*j/factor;
                    }
                    *(op+k)=*(pp+k)+sum;
                }
                if(write_frame(op, hp.channels, hp.encoding-1)==0)
                    return 0;
            }

            for(int i=0;i<hp.channels;i++){
                *(pp+i)=*(ip+i);
            }
            if(write_frame(ip, hp.channels, hp.encoding-1)==0)
                return 0;
        }
    }
    if(check_data_size>calc_data_size)
        return 0;
    return 1;
    }

/**
 * @brief Read the header of a Sun audio file and check it for validity.
 * @details  This function reads 24 bytes of data from the standard input and
 * interprets it as the header of a Sun audio file.  The data is decoded into
 * six unsigned int values, assuming big-endian byte order.   The decoded values
 * are stored into the AUDIO_HEADER structure pointed at by hp.
 * The header is then checked for validity, which means:  no error occurred
 * while reading the header data, the magic number is valid, the data offset
 * is a multiple of 8, the value of encoding field is one of {2, 3, 4, 5},
 * and the value of the channels field is one of {1, 2}.
 *
 * @param hp  A pointer to the AUDIO_HEADER structure that is to receive
 * the data.
 * @return  1 if a valid header was read, otherwise 0.
 */
int read_header(AUDIO_HEADER *hp){
    unsigned int byte_count=0;
    unsigned int byte1;
    unsigned int byte2;
    unsigned int byte3;
    unsigned int byte4;
    if(((byte1=getchar())!=EOF) && ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->magic_number=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    else
        return 0;
    if(((byte1=getchar())!=EOF)&& ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->data_offset=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    else
        return 0;
    if(((byte1=getchar())!=EOF)&& ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->data_size=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    else
        return 0;
    if(((byte1=getchar())!=EOF)&& ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->encoding=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    if(((byte1=getchar())!=EOF)&& ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->sample_rate=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    else
        return 0;
    if(((byte1=getchar())!=EOF)&& ((byte2=getchar())!=EOF) && ((byte3=getchar())!=EOF)&&((byte4=getchar())!=EOF)){
        hp->channels=byte1<<24|byte2<<16|byte3<<8|byte4;
        byte_count+=4;
    }
    else
        return 0;

    //Check if magic_number is valid
    if(!(hp->magic_number==AUDIO_MAGIC)){
        return 0;
    }
    //Check if data offset is a multiple of 8
    if(hp->data_offset % 8!=0){
        return 0;
    }

    //Check if encoding value is valid or not
    if(!(hp->encoding==2 ||hp->encoding==3 || hp->encoding==4 || hp->encoding==5)){
        return 0;
    }
    //Check if channels is valid
    if(!(hp->channels==1 ||hp->channels==2)){
        return 0;
    }
    return 1;
}

/* @brief  Write the header of a Sun audio file to the standard output.
 * @details  This function takes the pointer to the AUDIO_HEADER structure passed
 * as an argument, encodes this header into 24 bytes of data according to the Sun
 * audio file format specifications, and writes this data to the standard output.
 *
 * @param  hp  A pointer to the AUDIO_HEADER structure that is to be output.
 * @return  1 if the function is successful at writing the data; otherwise 0.
 */
int write_header(AUDIO_HEADER *hp){
        unsigned int byte1=(hp->magic_number&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        unsigned int byte2=(hp->magic_number&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        unsigned int byte3=(hp->magic_number&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        unsigned int byte4=(hp->magic_number&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;
        byte1=(hp->data_offset&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        byte2=(hp->data_offset&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        byte3=(hp->data_offset&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        byte4=(hp->data_offset&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;

        byte1=(hp->data_size&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        byte2=(hp->data_size&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        byte3=(hp->data_size&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        byte4=(hp->data_size&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;

        byte1=(hp->encoding&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        byte2=(hp->encoding&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        byte3=(hp->encoding&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        byte4=(hp->encoding&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;

        byte1=(hp->sample_rate&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        byte2=(hp->sample_rate&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        byte3=(hp->sample_rate&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        byte4=(hp->sample_rate&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;

        byte1=(hp->channels&0xff000000)>>24;
        if(putchar(byte1)==EOF)
            return 0;
        byte2=(hp->channels&0x00ff0000)>>16;
        if(putchar(byte2)==EOF)
            return 0;
        byte3=(hp->channels&0x0000ff00)>>8;
        if(putchar(byte3)==EOF)
            return 0;
        byte4=(hp->channels&0x000000ff);
        if(putchar(byte4)==EOF)
            return 0;
    return 1;
}

/**
 * @brief  Read annotation data for a Sun audio file from the standard input,
 * storing the contents in a specified buffer.
 * @details  This function takes a pointer 'ap' to a buffer capable of holding at
 * least 'size' characters, and it reads 'size' characters from the standard input,
 * storing the characters read in the specified buffer.  It is checked that the
 * data read is terminated by at least one null ('\0') byte.
 *
 * @param  ap  A pointer to the buffer that is to receive the annotation data.
 * @param  size  The number of bytes of data to be read.
 * @return  1 if 'size' bytes of valid annotation data were successfully read;
 * otherwise 0.
 */
int read_annotation(char *ap, unsigned int size){
    int i;
    for(i=0;i<size;i++){
        if(scanf("%c",(ap+i))!=1)
            return 0;
    }
    //check if data is terminated with '\0'
    if(*(ap+size)!='\0'){
        return 0;
    }
    return 1;
}

/**
 * @brief  Write annotation data for a Sun audio file to the standard output.
 * @details  This function takes a pointer 'ap' to a buffer containing 'size'
 * characters, and it writes 'size' characters from that buffer to the standard
 * output.
 *
 * @param  ap  A pointer to the buffer containing the annotation data to be
 * written.
 * @param  size  The number of bytes of data to be written.
 * @return  1 if 'size' bytes of data were successfully written; otherwise 0.
 */
int write_annotation(char *ap, unsigned int size){
    for(int i=0;i<size;i++){
        if(printf("%c",*(ap+i))!=1)
            return 0;
    }
    return 1;
}

/**
 * @brief Read, from the standard input, a single frame of audio data having
 * a specified number of channels and bytes per sample.
 * @details  This function takes a pointer 'fp' to a buffer having sufficient
 * space to hold 'channels' values of type 'int', it reads
 * 'channels * bytes_per_sample' data bytes from the standard input,
 * interpreting each successive set of 'bytes_per_sample' data bytes as
 * the big-endian representation of a signed integer sample value, and it
 * stores the decoded sample values into the specified buffer.
 *
 * @param  fp  A pointer to the buffer that is to receive the decoded sample
 * values.
 * @param  channels  The number of channels.
 * @param  bytes_per_sample  The number of bytes per sample.
 * @return  1 if a complete frame was read without error; otherwise 0.
 */
int read_frame(int *fp, int channels, int bytes_per_sample){

    int i=0;
    while(i<channels){
        if(read_sample((fp+i),bytes_per_sample)==0)
            return 0;
            if(bytes_per_sample==1){
                if((*(fp+i)& 0x00000080)==0x00000080){
                    *(fp+i) |= 0xffffff00;
                }
                else{
                    *(fp+i) &= 0x000000ff;
                }
            }
            else if(bytes_per_sample==2){
                if((*(fp+i)& 0x00008000)==0x00008000){
                    *(fp+i) |= 0xffff0000;
                }
                else{
                    *(fp+i) &= 0x0000ffff;
                }
            }
            else if(bytes_per_sample==3){
                if((*(fp+i)& 0x00800000)==0x00800000){
                    *(fp+i) |= 0xff000000;
                }
            }
        i++;
    }
    return 1;
}

/**
 * @brief  Write, to the standard output, a single frame of audio data having
 * a specified number of channels and bytes per sample.
 * @details  This function takes a pointer 'fp' to a buffer that contains
 * 'channels' values of type 'int', and it writes these data values to the
 * standard output using big-endian byte order, resulting in a total of
 * 'channels * bytes_per_sample' data bytes written.
 *
 * @param  fp  A pointer to the buffer that contains the sample values to
 * be written.
 * @param  channels  The number of channels.
 * @param  bytes_per_sample  The number of bytes per sample.
 * @return  1 if the complete frame was written without error; otherwise 0.
 */
int write_frame(int *fp, int channels, int bytes_per_sample){
    int i=0;
    while(i<channels){
        if(write_sample(fp+i,bytes_per_sample)==0)
            return 0;

        i++;
    }
    return 1;
}

/*@brief checks if factor is valid or not.
 * @details This function will check if the factor is valid or not
 * returning 1 if validation succeeds and 0 if validation fails.
 * @param key The key value passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 */
int check_factor_valid(char* factor){

    int i=0;
    while(*(factor+i)){
        if(!(*(factor+i)>='0' && *(factor+i)<='9'))
            return 0;
        i++;

    }
    unsigned long fctr=string_to_unsigned_long(factor);
    if(fctr>=1 && fctr<=1024)
        return 1;
    else
        return 0;
}

/*@brief checks if key is valid or not.
 * @details This function will check if the key is valid or not
 * returning 1 if validation succeeds and 0 if validation fails.
 * @param key The key value passed to the program from the CLI.
 * @return 1 if validation succeeds and 0 if validation fails.
 */
int check_key_valid(char* key){
    int len=0;
    while(*(key+len)){
        if(((*(key+len)>='0' && *(key+len)<='9') || (*(key+len)>='a' && *(key+len)<='z') || (*(key+len)>='A' && *(key+len)<='Z'))){
            len++;
        }
        else
            return 0;
    }
    if(len>8){
        return 0;
    }
    else
        return 1;
}

/*@brief checks converts a given string to an unsigned long.
 * @details This function will convert the given string into an unsigned long and returns it.
 * @param str The string passed to the program from the CLI.
 * @return an unsigned number.
 */
unsigned  string_to_unsigned_long(char* str){
    int i=0;
    unsigned  num=0;
    while(*(str+i)){
        num*=10;
        num+=*(str+i)-'0';
        i++;
    }
    return num;
}

/*@brief Converts Endianness.
 * @details This function will convert the endianness of given unsigned int  and returns it.
 * @param num The num  that needs to be converted.
 * @return an unsigned int number with changed endianness.
 */
unsigned int convertEndianess(unsigned int big){
    int byte1 = (big & 0x000000ff) << 24u;
    int byte2 = (big & 0x0000ff00) << 8u;
    int byte3 = (big & 0x00ff0000) >> 8u;
    int byte4 = (big & 0xff000000) >> 24u;
    big = byte1|byte2|byte3|byte4;
    return big;
}

/*@brief Gets the factor which is set in global_options.
 * @details This function will get the factor recorded in bits(57-48) of global_options
 * and returns it.
 * @return Returns the factor.
 */
unsigned int getFactor(){
    unsigned int factor;;
    unsigned long mask=0x03ff000000000000;
    factor=(global_options&(mask))>>48;
    return factor+1;
}

/*@brief Gets the key which is set in global_options.
 * @details This function will get the factor recorded in bits(31-0) of global_options
 * and returns it.
 * @return Returns the key.
 */
unsigned int getKey(){
    unsigned int key;
    unsigned long mask=0x00000000ffffffff;
    key=(global_options&(mask));
    return key;
}

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
unsigned int prepend_annotation(char** argv, unsigned int annotation_size){

    unsigned int i=0;
    unsigned int j=0;
    unsigned int k=0;
    while(*(argv+j)){
        k=0;
        while(*(*(argv+j)+k)!='\0'){
            *(output_annotation+i)=*(*(argv+j)+k);
            i++;
            k++;
        }
        *(output_annotation+i)=' ';
        i++;
        j++;
    }
    if(annotation_size==0){
        *(output_annotation+i-1)='\n';
        *(output_annotation+i)='\0';
        return i;
    }
    else if(annotation_size!=0){
        *(output_annotation+i-1)='\n';
        j=0;
        while(*(input_annotation+j)!='\0'){
            *(output_annotation+i)=*(input_annotation+j);
            i++;
            j++;
        }
        return i;
    }
    return i;
}

/*@brief takes a hex string and convert it to a 32 bit number (max 8 hex digits).
 * @details This function will take a hex string and converts it to a 32 bit number (max 8 hex digits)
 * and returns it.
 * @param hex is the string passed that needs to be converted into a 32 bit number.
 * @return Returns a 32 bit number.
 */
unsigned int hex2int(char *hex) {
    unsigned int val = 0;
    while (*hex) {
        // get current character then increment
        unsigned int byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9') byte = byte - '0';
        else if (byte >= 'a' && byte <='f') byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <='F') byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}

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
int read_sample(int *fp,int bytes_per_sample){
    char* buff=(char*)fp;
    int c;
    int i;
    for(i=0;i<bytes_per_sample;i++){
        if((c=getchar())!=EOF)
            *(buff+bytes_per_sample-i-1)=c;
        else
            return 0;
    }

    return 1;
}

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
int write_sample(int *fp, int bytes_per_sample){
    char* buff=(char*)fp;
    for(int i=0;i<bytes_per_sample;i++){
        if(putchar(*(buff+bytes_per_sample-i-1))==EOF)
            return 0;
    }
    return 1;
}
