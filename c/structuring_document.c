// Solves: https://www.hackerrank.com/challenges/structuring-the-document/problem?isFullScreen=true

// START HACKERRANK PROVIDED CODE: CANT EDIT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

struct word {
    char* data;
};

struct sentence {
    struct word* data;
    int word_count;//denotes number of words in a sentence
};

struct paragraph {
    struct sentence* data  ;
    int sentence_count;//denotes number of sentences in a paragraph
};

struct document {
    struct paragraph* data;
    int paragraph_count;//denotes number of paragraphs in a document
};
// END HACKERRANK PROVIDED CODE: CANT EDIT

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n);
struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m);
struct paragraph kth_paragraph(struct document Doc, int k);

static struct paragraph paragraphs[MAX_PARAGRAPHS];
static struct sentence sentences[MAX_CHARACTERS];
static struct word words[MAX_CHARACTERS];

struct document get_document(char* text) {
    char ch;
    int token_start = 0;
    int cur_idx = 0;
    int word_idx = 0;
    int sentence_idx = 0;
    int paragraph_idx = 0;
    struct document result = {paragraphs, 0};

    // initialize first paragraph and sentence.
    sentences[sentence_idx].data = &words[word_idx];
    sentences[sentence_idx].word_count = 0;
    paragraphs[paragraph_idx].data = &sentences[sentence_idx];
    paragraphs[paragraph_idx].sentence_count = 0;

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
                words[word_idx++].data = &text[token_start];
                sentences[sentence_idx].word_count++;
            }
            else if (ch == '.')
            {
                words[word_idx++].data = &text[token_start];
                sentences[sentence_idx].word_count++;
                paragraphs[paragraph_idx].sentence_count++;
                // prepare the next sentence
                sentences[++sentence_idx].data = &words[word_idx];  
            }
            else if (ch == '\n')
            {
                // prepare the next paragraph
                paragraphs[++paragraph_idx].data = &sentences[sentence_idx];
                result.paragraph_count++;
            }   
            else if (ch == '\0')
            {
                result.paragraph_count++;
                break;
            }
            
            // move start of next token to the next character
            token_start = cur_idx + 1;
        }

        cur_idx++;
    }

    return result;
}

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n) {
    struct sentence sen = kth_sentence_in_mth_paragraph(Doc, m, n);
    // input is 1 indexed
    return sen.data[k-1];
}

struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m) { 
    struct paragraph para = kth_paragraph(Doc, m);
    // input is 1 indexed
    return para.data[k-1];
}

struct paragraph kth_paragraph(struct document Doc, int k) {
    // input is 1 indexed
    return Doc.data[k-1];
}
