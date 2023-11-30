//! @file
//! @author Aiden Munro

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ctype.h>
#include "textfuncs.h"
#include "errors.h"

void CreateText(Text* text, const char* filename, size_t sortmode)
{
    myAssert(text, NULL_PTR);

    text->size     = getSize(filename);
    text->buffer   = parseBuf(text, filename);
    // text->numTokens = countTokens(text);
    // text->lineptrs = getTokenPointers(text);
    // text->tokens    = getTokens(text);

    // generalSort(text, sortmode);
}

void AppendText(Text* text, const char* filename)
{
  myAssert(text, NULL_PTR);

  FILE* fp = fopen(filename, "a");

  for (size_t i = 0; i < text->numTokens; i++)
    {
      char* str = getToken(text, i);
      if (*str != '\r') //skip tokens with spaces
          fputs(str, fp);
    }
  fputs("------------------------------------------------\n", fp);

  fclose(fp);
}

void DestroyText(Text* text)
{
  // free((void*)text->lineptrs);
  free((void*)text->buffer);
  // free((void*)text->tokens);
}

char* const* getTokenPointers(Text *text)
{
    myAssert(text, NULL_PTR);

    char** lineptrs = (char**)calloc(text->numTokens, sizeof(char*));
    
    *lineptrs = text->buffer;

    char* textptr = strchr(text->buffer, ' '); // space for Differentiator

    while (textptr != NULL)
      {
        *textptr = '\0';
        lineptrs++;
        *lineptrs = textptr + 1;
        textptr = strchr(textptr + 1, ' '); 
      }

    lineptrs -= (text->numTokens - 1);

    return (char* const*)lineptrs;
}

char* parseBuf(Text* text, const char* filename)
{   
    myAssert(text, NULL_PTR);

    FILE* fp = fopen(filename, "rb");

    char* buffer = (char*)calloc(text->size + 2, sizeof(char));
    fread(buffer, sizeof(char), text->size, fp);
    buffer[text->size] = '\0';
  
    fclose(fp);

    return buffer;
}

size_t getSize(const char* filename)
{
    myAssert(filename, NULL_PTR);

    struct stat stats = {};
    stat(filename, &stats);

    return stats.st_size;
}

size_t countTokens(const Text* text)
{
    myAssert(text, NULL_PTR);

    size_t tokens = 1;
    for (size_t i = 0; i < text->size; i++)
        if (text->buffer[i] == ' ') // space Differentiator
            tokens++;

    return tokens;
}

char* getToken(Text* text, size_t numToken)
{
    myAssert(numToken < text->numTokens, INDEX_OUT_OF_RANGE);

    return text->tokens[numToken].string;
}

struct Token* getTokens(Text* text)
{
    myAssert(text, NULL_PTR);

    Token* tokens = (struct Token*)calloc(text->numTokens, sizeof(Token));

    for (size_t i = 0; i < text->numTokens; i++)
      {
        tokens[i].length = strlen(text->lineptrs[i]);
        tokens[i].string = text->lineptrs[i];
      }
    
    return tokens;
}

void generalSort(Text* text, size_t sortmode)
{
    myAssert(text, NULL_PTR);

    switch (sortmode)
    {
    case FORWARDS:
        quickSort((void*)text->tokens, 0, text->numTokens - 1, sizeof(struct Token), compareStringForw);
        break;
    
    case BACKWARDS:
        quickSort((void*)text->tokens, 0, text->numTokens - 1, sizeof(struct Token), compareStringBack);
        break;

    case NONE:
        break;

    default:
        break;
    }
}

void bubbleSort(void* array, size_t numElems, const size_t elemSize, compareFunc_t compareFunc)
{
    myAssert(array, NULL_PTR);
    myAssert(numElems, NULL_PTR);
    myAssert(elemSize, NULL_PTR);

    for (size_t i = 0; i < numElems - 1; i++)
      for (size_t j = 0; j < numElems - 1 - i; j++)
        {
          if (compareFunc(array + j * elemSize, array + (j + 1) * elemSize) > 0)
            {
              SWAP(array + j * elemSize,  array + (j + 1) * elemSize, elemSize);
            }
        }             
}

void quickSort(void* array, int start, int end, size_t elemSize, compareFunc_t compareFunc)
{
    myAssert(array, NULL_PTR);
    myAssert(elemSize, NULL_PTR);

    if (start >= end)
        return;

    int point = partition(array, start, end, elemSize, compareFunc);

    quickSort(array, start, point - 1, elemSize, compareFunc);
    quickSort(array, point + 1, end, elemSize, compareFunc);

}

int partition(void* array, int left, int right, size_t elemSize, compareFunc_t compareFunc)
{
    myAssert(array, NULL_PTR);

    int pivot = left;

    while (left < right)
      {
        while (compareFunc(array + left * elemSize, array + pivot * elemSize) < 0 && left < right)
            left++;

        while (compareFunc(array + right * elemSize, array + pivot * elemSize) >= 0 && left < right)
            right--;

        if (left < right)
            SWAP(array + left * elemSize, array + right * elemSize, elemSize);
      }
    
    SWAP(array + pivot * elemSize, array + right * elemSize, elemSize);

    return right;
}


int compareStringForw(const void* a, const void* b)
{
    myAssert(a, NULL_PTR);
    myAssert(b, NULL_PTR);
    
    struct Token* str1 = (Token*)a;
    struct Token* str2 = (Token*)b;

    char* strptr1 = str1->string;
    char* strptr2 = str2->string;

    char* fixptr1 = str1->string + str1->length - 1;
    char* fixptr2 = str2->string + str2->length - 1; 

    do
      {
        while(!isalpha(*strptr1))
            strptr1++;
        while(!isalpha(*strptr2))
            strptr2++;
        if (*strptr1 != *strptr2)
            return *strptr1 - *strptr2;
        else
          {
            strptr1++;
            strptr2++;
          }  
      } while(strptr1 < fixptr1 && strptr2 < fixptr2);
    
    return 0;
}

int compareStringBack(const void* a, const void* b)
{
    myAssert(a, NULL_PTR);
    myAssert(b, NULL_PTR);

    struct Token* str1 = (Token*)a;
    struct Token* str2 = (Token*)b;

    char* strptr1 = str1->string + str1->length - 1;
    char* strptr2 = str2->string + str2->length - 1; 

    char* fixptr1 = str1->string;
    char* fixptr2 = str2->string; 
  
    do 
      {
        while (!isalpha(*strptr1))
            strptr1--;
        while (!isalpha(*strptr2))
            strptr2--;
        if (*strptr1 != *strptr2)
            return *strptr1 - *strptr2;
        else
          {
            strptr1--;
            strptr2--;
          }
      } while(strptr1 > fixptr1 && strptr2 > fixptr2);
    
    return 0;
} 

size_t CheckFile(const char* filename)
{
    myAssert(filename, NULL_PTR);

    FILE* fp = fopen(filename, "rb");
        if (fp == NULL)
          {
            printf("Unable to open file: \"%s\"\n", filename);
            return INCORRECT;
          }

    fclose(fp);

    const char* dotptr = strchr(filename, (int)('.')) + 1;

    if(strcmp("txt", dotptr) || strcmp("rtf", dotptr))
        return CORRECT;
      
    return INCORRECT;
}

int StringIsEmpty(const Token* line)
{
    for (size_t i = 0; i < line->length; i++)
        if (!isspace(line->string[i]))
            return 0;
            
    return 1;
}