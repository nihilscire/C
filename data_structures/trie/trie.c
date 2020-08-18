#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define ALPHABET_SIZE 26
#define INPUT_WORD_SIZE 100

struct trieNode
{
    struct trieNode *children[ALPHABET_SIZE];
    char character;
    int isEndOfWord;

};

struct trie
{
    struct trieNode* root;
    int n;
    int height;
};

typedef struct trieNode trieNode_t;
typedef struct trie trie_t;

static trieNode_t* newTrieNode(void);
static void deleteRecur(trieNode_t*);
static void insertWordRecur(trieNode_t*, char*);
static trieNode_t* searchWordRecur(trieNode_t*, char*);
static void searchByPrefixRecur(trieNode_t*, char*, int);
static void printFiller(char*, int);

trie_t* newTrie(void);
void deleteTrie(trie_t**);
void readTrieFromFile(trie_t*, const char*);
void insertWord(trie_t*, char*);
trieNode_t* searchWord(trie_t*, char*);
void searchByPrefix(trie_t*, char*);

static trieNode_t* newTrieNode()
{
    trieNode_t* node = (trieNode_t*) util_calloc(1, sizeof(trieNode_t));

    return node;
}

static void deleteRecur(trieNode_t* root)
{
    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
    {
        if(root->children[i] != NULL)
        {
            deleteRecur(root->children[i]);
        }
    }

    free(root);
}

static void insertWordRecur(trieNode_t* root, char* word)
{
    if (*word)
    {
        char character = *word;
        if (root->children[character - 'a'] == NULL)
        {
            trieNode_t* node = newTrieNode();
            node->character = character;
            root->children[character - 'a'] = node;
        }

        word++;
        insertWordRecur(root->children[character - 'a'], word);
    }
    else
    {
        root->isEndOfWord = 1;
    }

    return;
}

static trieNode_t* searchWordRecur(trieNode_t* root, char* word)
{
    while (*word)
    {
        char character = *word;
        if (root->children[character - 'a'] != NULL)
        {
            root = root->children[character - 'a'];
            word++;
        }
        else
        {
            printf("No possible words!!\n");
            return NULL;
        }
    
    }
    return root;
}

static void searchByPrefixRecur(trieNode_t* node, char* filler, int filledLen)
{
    if (node == NULL)
        return;

    filler[filledLen] = node->character;
    filledLen++;

    if (node->isEndOfWord)
    {
        printFiller(filler, filledLen);
    }

    int i;
    for (i = 0; i < ALPHABET_SIZE; i++)
    {
        if(node->children[i])
        {
            searchByPrefixRecur(node->children[i], filler, filledLen);
        }
    }
}

static void printFiller(char* filler, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {
        printf("%c", filler[i]);
    }
    printf("\n");
}

trie_t* newTrie()
{
    trie_t* t = (trie_t*)util_calloc(1, sizeof(trie_t));

    return t;
}

void deleteTrie(trie_t** t)
{
    deleteRecur((*t)->root);

    free(*t);

    *t = NULL;
}

void readTrieFromFile(trie_t* t, const char* filename)
{
    int n = 0;
    int i;

    char** words = NULL;

    FILE *fp = util_fopen(filename, "r");

    fscanf(fp, "%d", &n);

    t->n = n;
    t->root = (trieNode_t*)util_calloc(1, sizeof(trieNode_t));

    words = (char**) util_calloc(n, sizeof(char*));

    for (i = 0; i < n; ++i)
    {
        words[i] = (char*)util_calloc(INPUT_WORD_SIZE, sizeof(char));
        fscanf(fp, "%s", words[i]);
        insertWord(t, words[i]);
    }

    for (i = 0; i < n; i++)
    {
        free(words[i]);
    }

    free(words);

    fclose(fp);
}

void insertWord(trie_t* trie, char* word)
{
    int len = strlen(word);

    if(trie->height < len)
        trie->height = len;

    insertWordRecur(trie->root, word);
}

trieNode_t* searchWord(trie_t* trie, char* word)
{
    return searchWordRecur(trie->root, word);
}

void searchByPrefix(trie_t* trie, char* prefix)
{
    trieNode_t* tmp = searchWord(trie, prefix);

    if(tmp == NULL)
    {
        fprintf(stdout, "prefix not found!\n");
        return;
    }
    
    int len = strlen(prefix);
    
    char* filler = (char*)util_calloc(trie->height, sizeof(char));

    strncpy(filler, prefix, len*sizeof(char));

    searchByPrefixRecur(tmp, filler, len - 1);

    free(filler);
}

static void printDFS(trieNode_t* root)
{
    fprintf(stdout, "%c ", root->character);
    if(root->isEndOfWord)
        fprintf(stdout, "\n");

    int i;
    for(i = 0; i < ALPHABET_SIZE; ++i)
    {
        if(root->children[i] != NULL)
        {
            printDFS(root->children[i]);
        }
    }
}

int main()
{
    trie_t* trie = newTrie();

    readTrieFromFile(trie, "dictionary.txt");

    printDFS(trie->root);

    searchByPrefix(trie, "aba");

    deleteTrie(&trie);

    return 0;
}