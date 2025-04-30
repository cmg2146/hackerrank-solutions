// Solves: https://www.hackerrank.com/challenges/querying-the-document/problem?isFullScreen=true

#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

static char*** paragraphs[MAX_PARAGRAPHS];
static char** sentences[MAX_CHARACTERS];
static char* words[MAX_CHARACTERS];

char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n);
char** kth_sentence_in_mth_paragraph(char**** document, int k, int m);
char*** kth_paragraph(char**** document, int k);
char**** get_document(char* text);

char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n) {
    char** sentence = kth_sentence_in_mth_paragraph(document, m, n);
    // input is 1 indexed
    return sentence[k-1];
}

char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) { 
    char*** paragraph = kth_paragraph(document, m);
    // input is 1 indexed
    return paragraph[k-1];
}

char*** kth_paragraph(char**** document, int k) {
    // input is 1 indexed
    return document[k-1];
}

char**** get_document(char* text) {
    char ch;
    int token_start = 0;
    int cur_idx = 0;
    int word_idx = 0;
    int sentence_idx = 0;
    int paragraph_idx = 0;
    
    // initialize first paragraph and sentence
    sentences[sentence_idx] = &words[word_idx];
    paragraphs[paragraph_idx] = &sentences[sentence_idx];

    while (1)
    {
        ch = text[cur_idx];
        if ((ch == ' ') || (ch == '.') || (ch == '\n') || (ch == '\0'))
        {
            // replace delimiter with null to mark end of token
            text[cur_idx] = '\0';

            // check for end of word, sentence, paragraph, document. assumes
            // sentences always end with a period, words always end with space
            // or period.
            if (ch == ' ')
            {
                words[word_idx++] = &text[token_start];
            }
            else if (ch == '.')
            {
                words[word_idx++] = &text[token_start];
                // prepare the next sentence
                sentences[++sentence_idx] = &words[word_idx];
            }
            else if (ch == '\n')
            {
                // prepare the next paragraph
                paragraphs[++paragraph_idx] = &sentences[sentence_idx];
            }   
            else if (ch == '\0')
            {
                break;
            }
            
            // move start of next token to the next character
            token_start = cur_idx + 1;
        }

        cur_idx++;
    }

    return paragraphs;
}
