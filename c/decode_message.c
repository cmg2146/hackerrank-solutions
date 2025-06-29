// Solves: http://www.karrels.org/Ed/ACM/91/prob_f.html

#include <stdio.h>
#include <string.h>

#define MAX_KEY_LEN (7)
#define SEGMENT_START_LEN (3)

typedef unsigned int UINT;

static UINT get_number_of_keys(UINT key_len);
static UINT get_key_offset(UINT key_len);
static char* get_message_start(char* header);
static char* parse_binary(char* s, UINT* ret_val, UINT len);
static void decode_message(char* header, char* message);

static UINT get_number_of_keys(UINT key_len)
{
    return (1U << key_len) - 1;
}

static UINT get_key_offset(UINT key_len)
{
    UINT result = 0;
    for (UINT i = 1; i < key_len; i++)
    {
        result += get_number_of_keys(i);
    }

    return result;
}

static char* get_message_start(char* header)
{
    // header appears on a line by itself and the actual message will begin after the newline
    // character
    while ((*header++) != '\n');
    return header;
}

static char* parse_binary(char* s, UINT* ret_val, UINT len)
{
    char bit;
    UINT bit_val;

    *ret_val = 0;

    for (UINT index = 0; index < len; index++)
    {
        while (bit = *s++, bit == '\r');  // ignore carriage returns
        bit_val = (bit == '1') ? 1 : 0;
        *ret_val = ((*ret_val) * 2) + bit_val;
    }

    return s;
}

static void decode_message(char* header, char* message)
{
    UINT key_len;
    UINT key;

    // the start of each message segment is 3 bits indicating the key length.
    // If the 3 bits are all zeros (000 - key length = 0), there are no more segments and the
    // message has ended.
    while (message = parse_binary(message, &key_len, SEGMENT_START_LEN), key_len > 0)
    {
        UINT num_keys = get_number_of_keys(key_len);
        UINT key_offset = get_key_offset(key_len);

        // parse each key value in the segment. If the key value is equal to the number of keys
        // (all 1 bits), then the segment has ended and we move on to the next segment.
        while (message = parse_binary(message, &key, key_len), key < num_keys)
        {
            // print the value from the header by using the key as the lookup
            UINT lookup = key_offset + key;
            printf("%c", header[lookup]);
        }
    }
}

int main()
{
    // hard code the input for now
    #define NUM_ENTRIES (2)
    char* lines[NUM_ENTRIES] = {
        "TNM AEIOU\n0010101100011\r1010001001110110011\r11000",
        "$#**\\\n0100000101101100011100101000"
    };

    for (int entry_num = 0; entry_num < NUM_ENTRIES; entry_num++)
    {
        char* header = lines[entry_num];  // header begins at start of line
        char* message = get_message_start(header);

        // now decode the message
        decode_message(header, message);
        printf("\n");
    }
    
    return 0;
}
