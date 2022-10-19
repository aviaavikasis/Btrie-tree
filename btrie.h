#ifndef __ILRD_OL127_128_BINARY_TRIE_H__
#define __ILRD_OL127_128_BINARY_TRIE_H__

#include <stddef.h> /* size_t */
#include "utils.h" /* status_ty */
#include "dhcp.h"

typedef struct btrie btrie_ty;

btrie_ty *BTrieCreate(void);

void BTrieDestroy(btrie_ty *btrie);

status_ty BTrieInsert(btrie_ty *btrie, const char *expression);

status_ty BTrieRemove(btrie_ty *btrie, const char *expression);

size_t BTrieSize(const btrie_ty *btrie);

return_status_ty BTrieFindEqualOrLargeStr(btrie_ty *btrie, char *o_str, char*expression);

#endif /* __ILRD_OL127_128_BINARY_TRIE_H__ */
