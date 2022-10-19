#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "btrie.h"

void BTrieCreateAndDestroyTest(void);
void BTrieDestroyTest(void);
void BTrieInsertTest(void);
void BTrieRemoveTest(void);
void BTrieSizeTest(void);
void BTrieFindEqualOrLargeStrTest(void);


int main(void)
{
	BTrieCreateAndDestroyTest();
	BTrieInsertTest();

	BTrieRemoveTest();
	BTrieFindEqualOrLargeStrTest();
	BTrieSizeTest();
	printf("\n->->->->->-> success!! <-<-<-<-<-<-\n\n");	
	
	return 0;
}


void BTrieCreateAndDestroyTest(void)
{
	btrie_ty *btrie = BTrieCreate();

	BTrieDestroy(btrie);
}


void BTrieInsertTest(void)
{

	char *expression1 = "00010100101110110000000000000000";  
	char *expression2 = "00010100111110110000100011000000";
	char *expression3 = "00010100111000110000100011001100";
	char *equal_expression3 = "00010100111000110000100011001100";
	char *equal_expression1 = "00010100101110110000000000000000";  
	
	btrie_ty *btrie = BTrieCreate();
	
	assert(SUCCESS == BTrieInsert(btrie, expression1));
	assert(SUCCESS == BTrieInsert(btrie, expression2));
	assert(SUCCESS == BTrieInsert(btrie, expression3));
	assert(FAIL == BTrieInsert(btrie, equal_expression3));
	assert(FAIL == BTrieInsert(btrie, equal_expression1));

	BTrieDestroy(btrie);

}

void BTrieRemoveTest(void)
{
	char *expression1 = "00010100101110110000000000000000";  
	char *expression2 = "00010100111110110000100011000000";
	char *expression3 = "00010100111000110000100011001100";
	/*
	char *equal_expression3 = "00010100111000110000100011001100";
	char *equal_expression1 = "00010100101110110000000000000000";  
	*/
	btrie_ty *btrie = BTrieCreate();
	
	assert(SUCCESS == BTrieInsert(btrie, expression1));
	assert(SUCCESS == BTrieInsert(btrie, expression2));
	assert(SUCCESS == BTrieInsert(btrie, expression3));

	assert(SUCCESS == BTrieRemove(btrie, expression1));
	assert(SUCCESS == BTrieRemove(btrie, expression3));
	
	BTrieDestroy(btrie);

}


void BTrieFindEqualOrLargeStrTest(void)
{
	int i = 0;
	char *result = NULL;
	char *exp = "00000";

	btrie_ty *btrie = BTrieCreate();

	
	for( ; i < 32 ; ++i)
	{
		char *result = (char*)calloc(6, sizeof(char));
		BTrieFindEqualOrLargeStr(btrie, result, exp);
		printf("\n%s\n", result);	
		assert(SUCCESS == BTrieInsert(btrie, result));
		
		free(result);
	}
		
	BTrieDestroy(btrie);
}


void BTrieSizeTest(void)
{

	int i = 0;
	char *result = NULL;
	char *exp = "00000";

	btrie_ty *btrie = BTrieCreate();

	assert(0 == BTrieSize(btrie));
	
	for( ; i < 32 ; ++i)
	{
		char *result = (char*)calloc(6, sizeof(char));
		BTrieFindEqualOrLargeStr(btrie, result, exp);
		printf("\n%s\n", result);	
		assert(SUCCESS == BTrieInsert(btrie, result));
		
		free(result);
	}
	
	assert(32 == BTrieSize(btrie));
	assert(SUCCESS == BTrieRemove(btrie, "00011"));
	assert(31 == BTrieSize(btrie));	
	assert(SUCCESS == BTrieRemove(btrie, "00000"));
	assert(30 == BTrieSize(btrie));
		
	BTrieDestroy(btrie);


}























