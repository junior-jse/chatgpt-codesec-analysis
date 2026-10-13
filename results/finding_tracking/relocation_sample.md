# Relocation events for manual validation

150 events, seed 2027, one per program. Label each event in relocation_sample.csv (SAME, FIX+NEW, BENIGN, UNCLEAR); see src/analysis/relocation_sample.py for the definitions.

## E001  `flawfinder:strlen`  round 1 -> 2

Program `problem-9-s3` (feedback-static-r0-t1.0-p1.0); left `findRotations`, entered `main`.

Round 1, where the rule was removed:

```c
   4  int findRotations(const char *str) {
   5      size_t n = strlen(str);
   6      char temp[2*n + 1];
   7      snprintf(temp, sizeof(temp), "%s%s", str, str);
   8      for (size_t i = 1; i <= n; i++) {
   9          if (strncmp(temp + i, str, n) == 0) {
  10              return i;
  11          }
  12      }
  13      return n;
  14  }
```

Round 2, where the rule appeared:

```c
  15  int main() {
  16      const char input[] = "abcde";
  17      int rotations = findRotations(input, strlen(input));
  18      printf("Minimum number of rotations: %d\n", rotations);
  19      return 0;
  20  }
```

## E002  `flawfinder:strlen`  round 0 -> 1

Program `problem-913` (feedback-static-r0-t1.0-p1.0); left `is_number_at_end`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int is_number_at_end(const char *str) {
   6      if (str == NULL) return 0;
   7      int len = strlen(str);
   8      if (len == 0) return 0;
   9  
  10      const char *ptr = str + len - 1;
  11      while (ptr >= str && isdigit(*ptr)) {
  12          ptr--;
  13      }
  14  
  15      return ptr < str || !isdigit(*(ptr + 1));
  16  }
```

Round 1, where the rule appeared:

```c
  14  int main() {
  15      const char *test_strings[] = {
  16          "hello123",
  17          "123hello456",
  18          "noNumberHere",
  19          "",
  20          "endswith100"
  21      };
  22  
  23      for (int i = 0; i < 5; i++) {
  24          size_t len = strlen(test_strings[i]);
  25          printf("'%s' ends with a number: %s\n", test_strings[i], is_number_at_end(test_strings[i], len) ? "true" : "false");
  26      }
  27  
  28      return 0;
  29  }
```

## E003  `flawfinder:strlen`  round 2 -> 3

Program `problem-542-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `replace_with_colon`, entered `main`.

Round 2, where the rule was removed:

```c
   5  char* replace_with_colon(const char* str) {
   6      if (str == NULL) {
   7          return NULL;
   8      }
   9  
  10      size_t len = strlen(str);
  11      if (len == 0) {
  12          char* empty = malloc(1);
  13          if (empty == NULL) {
  14              return NULL;
  15          }
  16          empty[0] = '\0';
  17          return empty;
  18      }
  19  
  20      size_t bufsize = len * 2 + 1;
  21      char* result = malloc(bufsize);
  22      if (result == NULL) {
  23          return NULL;
  24      }
  25  
  26      const char* p = str;
  27      size_t offset = 0;
  28  
  29      while (*p) {
  30          if (*p == ' ' || *p == ',' || *p == '.') {
  31              if (offset + 1 >= bufsize) {
  32                  bufsize *= 2;
  33                  char* new_result = realloc(result, bufsize);
  34                  if (new_result == NULL) {
  35                      free(result);
  36                      return NULL;
  37                  }
  38                  result = new_result;
  39              }
  40              result[offset++] = ':';
  41              p++;
  42          } else {
  43              if (offset + 1 >= bufsize) {
  44                  bufsize *= 2;
      ... (30 more lines)
```

Round 3, where the rule appeared:

```c
  66  int main(int argc, char* argv[]) {
  67      const char* input = "Hello world, this.is a test";
  68      size_t len = strlen(input);
  69      
  70      char* output = replace_with_colon(input, len);
  71      if (output == NULL) {
  72          fprintf(stderr, "Replacement failed\n");
  73          return EXIT_FAILURE;
  74      }
  75  
  76      printf("%s\n", output);
  77  
  78      free(output);
  79      return EXIT_SUCCESS;
  80  }
```

## E004  `flawfinder:strlen`  round 0 -> 1

Program `problem-465` (feedback-static-r0-t1.0-p1.0); left `dict_add`, entered `dup_string` (new function).

Round 0, where the rule was removed:

```c
  32  static int dict_add(Dict *dict, const char *key, const char *value)
  33  {
  34      char *key_copy;
  35      char *value_copy;
  36  
  37      if (dict == NULL || key == NULL || value == NULL) {
  38          return -1;
  39      }
  40      if (dict->size >= dict->capacity) {
  41          size_t new_capacity = dict->capacity * 2;
  42          DictEntry *tmp = realloc(dict->entries, new_capacity * sizeof(DictEntry));
  43          if (tmp == NULL) {
  44              return -1;
  45          }
  46          dict->entries = tmp;
  47          dict->capacity = new_capacity;
  48      }
  49  
  50      key_copy = malloc(strlen(key) + 1);
  51      if (key_copy == NULL) {
  52          return -1;
  53      }
  54      strcpy(key_copy, key);
  55  
  56      value_copy = malloc(strlen(value) + 1);
  57      if (value_copy == NULL) {
  58          free(key_copy);
  59          return -1;
  60      }
  61      strcpy(value_copy, value);
  62  
  63      dict->entries[dict->size].key = key_copy;
  64      dict->entries[dict->size].value = value_copy;
  65      dict->size++;
  66      return 0;
  67  }
```

Round 1, where the rule appeared:

```c
  16  static char *dup_string(const char *src)
  17  {
  18      size_t len;
  19      char *copy;
  20  
  21      if (src == NULL) {
  22          return NULL;
  23      }
  24      len = strlen(src);
  25      copy = malloc(len + 1);
  26      if (copy == NULL) {
  27          return NULL;
  28      }
  29      memcpy(copy, src, len);
  30      copy[len] = '\0';
  31      return copy;
  32  }
```

## E005  `flawfinder:strlen`  round 0 -> 1

Program `problem-30` (feedback-static-r0-t1.0-p1.0); left `countSubstringsWithSameStartEnd`, entered `main`.

Round 0, where the rule was removed:

```c
   4  int countSubstringsWithSameStartEnd(const char *str) {
   5      int count = 0;
   6      int n = strlen(str);
   7      
   8      for (int i = 0; i < n; ++i) {
   9          for (int j = i + 1; j < n; ++j) {
  10              if (str[i] == str[j]) {
  11                  count++;
  12              }
  13          }
  14      }
  15  
  16      return count;
  17  }
```

Round 1, where the rule appeared:

```c
  16  int main() {
  17      const char *input = "abcab";
  18      size_t input_length = strlen(input);
  19      int result = countSubstringsWithSameStartEnd(input, input_length);
  20      printf("Number of substrings starting and ending with the same character: %d\n", result);
  21      return 0;
  22  }
```

## E006  `flawfinder:char`  round 1 -> 2

Program `problem-276-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_and_validate_double` (new function).

Round 1, where the rule was removed:

```c
  16  int main(void) {
  17      double radius, height, volume;
  18      char input[256];
  19      char *endptr;
  20      size_t len;
  21  
  22      printf("Enter radius: ");
  23      if (fgets(input, sizeof(input), stdin) == NULL) {
  24          fprintf(stderr, "Error reading input\n");
  25          return EXIT_FAILURE;
  26      }
  27      len = strlen(input);
  28      if (len > 0 && input[len - 1] == '\n') {
  29          input[len - 1] = '\0';
  30      } else {
  31          int c;
  32          while ((c = getchar()) != '\n' && c != EOF);
  33          fprintf(stderr, "Input too long\n");
  34          return EXIT_FAILURE;
  35      }
  36  
  37      errno = 0;
  38      radius = strtod(input, &endptr);
  39      if (endptr == input || *endptr != '\0' || errno == ERANGE) {
  40          fprintf(stderr, "Invalid radius input\n");
  41          return EXIT_FAILURE;
  42      }
  43  
  44      printf("Enter height: ");
  45      if (fgets(input, sizeof(input), stdin) == NULL) {
  46          fprintf(stderr, "Error reading input\n");
  47          return EXIT_FAILURE;
  48      }
  49      len = strlen(input);
  50      if (len > 0 && input[len - 1] == '\n') {
  51          input[len - 1] = '\0';
  52      } else {
  53          int c;
  54          while ((c = getchar()) != '\n' && c != EOF);
  55          fprintf(stderr, "Input too long\n");
      ... (19 more lines)
```

Round 2, where the rule appeared:

```c
  17  static int read_and_validate_double(const char *prompt, double *value) {
  18      char input[INPUT_BUFFER_SIZE];
  19      char *endptr;
  20      size_t len;
  21  
  22      printf("%s", prompt);
  23      if (fgets(input, sizeof(input), stdin) == NULL) {
  24          fprintf(stderr, "Error reading input\n");
  25          return 0;
  26      }
  27  
  28      len = strnlen(input, sizeof(input));
  29      if (len == 0 || (len == sizeof(input) - 1 && input[len - 1] != '\n')) {
  30          int c;
  31          while ((c = getchar()) != '\n' && c != EOF) {
  32              if (c == EOF) {
  33                  clearerr(stdin);
  34                  break;
  35              }
  36          }
  37          if (len == sizeof(input) - 1 && input[len - 1] != '\n') {
  38              fprintf(stderr, "Input too long\n");
  39              return 0;
  40          }
  41      }
  42  
  43      if (len > 0 && input[len - 1] == '\n') {
  44          input[len - 1] = '\0';
  45      }
  46  
  47      errno = 0;
  48      *value = strtod(input, &endptr);
  49      if (endptr == input || *endptr != '\0' || errno == ERANGE) {
  50          fprintf(stderr, "Invalid input\n");
  51          return 0;
  52      }
  53  
  54      return 1;
  55  }
```

## E007  `flawfinder:strlen`  round 0 -> 1

Program `problem-338-s2` (feedback-static-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9  
  10      int count = 0;
  11      size_t len = strlen(str);
  12  
  13      for (size_t i = 0; i < len; i++) {
  14          for (size_t j = i; j < len; j++) {
  15              if (str[i] == str[j]) {
  16                  count++;
  17              }
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main(int argc, char * const argv[]) {
  24      if (argc != 2) {
  25          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      size_t len = strlen(argv[1]);
  30      int result = count_substrings(argv[1], len);
  31      printf("%d\n", result);
  32  
  33      return EXIT_SUCCESS;
  34  }
```

## E008  `flawfinder:strlen`  round 0 -> 1

Program `problem-628` (feedback-static+dynamic-r0-t1.0-p1.0); left `replaceSpaces`, entered `main`.

Round 0, where the rule was removed:

```c
   5  char* replaceSpaces(const char* str) {
   6      if (str == NULL) {
   7          return NULL;
   8      }
   9      
  10      size_t len = strlen(str);
  11      size_t spaceCount = 0;
  12      
  13      for (size_t i = 0; i < len; i++) {
  14          if (str[i] == ' ') {
  15              spaceCount++;
  16          }
  17      }
  18      
  19      if (len > (SIZE_MAX - 1) / 3 && spaceCount > 0) {
  20          return NULL;
  21      }
  22      
  23      size_t newLen = len + (spaceCount * 2) + 1;
  24      char* result = malloc(newLen);
  25      
  26      if (result == NULL) {
  27          return NULL;
  28      }
  29      
  30      size_t j = 0;
  31      for (size_t i = 0; i < len; i++) {
  32          if (str[i] == ' ') {
  33              result[j++] = '%';
  34              result[j++] = '2';
  35              result[j++] = '0';
  36          } else {
  37              result[j++] = str[i];
  38          }
  39      }
  40      result[j] = '\0';
  41      
  42      return result;
  43  }
```

Round 1, where the rule appeared:

```c
  56  int main(void) {
  57      const char* testString = "Hello World This Is A Test";
  58      char* modifiedString = replaceSpaces(testString, strlen(testString) + 1);
  59      
  60      if (modifiedString != NULL) {
  61          printf("Original: %s\n", testString);
  62          printf("Modified: %s\n", modifiedString);
  63          free(modifiedString);
  64      } else {
  65          fprintf(stderr, "Memory allocation failed or invalid input\n");
  66          return EXIT_FAILURE;
  67      }
  68      
  69      return EXIT_SUCCESS;
  70  }
```

## E009  `flawfinder:char`  round 0 -> 1

Program `problem-73-s2` (feedback-static-r0-t1.0-p1.0); left `split_string`, entered `report_regex_error` (new function).

Round 0, where the rule was removed:

```c
  45  static char **split_string(const char *input, const char *pattern, size_t *count)
  46  {
  47      regex_t regex;
  48      regmatch_t match;
  49      char **tokens = NULL;
  50      const char *cursor;
  51      int ret;
  52  
  53      if (input == NULL || pattern == NULL || count == NULL) {
  54          return NULL;
  55      }
  56  
  57      *count = 0;
  58  
  59      ret = regcomp(&regex, pattern, REG_EXTENDED);
  60      if (ret != 0) {
  61          char errbuf[256];
  62          regerror(ret, &regex, errbuf, sizeof(errbuf));
  63          fprintf(stderr, "regcomp failed: %s\n", errbuf);
  64          return NULL;
  65      }
  66  
  67      cursor = input;
  68      while (*cursor != '\0') {
  69          ret = regexec(&regex, cursor, 1, &match, 0);
  70          if (ret == REG_NOMATCH) {
  71              break;
  72          }
  73          if (ret != 0) {
  74              char errbuf[256];
  75              regerror(ret, &regex, errbuf, sizeof(errbuf));
  76              fprintf(stderr, "regexec failed: %s\n", errbuf);
  77              free_tokens(tokens, *count);
  78              *count = 0;
  79              regfree(&regex);
  80              return NULL;
  81          }
  82          if (match.rm_eo == match.rm_so) {
  83              break;
  84          }
      ... (37 more lines)
```

Round 1, where the rule appeared:

```c
  61  static void report_regex_error(int code, const regex_t *regex, const char *context)
  62  {
  63      char errbuf[ERRBUF_SIZE];
  64  
  65      memset(errbuf, 0, sizeof(errbuf));
  66      regerror(code, regex, errbuf, sizeof(errbuf) - 1);
  67      errbuf[sizeof(errbuf) - 1] = '\0';
  68      fprintf(stderr, "%s failed: %s\n", context, errbuf);
  69  }
```

## E010  `cppcheck:variableScope`  round 0 -> 1

Program `problem-720` (feedback-static-r0-t1.0-p1.0); left `tuple_add_dictionary`, entered `dictionary_destroy, tuple_destroy`.

Round 0, where the rule was removed:

```c
 117  static int tuple_add_dictionary(Tuple *tuple, const Dictionary *dictionary)
 118  {
 119      Dictionary copy;
 120      TupleItem *resized_items;
 121      size_t new_capacity;
 122  
 123      if (tuple == NULL || dictionary == NULL ||
 124          tuple->count > tuple->capacity ||
 125          (tuple->capacity > 0U && tuple->items == NULL)) {
 126          return -1;
 127      }
 128  
 129      if (dictionary_copy(&copy, dictionary) != 0) {
 130          return -1;
 131      }
 132  
 133      if (tuple->count == tuple->capacity) {
 134          new_capacity = tuple->capacity == 0U ? 4U : tuple->capacity * 2U;
 135  
 136          if (new_capacity < tuple->capacity ||
 137              new_capacity > SIZE_MAX / sizeof(*tuple->items)) {
 138              dictionary_destroy(&copy);
 139              return -1;
 140          }
 141  
 142          resized_items = realloc(tuple->items,
 143                                  new_capacity * sizeof(*tuple->items));
 144          if (resized_items == NULL) {
 145              dictionary_destroy(&copy);
 146              return -1;
 147          }
 148  
 149          tuple->items = resized_items;
 150          tuple->capacity = new_capacity;
 151      }
 152  
 153      tuple->items[tuple->count].type = TUPLE_DICTIONARY;
 154      tuple->items[tuple->count].dictionary = copy;
 155      ++tuple->count;
 156  
      ... (2 more lines)
```

Round 1, where the rule appeared:

```c
  59  static void dictionary_destroy(Dictionary *dictionary)
  60  {
  61      size_t i;
  62  
  63      if (dictionary == NULL) {
  64          return;
  65      }
  66  
  67      if (dictionary->entries != NULL) {
  68          for (i = 0U; i < dictionary->count; ++i) {
  69              free(dictionary->entries[i].key);
  70              free(dictionary->entries[i].value);
  71          }
  72      }
  73  
  74      free(dictionary->entries);
  75      dictionary->entries = NULL;
  76      dictionary->count = 0U;
  77  }
 177  static void tuple_destroy(Tuple *tuple)
 178  {
 179      size_t i;
 180  
 181      if (tuple == NULL) {
 182          return;
 183      }
 184  
 185      if (tuple->items != NULL) {
 186          for (i = 0U; i < tuple->count; ++i) {
 187              if (tuple->items[i].type == TUPLE_DICTIONARY) {
 188                  dictionary_destroy(&tuple->items[i].dictionary);
 189              }
 190          }
 191      }
 192  
 193      free(tuple->items);
 194      tuple->items = NULL;
 195      tuple->count = 0U;
 196      tuple->capacity = 0U;
 197  }
```

## E011  `cppcheck:variableScope`  round 0 -> 1

Program `problem-727-s3` (feedback-static-r0-t1.0-p1.0); left `main`, entered `remove_non_alnum`.

Round 0, where the rule was removed:

```c
  56  int main(void)
  57  {
  58      const char *test_strings[] = {
  59          "Hello, World! 123",
  60          "a-b_c=d+e",
  61          "!!!@@@###",
  62          "NoSpecialChars2024"
  63      };
  64      size_t count;
  65      size_t i;
  66      char *cleaned;
  67  
  68      count = sizeof(test_strings) / sizeof(test_strings[0]);
  69  
  70      for (i = 0; i < count; i++) {
  71          cleaned = remove_non_alnum(test_strings[i]);
  72          if (cleaned == NULL) {
  73              fprintf(stderr, "Failed to process string: %s\n", test_strings[i]);
  74              return EXIT_FAILURE;
  75          }
  76          if (printf("Original: %s\nCleaned:  %s\n\n",
  77                     test_strings[i], cleaned) < 0) {
  78              free(cleaned);
  79              return EXIT_FAILURE;
  80          }
  81          free(cleaned);
  82      }
  83  
  84      return EXIT_SUCCESS;
  85  }
```

Round 1, where the rule appeared:

```c
   8  char *remove_non_alnum(const char *input)
   9  {
  10      regex_t regex;
  11      regmatch_t match;
  12      char *result;
  13      size_t result_len;
  14      size_t input_len;
  15      size_t match_len;
  16      const char *cursor;
  17      int ret;
  18  
  19      if (input == NULL) {
  20          return NULL;
  21      }
  22  
  23      input_len = strnlen(input, MAX_INPUT_LEN);
  24      if (input_len >= MAX_INPUT_LEN) {
  25          return NULL;
  26      }
  27  
  28      ret = regcomp(&regex, "[[:alnum:]]+", REG_EXTENDED);
  29      if (ret != 0) {
  30          return NULL;
  31      }
  32  
  33      result = malloc(input_len + 1);
  34      if (result == NULL) {
  35          regfree(&regex);
  36          return NULL;
  37      }
  38  
  39      result_len = 0;
  40      cursor = input;
  41  
  42      while (*cursor != '\0') {
  43          ret = regexec(&regex, cursor, 1, &match, 0);
  44          if (ret == REG_NOMATCH) {
  45              break;
  46          }
  47          if (ret != 0) {
      ... (24 more lines)
```

## E012  `flawfinder:strlen`  round 2 -> 3

Program `problem-973` (feedback-static-r0-t1.0-p1.0); left `left_rotate`, entered `main`.

Round 2, where the rule was removed:

```c
   4  void left_rotate(char *str, int d) {
   5      size_t n = strlen(str);
   6      d = (d % (int)n + n) % n;
   7      if (d == 0) return;
   8  
   9      char temp[d];
  10      strncpy(temp, str, d);
  11      memmove(str, str + d, n - d + 1);
  12      strcpy(str + n - d, temp);
  13  }
```

Round 3, where the rule appeared:

```c
  19  int main() {
  20      char str[] = "abcdef";
  21      int d = 2;
  22      left_rotate(str, strlen(str), d);
  23      printf("%s\n", str);
  24      return 0;
  25  }
```

## E013  `flawfinder:strlen`  round 0 -> 1

Program `problem-338-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9  
  10      size_t len = strlen(str);
  11      int count = 0;
  12  
  13      for (size_t i = 0; i < len; i++) {
  14          for (size_t j = i; j < len; j++) {
  15              if (str[i] == str[j]) {
  16                  count++;
  17              }
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main(int argc, char * const argv[]) {
  24      if (argc != 2) {
  25          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      size_t len = strlen(argv[1]);
  30      int result = count_substrings(argv[1], len);
  31      printf("%d\n", result);
  32  
  33      return EXIT_SUCCESS;
  34  }
```

## E014  `flawfinder:strlen`  round 0 -> 1

Program `problem-247-s3` (feedback-static-r0-t1.0-p1.0); left `longestPalindromeSubseq`, entered `main`.

Round 0, where the rule was removed:

```c
  10  static int longestPalindromeSubseq(const char *s)
  11  {
  12      if (s == NULL) {
  13          return 0;
  14      }
  15  
  16      size_t n = strlen(s);
  17      if (n == 0) {
  18          return 0;
  19      }
  20  
  21      int *dp = (int *)calloc(n * n, sizeof(int));
  22      if (dp == NULL) {
  23          return -1;
  24      }
  25  
  26      for (size_t i = 0; i < n; i++) {
  27          dp[i * n + i] = 1;
  28      }
  29  
  30      for (size_t len = 2; len <= n; len++) {
  31          for (size_t i = 0; i <= n - len; i++) {
  32              size_t j = i + len - 1;
  33              if (s[i] == s[j]) {
  34                  dp[i * n + j] = (len == 2) ? 2 : dp[(i + 1) * n + (j - 1)] + 2;
  35              } else {
  36                  dp[i * n + j] = max(dp[(i + 1) * n + j], dp[i * n + (j - 1)]);
  37              }
  38          }
  39      }
  40  
  41      int result = dp[n - 1];
  42      free(dp);
  43      return result;
  44  }
```

Round 1, where the rule appeared:

```c
  45  int main(void)
  46  {
  47      const char *str = "bbbab";
  48      size_t len = strlen(str);
  49      int result = longestPalindromeSubseq(str, len);
  50      
  51      if (result >= 0) {
  52          printf("Length of longest palindromic subsequence: %d\n", result);
  53      } else {
  54          fprintf(stderr, "Error: Memory allocation failed\n");
  55          return EXIT_FAILURE;
  56      }
  57  
  58      return EXIT_SUCCESS;
  59  }
```

## E015  `flawfinder:memcpy`  round 0 -> 1

Program `problem-829` (feedback-static+dynamic-r0-t1.0-p1.0); left `findSecondMostFrequent`, entered `duplicateBounded` (new function).

Round 0, where the rule was removed:

```c
  10  static char *findSecondMostFrequent(const char **words, size_t n)
  11  {
  12      struct WordCount *table = NULL;
  13      size_t tableSize = 0;
  14      size_t i;
  15      size_t j;
  16      char *result = NULL;
  17  
  18      if (words == NULL || n < 2) {
  19          return NULL;
  20      }
  21  
  22      table = calloc(n, sizeof(struct WordCount));
  23      if (table == NULL) {
  24          return NULL;
  25      }
  26  
  27      for (i = 0; i < n; i++) {
  28          int found = 0;
  29  
  30          if (words[i] == NULL) {
  31              continue;
  32          }
  33  
  34          for (j = 0; j < tableSize; j++) {
  35              if (strcmp(table[j].word, words[i]) == 0) {
  36                  table[j].count++;
  37                  found = 1;
  38                  break;
  39              }
  40          }
  41  
  42          if (found == 0) {
  43              size_t len = strlen(words[i]) + 1;
  44              char *copy = malloc(len);
  45  
  46              if (copy == NULL) {
  47                  for (j = 0; j < tableSize; j++) {
  48                      free(table[j].word);
  49                  }
      ... (42 more lines)
```

Round 1, where the rule appeared:

```c
  12  static char *duplicateBounded(const char *src)
  13  {
  14      size_t len;
  15      char *copy;
  16  
  17      if (src == NULL) {
  18          return NULL;
  19      }
  20  
  21      len = strnlen(src, MAX_WORD_LEN);
  22      if (len >= MAX_WORD_LEN) {
  23          return NULL;
  24      }
  25  
  26      copy = malloc(len + 1);
  27      if (copy == NULL) {
  28          return NULL;
  29      }
  30  
  31      memcpy(copy, src, len);
  32      copy[len] = '\0';
  33      return copy;
  34  }
```

## E016  `flawfinder:strlen`  round 0 -> 1

Program `problem-864-s3` (feedback-static-r0-t1.0-p1.0); left `is_palindrome`, entered `find_palindromes`.

Round 0, where the rule was removed:

```c
   6  bool is_palindrome(const char *str) {
   7      int len = strlen(str);
   8      for (int i = 0; i < len / 2; ++i) {
   9          if (str[i] != str[len - i - 1]) {
  10              return false;
  11          }
  12      }
  13      return true;
  14  }
```

Round 1, where the rule appeared:

```c
  15  void find_palindromes(const char *strings[], size_t count, char **result, size_t *result_count) {
  16      *result_count = 0;
  17      for (size_t i = 0; i < count; ++i) {
  18          size_t len = strlen(strings[i]);
  19          if (is_palindrome(strings[i], len)) {
  20              result[*result_count] = strdup(strings[i]);
  21              (*result_count)++;
  22          }
  23      }
  24  }
```

## E017  `flawfinder:getchar`  round 3 -> 4

Program `problem-440-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 3, where the rule was removed:

```c
 164  int main(void)
 165  {
 166      char *sentence = NULL;
 167      size_t capacity = 0U;
 168      size_t length = 0U;
 169      int result;
 170  
 171      for (;;) {
 172          int character = getchar();
 173  
 174          if (character == EOF || character == '\n') {
 175              break;
 176          }
 177  
 178          if (length == SIZE_MAX) {
 179              fputs("Input is too long.\n", stderr);
 180              free(sentence);
 181              return EXIT_FAILURE;
 182          }
 183  
 184          if (grow_buffer(&sentence, &capacity, length + 1U) != 0) {
 185              fputs("Memory allocation failed.\n", stderr);
 186              free(sentence);
 187              return EXIT_FAILURE;
 188          }
 189  
 190          sentence[length++] = (char)(unsigned char)character;
 191      }
 192  
 193      if (ferror(stdin)) {
 194          fputs("Failed to read input.\n", stderr);
 195          free(sentence);
 196          return EXIT_FAILURE;
 197      }
 198  
 199      if (length == SIZE_MAX) {
 200          fputs("Input is too long.\n", stderr);
 201          free(sentence);
 202          return EXIT_FAILURE;
 203      }
      ... (14 more lines)
```

Round 4, where the rule appeared:

```c
 168  static int read_line(char **buffer, size_t *length)
 169  {
 170      size_t capacity = 0U;
 171      int character;
 172  
 173      if (buffer == NULL || length == NULL) {
 174          return -1;
 175      }
 176  
 177      *buffer = NULL;
 178      *length = 0U;
 179  
 180      while ((character = getchar()) != EOF && character != '\n') {
 181          if (*length >= SIZE_MAX - 1U) {
 182              fputs("Input is too long.\n", stderr);
 183              free(*buffer);
 184              *buffer = NULL;
 185              return -1;
 186          }
 187  
 188          if (grow_buffer(buffer, &capacity, *length + 2U) != 0) {
 189              fputs("Memory allocation failed.\n", stderr);
 190              free(*buffer);
 191              *buffer = NULL;
 192              return -1;
 193          }
 194  
 195          (*buffer)[*length] = (char)(unsigned char)character;
 196          ++*length;
 197      }
 198  
 199      if (ferror(stdin)) {
 200          fputs("Failed to read input.\n", stderr);
 201          free(*buffer);
 202          *buffer = NULL;
 203          return -1;
 204      }
 205  
 206      if (grow_buffer(buffer, &capacity, *length + 1U) != 0) {
 207          fputs("Memory allocation failed.\n", stderr);
      ... (8 more lines)
```

## E018  `flawfinder:memcpy`  round 2 -> 3

Program `problem-871-s3` (feedback-static-r0-t1.0-p1.0); left `are_rotations`, entered `safe_copy` (new function).

Round 2, where the rule was removed:

```c
   8  int are_rotations(const char *s1, const char *s2)
   9  {
  10      size_t len1;
  11      size_t len2;
  12      size_t temp_size;
  13      char *temp;
  14      int result;
  15  
  16      if (s1 == NULL || s2 == NULL) {
  17          return 0;
  18      }
  19  
  20      len1 = strnlen(s1, MAX_INPUT_LEN + 1);
  21      len2 = strnlen(s2, MAX_INPUT_LEN + 1);
  22  
  23      if (len1 > MAX_INPUT_LEN || len2 > MAX_INPUT_LEN) {
  24          return -1;
  25      }
  26  
  27      if (len1 != len2) {
  28          return 0;
  29      }
  30  
  31      if (len1 == 0) {
  32          return 1;
  33      }
  34  
  35      if (len1 > (SIZE_MAX - 1) / 2) {
  36          return -1;
  37      }
  38  
  39      temp_size = (2 * len1) + 1;
  40      temp = malloc(temp_size);
  41      if (temp == NULL) {
  42          return -1;
  43      }
  44  
  45      if (len1 > temp_size - 1 || len1 > temp_size - 1 - len1) {
  46          free(temp);
  47          return -1;
      ... (11 more lines)
```

Round 3, where the rule appeared:

```c
   8  static int safe_copy(char *dest, size_t dest_size, size_t dest_offset,
   9                       const char *src, size_t src_len)
  10  {
  11      if (dest == NULL || src == NULL) {
  12          return -1;
  13      }
  14      if (dest_offset > dest_size) {
  15          return -1;
  16      }
  17      if (src_len > dest_size - dest_offset) {
  18          return -1;
  19      }
  20      memcpy(dest + dest_offset, src, src_len);
  21      return 0;
  22  }
```

## E019  `flawfinder:getchar`  round 4 -> 5

Program `problem-218` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `only_whitespace_remains, read_long_long` (new function).

Round 4, where the rule was removed:

```c
  32  int main(void)
  33  {
  34      char input[256];
  35      char *cursor;
  36      char *end;
  37      long long first;
  38      long long second;
  39  
  40      if (fgets(input, sizeof input, stdin) == NULL) {
  41          fputs("Invalid input\n", stderr);
  42          return EXIT_FAILURE;
  43      }
  44  
  45      if (strchr(input, '\n') == NULL && !feof(stdin)) {
  46          int ch;
  47  
  48          while ((ch = getchar()) != '\n' && ch != EOF) {
  49          }
  50  
  51          fputs("Invalid input\n", stderr);
  52          return EXIT_FAILURE;
  53      }
  54  
  55      cursor = input;
  56      errno = 0;
  57      first = strtoll(cursor, &end, 10);
  58  
  59      if (cursor == end || errno == ERANGE) {
  60          fputs("Invalid input\n", stderr);
  61          return EXIT_FAILURE;
  62      }
  63  
  64      cursor = end;
  65      errno = 0;
  66      second = strtoll(cursor, &end, 10);
  67  
  68      if (cursor == end || errno == ERANGE) {
  69          fputs("Invalid input\n", stderr);
  70          return EXIT_FAILURE;
  71      }
      ... (17 more lines)
```

Round 5, where the rule appeared:

```c
  93  static int only_whitespace_remains(void)
  94  {
  95      int ch;
  96  
  97      while ((ch = getchar()) != EOF) {
  98          if (ch != ' ' && ch != '\t' && ch != '\n' &&
  99              ch != '\r' && ch != '\f' && ch != '\v') {
 100              return 0;
 101          }
 102      }
 103  
 104      return !ferror(stdin);
 105  }
  32  static int read_long_long(long long *value)
  33  {
  34      int ch;
  35      int sign = 1;
  36      unsigned long long result = 0;
  37      unsigned long long limit;
  38  
  39      do {
  40          ch = getchar();
  41      } while (ch == ' ' || ch == '\t' || ch == '\n' ||
  42               ch == '\r' || ch == '\f' || ch == '\v');
  43  
  44      if (ch == EOF) {
  45          return 0;
  46      }
  47  
  48      if (ch == '+' || ch == '-') {
  49          if (ch == '-') {
  50              sign = -1;
  51          }
  52          ch = getchar();
  53      }
  54  
  55      if (ch < '0' || ch > '9') {
  56          return 0;
  57      }
  58  
  59      limit = sign < 0
  60                  ? (unsigned long long)LLONG_MAX + 1ULL
  61                  : (unsigned long long)LLONG_MAX;
  62  
  63      do {
  64          unsigned int digit = (unsigned int)(ch - '0');
  65  
  66          if (result > (limit - digit) / 10ULL) {
  67              return 0;
  68          }
  69  
  70          result = result * 10ULL + digit;
  71          ch = getchar();
      ... (20 more lines)
```

## E020  `flawfinder:strlen`  round 0 -> 1

Program `problem-619` (feedback-static-r0-t1.0-p1.0); left `move_numbers_to_string`, entered `main`.

Round 0, where the rule was removed:

```c
   6  char* move_numbers_to_string(const char* input) {
   7      if (input == NULL) {
   8          return NULL;
   9      }
  10      
  11      size_t len = strlen(input);
  12      char* result = (char*)malloc((len + 1) * sizeof(char));
  13      if (result == NULL) {
  14          return NULL;
  15      }
  16      
  17      size_t num_count = 0;
  18      size_t j = 0;
  19      
  20      for (size_t i = 0; i < len; i++) {
  21          if (isdigit((unsigned char)input[i])) {
  22              num_count++;
  23          } else {
  24              result[j++] = input[i];
  25          }
  26      }
  27      
  28      for (size_t i = 0; i < len && num_count > 0; i++) {
  29          if (isdigit((unsigned char)input[i])) {
  30              result[j++] = input[i];
  31              num_count--;
  32          }
  33      }
  34      
  35      result[j] = '\0';
  36      return result;
  37  }
```

Round 1, where the rule appeared:

```c
  43  int main(void) {
  44      const char* test_str = "a1b2c3d4";
  45      size_t test_len = strlen(test_str) + 1;
  46      char* result = move_numbers_to_string(test_str, test_len);
  47      
  48      if (result != NULL) {
  49          printf("Original: %s\n", test_str);
  50          printf("Modified: %s\n", result);
  51          free(result);
  52          result = NULL;
  53      } else {
  54          fprintf(stderr, "Memory allocation failed\n");
  55          return EXIT_FAILURE;
  56      }
  57      
  58      return EXIT_SUCCESS;
  59  }
```

## E021  `flawfinder:strlen`  round 0 -> 1

Program `problem-39` (feedback-static+dynamic-r0-t1.0-p1.0); left `canRearrange`, entered `main`.

Round 0, where the rule was removed:

```c
   6  bool canRearrange(const char *str) {
   7      if (str == NULL) {
   8          return false;
   9      }
  10      
  11      size_t len = strlen(str);
  12      if (len == 0) {
  13          return true;
  14      }
  15      
  16      int freq[26] = {0};
  17      for (size_t i = 0; i < len; i++) {
  18          if (str[i] >= 'a' && str[i] <= 'z') {
  19              freq[str[i] - 'a']++;
  20          } else if (str[i] >= 'A' && str[i] <= 'Z') {
  21              freq[str[i] - 'A']++;
  22          }
  23      }
  24      
  25      int maxFreq = 0;
  26      for (int i = 0; i < 26; i++) {
  27          if (freq[i] > maxFreq) {
  28              maxFreq = freq[i];
  29          }
  30      }
  31      
  32      return maxFreq <= (int)((len + 1) / 2);
  33  }
```

Round 1, where the rule appeared:

```c
  42  int main(void) {
  43      const char *tests[] = {
  44          "aab",
  45          "aaab",
  46          "abc",
  47          "aabb",
  48          "aaabb",
  49          "aabbc",
  50          "abcd",
  51          "aabbcc",
  52          "aaabbb",
  53          NULL
  54      };
  55  
  56      for (int i = 0; tests[i] != NULL; i++) {
  57          printf("String: %s -> %s\n", tests[i], canRearrange(tests[i], strlen(tests[i]) + 1) ? "true" : "false");
  58      }
  59  
  60      return 0;
  61  }
```

## E022  `flawfinder:strlen`  round 1 -> 2

Program `problem-391-s3` (feedback-static-r0-t1.0-p1.0); left `create_node`, entered `lists_to_nested_dict`.

Round 1, where the rule was removed:

```c
  11  Dict *create_node(const char *key) {
  12      Dict *node = malloc(sizeof(Dict));
  13      if (!node) return NULL;
  14      size_t len = strlen(key) + 1;
  15      node->key = malloc(len);
  16      if (!node->key) {
  17          free(node);
  18          return NULL;
  19      }
  20      snprintf(node->key, len, "%s", key);
  21      node->child = NULL;
  22      node->next = NULL;
  23      return node;
  24  }
```

Round 2, where the rule appeared:

```c
  40  int lists_to_nested_dict(Dict **root, char ***lists, const int *sizes, int list_count) {
  41      if (!root || !lists || !sizes || list_count <= 0) return -1;
  42      
  43      if (!*root && sizes[0] > 0) {
  44          *root = create_node(lists[0][0], strlen(lists[0][0]));
  45          if (!*root) return -1;
  46      }
  47  
  48      for (int i = 1; i < list_count; i++) {
  49          Dict *current = *root;
  50          for (int j = 0; j < sizes[i]; j++) {
  51              current = find_or_add_child(current, lists[i][j], strlen(lists[i][j]));
  52              if (!current) return -1;
  53          }
  54      }
  55      return 0;
  56  }
```

## E023  `flawfinder:strncpy`  round 2 -> 3

Program `problem-464` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `set_entry` (new function).

Round 2, where the rule was removed:

```c
  35  int main(void) {
  36      Dictionary dict;
  37      dict.count = 0;
  38  
  39      if (dict.count < MAX_ENTRIES) {
  40          strncpy(dict.entries[dict.count].key, "first", MAX_KEY_LENGTH - 1);
  41          dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
  42          strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
  43          dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
  44          dict.count++;
  45      }
  46  
  47      if (dict.count < MAX_ENTRIES) {
  48          strncpy(dict.entries[dict.count].key, "second", MAX_KEY_LENGTH - 1);
  49          dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
  50          strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
  51          dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
  52          dict.count++;
  53      }
  54  
  55      if (dict.count < MAX_ENTRIES) {
  56          strncpy(dict.entries[dict.count].key, "third", MAX_KEY_LENGTH - 1);
  57          dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
  58          strncpy(dict.entries[dict.count].value, "apple", MAX_VALUE_LENGTH - 1);
  59          dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
  60          dict.count++;
  61      }
  62  
  63      if (all_values_same(&dict)) {
  64          printf("All values are same\n");
  65      } else {
  66          printf("Values are different\n");
  67      }
  68  
  69      if (dict.count < MAX_ENTRIES) {
  70          strncpy(dict.entries[dict.count].key, "fourth", MAX_KEY_LENGTH - 1);
  71          dict.entries[dict.count].key[MAX_KEY_LENGTH - 1] = '\0';
  72          strncpy(dict.entries[dict.count].value, "orange", MAX_VALUE_LENGTH - 1);
  73          dict.entries[dict.count].value[MAX_VALUE_LENGTH - 1] = '\0';
  74          dict.count++;
      ... (10 more lines)
```

Round 3, where the rule appeared:

```c
  35  static void set_entry(DictionaryEntry *entry, const char *key, const char *value) {
  36      if (key != NULL) {
  37          strncpy(entry->key, key, MAX_KEY_LENGTH - 1);
  38          entry->key[MAX_KEY_LENGTH - 1] = '\0';
  39      } else {
  40          entry->key[0] = '\0';
  41      }
  42  
  43      if (value != NULL) {
  44          strncpy(entry->value, value, MAX_VALUE_LENGTH - 1);
  45          entry->value[MAX_VALUE_LENGTH - 1] = '\0';
  46      } else {
  47          entry->value[0] = '\0';
  48      }
  49  }
```

## E024  `flawfinder:getchar`  round 2 -> 3

Program `problem-759` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 2, where the rule was removed:

```c
  60  int main(void)
  61  {
  62      char buffer[MAX_INPUT_LEN];
  63      size_t len;
  64  
  65      printf("Enter a number: ");
  66  
  67      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  68          fprintf(stderr, "Input error\n");
  69          return 1;
  70      }
  71  
  72      buffer[sizeof(buffer) - 1] = '\0';
  73  
  74      len = strnlen(buffer, sizeof(buffer));
  75      if (len > 0 && buffer[len - 1] == '\n') {
  76          buffer[len - 1] = '\0';
  77      } else if (len == sizeof(buffer) - 1) {
  78          int ch;
  79          while ((ch = getchar()) != '\n' && ch != EOF) {
  80              ;
  81          }
  82          fprintf(stderr, "Input too long\n");
  83          return 1;
  84      }
  85  
  86      if (is_decimal_precision_two(buffer, sizeof(buffer))) {
  87          printf("Valid decimal with precision of 2: %s\n", buffer);
  88      } else {
  89          printf("Invalid decimal with precision of 2: %s\n", buffer);
  90      }
  91  
  92      return 0;
  93  }
```

Round 3, where the rule appeared:

```c
  61  static bool read_line(char *buffer, size_t size)
  62  {
  63      size_t len;
  64  
  65      if (buffer == NULL || size < 2) {
  66          return false;
  67      }
  68  
  69      if (fgets(buffer, (int)size, stdin) == NULL) {
  70          return false;
  71      }
  72  
  73      buffer[size - 1] = '\0';
  74  
  75      len = strnlen(buffer, size);
  76      if (len > 0 && buffer[len - 1] == '\n') {
  77          buffer[len - 1] = '\0';
  78          return true;
  79      }
  80  
  81      if (len == size - 1) {
  82          int ch;
  83          do {
  84              ch = getchar();
  85          } while (ch != '\n' && ch != EOF);
  86          return false;
  87      }
  88  
  89      return true;
  90  }
```

## E025  `flawfinder:strlen`  round 0 -> 1

Program `problem-532` (feedback-static+dynamic-r0-t1.0-p1.0); left `is_permutation`, entered `main`.

Round 0, where the rule was removed:

```c
   5  bool is_permutation(const char *str1, const char *str2) {
   6      if (str1 == NULL || str2 == NULL) {
   7          return false;
   8      }
   9      
  10      size_t len1 = strlen(str1);
  11      size_t len2 = strlen(str2);
  12      
  13      if (len1 != len2) {
  14          return false;
  15      }
  16      
  17      int char_count[256] = {0};
  18      
  19      for (size_t i = 0; i < len1; i++) {
  20          char_count[(unsigned char)str1[i]]++;
  21          char_count[(unsigned char)str2[i]]--;
  22      }
  23      
  24      for (int i = 0; i < 256; i++) {
  25          if (char_count[i] != 0) {
  26              return false;
  27          }
  28      }
  29      
  30      return true;
  31  }
```

Round 1, where the rule appeared:

```c
  30  int main(void) {
  31      const char *test1_str1 = "listen";
  32      const char *test1_str2 = "silent";
  33      
  34      const char *test2_str1 = "hello";
  35      const char *test2_str2 = "world";
  36      
  37      const char *test3_str1 = "aab";
  38      const char *test3_str2 = "aba";
  39      
  40      printf("Test 1: \"%s\" and \"%s\" - %s\n", 
  41             test1_str1, test1_str2, 
  42             is_permutation(test1_str1, strlen(test1_str1), test1_str2, strlen(test1_str2)) ? "Permutations" : "Not permutations");
  43      
  44      printf("Test 2: \"%s\" and \"%s\" - %s\n", 
  45             test2_str1, test2_str2, 
  46             is_permutation(test2_str1, strlen(test2_str1), test2_str2, strlen(test2_str2)) ? "Permutations" : "Not permutations");
  47      
  48      printf("Test 3: \"%s\" and \"%s\" - %s\n", 
  49             test3_str1, test3_str2, 
  50             is_permutation(test3_str1, strlen(test3_str1), test3_str2, strlen(test3_str2)) ? "Permutations" : "Not permutations");
  51      
  52      return 0;
  53  }
```

## E026  `flawfinder:strlen`  round 1 -> 2

Program `problem-823-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `starts_with_regex`, entered `main`.

Round 1, where the rule was removed:

```c
   6  int starts_with_regex(const char *str, const char *pattern) {
   7      regex_t regex;
   8      int ret;
   9      regmatch_t match[1];
  10      char *anchored_pattern;
  11      size_t pattern_len;
  12      int result = 0;
  13  
  14      if (str == NULL || pattern == NULL) {
  15          return 0;
  16      }
  17  
  18      pattern_len = strlen(pattern);
  19      if (pattern_len > SIZE_MAX - 2) {
  20          return 0;
  21      }
  22  
  23      anchored_pattern = malloc(pattern_len + 2);
  24      if (anchored_pattern == NULL) {
  25          return 0;
  26      }
  27  
  28      anchored_pattern[0] = '^';
  29      snprintf(anchored_pattern + 1, pattern_len + 1, "%s", pattern);
  30  
  31      ret = regcomp(&regex, anchored_pattern, REG_EXTENDED);
  32      free(anchored_pattern);
  33  
  34      if (ret != 0) {
  35          return 0;
  36      }
  37  
  38      ret = regexec(&regex, str, 1, match, 0);
  39      if (ret == 0 && match[0].rm_so == 0) {
  40          result = 1;
  41      }
  42  
  43      regfree(&regex);
  44      return result;
  45  }
```

Round 2, where the rule appeared:

```c
  59  int main(void) {
  60      const char *str = "hello world";
  61      const char *pattern = "hello";
  62      size_t str_len = strlen(str);
  63      size_t pattern_len = strlen(pattern);
  64  
  65      if (starts_with_regex(str, pattern, str_len, pattern_len)) {
  66          printf("String starts with the pattern.\n");
  67      } else {
  68          printf("String does not start with the pattern.\n");
  69      }
  70  
  71      return 0;
  72  }
```

## E027  `flawfinder:strlen`  round 4 -> 5

Program `problem-73-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `split_string`, entered `main`.

Round 4, where the rule was removed:

```c
   6  char **split_string(const char *str, const char *pattern, int *count) {
   7      regex_t regex;
   8      regmatch_t match;
   9      char **result = NULL;
  10      char **temp = NULL;
  11      const char *cursor = str;
  12      size_t capacity = 10;
  13      size_t size = 0;
  14      size_t start = 0;
  15      size_t end;
  16      size_t len;
  17      size_t str_len;
  18  
  19      if (str == NULL || pattern == NULL || count == NULL) {
  20          return NULL;
  21      }
  22  
  23      *count = 0;
  24      str_len = strlen(str);
  25  
  26      if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
  27          return NULL;
  28      }
  29  
  30      result = malloc(capacity * sizeof(char *));
  31      if (result == NULL) {
  32          regfree(&regex);
  33          return NULL;
  34      }
  35  
  36      while (*cursor != '\0') {
  37          if (regexec(&regex, cursor, 1, &match, 0) == 0) {
  38              end = start + (size_t)match.rm_so;
  39              
  40              if (end > start) {
  41                  if (size >= capacity) {
  42                      capacity *= 2;
  43                      temp = realloc(result, capacity * sizeof(char *));
  44                      if (temp == NULL) {
  45                          for (size_t i = 0; i < size; i++) {
      ... (75 more lines)
```

Round 5, where the rule appeared:

```c
 153  int main(void) {
 154      const char *input = "apple,banana;orange:grape|mango";
 155      const char *pattern = "[,;:|]";
 156      int count = 0;
 157      size_t input_len = strlen(input);
 158      char **tokens = split_string(input, input_len, pattern, &count);
 159  
 160      if (tokens != NULL) {
 161          for (int i = 0; i < count; i++) {
 162              printf("%s\n", tokens[i]);
 163          }
 164          free_split_result(tokens, count);
 165      }
 166  
 167      return 0;
 168  }
```

## E028  `flawfinder:strncpy`  round 3 -> 4

Program `problem-370` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `print_tuples, safe_strncpy` (new function).

Round 3, where the rule was removed:

```c
  36  int main(void) {
  37      Tuple tuples[] = {
  38          {1, 3.14f, "pi"},
  39          {2, 1.41f, "sqrt2"},
  40          {3, 2.72f, "e"},
  41          {4, 0.58f, "gamma"},
  42          {5, 1.62f, "phi"}
  43      };
  44      
  45      size_t i;
  46      for (i = 0; i < sizeof(tuples) / sizeof(tuples[0]); i++) {
  47          strncpy(tuples[i].label, tuples[i].label, sizeof(tuples[i].label) - 1);
  48          tuples[i].label[sizeof(tuples[i].label) - 1] = '\0';
  49      }
  50      
  51      size_t tuple_count = sizeof(tuples) / sizeof(tuples[0]);
  52      
  53      printf("Before sorting:\n");
  54      print_tuples(tuples, tuple_count);
  55      
  56      sort_tuples_by_float(tuples, tuple_count);
  57      
  58      printf("\nAfter sorting by float value:\n");
  59      print_tuples(tuples, tuple_count);
  60      
  61      return 0;
  62  }
```

Round 4, where the rule appeared:

```c
  28  void print_tuples(const Tuple *tuples, size_t count) {
  29      size_t i;
  30      for (i = 0; i < count; i++) {
  31          char safe_label[33];
  32          strncpy(safe_label, tuples[i].label, sizeof(safe_label) - 1);
  33          safe_label[sizeof(safe_label) - 1] = '\0';
  34          printf("ID: %d, Value: %.2f, Label: %.31s\n", 
  35                 tuples[i].id, tuples[i].value, safe_label);
  36      }
  37  }
  39  void safe_strncpy(char *dest, const char *src, size_t dest_size) {
  40      if (dest == NULL || src == NULL || dest_size == 0) {
  41          return;
  42      }
  43      strncpy(dest, src, dest_size - 1);
  44      dest[dest_size - 1] = '\0';
  45  }
```

## E029  `flawfinder:memcpy`  round 0 -> 1

Program `problem-544-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `flatten_tuple_list`, entered `append_bytes` (new function).

Round 0, where the rule was removed:

```c
  22  static char *flatten_tuple_list(const Tuple *tuples, size_t count)
  23  {
  24      const char *tuple_separator = ", ";
  25      const char *item_separator = ": ";
  26      const size_t tuple_separator_length = strlen(tuple_separator);
  27      const size_t item_separator_length = strlen(item_separator);
  28      size_t total_length = 0;
  29      char *output;
  30      char *cursor;
  31  
  32      if (count > 0 && tuples == NULL) {
  33          errno = EINVAL;
  34          return NULL;
  35      }
  36  
  37      for (size_t i = 0; i < count; ++i) {
  38          size_t first_length;
  39          size_t second_length;
  40  
  41          if (tuples[i].first == NULL || tuples[i].second == NULL) {
  42              errno = EINVAL;
  43              return NULL;
  44          }
  45  
  46          first_length = strlen(tuples[i].first);
  47          second_length = strlen(tuples[i].second);
  48  
  49          if (!checked_add_size(total_length, first_length, &total_length) ||
  50              !checked_add_size(total_length, item_separator_length, &total_length) ||
  51              !checked_add_size(total_length, second_length, &total_length) ||
  52              (i + 1 < count &&
  53               !checked_add_size(total_length, tuple_separator_length,
  54                                 &total_length))) {
  55              errno = EOVERFLOW;
  56              return NULL;
  57          }
  58      }
  59  
  60      if (!checked_add_size(total_length, 1, &total_length)) {
  61          errno = EOVERFLOW;
      ... (32 more lines)
```

Round 1, where the rule appeared:

```c
  24  static int append_bytes(char **cursor, size_t *remaining,
  25                          const char *source, size_t length)
  26  {
  27      if (cursor == NULL || *cursor == NULL || remaining == NULL ||
  28          (length > 0 && source == NULL) || length > *remaining) {
  29          return 0;
  30      }
  31  
  32      if (length > 0) {
  33          memcpy(*cursor, source, length);
  34          *cursor += length;
  35          *remaining -= length;
  36      }
  37  
  38      return 1;
  39  }
```

## E030  `flawfinder:strlen`  round 0 -> 1

Program `problem-450` (feedback-static-r0-t1.0-p1.0); left `main`, entered `extract_strings`.

Round 0, where the rule was removed:

```c
  40  int main(void) {
  41      const char *strings[] = {"Hello", "World", "C", "Programming"};
  42      size_t count = sizeof(strings) / sizeof(strings[0]);
  43      size_t extract_size = 3;
  44  
  45      char **input = (char **)malloc(count * sizeof(char *));
  46      if (input == NULL) {
  47          return 1;
  48      }
  49  
  50      for (size_t i = 0; i < count; i++) {
  51          input[i] = (char *)malloc((strlen(strings[i]) + 1) * sizeof(char));
  52          if (input[i] == NULL) {
  53              for (size_t j = 0; j < i; j++) {
  54                  free(input[j]);
  55              }
  56              free(input);
  57              return 1;
  58          }
  59          strcpy(input[i], strings[i]);
  60      }
  61  
  62      char **extracted = extract_strings(input, count, extract_size);
  63      if (extracted == NULL) {
  64          for (size_t i = 0; i < count; i++) {
  65              free(input[i]);
  66          }
  67          free(input);
  68          return 1;
  69      }
  70  
  71      for (size_t i = 0; i < count; i++) {
  72          printf("%s\n", extracted[i]);
  73          free(extracted[i]);
  74          free(input[i]);
  75      }
  76  
  77      free(extracted);
  78      free(input);
  79      return 0;
      ... (1 more lines)
```

Round 1, where the rule appeared:

```c
   5  char **extract_strings(char **input, size_t count, size_t size) {
   6      if (input == NULL || count == 0 || size == 0) {
   7          return NULL;
   8      }
   9  
  10      char **result = (char **)malloc(count * sizeof(char *));
  11      if (result == NULL) {
  12          return NULL;
  13      }
  14  
  15      for (size_t i = 0; i < count; i++) {
  16          if (input[i] == NULL) {
  17              for (size_t j = 0; j < i; j++) {
  18                  free(result[j]);
  19              }
  20              free(result);
  21              return NULL;
  22          }
  23  
  24          result[i] = (char *)malloc((size + 1) * sizeof(char));
  25          if (result[i] == NULL) {
  26              for (size_t j = 0; j < i; j++) {
  27                  free(result[j]);
  28              }
  29              free(result);
  30              return NULL;
  31          }
  32  
  33          size_t src_len = strlen(input[i]);
  34          size_t copy_len = src_len < size ? src_len : size;
  35          memcpy(result[i], input[i], copy_len);
  36          result[i][copy_len] = '\0';
  37      }
  38  
  39      return result;
  40  }
```

## E031  `flawfinder:strlen`  round 0 -> 1

Program `problem-674-s3` (feedback-static-r0-t1.0-p1.0); left `remove_duplicates`, entered `split_string`.

Round 0, where the rule was removed:

```c
  40  char* remove_duplicates(const char* str) {
  41      int word_count = 0;
  42      char** words = split_string(str, &word_count);
  43      if (!words) return NULL;
  44      
  45      int unique_count = 0;
  46      for (int i = 0; i < word_count; i++) {
  47          int is_duplicate = 0;
  48          for (int j = 0; j < unique_count; j++) {
  49              if (strcmp(words[i], words[j]) == 0) {
  50                  is_duplicate = 1;
  51                  break;
  52              }
  53          }
  54          if (!is_duplicate) {
  55              words[unique_count++] = words[i];
  56          }
  57      }
  58      
  59      char* result = (char*)malloc((strlen(str) + 1) * sizeof(char));
  60      if (!result) {
  61          free_words(words, word_count);
  62          return NULL;
  63      }
  64      result[0] = '\0';
  65      
  66      for (int i = 0; i < unique_count; i++) {
  67          strcat(result, words[i]);
  68          if (i < unique_count - 1) {
  69              strcat(result, " ");
  70          }
  71      }
  72      
  73      free_words(words, word_count);
  74      return result;
  75  }
```

Round 1, where the rule appeared:

```c
   9  char** split_string(const char* str, int* word_count) {
  10      char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
  11      if (!words) return NULL;
  12      
  13      *word_count = 0;
  14      char temp_str[strlen(str) + 1];
  15      strncpy(temp_str, str, sizeof(temp_str) - 1);
  16      temp_str[sizeof(temp_str) - 1] = '\0';
  17      
  18      char* token = strtok(temp_str, " ");
  19      while (token && *word_count < MAX_WORDS) {
  20          words[*word_count] = (char*)malloc((strlen(token) + 1) * sizeof(char));
  21          if (!words[*word_count]) {
  22              while (*word_count > 0) free(words[--*word_count]);
  23              free(words);
  24              return NULL;
  25          }
  26          strncpy(words[*word_count], token, strlen(token) + 1);
  27          (*word_count)++;
  28          token = strtok(NULL, " ");
  29      }
  30      
  31      return words;
  32  }
```

## E032  `flawfinder:memcpy`  round 0 -> 1

Program `problem-500-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `concatenate_list`, entered `create_node`.

Round 0, where the rule was removed:

```c
  10  char *concatenate_list(Node *head) {
  11      size_t total_length = 0;
  12      Node *current = head;
  13      size_t offset = 0;
  14      char *result;
  15  
  16      while (current != NULL) {
  17          total_length += strlen(current->data);
  18          current = current->next;
  19      }
  20  
  21      result = (char *)malloc(total_length + 1);
  22      if (result == NULL) {
  23          return NULL;
  24      }
  25  
  26      current = head;
  27      while (current != NULL) {
  28          size_t len = strlen(current->data);
  29          memcpy(result + offset, current->data, len);
  30          offset += len;
  31          current = current->next;
  32      }
  33      result[offset] = '\0';
  34  
  35      return result;
  36  }
```

Round 1, where the rule appeared:

```c
  38  Node *create_node(const char *str) {
  39      size_t len = strlen(str);
  40      Node *node = (Node *)malloc(sizeof(Node));
  41      if (node == NULL) {
  42          return NULL;
  43      }
  44      node->data = (char *)malloc(len + 1);
  45      if (node->data == NULL) {
  46          free(node);
  47          return NULL;
  48      }
  49      memcpy(node->data, str, len + 1);
  50      node->length = len;
  51      node->next = NULL;
  52      return node;
  53  }
```

## E033  `flawfinder:strlen`  round 1 -> 2

Program `problem-478-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `remove_lowercase_substrings`, entered `main`.

Round 1, where the rule was removed:

```c
  24  char *remove_lowercase_substrings(const char *input)
  25  {
  26      size_t input_len;
  27      size_t i;
  28      size_t j;
  29      size_t result_idx;
  30      char *result;
  31      
  32      if (input == NULL) {
  33          return NULL;
  34      }
  35      
  36      input_len = strlen(input);
  37      result = malloc(input_len + 1);
  38      
  39      if (result == NULL) {
  40          return NULL;
  41      }
  42      
  43      result_idx = 0;
  44      i = 0;
  45      
  46      while (i < input_len) {
  47          if (islower((unsigned char)input[i])) {
  48              j = i;
  49              while (j < input_len && islower((unsigned char)input[j])) {
  50                  j++;
  51              }
  52              if (!is_all_lowercase(input + i, j - i)) {
  53                  if (result_idx + (j - i) <= input_len) {
  54                      memcpy(result + result_idx, input + i, j - i);
  55                      result_idx += (j - i);
  56                  }
  57              }
  58              i = j;
  59          } else {
  60              if (result_idx < input_len) {
  61                  result[result_idx] = input[i];
  62                  result_idx++;
  63              }
      ... (8 more lines)
```

Round 2, where the rule appeared:

```c
  71  int main(void)
  72  {
  73      char test1[] = "HelloWORLDthisISaTEST";
  74      char test2[] = "abcDEFghi";
  75      char test3[] = "ALLUPPERCASE";
  76      char test4[] = "alllowercase";
  77      const char test5[] = "";
  78      char *result;
  79      
  80      result = remove_lowercase_substrings(test1, strlen(test1));
  81      if (result != NULL) {
  82          printf("Input: %s\n", test1);
  83          printf("Output: %s\n\n", result);
  84          free(result);
  85      }
  86      
  87      result = remove_lowercase_substrings(test2, strlen(test2));
  88      if (result != NULL) {
  89          printf("Input: %s\n", test2);
  90          printf("Output: %s\n\n", result);
  91          free(result);
  92      }
  93      
  94      result = remove_lowercase_substrings(test3, strlen(test3));
  95      if (result != NULL) {
  96          printf("Input: %s\n", test3);
  97          printf("Output: %s\n\n", result);
  98          free(result);
  99      }
 100      
 101      result = remove_lowercase_substrings(test4, strlen(test4));
 102      if (result != NULL) {
 103          printf("Input: %s\n", test4);
 104          printf("Output: %s\n\n", result);
 105          free(result);
 106      }
 107      
 108      result = remove_lowercase_substrings(test5, strlen(test5));
 109      if (result != NULL) {
 110          printf("Input: (empty)\n");
      ... (6 more lines)
```

## E034  `flawfinder:strlen`  round 0 -> 1

Program `problem-39` (feedback-static-r0-t1.0-p1.0); left `can_rearrange`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int can_rearrange(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9      
  10      size_t len = strlen(str);
  11      if (len <= 1) {
  12          return 1;
  13      }
  14      
  15      int freq[256] = {0};
  16      size_t max_freq = 0;
  17      
  18      for (size_t i = 0; i < len; i++) {
  19          unsigned char c = (unsigned char)str[i];
  20          freq[c]++;
  21          if ((size_t)freq[c] > max_freq) {
  22              max_freq = (size_t)freq[c];
  23          }
  24      }
  25      
  26      if (max_freq > (len + 1) / 2) {
  27          return 0;
  28      }
  29      
  30      return 1;
  31  }
```

Round 1, where the rule appeared:

```c
  32  int main(int argc, char * const argv[]) {
  33      if (argc != 2) {
  34          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  35          return 1;
  36      }
  37      
  38      int result = can_rearrange(argv[1], strlen(argv[1]));
  39      printf("%d\n", result);
  40      
  41      return 0;
  42  }
```

## E035  `flawfinder:fgetc`  round 2 -> 3

Program `problem-377-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_character_line` (new function).

Round 2, where the rule was removed:

```c
  79  int main(void)
  80  {
  81      char *string = read_line(stdin);
  82      int character;
  83      int input;
  84  
  85      if (string == NULL) {
  86          return EXIT_FAILURE;
  87      }
  88  
  89      character = fgetc(stdin);
  90      if (character == EOF) {
  91          free(string);
  92          return EXIT_FAILURE;
  93      }
  94  
  95      do {
  96          input = fgetc(stdin);
  97      } while (input != '\n' && input != EOF);
  98  
  99      if (input == EOF && ferror(stdin)) {
 100          free(string);
 101          return EXIT_FAILURE;
 102      }
 103  
 104      remove_character(string, character);
 105  
 106      if (puts(string) == EOF) {
 107          free(string);
 108          return EXIT_FAILURE;
 109      }
 110  
 111      free(string);
 112  
 113      if (fflush(stdout) == EOF) {
 114          return EXIT_FAILURE;
 115      }
 116  
 117      return EXIT_SUCCESS;
 118  }
```

Round 3, where the rule appeared:

```c
  78  static int read_character_line(FILE *stream, unsigned char *character)
  79  {
  80      int ch = fgetc(stream);
  81  
  82      if (ch == EOF) {
  83          return 0;
  84      }
  85  
  86      *character = (unsigned char)ch;
  87  
  88      while (ch != '\n') {
  89          ch = fgetc(stream);
  90  
  91          if (ch == EOF) {
  92              return ferror(stream) ? 0 : 1;
  93          }
  94      }
  95  
  96      return 1;
  97  }
```

## E036  `flawfinder:strlen`  round 0 -> 1

Program `problem-338-s2` (feedback-static-r0-t1.0-p1.0); left `count_substrings_with_same_ends`, entered `main`.

Round 0, where the rule was removed:

```c
   7  int count_substrings_with_same_ends(const char *str, size_t *result)
   8  {
   9      size_t count;
  10      size_t len;
  11      size_t i;
  12      size_t j;
  13  
  14      if (str == NULL || result == NULL) {
  15          return -1;
  16      }
  17  
  18      len = strlen(str);
  19      count = 0;
  20  
  21      for (i = 0; i < len; i++) {
  22          for (j = i; j < len; j++) {
  23              if (str[i] == str[j]) {
  24                  if (count == SIZE_MAX) {
  25                      return -2;
  26                  }
  27                  count++;
  28              }
  29          }
  30      }
  31  
  32      *result = count;
  33      return 0;
  34  }
```

Round 1, where the rule appeared:

```c
  35  int main(int argc, const char *argv[])
  36  {
  37      size_t result;
  38      int status;
  39      size_t len;
  40  
  41      if (argc != 2) {
  42          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  43          return EXIT_FAILURE;
  44      }
  45  
  46      if (argv[1] == NULL) {
  47          fprintf(stderr, "Error: Invalid input\n");
  48          return EXIT_FAILURE;
  49      }
  50  
  51      len = strlen(argv[1]);
  52  
  53      status = count_substrings_with_same_ends(argv[1], len, &result);
  54  
  55      if (status == -1) {
  56          fprintf(stderr, "Error: Invalid argument passed to function\n");
  57          return EXIT_FAILURE;
  58      } else if (status == -2) {
  59          fprintf(stderr, "Error: Arithmetic overflow detected\n");
  60          return EXIT_FAILURE;
  61      } else if (status != 0) {
  62          fprintf(stderr, "Error: Unknown error occurred\n");
  63          return EXIT_FAILURE;
  64      }
  65  
  66      printf("Number of substrings with same first and last characters: %zu\n", result);
  67  
  68      return EXIT_SUCCESS;
  69  }
```

## E037  `flawfinder:strlen`  round 0 -> 1

Program `problem-676-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `filter_alphanumeric`.

Round 0, where the rule was removed:

```c
  28  int main(void) {
  29      const char *input = "Hello, World! 123 @#$";
  30      char *output;
  31      
  32      if (input == NULL) {
  33          return 1;
  34      }
  35      
  36      output = malloc(strlen(input) + 1);
  37      if (output == NULL) {
  38          return 1;
  39      }
  40      
  41      filter_alphanumeric(input, output);
  42      printf("%s\n", output);
  43      
  44      free(output);
  45      return 0;
  46  }
```

Round 1, where the rule appeared:

```c
   6  void filter_alphanumeric(const char *input, char *output, size_t out_size) {
   7      regex_t regex;
   8      regmatch_t pmatch[1];
   9      const char *pattern = "[^a-zA-Z0-9]+";
  10      const char *cursor = input;
  11      size_t out_len = 0;
  12      
  13      if (out_size == 0 || input == NULL || output == NULL) {
  14          return;
  15      }
  16      
  17      output[0] = '\0';
  18      
  19      if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
  20          return;
  21      }
  22      
  23      while (regexec(&regex, cursor, 1, pmatch, 0) == 0) {
  24          size_t len = pmatch[0].rm_so;
  25          if (out_len + len >= out_size) {
  26              len = out_size - out_len - 1;
  27          }
  28          if (len > 0) {
  29              memcpy(output + out_len, cursor, len);
  30              out_len += len;
  31          }
  32          cursor += pmatch[0].rm_eo;
  33      }
  34      
  35      size_t remaining_len = strlen(cursor);
  36      if (out_len + remaining_len >= out_size) {
  37          remaining_len = out_size - out_len - 1;
  38      }
  39      
  40      if (remaining_len > 0) {
  41          memcpy(output + out_len, cursor, remaining_len);
  42          out_len += remaining_len;
  43      }
  44      
  45      output[out_len] = '\0';
      ... (2 more lines)
```

## E038  `flawfinder:memcpy`  round 4 -> 5

Program `problem-869-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `safe_copy_array` (new function).

Round 4, where the rule was removed:

```c
  78  int main(void) {
  79      int arr1[] = {1, 2, 3};
  80      int arr2[] = {4, 5, 6};
  81      int arr3[] = {7, 8, 9};
  82      int arr4[] = {10, 11, 12};
  83  
  84      size_t size1 = sizeof(arr1) / sizeof(arr1[0]);
  85      size_t size2 = sizeof(arr2) / sizeof(arr2[0]);
  86      size_t size3 = sizeof(arr3) / sizeof(arr3[0]);
  87      size_t size4 = sizeof(arr4) / sizeof(arr4[0]);
  88  
  89      size_t alloc1 = size1 * sizeof(int);
  90      size_t alloc2 = size2 * sizeof(int);
  91      size_t alloc3 = size3 * sizeof(int);
  92      size_t alloc4 = size4 * sizeof(int);
  93  
  94      int *data1 = (int *)malloc(alloc1);
  95      int *data2 = (int *)malloc(alloc2);
  96      int *data3 = (int *)malloc(alloc3);
  97      int *data4 = (int *)malloc(alloc4);
  98  
  99      if (!data1 || !data2 || !data3 || !data4) {
 100          free(data1);
 101          free(data2);
 102          free(data3);
 103          free(data4);
 104          return 1;
 105      }
 106  
 107      if (memcpy(data1, arr1, alloc1) != data1) {
 108          free(data1);
 109          free(data2);
 110          free(data3);
 111          free(data4);
 112          return 1;
 113      }
 114      if (memcpy(data2, arr2, alloc2) != data2) {
 115          free(data1);
 116          free(data2);
 117          free(data3);
      ... (66 more lines)
```

Round 5, where the rule appeared:

```c
  78  int *safe_copy_array(const int *src, size_t count) {
  79      if (count == 0 || src == NULL) {
  80          return NULL;
  81      }
  82      size_t alloc_size = count * sizeof(int);
  83      if (alloc_size / sizeof(int) != count) {
  84          return NULL;
  85      }
  86      int *dest = (int *)malloc(alloc_size);
  87      if (!dest) {
  88          return NULL;
  89      }
  90      memcpy(dest, src, alloc_size);
  91      return dest;
  92  }
```

## E039  `flawfinder:memcpy`  round 3 -> 4

Program `problem-703` (feedback-static+dynamic-r0-t1.0-p1.0); left `dictionary_put`, entered `safe_strdup` (new function).

Round 3, where the rule was removed:

```c
  42  int dictionary_put(dictionary *dict, const char *key, int value) {
  43      if (!dict || !key) {
  44          return 0;
  45      }
  46      unsigned long hash = hash_key(key);
  47      size_t index = hash % dict->bucket_count;
  48  
  49      dict_entry *entry = dict->buckets[index];
  50      while (entry) {
  51          if (strcmp(entry->key, key) == 0) {
  52              entry->value = value;
  53              return 1;
  54          }
  55          entry = entry->next;
  56      }
  57  
  58      dict_entry *new_entry = (dict_entry *)malloc(sizeof(dict_entry));
  59      if (!new_entry) {
  60          return 0;
  61      }
  62      new_entry->key = NULL;
  63      new_entry->value = 0;
  64      new_entry->next = NULL;
  65  
  66      size_t key_len = strlen(key);
  67      if (key_len >= SIZE_MAX) {
  68          free(new_entry);
  69          return 0;
  70      }
  71      new_entry->key = (char *)malloc(key_len + 1);
  72      if (!new_entry->key) {
  73          free(new_entry);
  74          return 0;
  75      }
  76      memcpy(new_entry->key, key, key_len);
  77      new_entry->key[key_len] = '\0';
  78      new_entry->value = value;
  79      new_entry->next = dict->buckets[index];
  80      dict->buckets[index] = new_entry;
  81      return 1;
      ... (1 more lines)
```

Round 4, where the rule appeared:

```c
  43  static char *safe_strdup(const char *key) {
  44      if (!key) {
  45          return NULL;
  46      }
  47      size_t max_safe_len = SIZE_MAX - 1;
  48      size_t len = 0;
  49      while (len < max_safe_len && key[len] != '\0') {
  50          len++;
  51      }
  52      if (len == max_safe_len) {
  53          return NULL;
  54      }
  55      char *copy = (char *)malloc(len + 1);
  56      if (!copy) {
  57          return NULL;
  58      }
  59      memcpy(copy, key, len);
  60      copy[len] = '\0';
  61      return copy;
  62  }
```

## E040  `cppcheck:variableScope`  round 0 -> 1

Program `problem-676` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `remove_non_alphanumeric`.

Round 0, where the rule was removed:

```c
  54  int main(void)
  55  {
  56      const char *test_strings[] = {
  57          "Hello, World! 123",
  58          "a@b#c$d%e^1&2*3",
  59          "!!!???...",
  60          "AlreadyClean123"
  61      };
  62      size_t count;
  63      size_t i;
  64      char *cleaned;
  65  
  66      count = sizeof(test_strings) / sizeof(test_strings[0]);
  67  
  68      for (i = 0; i < count; i++) {
  69          cleaned = remove_non_alphanumeric(test_strings[i]);
  70          if (cleaned == NULL) {
  71              fprintf(stderr, "Failed to process string: %s\n",
  72                      test_strings[i]);
  73              return EXIT_FAILURE;
  74          }
  75          printf("Original: %s\nCleaned:  %s\n\n",
  76                 test_strings[i], cleaned);
  77          free(cleaned);
  78      }
  79  
  80      return EXIT_SUCCESS;
  81  }
```

Round 1, where the rule appeared:

```c
   8  char *remove_non_alphanumeric(const char *input)
   9  {
  10      regex_t regex;
  11      regmatch_t match;
  12      char *result;
  13      size_t input_len;
  14      size_t result_pos;
  15      size_t match_len;
  16      const char *cursor;
  17      int ret;
  18  
  19      if (input == NULL) {
  20          return NULL;
  21      }
  22  
  23      input_len = strnlen(input, MAX_INPUT_LEN);
  24      if (input_len >= MAX_INPUT_LEN) {
  25          return NULL;
  26      }
  27  
  28      result = malloc(input_len + 1);
  29      if (result == NULL) {
  30          return NULL;
  31      }
  32  
  33      ret = regcomp(&regex, "[[:alnum:]]+", REG_EXTENDED);
  34      if (ret != 0) {
  35          free(result);
  36          return NULL;
  37      }
  38  
  39      result_pos = 0;
  40      cursor = input;
  41  
  42      while (*cursor != '\0') {
  43          ret = regexec(&regex, cursor, 1, &match, 0);
  44          if (ret != 0) {
  45              break;
  46          }
  47          if (match.rm_eo <= match.rm_so) {
      ... (17 more lines)
```

## E041  `flawfinder:strlen`  round 1 -> 2

Program `problem-102-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `snake_to_camel`, entered `main`.

Round 1, where the rule was removed:

```c
   6  char *snake_to_camel(const char *snake)
   7  {
   8      size_t len;
   9      size_t i;
  10      size_t j;
  11      int capitalize_next;
  12      char *camel;
  13  
  14      if (snake == NULL) {
  15          return NULL;
  16      }
  17  
  18      len = strlen(snake);
  19      camel = malloc(len + 1);
  20      if (camel == NULL) {
  21          return NULL;
  22      }
  23  
  24      j = 0;
  25      i = 0;
  26      capitalize_next = 1;
  27      while (i < len) {
  28          if (snake[i] == '_') {
  29              capitalize_next = 1;
  30              i++;
  31          } else {
  32              if (capitalize_next) {
  33                  camel[j] = (char)toupper((unsigned char)snake[i]);
  34                  capitalize_next = 0;
  35              } else {
  36                  camel[j] = snake[i];
  37              }
  38              j++;
  39              i++;
  40          }
  41      }
  42      camel[j] = '\0';
  43  
  44      return camel;
  45  }
```

Round 2, where the rule appeared:

```c
  53  int main(void)
  54  {
  55      char input[MAX_INPUT];
  56      char *result;
  57      size_t len;
  58  
  59      if (fgets(input, sizeof(input), stdin) == NULL) {
  60          fprintf(stderr, "Failed to read input\n");
  61          return EXIT_FAILURE;
  62      }
  63  
  64      len = strlen(input);
  65      while (len > 0 && (input[len - 1] == '\n' || input[len - 1] == '\r')) {
  66          input[len - 1] = '\0';
  67          len--;
  68      }
  69  
  70      result = snake_to_camel(input, sizeof(input));
  71      if (result == NULL) {
  72          fprintf(stderr, "Conversion failed\n");
  73          return EXIT_FAILURE;
  74      }
  75  
  76      if (printf("%s\n", result) < 0) {
  77          free(result);
  78          return EXIT_FAILURE;
  79      }
  80  
  81      free(result);
  82      return EXIT_SUCCESS;
  83  }
```

## E042  `flawfinder:strlen`  round 0 -> 1

Program `problem-109-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_odd_rotations`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_odd_rotations(char *str) {
   6      int len = strlen(str);
   7      if (len == 0) return 0;
   8      
   9      int ones_count = 0;
  10      for (int i = 0; i < len; i++) {
  11          if (str[i] == '1') ones_count++;
  12      }
  13      
  14      if (ones_count % 2 == 0) return 0;
  15      
  16      int count = 0;
  17      char *rotated = (char *)malloc(len + 1);
  18      if (rotated == NULL) return -1;
  19      
  20      for (int i = 0; i < len; i++) {
  21          int idx = 0;
  22          for (int j = i; j < i + len; j++) {
  23              rotated[idx++] = str[j % len];
  24          }
  25          rotated[idx] = '\0';
  26          
  27          if (rotated[len - 1] == '1') {
  28              count++;
  29          }
  30      }
  31      
  32      free(rotated);
  33      return count;
  34  }
```

Round 1, where the rule appeared:

```c
  40  int main() {
  41      char str[101];
  42      
  43      printf("Enter binary string: ");
  44      if (fgets(str, sizeof(str), stdin) == NULL) {
  45          printf("Error reading input\n");
  46          return 1;
  47      }
  48      
  49      size_t len = strlen(str);
  50      if (len > 0 && str[len - 1] == '\n') {
  51          str[len - 1] = '\0';
  52          len--;
  53      }
  54      
  55      int result = count_odd_rotations(str, (int)sizeof(str));
  56      
  57      if (result == -1) {
  58          printf("Memory allocation failed\n");
  59      } else {
  60          printf("Count of rotations with odd value: %d\n", result);
  61      }
  62      
  63      return 0;
  64  }
```

## E043  `flawfinder:scanf`  round 0 -> 1

Program `problem-231` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_int64, read_size` (new function).

Round 0, where the rule was removed:

```c
  46  int main(void)
  47  {
  48      size_t rows;
  49  
  50      if (scanf("%zu", &rows) != 1 || rows == 0 ||
  51          rows > SIZE_MAX / sizeof(int64_t *)) {
  52          fputs("Invalid input\n", stderr);
  53          return EXIT_FAILURE;
  54      }
  55  
  56      int64_t **triangle = calloc(rows, sizeof(*triangle));
  57      if (triangle == NULL) {
  58          perror("calloc");
  59          return EXIT_FAILURE;
  60      }
  61  
  62      int status = EXIT_FAILURE;
  63  
  64      for (size_t row = 0; row < rows; ++row) {
  65          size_t count = row + 1;
  66  
  67          if (count > SIZE_MAX / sizeof(*triangle[row])) {
  68              fputs("Input is too large\n", stderr);
  69              goto cleanup;
  70          }
  71  
  72          triangle[row] = malloc(count * sizeof(*triangle[row]));
  73          if (triangle[row] == NULL) {
  74              perror("malloc");
  75              goto cleanup;
  76          }
  77  
  78          for (size_t col = 0; col < count; ++col) {
  79              if (scanf("%" SCNd64, &triangle[row][col]) != 1) {
  80                  fputs("Invalid input\n", stderr);
  81                  goto cleanup;
  82              }
  83          }
  84      }
  85  
      ... (21 more lines)
```

Round 1, where the rule appeared:

```c
   9  static int read_int64(int64_t *value)
  10  {
  11      char token[128];
  12  
  13      if (value == NULL || scanf("%127s", token) != 1) {
  14          return -1;
  15      }
  16  
  17      char *end;
  18      errno = 0;
  19      intmax_t parsed = strtoimax(token, &end, 10);
  20  
  21      if (errno == ERANGE || end == token || *end != '\0' ||
  22          parsed < INT64_MIN || parsed > INT64_MAX) {
  23          return -1;
  24      }
  25  
  26      *value = (int64_t)parsed;
  27      return 0;
  28  }
  30  static int read_size(size_t *value)
  31  {
  32      char token[128];
  33  
  34      if (value == NULL || scanf("%127s", token) != 1 || token[0] == '-') {
  35          return -1;
  36      }
  37  
  38      char *end;
  39      errno = 0;
  40      uintmax_t parsed = strtoumax(token, &end, 10);
  41  
  42      if (errno == ERANGE || end == token || *end != '\0' || parsed > SIZE_MAX) {
  43          return -1;
  44      }
  45  
  46      *value = (size_t)parsed;
  47      return 0;
  48  }
```

## E044  `flawfinder:strlen`  round 0 -> 1

Program `problem-61-s3` (feedback-static-r0-t1.0-p1.0); left `countSubstrings`, entered `main`.

Round 0, where the rule was removed:

```c
   4  int countSubstrings(char *str) {
   5      int n = strlen(str);
   6      int count = 0;
   7      
   8      for (int i = 0; i < n; i++) {
   9          int sum = 0;
  10          for (int j = i; j < n; j++) {
  11              sum += str[j] - '0';
  12              int len = j - i + 1;
  13              if (sum == len) {
  14                  count++;
  15              }
  16          }
  17      }
  18      
  19      return count;
  20  }
```

Round 1, where the rule appeared:

```c
  26  int main(void) {
  27      char str[1001];
  28      
  29      if (fgets(str, sizeof(str), stdin) != NULL) {
  30          size_t len = strlen(str);
  31          if (len > 0 && str[len - 1] == '\n') {
  32              str[len - 1] = '\0';
  33              len--;
  34          }
  35          if (len < sizeof(str)) {
  36              printf("%d\n", countSubstrings(str));
  37          }
  38      }
  39      
  40      return 0;
  41  }
```

## E045  `flawfinder:memcpy`  round 2 -> 3

Program `problem-421-s3` (feedback-static-r0-t1.0-p1.0); left `concatenate_tuple`, entered `safe_memcpy` (new function).

Round 2, where the rule was removed:

```c
  32  char *concatenate_tuple(const Tuple *tuple, const char *delimiter) {
  33      if (tuple == NULL || delimiter == NULL || tuple->elements == NULL || tuple->count == 0) {
  34          return NULL;
  35      }
  36  
  37      if (!is_valid_string(delimiter, SIZE_MAX / 2)) {
  38          return NULL;
  39      }
  40  
  41      size_t delimiter_length = safe_strlen(delimiter, SIZE_MAX / 2);
  42      size_t total_length = 0;
  43      size_t i;
  44  
  45      for (i = 0; i < tuple->count; i++) {
  46          if (tuple->elements[i] == NULL) {
  47              return NULL;
  48          }
  49          if (!is_valid_string(tuple->elements[i], SIZE_MAX / 2)) {
  50              return NULL;
  51          }
  52          size_t element_length = safe_strlen(tuple->elements[i], SIZE_MAX / 2);
  53          
  54          if (element_length > SIZE_MAX - total_length - 1) {
  55              return NULL;
  56          }
  57          total_length += element_length;
  58          
  59          if (i < tuple->count - 1) {
  60              if (delimiter_length > SIZE_MAX - total_length - 1) {
  61                  return NULL;
  62              }
  63              total_length += delimiter_length;
  64          }
  65      }
  66  
  67      if (total_length >= SIZE_MAX - 1) {
  68          return NULL;
  69      }
  70  
  71      char *result = malloc(total_length + 1);
      ... (30 more lines)
```

Round 3, where the rule appeared:

```c
  33  static bool safe_memcpy(void *dest, size_t dest_size, size_t offset, const void *src, size_t count) {
  34      if (dest == NULL || src == NULL) {
  35          return false;
  36      }
  37      if (offset > dest_size || count > dest_size - offset) {
  38          return false;
  39      }
  40      memcpy((char *)dest + offset, src, count);
  41      return true;
  42  }
```

## E046  `flawfinder:memcpy`  round 0 -> 1

Program `problem-582-s3` (feedback-static-r0-t1.0-p1.0); left `dict_insert`, entered `dup_bounded` (new function).

Round 0, where the rule was removed:

```c
  50  int dict_insert(Dictionary *dict, const char *key, const char *value)
  51  {
  52      unsigned long index;
  53      Entry *entry;
  54      size_t key_len;
  55      size_t value_len;
  56  
  57      if (dict == NULL || key == NULL || value == NULL) {
  58          return -1;
  59      }
  60  
  61      entry = malloc(sizeof(Entry));
  62      if (entry == NULL) {
  63          return -1;
  64      }
  65  
  66      key_len = strlen(key) + 1;
  67      value_len = strlen(value) + 1;
  68  
  69      entry->key = malloc(key_len);
  70      if (entry->key == NULL) {
  71          free(entry);
  72          return -1;
  73      }
  74  
  75      entry->value = malloc(value_len);
  76      if (entry->value == NULL) {
  77          free(entry->key);
  78          free(entry);
  79          return -1;
  80      }
  81  
  82      memcpy(entry->key, key, key_len);
  83      memcpy(entry->value, value, value_len);
  84  
  85      index = hash_key(key);
  86      entry->next = dict->buckets[index];
  87      dict->buckets[index] = entry;
  88      dict->count++;
  89      return 0;
      ... (1 more lines)
```

Round 1, where the rule appeared:

```c
  37  static char *dup_bounded(const char *src, size_t *out_len)
  38  {
  39      size_t len;
  40      char *copy;
  41  
  42      len = strnlen(src, MAX_STR_LEN);
  43      if (len >= MAX_STR_LEN) {
  44          return NULL;
  45      }
  46  
  47      copy = malloc(len + 1);
  48      if (copy == NULL) {
  49          return NULL;
  50      }
  51  
  52      memcpy(copy, src, len);
  53      copy[len] = '\0';
  54  
  55      if (out_len != NULL) {
  56          *out_len = len;
  57      }
  58      return copy;
  59  }
```

## E047  `flawfinder:memcpy`  round 3 -> 4

Program `problem-513` (feedback-static-r0-t1.0-p1.0); left `tuple_to_list`, entered `dup_buffer` (new function).

Round 3, where the rule was removed:

```c
  20  char **tuple_to_list(const int *tuple, size_t tuple_len, const char *insert_str, size_t *out_len)
  21  {
  22      char **list = NULL;
  23      size_t total = 0U;
  24      size_t insert_len = 0U;
  25      size_t i;
  26  
  27      if ((tuple == NULL) || (insert_str == NULL) || (out_len == NULL) || (tuple_len == 0U)) {
  28          return NULL;
  29      }
  30  
  31      if (tuple_len > (SIZE_MAX / 2U)) {
  32          return NULL;
  33      }
  34      total = tuple_len * 2U;
  35  
  36      if (total > (SIZE_MAX / sizeof(char *))) {
  37          return NULL;
  38      }
  39  
  40      insert_len = strnlen(insert_str, SIZE_MAX - 1U);
  41      if (insert_len >= (SIZE_MAX - 1U)) {
  42          return NULL;
  43      }
  44  
  45      list = (char **)calloc(total, sizeof(char *));
  46      if (list == NULL) {
  47          return NULL;
  48      }
  49  
  50      for (i = 0U; i < tuple_len; i++) {
  51          char numbuf[32];
  52          int written;
  53          size_t num_len;
  54          size_t idx = i * 2U;
  55  
  56          written = snprintf(numbuf, sizeof(numbuf), "%d", tuple[i]);
  57          if ((written < 0) || ((size_t)written >= sizeof(numbuf))) {
  58              cleanup_partial(list, idx);
  59              return NULL;
      ... (28 more lines)
```

Round 4, where the rule appeared:

```c
  21  static char *dup_buffer(const char *src, size_t len)
  22  {
  23      char *dst;
  24  
  25      if ((src == NULL) || (len >= (SIZE_MAX - 1U))) {
  26          return NULL;
  27      }
  28  
  29      dst = (char *)malloc(len + 1U);
  30      if (dst == NULL) {
  31          return NULL;
  32      }
  33  
  34      if (len > 0U) {
  35          memcpy(dst, src, len);
  36      }
  37      dst[len] = '\0';
  38      return dst;
  39  }
```

## E048  `flawfinder:char`  round 4 -> 5

Program `problem-621-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `increment_numeric_values`, entered `ll_to_str, safe_strtoll` (new function).

Round 4, where the rule was removed:

```c
   8  char* increment_numeric_values(const char* input, int k) {
   9      if (input == NULL) {
  10          return NULL;
  11      }
  12  
  13      size_t input_len = strlen(input);
  14      size_t result_size = input_len + 1;
  15      char* result = (char*)malloc(result_size);
  16      if (result == NULL) {
  17          return NULL;
  18      }
  19  
  20      size_t src_idx = 0;
  21      size_t dst_idx = 0;
  22  
  23      while (src_idx < input_len) {
  24          if (isdigit((unsigned char)input[src_idx])) {
  25              size_t num_start = src_idx;
  26              while (src_idx < input_len && isdigit((unsigned char)input[src_idx])) {
  27                  src_idx++;
  28              }
  29              size_t num_len = src_idx - num_start;
  30  
  31              char* num_str = (char*)malloc(num_len + 1);
  32              if (num_str == NULL) {
  33                  free(result);
  34                  return NULL;
  35              }
  36  
  37              memcpy(num_str, input + num_start, num_len);
  38              num_str[num_len] = '\0';
  39  
  40              errno = 0;
  41              long long num_val = strtoll(num_str, NULL, 10);
  42              free(num_str);
  43  
  44              if (errno == ERANGE) {
  45                  free(result);
  46                  return NULL;
  47              }
      ... (68 more lines)
```

Round 5, where the rule appeared:

```c
  46  static void ll_to_str(long long val, char* out) {
  47      if (val == 0) {
  48          out[0] = '0';
  49          return;
  50      }
  51      char* p = out;
  52      if (val < 0) {
  53          *p++ = '-';
  54          val = -val;
  55      }
  56      char tmp[32];
  57      int idx = 0;
  58      while (val > 0) {
  59          tmp[idx++] = (char)('0' + (val % 10));
  60          val /= 10;
  61      }
  62      while (idx > 0) {
  63          *p++ = tmp[--idx];
  64      }
  65  }
   8  static int safe_strtoll(const char* str, size_t len, long long* out) {
   9      if (len == 0 || len >= 64) return 0;
  10      char buf[64];
  11      memcpy(buf, str, len);
  12      buf[len] = '\0';
  13      
  14      char* endptr = NULL;
  15      errno = 0;
  16      long long val = strtoll(buf, &endptr, 10);
  17      if (errno == ERANGE || endptr != buf + len) return 0;
  18      *out = val;
  19      return 1;
  20  }
```

## E049  `flawfinder:memcpy`  round 0 -> 1

Program `problem-862` (feedback-static-r0-t1.0-p1.0); left `find_n_most_common`, entered `add_word`.

Round 0, where the rule was removed:

```c
  47  int find_n_most_common(const char *text, int n, WordCount *results) {
  48      if (text == NULL || results == NULL || n <= 0) {
  49          errno = EINVAL;
  50          return -1;
  51      }
  52      
  53      num_words = 0;
  54      char current_word[MAX_WORD_LEN];
  55      int cw_idx = 0;
  56      
  57      for (int i = 0; text[i] != '\0'; i++) {
  58          if (isalpha((unsigned char)text[i])) {
  59              if (cw_idx < MAX_WORD_LEN - 1) {
  60                  current_word[cw_idx++] = tolower((unsigned char)text[i]);
  61              }
  62          } else {
  63              if (cw_idx > 0) {
  64                  current_word[cw_idx] = '\0';
  65                  int idx = find_word(current_word);
  66                  if (idx >= 0) {
  67                      word_counts[idx].count++;
  68                  } else {
  69                      if (add_word(current_word) != 0) {
  70                          errno = ENOMEM;
  71                          return -1;
  72                      }
  73                  }
  74                  cw_idx = 0;
  75              }
  76          }
  77      }
  78      
  79      if (cw_idx > 0) {
  80          current_word[cw_idx] = '\0';
  81          int idx = find_word(current_word);
  82          if (idx >= 0) {
  83              word_counts[idx].count++;
  84          } else {
  85              if (add_word(current_word) != 0) {
  86                  errno = ENOMEM;
      ... (15 more lines)
```

Round 1, where the rule appeared:

```c
  27  static int add_word(const char *word, size_t len) {
  28      if (num_words >= MAX_WORDS) {
  29          return -1;
  30      }
  31      size_t copy_len = len < MAX_WORD_LEN - 1 ? len : MAX_WORD_LEN - 1;
  32      memcpy(word_counts[num_words].word, word, copy_len);
  33      word_counts[num_words].word[copy_len] = '\0';
  34      word_counts[num_words].count = 1;
  35      num_words++;
  36      return 0;
  37  }
```

## E050  `flawfinder:getchar`  round 1 -> 2

Program `problem-350` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `discard_line` (new function).

Round 1, where the rule was removed:

```c
  32  int main(void)
  33  {
  34      char *input;
  35      char *character_line;
  36      char *result;
  37      size_t input_length;
  38      int ch;
  39  
  40      input = malloc(INPUT_CAPACITY);
  41      character_line = malloc(INPUT_CAPACITY);
  42  
  43      if (input == NULL || character_line == NULL) {
  44          free(input);
  45          free(character_line);
  46          return EXIT_FAILURE;
  47      }
  48  
  49      if (fgets(input, INPUT_CAPACITY, stdin) == NULL) {
  50          free(input);
  51          free(character_line);
  52          return EXIT_FAILURE;
  53      }
  54  
  55      input_length = strnlen(input, INPUT_CAPACITY);
  56      if (input_length == INPUT_CAPACITY) {
  57          free(input);
  58          free(character_line);
  59          return EXIT_FAILURE;
  60      }
  61  
  62      if (input_length > 0 && input[input_length - 1] == '\n') {
  63          input[--input_length] = '\0';
  64      } else {
  65          while ((ch = getchar()) != '\n' && ch != EOF) {
  66          }
  67      }
  68  
  69      if (fgets(character_line, INPUT_CAPACITY, stdin) == NULL ||
  70          character_line[0] == '\0' ||
  71          character_line[0] == '\n') {
      ... (21 more lines)
```

Round 2, where the rule appeared:

```c
  34  static int discard_line(void)
  35  {
  36      int ch;
  37  
  38      do {
  39          ch = getchar();
  40      } while (ch != '\n' && ch != EOF);
  41  
  42      return ch;
  43  }
```

## E051  `flawfinder:strlen`  round 0 -> 1

Program `problem-964-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `is_length_even`, entered `main`.

Round 0, where the rule was removed:

```c
   5  bool is_length_even(const char *word) {
   6      if (word == NULL) {
   7          return false;
   8      }
   9      size_t len = strlen(word);
  10      return (len % 2) == 0;
  11  }
```

Round 1, where the rule appeared:

```c
  12  int main() {
  13      const char *test_word1 = "even";
  14      const char *test_word2 = "odd";
  15      
  16      printf("%s: %s\n", test_word1, is_length_even(test_word1, strlen(test_word1)) ? "Even" : "Odd");
  17      printf("%s: %s\n", test_word2, is_length_even(test_word2, strlen(test_word2)) ? "Even" : "Odd");
  18  
  19      return 0;
  20  }
```

## E052  `flawfinder:strlen`  round 0 -> 1

Program `problem-788` (feedback-static-r0-t1.0-p1.0); left `create_tuple`, entered `main`.

Round 0, where the rule was removed:

```c
  11  Tuple *create_tuple(const char *str, const int *list, size_t list_size) {
  12      if (str == NULL || (list == NULL && list_size > 0)) {
  13          return NULL;
  14      }
  15  
  16      Tuple *tuple = (Tuple *)malloc(sizeof(Tuple));
  17      if (tuple == NULL) {
  18          return NULL;
  19      }
  20  
  21      tuple->str = (char *)malloc(strlen(str) + 1);
  22      if (tuple->str == NULL) {
  23          free(tuple);
  24          return NULL;
  25      }
  26      strcpy(tuple->str, str);
  27  
  28      tuple->list_size = list_size;
  29      if (list_size > 0) {
  30          tuple->list = (int *)malloc(list_size * sizeof(int));
  31          if (tuple->list == NULL) {
  32              free(tuple->str);
  33              free(tuple);
  34              return NULL;
  35          }
  36          memcpy(tuple->list, list, list_size * sizeof(int));
  37      } else {
  38          tuple->list = NULL;
  39      }
  40  
  41      return tuple;
  42  }
```

Round 1, where the rule appeared:

```c
  58  int main(void) {
  59      const char *str = "example";
  60      size_t str_len = strlen(str);
  61      int list[] = {1, 2, 3, 4, 5};
  62      size_t list_size = sizeof(list) / sizeof(list[0]);
  63  
  64      Tuple *tuple = create_tuple(str, str_len, list, list_size);
  65      if (tuple == NULL) {
  66          fprintf(stderr, "Failed to create tuple\n");
  67          return EXIT_FAILURE;
  68      }
  69  
  70      printf("String: %s\n", tuple->str);
  71      printf("List: ");
  72      for (size_t i = 0; i < tuple->list_size; i++) {
  73          printf("%d ", tuple->list[i]);
  74      }
  75      printf("\n");
  76  
  77      free_tuple(tuple);
  78  
  79      return EXIT_SUCCESS;
  80  }
```

## E053  `flawfinder:memcpy`  round 2 -> 3

Program `problem-513-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `join_tuple_with_string, tuple_to_list_with_string`, entered `safe_memcpy` (new function).

Round 2, where the rule was removed:

```c
  30  char *join_tuple_with_string(char **tuple, size_t count, const char *separator) {
  31      size_t total_length;
  32      size_t separator_length;
  33      size_t i;
  34      char *result;
  35      char *current_pos;
  36      size_t remaining;
  37  
  38      if (tuple == NULL || separator == NULL || count == 0) {
  39          return NULL;
  40      }
  41  
  42      separator_length = safe_strlen(separator, SIZE_MAX);
  43      total_length = 1;
  44  
  45      for (i = 0; i < count; i++) {
  46          size_t item_length;
  47          size_t temp;
  48  
  49          if (tuple[i] == NULL) {
  50              return NULL;
  51          }
  52  
  53          item_length = safe_strlen(tuple[i], SIZE_MAX);
  54          if (!safe_add_size(item_length, separator_length, &temp)) {
  55              return NULL;
  56          }
  57          if (!safe_add_size(total_length, temp, &total_length)) {
  58              return NULL;
  59          }
  60      }
  61  
  62      result = malloc(total_length);
  63      if (result == NULL) {
  64          return NULL;
  65      }
  66  
  67      current_pos = result;
  68      remaining = total_length;
  69  
      ... (27 more lines)
  98  StringList *tuple_to_list_with_string(char **tuple, size_t count, const char *separator) {
  99      StringList *list;
 100      size_t i;
 101      size_t separator_length;
 102      size_t total_count;
 103  
 104      if (tuple == NULL || separator == NULL || count == 0) {
 105          return NULL;
 106      }
 107  
 108      if (count > SIZE_MAX / 2) {
 109          return NULL;
 110      }
 111      total_count = count * 2;
 112  
 113      list = malloc(sizeof(StringList));
 114      if (list == NULL) {
 115          return NULL;
 116      }
 117  
 118      list->count = total_count;
 119      list->items = calloc(total_count, sizeof(char *));
 120      if (list->items == NULL) {
 121          free(list);
 122          return NULL;
 123      }
 124  
 125      separator_length = safe_strlen(separator, SIZE_MAX);
 126  
 127      for (i = 0; i < count; i++) {
 128          size_t item_length;
 129  
 130          if (tuple[i] == NULL) {
 131              size_t j;
 132              for (j = 0; j < i * 2; j++) {
 133                  free(list->items[j]);
 134              }
 135              free(list->items);
 136              free(list);
 137              return NULL;
      ... (46 more lines)
```

Round 3, where the rule appeared:

```c
  30  static int safe_memcpy(void *dest, size_t dest_size, const void *src, size_t src_size) {
  31      if (dest == NULL || src == NULL) {
  32          return 0;
  33      }
  34      if (src_size > dest_size) {
  35          return 0;
  36      }
  37      if (src_size > 0) {
  38          memcpy(dest, src, src_size);
  39      }
  40      return 1;
  41  }
```

## E054  `flawfinder:getchar`  round 4 -> 5

Program `problem-789-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `clear_input_stream` (new function).

Round 4, where the rule was removed:

```c
  26  int main(void) {
  27      char input_buffer[512];
  28      int num_sides = 0;
  29      double side_len = 0.0;
  30      double perimeter = 0.0;
  31      char *end_ptr = NULL;
  32      long parsed_sides = 0;
  33      size_t len = 0;
  34  
  35      printf("Enter number of sides: ");
  36      if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
  37          fprintf(stderr, "Error reading input.\n");
  38          return EXIT_FAILURE;
  39      }
  40  
  41      len = strnlen(input_buffer, sizeof(input_buffer));
  42      if (len == 0 || input_buffer[len - 1] != '\n') {
  43          if (len >= sizeof(input_buffer) - 1) {
  44              int c;
  45              while ((c = getchar()) != '\n' && c != EOF) {
  46              }
  47          }
  48          fprintf(stderr, "Invalid input for number of sides.\n");
  49          return EXIT_FAILURE;
  50      }
  51  
  52      input_buffer[len - 1] = '\0';
  53      errno = 0;
  54      parsed_sides = strtol(input_buffer, &end_ptr, 10);
  55      if (errno == ERANGE || end_ptr == input_buffer || *end_ptr != '\0') {
  56          fprintf(stderr, "Invalid input for number of sides.\n");
  57          return EXIT_FAILURE;
  58      }
  59      if (parsed_sides < INT_MIN || parsed_sides > INT_MAX) {
  60          fprintf(stderr, "Invalid input for number of sides.\n");
  61          return EXIT_FAILURE;
  62      }
  63      num_sides = (int)parsed_sides;
  64  
  65      printf("Enter side length: ");
      ... (34 more lines)
```

Round 5, where the rule appeared:

```c
  28  static void clear_input_stream(void) {
  29      int c;
  30      while ((c = getchar()) != '\n' && c != EOF) {
  31      }
  32  }
```

## E055  `flawfinder:fgetc`  round 2 -> 3

Program `problem-179-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 2, where the rule was removed:

```c
  53  int main(void)
  54  {
  55      char *input = NULL;
  56      size_t capacity = 0;
  57      size_t length = 0;
  58      char *start;
  59      char *end;
  60      unsigned long long number;
  61      int character;
  62  
  63      while ((character = fgetc(stdin)) != '\n' && character != EOF) {
  64          if (length + 1 >= capacity) {
  65              size_t new_capacity;
  66              char *temporary;
  67  
  68              if (capacity == 0) {
  69                  new_capacity = 64;
  70              } else {
  71                  if (capacity > SIZE_MAX / 2) {
  72                      free(input);
  73                      fputs("Input is too large.\n", stderr);
  74                      return EXIT_FAILURE;
  75                  }
  76                  new_capacity = capacity * 2;
  77              }
  78  
  79              temporary = realloc(input, new_capacity);
  80              if (temporary == NULL) {
  81                  free(input);
  82                  fputs("Failed to allocate memory.\n", stderr);
  83                  return EXIT_FAILURE;
  84              }
  85  
  86              input = temporary;
  87              capacity = new_capacity;
  88          }
  89  
  90          input[length++] = (char)character;
  91      }
  92  
      ... (64 more lines)
```

Round 3, where the rule appeared:

```c
  54  static char *read_line(FILE *stream)
  55  {
  56      char *buffer = NULL;
  57      size_t capacity = 0;
  58      size_t length = 0;
  59      int character;
  60  
  61      while ((character = fgetc(stream)) != '\n' && character != EOF) {
  62          if (length == SIZE_MAX - 1) {
  63              free(buffer);
  64              errno = EOVERFLOW;
  65              return NULL;
  66          }
  67  
  68          if (length + 1 >= capacity) {
  69              size_t new_capacity;
  70              char *temporary;
  71  
  72              if (capacity == 0) {
  73                  new_capacity = 64;
  74              } else {
  75                  if (capacity > SIZE_MAX / 2) {
  76                      free(buffer);
  77                      errno = EOVERFLOW;
  78                      return NULL;
  79                  }
  80                  new_capacity = capacity * 2;
  81              }
  82  
  83              temporary = realloc(buffer, new_capacity);
  84              if (temporary == NULL) {
  85                  free(buffer);
  86                  return NULL;
  87              }
  88  
  89              buffer = temporary;
  90              capacity = new_capacity;
  91          }
  92  
  93          buffer[length++] = (char)character;
      ... (32 more lines)
```

## E056  `flawfinder:strlen`  round 0 -> 1

Program `problem-175` (feedback-static+dynamic-r0-t1.0-p1.0); left `isValidParentheses`, entered `main`.

Round 0, where the rule was removed:

```c
   5  bool isValidParentheses(const char* s) {
   6      int len = strlen(s);
   7      if (len % 2 != 0) return false;
   8  
   9      int stack[len];
  10      int top = -1;
  11  
  12      for (int i = 0; i < len; i++) {
  13          if (s[i] == '(') {
  14              stack[++top] = i;
  15          } else if (s[i] == ')') {
  16              if (top == -1) return false;
  17              --top;
  18          }
  19      }
  20  
  21      return top == -1;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main() {
  24      const char* test1 = "()";
  25      const char* test2 = "(()())";
  26      const char* test3 = "(()";
  27      const char* test4 = "())(";
  28  
  29      printf("%s: %s\n", test1, isValidParentheses(test1, strlen(test1)) ? "Valid" : "Invalid");
  30      printf("%s: %s\n", test2, isValidParentheses(test2, strlen(test2)) ? "Valid" : "Invalid");
  31      printf("%s: %s\n", test3, isValidParentheses(test3, strlen(test3)) ? "Valid" : "Invalid");
  32      printf("%s: %s\n", test4, isValidParentheses(test4, strlen(test4)) ? "Valid" : "Invalid");
  33  
  34      return 0;
  35  }
```

## E057  `flawfinder:fgetc`  round 3 -> 4

Program `problem-891-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `discard_remaining_input`, entered `read_number`.

Round 3, where the rule was removed:

```c
  23  static bool discard_remaining_input(void)
  24  {
  25      int character;
  26  
  27      do {
  28          character = fgetc(stdin);
  29      } while (character != '\n' && character != EOF);
  30  
  31      return character == '\n';
  32  }
```

Round 4, where the rule appeared:

```c
  34  static bool read_number(const char *prompt, long long *value)
  35  {
  36      char *buffer = NULL;
  37      size_t capacity = 0;
  38      size_t length = 0;
  39      char *end = NULL;
  40      long long parsed_value;
  41      int character;
  42  
  43      if (prompt == NULL || value == NULL) {
  44          return false;
  45      }
  46  
  47      if (fputs(prompt, stdout) == EOF || fflush(stdout) == EOF) {
  48          return false;
  49      }
  50  
  51      while ((character = fgetc(stdin)) != '\n' && character != EOF) {
  52          if (length + 1 >= capacity) {
  53              size_t new_capacity = capacity == 0 ? 32 : capacity * 2;
  54              char *new_buffer;
  55  
  56              if (new_capacity <= capacity) {
  57                  free(buffer);
  58                  discard_remaining_input();
  59                  return false;
  60              }
  61  
  62              new_buffer = realloc(buffer, new_capacity);
  63              if (new_buffer == NULL) {
  64                  free(buffer);
  65                  discard_remaining_input();
  66                  return false;
  67              }
  68  
  69              buffer = new_buffer;
  70              capacity = new_capacity;
  71          }
  72  
  73          buffer[length++] = (char)character;
      ... (43 more lines)
```

## E058  `flawfinder:strlen`  round 0 -> 1

Program `problem-964` (feedback-static+dynamic-r0-t1.0-p1.0); left `is_word_length_even`, entered `main`.

Round 0, where the rule was removed:

```c
   4  int is_word_length_even(const char *word) {
   5      if (word == NULL) {
   6          return 0;
   7      }
   8      size_t length = strlen(word);
   9      return (length % 2 == 0) ? 1 : 0;
  10  }
```

Round 1, where the rule appeared:

```c
  11  int main() {
  12      const char *test_word = "example";
  13      size_t test_word_len = strlen(test_word);
  14      int result = is_word_length_even(test_word, test_word_len);
  15      if (result) {
  16          printf("The word length is even.\n");
  17      } else {
  18          printf("The word length is odd.\n");
  19      }
  20      return 0;
  21  }
```

## E059  `flawfinder:memcpy`  round 0 -> 1

Program `problem-544-s3` (feedback-static-r0-t1.0-p1.0); left `flatten_tuple_list`, entered `append_bytes` (new function).

Round 0, where the rule was removed:

```c
  22  static char *flatten_tuple_list(const Tuple *tuples, size_t count)
  23  {
  24      const char *tuple_separator = ", ";
  25      const char *item_separator = ": ";
  26      const size_t tuple_separator_length = strlen(tuple_separator);
  27      const size_t item_separator_length = strlen(item_separator);
  28      size_t total_length = 0;
  29      char *output;
  30      char *cursor;
  31  
  32      if (count > 0 && tuples == NULL) {
  33          errno = EINVAL;
  34          return NULL;
  35      }
  36  
  37      for (size_t i = 0; i < count; ++i) {
  38          size_t first_length;
  39          size_t second_length;
  40  
  41          if (tuples[i].first == NULL || tuples[i].second == NULL) {
  42              errno = EINVAL;
  43              return NULL;
  44          }
  45  
  46          first_length = strlen(tuples[i].first);
  47          second_length = strlen(tuples[i].second);
  48  
  49          if (!checked_add_size(total_length, first_length, &total_length) ||
  50              !checked_add_size(total_length, item_separator_length, &total_length) ||
  51              !checked_add_size(total_length, second_length, &total_length) ||
  52              (i + 1 < count &&
  53               !checked_add_size(total_length, tuple_separator_length,
  54                                 &total_length))) {
  55              errno = EOVERFLOW;
  56              return NULL;
  57          }
  58      }
  59  
  60      if (!checked_add_size(total_length, 1, &total_length)) {
  61          errno = EOVERFLOW;
      ... (32 more lines)
```

Round 1, where the rule appeared:

```c
  24  static int append_bytes(char **cursor, size_t *remaining,
  25                          const char *source, size_t length)
  26  {
  27      if (cursor == NULL || *cursor == NULL || remaining == NULL ||
  28          (length > 0 && source == NULL) || length > *remaining) {
  29          return 0;
  30      }
  31  
  32      if (length > 0) {
  33          memcpy(*cursor, source, length);
  34          *cursor += length;
  35          *remaining -= length;
  36      }
  37  
  38      return 1;
  39  }
```

## E060  `fuzz:UBSan:signed integer overflow`  round 1 -> 2

Program `problem-504-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `cube_sum`, entered `is_valid_input`.

Round 1, where the rule was removed:

```c
  10  long long cube_sum(int n) {
  11      long long sum = 0;
  12      for (int i = 1; i <= n; i++) {
  13          if (!is_valid_input(i)) {
  14              fprintf(stderr, "Input too large, causing overflow.\n");
  15              exit(EXIT_FAILURE);
  16          }
  17          sum += (long long)i * i * i;
  18      }
  19      return sum;
  20  }
```

Round 2, where the rule appeared:

```c
   6  bool is_valid_input(int n) {
   7      return n > 0 && (long long)n * n * n <= LLONG_MAX;
   8  }
```

## E061  `flawfinder:strlen`  round 0 -> 1

Program `problem-532-s3` (feedback-static-r0-t1.0-p1.0); left `are_permutations`, entered `main`.

Round 0, where the rule was removed:

```c
   7  int are_permutations(const char *str1, const char *str2) {
   8      if (str1 == NULL || str2 == NULL) {
   9          return 0;
  10      }
  11  
  12      size_t len1 = strlen(str1);
  13      size_t len2 = strlen(str2);
  14  
  15      if (len1 != len2) {
  16          return 0;
  17      }
  18  
  19      size_t *count = (size_t *)calloc(ASCII_SIZE, sizeof(size_t));
  20      if (count == NULL) {
  21          return 0;
  22      }
  23  
  24      for (size_t i = 0; i < len1; i++) {
  25          unsigned char c1 = (unsigned char)str1[i];
  26          unsigned char c2 = (unsigned char)str2[i];
  27          count[c1]++;
  28          count[c2]--;
  29      }
  30  
  31      int result = 1;
  32      for (size_t i = 0; i < ASCII_SIZE; i++) {
  33          if (count[i] != 0) {
  34              result = 0;
  35              break;
  36          }
  37      }
  38  
  39      free(count);
  40      return result;
  41  }
```

Round 1, where the rule appeared:

```c
  40  int main(int argc, char * const argv[]) {
  41      if (argc != 3) {
  42          fprintf(stderr, "Usage: %s <string1> <string2>\n", argv[0]);
  43          return EXIT_FAILURE;
  44      }
  45  
  46      if (are_permutations(argv[1], strlen(argv[1]), argv[2], strlen(argv[2]))) {
  47          printf("Permutations\n");
  48      } else {
  49          printf("Not Permutations\n");
  50      }
  51  
  52      return EXIT_SUCCESS;
  53  }
```

## E062  `flawfinder:char`  round 2 -> 3

Program `problem-318` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_side_sum` (new function).

Round 2, where the rule was removed:

```c
  29  int main(void)
  30  {
  31      char input[256];
  32      char *end;
  33      double side_sum;
  34      double volume;
  35  
  36      if (fgets(input, sizeof input, stdin) == NULL) {
  37          fputs("Failed to read the sum of sides.\n", stderr);
  38          return EXIT_FAILURE;
  39      }
  40  
  41      if (strchr(input, '\n') == NULL && !feof(stdin)) {
  42          int ch;
  43  
  44          do {
  45              ch = fgetc(stdin);
  46          } while (ch != '\n' && ch != EOF);
  47  
  48          fputs("Invalid sum of sides.\n", stderr);
  49          return EXIT_FAILURE;
  50      }
  51  
  52      errno = 0;
  53      side_sum = strtod(input, &end);
  54  
  55      if (errno == ERANGE || end == input) {
  56          fputs("Invalid sum of sides.\n", stderr);
  57          return EXIT_FAILURE;
  58      }
  59  
  60      while (*end == ' ' || *end == '\t' || *end == '\n' ||
  61             *end == '\r' || *end == '\f' || *end == '\v') {
  62          ++end;
  63      }
  64  
  65      if (*end != '\0' ||
  66          maximum_cuboid_volume(side_sum, &volume) != 0) {
  67          fputs("Invalid sum of sides.\n", stderr);
  68          return EXIT_FAILURE;
      ... (8 more lines)
```

Round 3, where the rule appeared:

```c
  28  static int read_side_sum(double *side_sum)
  29  {
  30      char input[256];
  31      char extra;
  32      char *end;
  33  
  34      if (side_sum == NULL) {
  35          return -1;
  36      }
  37  
  38      if (fgets(input, sizeof input, stdin) == NULL) {
  39          return -1;
  40      }
  41  
  42      if (sscanf(input, "%*255[^\n]%c", &extra) == 1 && extra != '\n') {
  43          return -1;
  44      }
  45  
  46      errno = 0;
  47      *side_sum = strtod(input, &end);
  48  
  49      if (end == input || errno == ERANGE) {
  50          return -1;
  51      }
  52  
  53      while (*end == ' ' || *end == '\t' || *end == '\n' ||
  54             *end == '\r' || *end == '\f' || *end == '\v') {
  55          ++end;
  56      }
  57  
  58      return *end == '\0' ? 0 : -1;
  59  }
```

## E063  `flawfinder:memcpy`  round 1 -> 2

Program `problem-390-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `insert_string_at_beginning`, entered `create_node`.

Round 1, where the rule was removed:

```c
  10  void insert_string_at_beginning(struct Node *head, const char *prefix) {
  11      if (head == NULL || prefix == NULL) {
  12          return;
  13      }
  14  
  15      struct Node *current = head;
  16      size_t prefix_len = strnlen(prefix, SIZE_MAX);
  17  
  18      while (current != NULL) {
  19          if (current->data == NULL) {
  20              current = current->next;
  21              continue;
  22          }
  23  
  24          size_t current_len = strnlen(current->data, SIZE_MAX);
  25          size_t new_len = prefix_len + current_len;
  26          
  27          char *new_data = (char *)malloc(new_len + 1);
  28          if (new_data == NULL) {
  29              fprintf(stderr, "Memory allocation failed\n");
  30              current = current->next;
  31              continue;
  32          }
  33  
  34          memcpy(new_data, prefix, prefix_len);
  35          memcpy(new_data + prefix_len, current->data, current_len);
  36          new_data[new_len] = '\0';
  37  
  38          free(current->data);
  39          current->data = new_data;
  40  
  41          current = current->next;
  42      }
  43  }
```

Round 2, where the rule appeared:

```c
  44  struct Node *create_node(const char *str) {
  45      if (str == NULL) {
  46          return NULL;
  47      }
  48  
  49      struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  50      if (new_node == NULL) {
  51          return NULL;
  52      }
  53  
  54      size_t len = strlen(str);
  55      new_node->data = (char *)malloc(len + 1);
  56      if (new_node->data == NULL) {
  57          free(new_node);
  58          return NULL;
  59      }
  60      memcpy(new_node->data, str, len + 1);
  61      new_node->next = NULL;
  62      return new_node;
  63  }
```

## E064  `flawfinder:char`  round 0 -> 1

Program `problem-343-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `count_digits_and_letters`.

Round 0, where the rule was removed:

```c
  33  int main(void)
  34  {
  35      char input[1024];
  36  
  37      if (fgets(input, sizeof input, stdin) == NULL) {
  38          if (ferror(stdin) != 0) {
  39              return 1;
  40          }
  41          input[0] = '\0';
  42      }
  43  
  44      CharacterCounts counts = count_digits_and_letters(input);
  45  
  46      if (printf("Digits: %zu\nLetters: %zu\n",
  47                 counts.digits,
  48                 counts.letters) < 0) {
  49          return 1;
  50      }
  51  
  52      return 0;
  53  }
```

Round 1, where the rule appeared:

```c
   9  static int count_digits_and_letters(CharacterCounts *counts)
  10  {
  11      unsigned char buffer[4096];
  12  
  13      counts->digits = 0;
  14      counts->letters = 0;
  15  
  16      while (fgets((char *)buffer, sizeof buffer, stdin) != NULL) {
  17          for (size_t i = 0; buffer[i] != '\0'; ++i) {
  18              if (isdigit(buffer[i]) != 0) {
  19                  ++counts->digits;
  20              } else if (isalpha(buffer[i]) != 0) {
  21                  ++counts->letters;
  22              }
  23          }
  24      }
  25  
  26      return ferror(stdin) == 0;
  27  }
```

## E065  `flawfinder:strlen`  round 2 -> 3

Program `problem-337-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `match_word_at_end`, entered `main`.

Round 2, where the rule was removed:

```c
   5  int match_word_at_end(const char *str, const char *word) {
   6      size_t str_len;
   7      size_t word_len;
   8      size_t effective_str_len;
   9      const char *p;
  10  
  11      if (str == NULL || word == NULL) {
  12          return 0;
  13      }
  14  
  15      str_len = strlen(str);
  16      word_len = strlen(word);
  17  
  18      if (str_len == 0 || word_len == 0) {
  19          return 0;
  20      }
  21  
  22      p = str + str_len - 1;
  23  
  24      while (p >= str && ispunct((unsigned char)*p)) {
  25          p--;
  26      }
  27  
  28      if (p < str) {
  29          return 0;
  30      }
  31  
  32      effective_str_len = (size_t)(p - str + 1);
  33  
  34      if (effective_str_len < word_len) {
  35          return 0;
  36      }
  37  
  38      if (effective_str_len > word_len) {
  39          if (!isspace((unsigned char)str[effective_str_len - word_len - 1])) {
  40              return 0;
  41          }
  42      }
  43  
  44      for (size_t i = 0; i < word_len; i++) {
      ... (7 more lines)
```

Round 3, where the rule appeared:

```c
  44  int main(void) {
  45      const char *test_str1 = "Hello world!";
  46      const char *test_str2 = "Hello world";
  47      const char *test_str3 = "world";
  48      const char *test_str4 = "Helloworld";
  49      const char *test_str5 = "Hello world...";
  50      const char *test_str6 = "Hello world, how are you?";
  51      const char *test_word = "world";
  52  
  53      printf("%d\n", match_word_at_end(test_str1, strlen(test_str1), test_word, strlen(test_word)));
  54      printf("%d\n", match_word_at_end(test_str2, strlen(test_str2), test_word, strlen(test_word)));
  55      printf("%d\n", match_word_at_end(test_str3, strlen(test_str3), test_word, strlen(test_word)));
  56      printf("%d\n", match_word_at_end(test_str4, strlen(test_str4), test_word, strlen(test_word)));
  57      printf("%d\n", match_word_at_end(test_str5, strlen(test_str5), test_word, strlen(test_word)));
  58      printf("%d\n", match_word_at_end(test_str6, strlen(test_str6), test_word, strlen(test_word)));
  59  
  60      return 0;
  61  }
```

## E066  `flawfinder:getchar`  round 4 -> 5

Program `problem-343-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `flush_stdin` (new function).

Round 4, where the rule was removed:

```c
  27  int main(void)
  28  {
  29      char input[256];
  30      size_t digits = 0;
  31      size_t letters = 0;
  32      size_t len;
  33  
  34      printf("Enter a string: ");
  35      fflush(stdout);
  36  
  37      if (fgets(input, sizeof(input), stdin) == NULL) {
  38          fprintf(stderr, "Error reading input\n");
  39          return 1;
  40      }
  41  
  42      input[sizeof(input) - 1] = '\0';
  43  
  44      len = strnlen(input, sizeof(input));
  45      if (len > 0 && input[len - 1] == '\n') {
  46          input[len - 1] = '\0';
  47      } else {
  48          int ch;
  49          while ((ch = getchar()) != '\n' && ch != EOF) {
  50          }
  51      }
  52  
  53      count_digits_letters(input, sizeof(input), &digits, &letters);
  54  
  55      printf("Digits: %zu\n", digits);
  56      printf("Letters: %zu\n", letters);
  57  
  58      return 0;
  59  }
```

Round 5, where the rule appeared:

```c
  27  static void flush_stdin(void)
  28  {
  29      int ch;
  30      while ((ch = getchar()) != '\n' && ch != EOF) {
  31      }
  32  }
```

## E067  `flawfinder:memcpy`  round 0 -> 1

Program `problem-966-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `add_tuple` (new function).

Round 0, where the rule was removed:

```c
  62  int main() {
  63      int capacity = 5;
  64      TupleList list = create_tuple_list(capacity);
  65      if (list.items == NULL) {
  66          return EXIT_FAILURE;
  67      }
  68  
  69      int a[] = {1, 2};
  70      int b[] = {0};
  71      int d[] = {5, 6, 7};
  72  
  73      list.items[0].elements = (int *)malloc(2 * sizeof(int));
  74      memcpy(list.items[0].elements, a, 2 * sizeof(int));
  75      list.items[0].size = 2;
  76  
  77      list.items[1].elements = NULL;
  78      list.items[1].size = 0;
  79  
  80      list.items[2].elements = (int *)malloc(sizeof(int));
  81      memcpy(list.items[2].elements, b, sizeof(int));
  82      list.items[2].size = 1;
  83  
  84      list.items[3].elements = NULL;
  85      list.items[3].size = 0;
  86  
  87      list.items[4].elements = (int *)malloc(3 * sizeof(int));
  88      memcpy(list.items[4].elements, d, 3 * sizeof(int));
  89      list.items[4].size = 3;
  90  
  91      list.count = capacity;
  92  
  93      for (int i = 0; i < list.count; i++) {
  94          printf("Tuple %d: size=%d\n", i, list.items[i].size);
  95      }
  96  
  97      int removed = remove_empty_tuples(&list);
  98      printf("Removed: %d\n", removed);
  99  
 100      for (int i = 0; i < list.count; i++) {
 101          printf("Tuple %d: size=%d\n", i, list.items[i].size);
      ... (6 more lines)
```

Round 1, where the rule appeared:

```c
  66  int add_tuple(TupleList *list, const int *data, size_t size) {
  67      if (list == NULL || data == NULL) {
  68          return -1;
  69      }
  70      if (size > 0) {
  71          int *new_elements = (int *)malloc(size * sizeof(int));
  72          if (new_elements == NULL) {
  73              return -1;
  74          }
  75          memcpy(new_elements, data, size * sizeof(int));
  76          list->items[list->count].elements = new_elements;
  77      } else {
  78          list->items[list->count].elements = NULL;
  79      }
  80      list->items[list->count].size = size;
  81      list->count++;
  82      return 0;
  83  }
```

## E068  `flawfinder:strlen`  round 3 -> 4

Program `problem-30-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `countSubstringsWithSameStartAndEnd`, entered `main`.

Round 3, where the rule was removed:

```c
   4  int countSubstringsWithSameStartAndEnd(const char *str) {
   5      int count = 0;
   6      size_t len = strlen(str);
   7      for (size_t i = 0; i < len; ++i) {
   8          for (size_t j = i + 1; j < len; ++j) {
   9              if (str[i] == str[j]) {
  10                  count++;
  11              }
  12          }
  13      }
  14      return count;
  15  }
```

Round 4, where the rule appeared:

```c
  16  int main() {
  17      const char *exampleString = "abcab";
  18      size_t exampleStringLength = strlen(exampleString);
  19      int result = countSubstringsWithSameStartAndEnd(exampleString, exampleStringLength);
  20      printf("Number of substrings: %d\n", result);
  21      return 0;
  22  }
```

## E069  `flawfinder:strlen`  round 0 -> 1

Program `problem-437` (feedback-static+dynamic-r0-t1.0-p1.0); left `remove_odd_chars`, entered `main`.

Round 0, where the rule was removed:

```c
   5  char *remove_odd_chars(const char *str) {
   6      size_t len;
   7      size_t new_len;
   8      char *result;
   9      size_t i;
  10      size_t j;
  11  
  12      if (str == NULL) {
  13          return NULL;
  14      }
  15  
  16      len = strlen(str);
  17      new_len = (len + 1) / 2;
  18  
  19      result = (char *)malloc(new_len + 1);
  20      if (result == NULL) {
  21          return NULL;
  22      }
  23  
  24      j = 0;
  25      for (i = 0; i < len; i += 2) {
  26          result[j++] = str[i];
  27      }
  28  
  29      result[j] = '\0';
  30  
  31      return result;
  32  }
```

Round 1, where the rule appeared:

```c
  32  int main(void) {
  33      const char *original = "abcdefghijklmnopqrstuvwxyz";
  34      size_t len = strlen(original);
  35      char *filtered = remove_odd_chars(original, len);
  36  
  37      if (filtered != NULL) {
  38          printf("Original: %s\n", original);
  39          printf("Filtered: %s\n", filtered);
  40          free(filtered);
  41      } else {
  42          fprintf(stderr, "Memory allocation failed\n");
  43          return 1;
  44      }
  45  
  46      filtered = remove_odd_chars(NULL, 0);
  47      if (filtered == NULL) {
  48          printf("Successfully handled NULL input\n");
  49      }
  50  
  51      return 0;
  52  }
```

## E070  `flawfinder:memcpy`  round 0 -> 1

Program `problem-779-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `safe_copy_list` (new function).

Round 0, where the rule was removed:

```c
  69  int main(void) {
  70      int data1[] = {1, 2, 3};
  71      int data2[] = {4, 5};
  72      int data3[] = {1, 2, 3};
  73      int data4[] = {6, 7, 8, 9};
  74      int data5[] = {4, 5};
  75      int data6[] = {1, 2, 3, 4};
  76  
  77      ListCollection collection = {0};
  78      collection.count = 6;
  79      collection.lists = malloc(collection.count * sizeof(List));
  80      if (collection.lists == NULL) {
  81          fprintf(stderr, "Memory allocation failed\n");
  82          return EXIT_FAILURE;
  83      }
  84  
  85      collection.lists[0].data = malloc(sizeof(data1));
  86      collection.lists[1].data = malloc(sizeof(data2));
  87      collection.lists[2].data = malloc(sizeof(data3));
  88      collection.lists[3].data = malloc(sizeof(data4));
  89      collection.lists[4].data = malloc(sizeof(data5));
  90      collection.lists[5].data = malloc(sizeof(data6));
  91  
  92      if (collection.lists[0].data == NULL || collection.lists[1].data == NULL ||
  93          collection.lists[2].data == NULL || collection.lists[3].data == NULL ||
  94          collection.lists[4].data == NULL || collection.lists[5].data == NULL) {
  95          fprintf(stderr, "Memory allocation failed\n");
  96          free_collection(&collection);
  97          return EXIT_FAILURE;
  98      }
  99  
 100      memcpy(collection.lists[0].data, data1, sizeof(data1));
 101      collection.lists[0].size = sizeof(data1) / sizeof(data1[0]);
 102  
 103      memcpy(collection.lists[1].data, data2, sizeof(data2));
 104      collection.lists[1].size = sizeof(data2) / sizeof(data2[0]);
 105  
 106      memcpy(collection.lists[2].data, data3, sizeof(data3));
 107      collection.lists[2].size = sizeof(data3) / sizeof(data3[0]);
 108  
      ... (15 more lines)
```

Round 1, where the rule appeared:

```c
  69  int safe_copy_list(List *dest, const int *src, size_t count) {
  70      if (dest == NULL || src == NULL || count == 0) {
  71          return 0;
  72      }
  73  
  74      if (dest->data == NULL || dest->size < count) {
  75          return 0;
  76      }
  77  
  78      memcpy(dest->data, src, count * sizeof(int));
  79      dest->size = count;
  80      return 1;
  81  }
```

## E071  `flawfinder:strlen`  round 0 -> 1

Program `problem-326-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `copy_string`, entered `find_most_frequent_word`.

Round 0, where the rule was removed:

```c
  11  static char *copy_string(const char *source)
  12  {
  13      size_t length;
  14      char *copy;
  15  
  16      if (source == NULL) {
  17          return NULL;
  18      }
  19  
  20      length = strlen(source);
  21  
  22      if (length == SIZE_MAX) {
  23          return NULL;
  24      }
  25  
  26      copy = malloc(length + 1);
  27      if (copy == NULL) {
  28          return NULL;
  29      }
  30  
  31      memcpy(copy, source, length + 1);
  32      return copy;
  33  }
```

Round 1, where the rule appeared:

```c
  32  static int find_most_frequent_word(const char *const strings[],
  33                                     const size_t lengths[],
  34                                     size_t string_count,
  35                                     char **result,
  36                                     size_t *occurrences)
  37  {
  38      WordCount *counts = NULL;
  39      size_t unique_count = 0;
  40      size_t capacity = 0;
  41      size_t best_index = 0;
  42      size_t i;
  43  
  44      if (strings == NULL || lengths == NULL || string_count == 0 ||
  45          result == NULL || occurrences == NULL) {
  46          return -1;
  47      }
  48  
  49      *result = NULL;
  50      *occurrences = 0;
  51  
  52      for (i = 0; i < string_count; ++i) {
  53          size_t j;
  54          size_t found_index = unique_count;
  55  
  56          if (strings[i] == NULL) {
  57              goto failure;
  58          }
  59  
  60          for (j = 0; j < unique_count; ++j) {
  61              size_t stored_length = strlen(counts[j].word);
  62  
  63              if (stored_length == lengths[i] &&
  64                  memcmp(counts[j].word, strings[i], lengths[i]) == 0) {
  65                  found_index = j;
  66                  break;
  67              }
  68          }
  69  
  70          if (found_index < unique_count) {
  71              if (counts[found_index].count == SIZE_MAX) {
      ... (85 more lines)
```

## E072  `flawfinder:strlen`  round 0 -> 1

Program `problem-30` (feedback-static-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9  
  10      size_t len = strlen(str);
  11      int count = 0;
  12  
  13      for (size_t i = 0; i < len; i++) {
  14          for (size_t j = i; j < len; j++) {
  15              if (str[i] == str[j]) {
  16                  count++;
  17              }
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main(int argc, char * const argv[]) {
  24      if (argc != 2) {
  25          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      size_t len = strlen(argv[1]);
  30      int result = count_substrings(argv[1], len);
  31      printf("%d\n", result);
  32  
  33      return EXIT_SUCCESS;
  34  }
```

## E073  `flawfinder:memcpy`  round 1 -> 2

Program `problem-421` (feedback-static-r0-t1.0-p1.0); left `tuple_join`, entered `safe_memcpy` (new function).

Round 1, where the rule was removed:

```c
  33  char *tuple_join(const Tuple *tuple, const char *delimiter)
  34  {
  35      size_t total_length;
  36      size_t delimiter_length;
  37      size_t i;
  38      char *result;
  39      char *current;
  40      const size_t MAX_STRING_LEN = 1024 * 1024;
  41      const size_t MAX_TOTAL_LEN = 10 * 1024 * 1024;
  42  
  43      if (tuple == NULL || delimiter == NULL) {
  44          return NULL;
  45      }
  46  
  47      if (!is_valid_string(delimiter, MAX_STRING_LEN)) {
  48          return NULL;
  49      }
  50  
  51      if (tuple->count == 0 || tuple->elements == NULL) {
  52          result = malloc(1);
  53          if (result != NULL) {
  54              result[0] = '\0';
  55          }
  56          return result;
  57      }
  58  
  59      if (tuple->count > SIZE_MAX / 2) {
  60          return NULL;
  61      }
  62  
  63      delimiter_length = safe_strlen(delimiter, MAX_STRING_LEN);
  64      total_length = 1;
  65  
  66      for (i = 0; i < tuple->count; i++) {
  67          size_t element_length;
  68          
  69          if (tuple->elements[i] == NULL) {
  70              return NULL;
  71          }
  72          
      ... (60 more lines)
```

Round 2, where the rule appeared:

```c
  33  static bool safe_memcpy(char *dest, const char *src, size_t len, size_t dest_size) {
  34      if (len > dest_size) {
  35          return false;
  36      }
  37      memcpy(dest, src, len);
  38      return true;
  39  }
```

## E074  `flawfinder:getchar`  round 2 -> 3

Program `problem-604-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `discard_remaining_input` (new function).

Round 2, where the rule was removed:

```c
  57  int main(void)
  58  {
  59      char *input;
  60      char *newline;
  61  
  62      input = calloc(MAX_INPUT_SIZE, sizeof(char));
  63      if (input == NULL) {
  64          fprintf(stderr, "Memory allocation failed\n");
  65          return EXIT_FAILURE;
  66      }
  67  
  68      printf("Enter a string: ");
  69      if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
  70          fprintf(stderr, "Error reading input\n");
  71          free(input);
  72          return EXIT_FAILURE;
  73      }
  74  
  75      input[MAX_INPUT_SIZE - 1] = '\0';
  76  
  77      newline = strchr(input, '\n');
  78      if (newline != NULL) {
  79          *newline = '\0';
  80      } else {
  81          int ch;
  82          while ((ch = getchar()) != '\n' && ch != EOF) {
  83          }
  84      }
  85  
  86      if (reverse_words(input, MAX_INPUT_SIZE) != 0) {
  87          fprintf(stderr, "Error reversing words\n");
  88          free(input);
  89          return EXIT_FAILURE;
  90      }
  91  
  92      printf("Reversed: %s\n", input);
  93  
  94      free(input);
  95      return EXIT_SUCCESS;
  96  }
```

Round 3, where the rule appeared:

```c
  57  static void discard_remaining_input(void)
  58  {
  59      int ch;
  60  
  61      do {
  62          ch = getchar();
  63      } while (ch != '\n' && ch != EOF);
  64  }
```

## E075  `flawfinder:strlen`  round 1 -> 2

Program `problem-771-s3` (feedback-static-r0-t1.0-p1.0); left `isBalanced`, entered `main`.

Round 1, where the rule was removed:

```c
  56  int isBalanced(const char *expression) {
  57      Stack stack;
  58      size_t len;
  59      size_t i;
  60  
  61      if (expression == NULL) {
  62          return 0;
  63      }
  64  
  65      len = strlen(expression);
  66      if (len >= MAX_SIZE) {
  67          return 0;
  68      }
  69  
  70      initStack(&stack);
  71  
  72      for (i = 0; i < len; i++) {
  73          char c = expression[i];
  74  
  75          if (c == '(' || c == '{' || c == '[') {
  76              if (!push(&stack, c)) {
  77                  return 0;
  78              }
  79          } else if (c == ')' || c == '}' || c == ']') {
  80              char topChar;
  81              if (!pop(&stack, &topChar)) {
  82                  return 0;
  83              }
  84              if (!isMatchingPair(topChar, c)) {
  85                  return 0;
  86              }
  87          }
  88      }
  89  
  90      return isEmpty(&stack);
  91  }
```

Round 2, where the rule appeared:

```c
 102  int main(void) {
 103      char input[MAX_SIZE];
 104      int result;
 105      size_t inputLen;
 106  
 107      printf("Enter an expression: ");
 108      if (fgets(input, sizeof(input), stdin) == NULL) {
 109          return 1;
 110      }
 111  
 112      inputLen = strlen(input);
 113      if (inputLen > 0 && input[inputLen - 1] == '\n') {
 114          input[inputLen - 1] = '\0';
 115      } else {
 116          int ch;
 117          while ((ch = getchar()) != '\n' && ch != EOF);
 118      }
 119  
 120      result = isBalanced(input);
 121  
 122      if (result) {
 123          printf("Balanced\n");
 124      } else {
 125          printf("Not Balanced\n");
 126      }
 127  
 128      return 0;
 129  }
```

## E076  `flawfinder:strlen`  round 0 -> 1

Program `problem-619` (feedback-static+dynamic-r0-t1.0-p1.0); left `move_numbers_to_string`, entered `main`.

Round 0, where the rule was removed:

```c
   6  char* move_numbers_to_string(const char* input) {
   7      if (input == NULL) {
   8          return NULL;
   9      }
  10      
  11      size_t len = strlen(input);
  12      char* result = (char*)malloc((len + 1) * sizeof(char));
  13      if (result == NULL) {
  14          return NULL;
  15      }
  16      
  17      size_t num_count = 0;
  18      size_t j = 0;
  19      
  20      for (size_t i = 0; i < len; i++) {
  21          if (isdigit((unsigned char)input[i])) {
  22              num_count++;
  23          } else {
  24              result[j++] = input[i];
  25          }
  26      }
  27      
  28      for (size_t i = 0; i < len && num_count > 0; i++) {
  29          if (isdigit((unsigned char)input[i])) {
  30              result[j++] = input[i];
  31              num_count--;
  32          }
  33      }
  34      
  35      result[j] = '\0';
  36      return result;
  37  }
```

Round 1, where the rule appeared:

```c
  43  int main(void) {
  44      const char* test_str = "a1b2c3d4";
  45      size_t max_len = strlen(test_str) + 1;
  46      char* result = move_numbers_to_string(test_str, max_len);
  47      
  48      if (result != NULL) {
  49          printf("Original: %s\n", test_str);
  50          printf("Modified: %s\n", result);
  51          free(result);
  52          result = NULL;
  53      } else {
  54          fprintf(stderr, "Memory allocation failed\n");
  55          return EXIT_FAILURE;
  56      }
  57      
  58      return EXIT_SUCCESS;
  59  }
```

## E077  `flawfinder:strlen`  round 0 -> 1

Program `problem-338-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9  
  10      int count = 0;
  11      size_t len = strlen(str);
  12  
  13      for (size_t i = 0; i < len; i++) {
  14          for (size_t j = i; j < len; j++) {
  15              if (str[i] == str[j]) {
  16                  count++;
  17              }
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main(int argc, char * const argv[]) {
  24      if (argc != 2) {
  25          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      size_t len = strlen(argv[1]);
  30      int result = count_substrings(argv[1], len);
  31      printf("%d\n", result);
  32  
  33      return EXIT_SUCCESS;
  34  }
```

## E078  `flawfinder:strlen`  round 0 -> 1

Program `problem-621` (feedback-static+dynamic-r0-t1.0-p1.0); left `increment_string_num`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int increment_string_num(char *str, int k) {
   6      int len = strlen(str);
   7      for (int i = 0; i < len; i++) {
   8          if (str[i] >= '0' && str[i] <= '9') {
   9              str[i] = (str[i] - '0' + k) % 10 + '0';
  10          }
  11      }
  12      return 0;
  13  }
```

Round 1, where the rule appeared:

```c
  14  int main() {
  15      char str1[1024];
  16      int k;
  17  
  18      printf("Enter a string: ");
  19      if (fgets(str1, sizeof(str1), stdin) == NULL) {
  20          return EXIT_FAILURE;
  21      }
  22  
  23      str1[strcspn(str1, "\n")] = 0;
  24  
  25      printf("Enter an integer k: ");
  26      if (scanf("%d", &k) != 1) {
  27          return EXIT_FAILURE;
  28      }
  29  
  30      size_t len = strlen(str1);
  31      increment_string_num(str1, len, k);
  32  
  33      printf("Updated string: %s\n", str1);
  34      return EXIT_SUCCESS;
  35  }
```

## E079  `flawfinder:getchar`  round 1 -> 2

Program `problem-822-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `flush_stdin` (new function).

Round 1, where the rule was removed:

```c
  64  int main(void)
  65  {
  66      char buffer[BUFFER_SIZE];
  67      size_t length;
  68  
  69      printf("Enter password: ");
  70      fflush(stdout);
  71  
  72      if (fgets(buffer, (int)sizeof(buffer), stdin) == NULL) {
  73          fprintf(stderr, "Error reading input\n");
  74          return 1;
  75      }
  76  
  77      buffer[sizeof(buffer) - 1] = '\0';
  78  
  79      length = strnlen(buffer, sizeof(buffer));
  80  
  81      if (length > 0 && buffer[length - 1] == '\n') {
  82          buffer[length - 1] = '\0';
  83      } else {
  84          int ch;
  85          while ((ch = getchar()) != '\n' && ch != EOF) {
  86          }
  87      }
  88  
  89      if (is_valid_password(buffer, sizeof(buffer))) {
  90          printf("Password is valid\n");
  91      } else {
  92          printf("Password is invalid\n");
  93      }
  94  
  95      secure_clear(buffer, sizeof(buffer));
  96  
  97      return 0;
  98  }
```

Round 2, where the rule appeared:

```c
  23  static void flush_stdin(void)
  24  {
  25      int ch;
  26  
  27      do {
  28          ch = getchar();
  29      } while (ch != '\n' && ch != EOF);
  30  }
```

## E080  `flawfinder:memcpy`  round 3 -> 4

Program `problem-621-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `increment_numeric_values`, entered `extract_number` (new function).

Round 3, where the rule was removed:

```c
   8  char* increment_numeric_values(const char* input, int k) {
   9      if (input == NULL) {
  10          return NULL;
  11      }
  12      
  13      size_t input_len = strlen(input);
  14      if (input_len >= SIZE_MAX / 20) {
  15          return NULL;
  16      }
  17      size_t max_result_size = input_len * 20 + 1;
  18      char* result = (char*)malloc(max_result_size);
  19      if (result == NULL) {
  20          return NULL;
  21      }
  22      
  23      size_t result_pos = 0;
  24      size_t i = 0;
  25      
  26      while (i < input_len) {
  27          if (isdigit((unsigned char)input[i])) {
  28              size_t start = i;
  29              while (i < input_len && isdigit((unsigned char)input[i])) {
  30                  i++;
  31              }
  32              
  33              size_t num_len = i - start;
  34              if (num_len >= SIZE_MAX) {
  35                  free(result);
  36                  return NULL;
  37              }
  38              char* num_str = (char*)malloc(num_len + 1);
  39              if (num_str == NULL) {
  40                  free(result);
  41                  return NULL;
  42              }
  43              
  44              if (num_len > 0) {
  45                  memcpy(num_str, input + start, num_len);
  46              }
  47              num_str[num_len] = '\0';
      ... (57 more lines)
```

Round 4, where the rule appeared:

```c
  22  static char* extract_number(const char *input, size_t start, size_t len) {
  23      if (len == 0 || len >= SIZE_MAX) {
  24          return NULL;
  25      }
  26      char *num_str = (char*)malloc(len + 1);
  27      if (num_str == NULL) {
  28          return NULL;
  29      }
  30      memcpy(num_str, input + start, len);
  31      num_str[len] = '\0';
  32      return num_str;
  33  }
```

## E081  `flawfinder:strlen`  round 1 -> 2

Program `problem-923-s2` (feedback-static-r0-t1.0-p1.0); left `shortestCommonSupersequenceLength`, entered `main`.

Round 1, where the rule was removed:

```c
   5  int shortestCommonSupersequenceLength(const char *str1, const char *str2) {
   6      size_t len1 = strlen(str1);
   7      size_t len2 = strlen(str2);
   8      int dp[len1 + 1][len2 + 1];
   9  
  10      for (size_t i = 0; i <= len1; i++) {
  11          for (size_t j = 0; j <= len2; j++) {
  12              if (i == 0 || j == 0) {
  13                  dp[i][j] = i + j;
  14              } else if (str1[i - 1] == str2[j - 1]) {
  15                  dp[i][j] = dp[i - 1][j - 1] + 1;
  16              } else {
  17                  dp[i][j] = 1 + (dp[i - 1][j] < dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1]);
  18              }
  19          }
  20      }
  21      return dp[len1][len2];
  22  }
```

Round 2, where the rule appeared:

```c
  22  int main() {
  23      const char* str1 = "ABCD";
  24      const char* str2 = "ACDF";
  25      size_t len1 = strlen(str1);
  26      size_t len2 = strlen(str2);
  27      
  28      if (str1[len1 - 1] != '\0' || str2[len2 - 1] != '\0') {
  29          fprintf(stderr, "Input strings are not null-terminated\n");
  30          return 1;
  31      }
  32  
  33      int result = shortestCommonSupersequenceLength(str1, str2, len1, len2);
  34      printf("Length of the shortest common supersequence: %d\n", result);
  35      return 0;
  36  }
```

## E082  `flawfinder:strlen`  round 0 -> 1

Program `problem-864-s2` (feedback-static-r0-t1.0-p1.0); left `is_palindrome`, entered `find_palindromes`.

Round 0, where the rule was removed:

```c
   6  bool is_palindrome(const char *str) {
   7      int len = strlen(str);
   8      for (int i = 0; i < len / 2; ++i) {
   9          if (str[i] != str[len - 1 - i]) {
  10              return false;
  11          }
  12      }
  13      return true;
  14  }
```

Round 1, where the rule appeared:

```c
  15  void find_palindromes(char **strings, int count, char **result, int *result_count) {
  16      *result_count = 0;
  17      for (int i = 0; i < count; ++i) {
  18          size_t len = strlen(strings[i]);
  19          if (is_palindrome(strings[i], len)) {
  20              result[*result_count] = strings[i];
  21              (*result_count)++;
  22          }
  23      }
  24  }
```

## E083  `flawfinder:strlen`  round 0 -> 1

Program `problem-226-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `remove_odd_index_chars`, entered `main`.

Round 0, where the rule was removed:

```c
   5  char *remove_odd_index_chars(const char *input)
   6  {
   7      if (input == NULL) {
   8          return NULL;
   9      }
  10  
  11      size_t len = strlen(input);
  12      size_t output_len = (len + 1) / 2;
  13  
  14      char *result = (char *)malloc(output_len + 1);
  15      if (result == NULL) {
  16          return NULL;
  17      }
  18  
  19      size_t j = 0;
  20      for (size_t i = 0; i < len; i += 2) {
  21          result[j++] = input[i];
  22      }
  23      result[j] = '\0';
  24  
  25      return result;
  26  }
```

Round 1, where the rule appeared:

```c
  28  int main(void)
  29  {
  30      const char *test_string = "Hello, World!";
  31      char *result = remove_odd_index_chars(test_string, strlen(test_string) + 1);
  32  
  33      if (result == NULL) {
  34          fprintf(stderr, "Error: Failed to process string\n");
  35          return EXIT_FAILURE;
  36      }
  37  
  38      printf("Original: %s\n", test_string);
  39      printf("Result:   %s\n", result);
  40  
  41      free(result);
  42      result = NULL;
  43  
  44      return EXIT_SUCCESS;
  45  }
```

## E084  `flawfinder:strlen`  round 1 -> 2

Program `problem-967-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `contains_all_vowels`, entered `main`.

Round 1, where the rule was removed:

```c
   6  bool contains_all_vowels(const char *str) {
   7      bool vowels[5] = {false, false, false, false, false};
   8      int len = strlen(str);
   9  
  10      for (int i = 0; i < len; i++) {
  11          switch (tolower((unsigned char)str[i])) {
  12              case 'a': vowels[0] = true; break;
  13              case 'e': vowels[1] = true; break;
  14              case 'i': vowels[2] = true; break;
  15              case 'o': vowels[3] = true; break;
  16              case 'u': vowels[4] = true; break;
  17              default: break;
  18          }
  19      }
  20  
  21      for (int i = 0; i < 5; i++) {
  22          if (!vowels[i]) {
  23              return false;
  24          }
  25      }
  26  
  27      return true;
  28  }
```

Round 2, where the rule appeared:

```c
  30  int main() {
  31      const char *test_strings[] = {"hello", "education", "sequence", "aeiou", "CQWEN"};
  32      int num_tests = sizeof(test_strings) / sizeof(test_strings[0]);
  33  
  34      for (int i = 0; i < num_tests; i++) {
  35          size_t len = strlen(test_strings[i]);
  36          if (len == SIZE_MAX) {
  37              printf("Error: String length overflow.\n");
  38              continue;
  39          }
  40          if (contains_all_vowels(test_strings[i], len)) {
  41              printf("The string \"%s\" contains all vowels.\n", test_strings[i]);
  42          } else {
  43              printf("The string \"%s\" does not contain all vowels.\n", test_strings[i]);
  44          }
  45      }
  46  
  47      return 0;
  48  }
```

## E085  `flawfinder:strlen`  round 1 -> 2

Program `problem-61-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 1, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      size_t len = strlen(str);
   7      int count = 0;
   8      int *prefix_sum = (int *)malloc((len + 1) * sizeof(int));
   9      
  10      if (prefix_sum == NULL) {
  11          return 0;
  12      }
  13      
  14      prefix_sum[0] = 0;
  15      for (size_t i = 0; i < len; i++) {
  16          if (str[i] >= '0' && str[i] <= '9') {
  17              prefix_sum[i + 1] = prefix_sum[i] + (str[i] - '0');
  18          } else {
  19              free(prefix_sum);
  20              return 0;
  21          }
  22      }
  23      
  24      for (size_t i = 0; i < len; i++) {
  25          for (size_t j = i + 1; j <= len; j++) {
  26              size_t sub_len = j - i;
  27              int sub_sum = prefix_sum[j] - prefix_sum[i];
  28              if (sub_sum == (int)sub_len) {
  29                  count++;
  30              }
  31          }
  32      }
  33      
  34      free(prefix_sum);
  35      return count;
  36  }
```

Round 2, where the rule appeared:

```c
  37  int main(int argc, char * const argv[]) {
  38      if (argc != 2) {
  39          fprintf(stderr, "Usage: %s <numeric_string>\n", argv[0]);
  40          return 1;
  41      }
  42      
  43      size_t len = strlen(argv[1]);
  44      int result = count_substrings(argv[1], len);
  45      printf("%d\n", result);
  46      
  47      return 0;
  48  }
```

## E086  `flawfinder:fgetc`  round 1 -> 2

Program `problem-315-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 1, where the rule was removed:

```c
  58  int main(void)
  59  {
  60      char *input = NULL;
  61      size_t length = 0;
  62      size_t capacity = 0;
  63      int character;
  64  
  65      while ((character = fgetc(stdin)) != '\n' && character != EOF) {
  66          if (length == SIZE_MAX - 1) {
  67              free(input);
  68              fputs("Input is too long.\n", stderr);
  69              return EXIT_FAILURE;
  70          }
  71  
  72          if (length + 1 >= capacity) {
  73              size_t new_capacity;
  74  
  75              if (capacity == 0) {
  76                  new_capacity = 128;
  77              } else {
  78                  if (capacity > SIZE_MAX / 2) {
  79                      free(input);
  80                      fputs("Input is too long.\n", stderr);
  81                      return EXIT_FAILURE;
  82                  }
  83                  new_capacity = capacity * 2;
  84              }
  85  
  86              char *resized = realloc(input, new_capacity);
  87              if (resized == NULL) {
  88                  free(input);
  89                  fputs("Memory allocation failed.\n", stderr);
  90                  return EXIT_FAILURE;
  91              }
  92  
  93              input = resized;
  94              capacity = new_capacity;
  95          }
  96  
  97          input[length++] = (char)character;
      ... (38 more lines)
```

Round 2, where the rule appeared:

```c
  63  static int read_line(FILE *stream, char **line)
  64  {
  65      char *buffer = NULL;
  66      size_t length = 0;
  67      size_t capacity = 0;
  68      int character;
  69  
  70      if (stream == NULL || line == NULL) {
  71          return -1;
  72      }
  73  
  74      while ((character = fgetc(stream)) != EOF) {
  75          if (character == '\n') {
  76              break;
  77          }
  78  
  79          if (length == SIZE_MAX - 1) {
  80              free(buffer);
  81              return -1;
  82          }
  83  
  84          if (length + 1 >= capacity) {
  85              size_t new_capacity;
  86  
  87              if (capacity == 0) {
  88                  new_capacity = 128;
  89              } else {
  90                  if (capacity > SIZE_MAX / 2) {
  91                      free(buffer);
  92                      return -1;
  93                  }
  94  
  95                  new_capacity = capacity * 2;
  96              }
  97  
  98              if (new_capacity <= length + 1) {
  99                  free(buffer);
 100                  return -1;
 101              }
 102  
      ... (30 more lines)
```

## E087  `flawfinder:char`  round 2 -> 3

Program `problem-274-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_value` (new function).

Round 2, where the rule was removed:

```c
  17  int main(void)
  18  {
  19      char input[64];
  20      char *end;
  21      unsigned long value;
  22      uint64_t sum;
  23  
  24      if (fgets(input, sizeof input, stdin) == NULL) {
  25          fputs("Invalid input\n", stderr);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      if (input[0] != '\0' && input[sizeof input - 2U] != '\n') {
  30          int ch = fgetc(stdin);
  31  
  32          if (ch != '\n' && ch != EOF) {
  33              while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
  34              }
  35              fputs("Invalid input\n", stderr);
  36              return EXIT_FAILURE;
  37          }
  38      }
  39  
  40      errno = 0;
  41      end = input;
  42      value = strtoul(input, &end, 10);
  43  
  44      if (errno == ERANGE || end == input || value > 64UL) {
  45          fputs("Invalid input\n", stderr);
  46          return EXIT_FAILURE;
  47      }
  48  
  49      while (*end == ' ' || *end == '\t' || *end == '\n' ||
  50             *end == '\r' || *end == '\f' || *end == '\v') {
  51          ++end;
  52      }
  53  
  54      if (*end != '\0') {
  55          fputs("Invalid input\n", stderr);
  56          return EXIT_FAILURE;
      ... (13 more lines)
```

Round 3, where the rule appeared:

```c
  17  static int read_value(uint32_t *value)
  18  {
  19      char input[64];
  20      char *end;
  21      unsigned long parsed;
  22  
  23      if (value == NULL || fgets(input, sizeof input, stdin) == NULL) {
  24          return -1;
  25      }
  26  
  27      char *newline = NULL;
  28      for (size_t i = 0; i < sizeof input; ++i) {
  29          if (input[i] == '\n') {
  30              newline = &input[i];
  31              break;
  32          }
  33          if (input[i] == '\0') {
  34              break;
  35          }
  36      }
  37  
  38      if (newline == NULL) {
  39          int ch = fgetc(stdin);
  40  
  41          if (ch != '\n' && ch != EOF) {
  42              do {
  43                  ch = fgetc(stdin);
  44              } while (ch != '\n' && ch != EOF);
  45              return -1;
  46          }
  47      }
  48  
  49      errno = 0;
  50      end = input;
  51      parsed = strtoul(input, &end, 10);
  52  
  53      if (errno == ERANGE || end == input || parsed > 64UL) {
  54          return -1;
  55      }
  56  
      ... (12 more lines)
```

## E088  `flawfinder:getchar`  round 1 -> 2

Program `problem-566-s2` (feedback-static-r0-t1.0-p1.0); left `read_line`, entered `discard_rest_of_line` (new function).

Round 1, where the rule was removed:

```c
  58  static int read_line(char *buffer, size_t size)
  59  {
  60      size_t len = 0;
  61      int ch = 0;
  62  
  63      if (buffer == NULL || size == 0) {
  64          return -1;
  65      }
  66  
  67      if (fgets(buffer, (int)size, stdin) == NULL) {
  68          return -1;
  69      }
  70  
  71      len = strcspn(buffer, "\n");
  72  
  73      if (buffer[len] == '\0' && len == size - 1) {
  74          while ((ch = getchar()) != '\n' && ch != EOF) {
  75          }
  76          return -1;
  77      }
  78  
  79      buffer[len] = '\0';
  80      return 0;
  81  }
```

Round 2, where the rule appeared:

```c
  60  static void discard_rest_of_line(void)
  61  {
  62      int ch;
  63  
  64      do {
  65          ch = getchar();
  66      } while (ch != '\n' && ch != EOF);
  67  }
```

## E089  `flawfinder:memcpy`  round 0 -> 1

Program `problem-570-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `duplicate_string`, entered `remove_pattern` (new function).

Round 0, where the rule was removed:

```c
   5  static char *duplicate_string(const char *src)
   6  {
   7      size_t len;
   8      char *copy;
   9  
  10      if (src == NULL) {
  11          return NULL;
  12      }
  13  
  14      len = strlen(src) + 1U;
  15      copy = malloc(len);
  16      if (copy == NULL) {
  17          return NULL;
  18      }
  19  
  20      memcpy(copy, src, len);
  21      return copy;
  22  }
```

Round 1, where the rule appeared:

```c
   5  static char *remove_pattern(const char *src, const char *pattern)
   6  {
   7      size_t src_len;
   8      size_t pat_len;
   9      size_t out_len;
  10      const char *cursor;
  11      const char *hit;
  12      char *copy;
  13      char *dest;
  14  
  15      if (src == NULL || pattern == NULL) {
  16          return NULL;
  17      }
  18  
  19      src_len = strlen(src);
  20      pat_len = strlen(pattern);
  21  
  22      if (pat_len == 0U) {
  23          copy = malloc(src_len + 1U);
  24          if (copy == NULL) {
  25              return NULL;
  26          }
  27          memcpy(copy, src, src_len + 1U);
  28          return copy;
  29      }
  30  
  31      out_len = 0U;
  32      cursor = src;
  33      while ((hit = strstr(cursor, pattern)) != NULL) {
  34          out_len += (size_t)(hit - cursor);
  35          cursor = hit + pat_len;
  36      }
  37      out_len += strlen(cursor);
  38  
  39      copy = malloc(out_len + 1U);
  40      if (copy == NULL) {
  41          return NULL;
  42      }
  43  
  44      dest = copy;
      ... (16 more lines)
```

## E090  `flawfinder:fgetc`  round 1 -> 2

Program `problem-566` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 1, where the rule was removed:

```c
  23  int main(void)
  24  {
  25      char input[128];
  26      char *end;
  27      uintmax_t value;
  28      uintmax_t sum;
  29  
  30      if (fgets(input, sizeof input, stdin) == NULL) {
  31          return EXIT_FAILURE;
  32      }
  33  
  34      if (input[0] != '\0') {
  35          size_t length = 0;
  36  
  37          while (length < sizeof input && input[length] != '\0' &&
  38                 input[length] != '\n') {
  39              ++length;
  40          }
  41  
  42          if (length == sizeof input) {
  43              return EXIT_FAILURE;
  44          }
  45  
  46          if (input[length] != '\n' && !feof(stdin)) {
  47              int ch = fgetc(stdin);
  48  
  49              if (ch != '\n' && ch != EOF) {
  50                  while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
  51                  }
  52                  return EXIT_FAILURE;
  53              }
  54          }
  55      }
  56  
  57      errno = 0;
  58      end = NULL;
  59      value = strtoumax(input, &end, 10);
  60  
  61      if (errno == ERANGE || end == input) {
  62          return EXIT_FAILURE;
      ... (34 more lines)
```

Round 2, where the rule appeared:

```c
  24  static int read_line(char **line)
  25  {
  26      size_t length = 0;
  27      size_t capacity = 64;
  28      char *buffer;
  29      int ch;
  30  
  31      if (line == NULL) {
  32          return -1;
  33      }
  34  
  35      buffer = malloc(capacity);
  36      if (buffer == NULL) {
  37          return -1;
  38      }
  39  
  40      while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
  41          if (length + 1 >= capacity) {
  42              size_t new_capacity;
  43              char *new_buffer;
  44  
  45              if (capacity > SIZE_MAX / 2) {
  46                  free(buffer);
  47                  return -1;
  48              }
  49  
  50              new_capacity = capacity * 2;
  51              new_buffer = realloc(buffer, new_capacity);
  52              if (new_buffer == NULL) {
  53                  free(buffer);
  54                  return -1;
  55              }
  56  
  57              buffer = new_buffer;
  58              capacity = new_capacity;
  59          }
  60  
  61          buffer[length++] = (char)ch;
  62      }
  63  
      ... (14 more lines)
```

## E091  `flawfinder:strlen`  round 0 -> 1

Program `problem-676-s3` (feedback-static-r0-t1.0-p1.0); left `main`, entered `filter_alphanumeric`.

Round 0, where the rule was removed:

```c
  28  int main(void) {
  29      const char *input = "Hello, World! 123 @#$";
  30      char *output;
  31      
  32      if (input == NULL) {
  33          return 1;
  34      }
  35      
  36      output = malloc(strlen(input) + 1);
  37      if (output == NULL) {
  38          return 1;
  39      }
  40      
  41      filter_alphanumeric(input, output);
  42      printf("%s\n", output);
  43      
  44      free(output);
  45      return 0;
  46  }
```

Round 1, where the rule appeared:

```c
   6  void filter_alphanumeric(const char *input, char *output, size_t out_size) {
   7      regex_t regex;
   8      regmatch_t pmatch[1];
   9      const char *pattern = "[^a-zA-Z0-9]+";
  10      const char *cursor = input;
  11      char *out_cursor = output;
  12      size_t remaining = out_size;
  13  
  14      if (input == NULL || output == NULL || out_size == 0) {
  15          if (output != NULL && out_size > 0) {
  16              output[0] = '\0';
  17          }
  18          return;
  19      }
  20  
  21      output[0] = '\0';
  22  
  23      if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
  24          return;
  25      }
  26  
  27      while (regexec(&regex, cursor, 1, pmatch, 0) == 0) {
  28          size_t len = pmatch[0].rm_so;
  29          
  30          if (len >= remaining) {
  31              len = remaining - 1;
  32          }
  33          
  34          if (len > 0) {
  35              memcpy(out_cursor, cursor, len);
  36              out_cursor += len;
  37              remaining -= len;
  38          }
  39          
  40          cursor += pmatch[0].rm_eo;
  41      }
  42  
  43      size_t end_len = strlen(cursor);
  44      if (end_len >= remaining) {
  45          end_len = remaining - 1;
      ... (11 more lines)
```

## E092  `flawfinder:strlen`  round 0 -> 1

Program `problem-607-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `escape_regex_literal`, entered `find_literal` (new function).

Round 0, where the rule was removed:

```c
   7  static int escape_regex_literal(const char *literal, char **escaped)
   8  {
   9      static const char metacharacters[] = ".^$*+?()[]{}|\\";
  10      size_t length;
  11      size_t capacity;
  12      size_t position = 0;
  13      char *result;
  14  
  15      if (literal == NULL || escaped == NULL) {
  16          return -1;
  17      }
  18  
  19      length = strlen(literal);
  20      if (length > (SIZE_MAX - 1U) / 2U) {
  21          return -1;
  22      }
  23  
  24      capacity = (length * 2U) + 1U;
  25      result = malloc(capacity);
  26      if (result == NULL) {
  27          return -1;
  28      }
  29  
  30      for (size_t i = 0; i < length; ++i) {
  31          if (strchr(metacharacters, literal[i]) != NULL) {
  32              result[position++] = '\\';
  33          }
  34          result[position++] = literal[i];
  35      }
  36  
  37      result[position] = '\0';
  38      *escaped = result;
  39      return 0;
  40  }
```

Round 1, where the rule appeared:

```c
   6  static int find_literal(const char *text, const char *literal,
   7                          size_t *start, size_t *end)
   8  {
   9      const char *match;
  10      size_t offset;
  11      size_t literal_length;
  12  
  13      if (text == NULL || literal == NULL || start == NULL || end == NULL) {
  14          return -1;
  15      }
  16  
  17      match = strstr(text, literal);
  18      if (match == NULL) {
  19          return 0;
  20      }
  21  
  22      offset = (size_t)(match - text);
  23      literal_length = strlen(literal);
  24  
  25      if (literal_length > SIZE_MAX - offset) {
  26          return -1;
  27      }
  28  
  29      *start = offset;
  30      *end = offset + literal_length;
  31      return 1;
  32  }
```

## E093  `flawfinder:strlen`  round 0 -> 1

Program `problem-230-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `replace_spaces`, entered `main`.

Round 0, where the rule was removed:

```c
   5  char *replace_spaces(char *str, char replacement) {
   6      if (str == NULL) {
   7          return NULL;
   8      }
   9  
  10      size_t length = strlen(str);
  11      for (size_t i = 0; i < length; i++) {
  12          if (str[i] == ' ') {
  13              str[i] = replacement;
  14          }
  15      }
  16      return str;
  17  }
```

Round 1, where the rule appeared:

```c
  18  int main(void) {
  19      char input[256];
  20      char replacement;
  21      char *result;
  22      size_t len;
  23  
  24      printf("Enter a string: ");
  25      if (fgets(input, sizeof(input), stdin) == NULL) {
  26          fprintf(stderr, "Error reading input\n");
  27          return 1;
  28      }
  29  
  30      len = strlen(input);
  31      if (len > 0 && input[len - 1] == '\n') {
  32          input[len - 1] = '\0';
  33      }
  34      else if (len == sizeof(input) - 1) {
  35          int c;
  36          while ((c = getchar()) != '\n' && c != EOF);
  37      }
  38  
  39      printf("Enter replacement character: ");
  40      if (scanf(" %c", &replacement) != 1) {
  41          fprintf(stderr, "Error reading character\n");
  42          return 1;
  43      }
  44  
  45      result = replace_spaces(input, replacement);
  46      if (result == NULL) {
  47          fprintf(stderr, "Invalid input string\n");
  48          return 1;
  49      }
  50  
  51      printf("Modified string: %s\n", result);
  52      return 0;
  53  }
```

## E094  `flawfinder:strlen`  round 4 -> 5

Program `problem-482-s2` (feedback-static-r0-t1.0-p1.0); left `find_sequences`, entered `main`.

Round 4, where the rule was removed:

```c
   5  void find_sequences(const char *input) {
   6      regex_t re;
   7      int reti;
   8      char msgbuf[128];
   9  
  10      if (regcomp(&re, "[A-Z][a-z]+", 0) != 0) {
  11          fprintf(stderr, "Could not compile regex\n");
  12          return;
  13      }
  14  
  15      if ((reti = regexec(&re, input, 0, NULL, 0)) == REG_NOMATCH) {
  16          printf("No matches found\n");
  17      } else if (reti != 0) {
  18          regerror(reti, &re, msgbuf, sizeof(msgbuf));
  19          fprintf(stderr, "Regex match failed: %s\n", msgbuf);
  20      } else {
  21          regmatch_t pmatch[1];
  22          size_t input_len = strlen(input);
  23  
  24          while (regexec(&re, input, 1, pmatch, 0) == 0) {
  25              for (int i = 0; i < 1; i++) {
  26                  if (pmatch[i].rm_so == -1) {
  27                      break;
  28                  }
  29                  size_t len = pmatch[i].rm_eo - pmatch[i].rm_so;
  30                  if (len > 0 && pmatch[i].rm_eo <= input_len) {
  31                      printf("%.*s\n", (int)len, input + pmatch[i].rm_so);
  32                      input += pmatch[i].rm_eo;
  33                      input_len -= pmatch[i].rm_eo;
  34                  }
  35              }
  36          }
  37      }
  38  
  39      regfree(&re);
  40  }
```

Round 5, where the rule appeared:

```c
  41  int main() {
  42      const char *test_str = "HelloWorld isAnExample OfPattern Matching";
  43      size_t test_len = strlen(test_str);
  44      find_sequences(test_str, test_len);
  45      return 0;
  46  }
```

## E095  `flawfinder:char`  round 2 -> 3

Program `problem-459-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `remove_uppercase`, entered `report_regex_error` (new function).

Round 2, where the rule was removed:

```c
   9  static char *remove_uppercase(const char *input)
  10  {
  11      regex_t regex;
  12      regmatch_t match;
  13      char *result;
  14      size_t input_len;
  15      size_t result_pos;
  16      const char *cursor;
  17      int ret;
  18  
  19      if (input == NULL) {
  20          return NULL;
  21      }
  22  
  23      input_len = strnlen(input, MAX_INPUT_LEN);
  24      if (input_len >= MAX_INPUT_LEN) {
  25          fprintf(stderr, "Input exceeds maximum allowed length\n");
  26          return NULL;
  27      }
  28  
  29      ret = regcomp(&regex, "[A-Z]+", REG_EXTENDED);
  30      if (ret != 0) {
  31          char errbuf[ERRBUF_SIZE];
  32          regerror(ret, &regex, errbuf, sizeof(errbuf));
  33          errbuf[sizeof(errbuf) - 1] = '\0';
  34          fprintf(stderr, "Regex compilation failed: %s\n", errbuf);
  35          return NULL;
  36      }
  37  
  38      result = malloc(input_len + 1);
  39      if (result == NULL) {
  40          regfree(&regex);
  41          fprintf(stderr, "Memory allocation failed\n");
  42          return NULL;
  43      }
  44  
  45      result_pos = 0;
  46      cursor = input;
  47  
  48      while (*cursor != '\0') {
      ... (43 more lines)
```

Round 3, where the rule appeared:

```c
   9  static void report_regex_error(int ret, const regex_t *regex, const char *prefix)
  10  {
  11      char errbuf[ERRBUF_SIZE];
  12  
  13      memset(errbuf, 0, sizeof(errbuf));
  14      regerror(ret, regex, errbuf, sizeof(errbuf) - 1);
  15      errbuf[sizeof(errbuf) - 1] = '\0';
  16      fprintf(stderr, "%s: %s\n", prefix, errbuf);
  17  }
```

## E096  `flawfinder:memcpy`  round 3 -> 4

Program `problem-544-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `flatten_tuples`, entered `safe_memcpy` (new function).

Round 3, where the rule was removed:

```c
  30  char *flatten_tuples(const Tuple *tuples, size_t count, const char *separator) {
  31      if (tuples == NULL || separator == NULL || count == 0) {
  32          return NULL;
  33      }
  34  
  35      if (count > SIZE_MAX / 2) {
  36          return NULL;
  37      }
  38  
  39      size_t sep_len = safe_strlen(separator, 4096);
  40      size_t total_len = 0;
  41  
  42      for (size_t i = 0; i < count; i++) {
  43          if (tuples[i].key == NULL || tuples[i].value == NULL) {
  44              return NULL;
  45          }
  46  
  47          size_t key_len = safe_strlen(tuples[i].key, 4096);
  48          size_t value_len = safe_strlen(tuples[i].value, 4096);
  49  
  50          if (key_len == 0 || value_len == 0) {
  51              return NULL;
  52          }
  53  
  54          if (check_add_overflow(total_len, key_len, &total_len) != 0) {
  55              return NULL;
  56          }
  57          if (check_add_overflow(total_len, value_len, &total_len) != 0) {
  58              return NULL;
  59          }
  60  
  61          if (i < count - 1) {
  62              if (check_add_overflow(total_len, sep_len, &total_len) != 0) {
  63                  return NULL;
  64              }
  65          }
  66      }
  67  
  68      if (check_add_overflow(total_len, 1, &total_len) != 0) {
  69          return NULL;
      ... (43 more lines)
```

Round 4, where the rule appeared:

```c
  30  static int safe_memcpy(void *dest, const void *src, size_t n, size_t dest_size) {
  31      if (dest == NULL || src == NULL || n > dest_size) {
  32          return -1;
  33      }
  34      memcpy(dest, src, n);
  35      return 0;
  36  }
```

## E097  `flawfinder:strlen`  round 0 -> 1

Program `problem-720-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `duplicate_string`, entered `dictionary_copy`.

Round 0, where the rule was removed:

```c
  36  static char *duplicate_string(const char *source)
  37  {
  38      size_t length;
  39      char *copy;
  40  
  41      if (source == NULL) {
  42          return NULL;
  43      }
  44  
  45      length = strlen(source);
  46      if (length == SIZE_MAX) {
  47          return NULL;
  48      }
  49  
  50      copy = malloc(length + 1U);
  51      if (copy == NULL) {
  52          return NULL;
  53      }
  54  
  55      memcpy(copy, source, length + 1U);
  56      return copy;
  57  }
```

Round 1, where the rule appeared:

```c
  76  static int dictionary_copy(Dictionary *destination, const Dictionary *source)
  77  {
  78      Dictionary copy = { NULL, 0U };
  79      size_t i;
  80  
  81      if (destination == NULL || source == NULL) {
  82          return -1;
  83      }
  84  
  85      if (source->count > 0U && source->entries == NULL) {
  86          return -1;
  87      }
  88  
  89      if (source->count > SIZE_MAX / sizeof(*copy.entries)) {
  90          return -1;
  91      }
  92  
  93      if (source->count > 0U) {
  94          copy.entries = calloc(source->count, sizeof(*copy.entries));
  95          if (copy.entries == NULL) {
  96              return -1;
  97          }
  98      }
  99  
 100      copy.count = source->count;
 101  
 102      for (i = 0U; i < source->count; ++i) {
 103          const char *key = source->entries[i].key;
 104          const char *value = source->entries[i].value;
 105          size_t key_length;
 106          size_t value_length;
 107  
 108          if (key == NULL || value == NULL) {
 109              dictionary_destroy(&copy);
 110              return -1;
 111          }
 112  
 113          key_length = strlen(key);
 114          value_length = strlen(value);
 115  
      ... (16 more lines)
```

## E098  `flawfinder:getchar`  round 1 -> 2

Program `problem-194` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `flush_stdin` (new function).

Round 1, where the rule was removed:

```c
  37  int main(void)
  38  {
  39      char input[INPUT_SIZE];
  40      long decimal = 0;
  41      size_t len;
  42  
  43      printf("Enter an octal number: ");
  44  
  45      if (fgets(input, sizeof(input), stdin) == NULL) {
  46          fprintf(stderr, "Error: failed to read input.\n");
  47          return 1;
  48      }
  49  
  50      len = strnlen(input, sizeof(input));
  51      if (len > 0 && input[len - 1] == '\n') {
  52          input[len - 1] = '\0';
  53      } else {
  54          int ch;
  55          while ((ch = getchar()) != '\n' && ch != EOF) {
  56              ;
  57          }
  58      }
  59  
  60      if (octal_to_decimal(input, sizeof(input), &decimal) != 0) {
  61          fprintf(stderr, "Error: invalid octal number or value out of range.\n");
  62          return 1;
  63      }
  64  
  65      printf("Decimal value: %ld\n", decimal);
  66  
  67      return 0;
  68  }
```

Round 2, where the rule appeared:

```c
  37  static void flush_stdin(void)
  38  {
  39      int ch;
  40  
  41      do {
  42          ch = getchar();
  43      } while (ch != '\n' && ch != EOF);
  44  }
```

## E099  `flawfinder:strlen`  round 3 -> 4

Program `problem-395-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `first_non_repeated_char`.

Round 3, where the rule was removed:

```c
  40  int main(void) {
  41      char input[256];
  42  
  43      printf("Enter a string: ");
  44      if (fgets(input, sizeof(input), stdin) == NULL) {
  45          return 1;
  46      }
  47  
  48      size_t len = strlen(input);
  49      if (len > 0 && input[len - 1] == '\n') {
  50          input[len - 1] = '\0';
  51      } else if (len == sizeof(input) - 1 && input[len - 1] != '\n') {
  52          int c;
  53          while ((c = getchar()) != '\n' && c != EOF);
  54      }
  55  
  56      char result = first_non_repeated_char(input);
  57  
  58      if (result != '\0') {
  59          printf("First non-repeated character: %c\n", result);
  60      } else {
  61          printf("No non-repeated character found.\n");
  62      }
  63  
  64      return 0;
  65  }
```

Round 4, where the rule appeared:

```c
   6  char first_non_repeated_char(const char *str) {
   7      unsigned int count[UCHAR_MAX + 1];
   8      int first_pos[UCHAR_MAX + 1];
   9      size_t i;
  10  
  11      if (str == NULL) {
  12          return '\0';
  13      }
  14  
  15      for (i = 0; i <= UCHAR_MAX; i++) {
  16          count[i] = 0;
  17          first_pos[i] = -1;
  18      }
  19  
  20      for (i = 0; i < strlen(str) && str[i] != '\0'; i++) {
  21          unsigned char c = (unsigned char)str[i];
  22          if (count[c] < UINT_MAX) {
  23              count[c]++;
  24          }
  25          if (first_pos[c] == -1) {
  26              first_pos[c] = (int)i;
  27          }
  28      }
  29  
  30      int min_pos = INT_MAX;
  31      for (i = 0; i <= UCHAR_MAX; i++) {
  32          if (count[i] == 1 && first_pos[i] < min_pos) {
  33              min_pos = first_pos[i];
  34          }
  35      }
  36  
  37      if (min_pos != INT_MAX && min_pos >= 0 && (size_t)min_pos < strlen(str)) {
  38          return str[min_pos];
  39      }
  40  
  41      return '\0';
  42  }
```

## E100  `flawfinder:fgetc`  round 1 -> 2

Program `problem-11` (feedback-static-r0-t1.0-p1.0); left `main, read_line`, entered `read_target` (new function).

Round 1, where the rule was removed:

```c
 100  int main(void)
 101  {
 102      char *input;
 103      size_t input_length;
 104      char target;
 105      char *result;
 106      int ch;
 107  
 108      if (!read_line(&input, &input_length)) {
 109          return EXIT_FAILURE;
 110      }
 111  
 112      do {
 113          ch = fgetc(stdin);
 114      } while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ||
 115               ch == '\v' || ch == '\f');
 116  
 117      if (ch == EOF) {
 118          free(input);
 119          return EXIT_FAILURE;
 120      }
 121  
 122      target = (char)ch;
 123      result = remove_first_and_last(input, input_length, target);
 124      free(input);
 125  
 126      if (result == NULL) {
 127          return EXIT_FAILURE;
 128      }
 129  
 130      if (fputs(result, stdout) == EOF || fputc('\n', stdout) == EOF) {
 131          free(result);
 132          return EXIT_FAILURE;
 133      }
 134  
 135      free(result);
 136      return EXIT_SUCCESS;
 137  }
  49  static int read_line(char **line, size_t *length)
  50  {
  51      size_t capacity = 128;
  52      size_t used = 0;
  53      char *buffer;
  54      int ch;
  55  
  56      if (line == NULL || length == NULL) {
  57          return 0;
  58      }
  59  
  60      buffer = malloc(capacity);
  61      if (buffer == NULL) {
  62          return 0;
  63      }
  64  
  65      while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
  66          if (used == capacity - 1) {
  67              size_t new_capacity;
  68              char *new_buffer;
  69  
  70              if (capacity > SIZE_MAX / 2) {
  71                  free(buffer);
  72                  return 0;
  73              }
  74  
  75              new_capacity = capacity * 2;
  76              new_buffer = realloc(buffer, new_capacity);
  77              if (new_buffer == NULL) {
  78                  free(buffer);
  79                  return 0;
  80              }
  81  
  82              buffer = new_buffer;
  83              capacity = new_capacity;
  84          }
  85  
  86          buffer[used++] = (char)ch;
  87      }
  88  
      ... (10 more lines)
```

Round 2, where the rule appeared:

```c
 109  static int read_target(FILE *stream, char *target)
 110  {
 111      int ch;
 112  
 113      if (stream == NULL || target == NULL) {
 114          return 0;
 115      }
 116  
 117      do {
 118          ch = fgetc(stream);
 119      } while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n' ||
 120               ch == '\v' || ch == '\f');
 121  
 122      if (ch == EOF) {
 123          return 0;
 124      }
 125  
 126      *target = (char)(unsigned char)ch;
 127      return 1;
 128  }
```

## E101  `cppcheck:knownConditionTrueFalse`  round 4 -> 5

Program `problem-584-s2` (feedback-static-r0-t1.0-p1.0); left `safe_copy`, entered `find_adverbs`.

Round 4, where the rule was removed:

```c
  14  static int safe_copy(char *dest, const char *src, size_t len, size_t max_len) {
  15      if (dest == NULL) {
  16          return 0;
  17      }
  18      
  19      if (len >= max_len) {
  20          return 0;
  21      }
  22      
  23      if (len > 0) {
  24          if (src == NULL) {
  25              dest[0] = '\0';
  26              return 0;
  27          }
  28          if (len < max_len) {
  29              memcpy(dest, src, len);
  30          } else {
  31              return 0;
  32          }
  33      }
  34      
  35      dest[len] = '\0';
  36      return 1;
  37  }
```

Round 5, where the rule appeared:

```c
  35  int find_adverbs(const char *sentence, AdverbMatch matches[], int max_matches) {
  36      regex_t regex;
  37      regmatch_t pmatch[2];
  38      const char *pattern = "\\b[A-Za-z]+ly\\b";
  39      int match_count = 0;
  40      int ret;
  41      size_t sentence_len;
  42      size_t offset;
  43      
  44      if (sentence == NULL || matches == NULL || max_matches <= 0) {
  45          return 0;
  46      }
  47      
  48      ret = regcomp(&regex, pattern, REG_EXTENDED);
  49      if (ret != 0) {
  50          return 0;
  51      }
  52      
  53      sentence_len = strlen(sentence);
  54      
  55      if (sentence_len > SIZE_MAX / 2) {
  56          regfree(&regex);
  57          return 0;
  58      }
  59      
  60      offset = 0;
  61      
  62      while (match_count < (size_t)max_matches && offset < sentence_len) {
  63          const char *search_start = sentence + offset;
  64          size_t search_len = sentence_len - offset;
  65          char *temp = NULL;
  66          
  67          if (search_len == 0) {
  68              break;
  69          }
  70          
  71          temp = (char *)malloc(search_len + 1);
  72          if (temp == NULL) {
  73              break;
  74          }
      ... (43 more lines)
```

## E102  `flawfinder:strlen`  round 0 -> 1

Program `problem-30` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9  
  10      size_t len = strlen(str);
  11      int count = 0;
  12  
  13      for (size_t i = 0; i < len; i++) {
  14          for (size_t j = i; j < len; j++) {
  15              if (str[i] == str[j]) {
  16                  count++;
  17              }
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  23  int main(int argc, char * const argv[]) {
  24      if (argc != 2) {
  25          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  26          return EXIT_FAILURE;
  27      }
  28  
  29      size_t len = strlen(argv[1]);
  30      int result = count_substrings(argv[1], len);
  31      printf("%d\n", result);
  32  
  33      return EXIT_SUCCESS;
  34  }
```

## E103  `flawfinder:strlen`  round 0 -> 1

Program `problem-874-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `is_concatenation`, entered `main`.

Round 0, where the rule was removed:

```c
   5  bool is_concatenation(const char *str, const char *sub) {
   6      if (str == NULL || sub == NULL) {
   7          return false;
   8      }
   9  
  10      size_t str_len = strlen(str);
  11      size_t sub_len = strlen(sub);
  12  
  13      if (sub_len == 0 || str_len == 0 || str_len % sub_len != 0) {
  14          return false;
  15      }
  16  
  17      size_t repetitions = str_len / sub_len;
  18  
  19      for (size_t i = 1; i < repetitions; i++) {
  20          if (memcmp(str, str + (i * sub_len), sub_len) != 0) {
  21              return false;
  22          }
  23      }
  24  
  25      return true;
  26  }
```

Round 1, where the rule appeared:

```c
  25  int main(void) {
  26      const char *str1 = "abcabc";
  27      const char *str2 = "abc";
  28      
  29      if (is_concatenation(str1, strlen(str1), str2, strlen(str2))) {
  30          printf("True\n");
  31      } else {
  32          printf("False\n");
  33      }
  34  
  35      const char *str3 = "abcabd";
  36      if (is_concatenation(str3, strlen(str3), str2, strlen(str2))) {
  37          printf("True\n");
  38      } else {
  39          printf("False\n");
  40      }
  41  
  42      return 0;
  43  }
```

## E104  `flawfinder:strlen`  round 0 -> 1

Program `problem-61` (feedback-static-r0-t1.0-p1.0); left `count_matching_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   4  int count_matching_substrings(const char *str) {
   5      int length = strlen(str);
   6      int counter = 0;
   7      for (int i = 0; i < length; i++) {
   8          int sum = 0;
   9          for (int len = 1; i + len <= length; len++) {
  10              sum += str[i + len - 1] - '0';
  11              if (sum == len) {
  12                  counter++;
  13              }
  14          }
  15      }
  16      return counter;
  17  }
```

Round 1, where the rule appeared:

```c
  18  int main() {
  19      const char *input = "1112223";
  20      size_t input_length = strlen(input);
  21      int result = count_matching_substrings(input, input_length);
  22      printf("Number of substrings: %d\n", result);
  23      return 0;
  24  }
```

## E105  `flawfinder:memcpy`  round 4 -> 5

Program `problem-906` (feedback-static-r0-t1.0-p1.0); left `extract_date_from_url`, entered `safe_copy_substring` (new function).

Round 4, where the rule was removed:

```c
  10  int extract_date_from_url(const char *url, int *year, int *month, int *day) {
  11      if (url == NULL || year == NULL || month == NULL || day == NULL) {
  12          return -1;
  13      }
  14  
  15      size_t url_len = strlen(url);
  16      if (url_len == 0) {
  17          return -1;
  18      }
  19  
  20      regex_t regex;
  21      regmatch_t matches[4];
  22      const char *pattern = "([0-9]{4})[-/]([0-9]{1,2})[-/]([0-9]{1,2})";
  23      int ret;
  24  
  25      ret = regcomp(&regex, pattern, REG_EXTENDED);
  26      if (ret != 0) {
  27          return -1;
  28      }
  29  
  30      ret = regexec(&regex, url, 4, matches, 0);
  31      if (ret != 0) {
  32          regfree(&regex);
  33          return -1;
  34      }
  35  
  36      if (matches[1].rm_so == -1 || matches[2].rm_so == -1 || matches[3].rm_so == -1) {
  37          regfree(&regex);
  38          return -1;
  39      }
  40  
  41      if (matches[1].rm_so < 0 || matches[1].rm_eo > (regoff_t)url_len ||
  42          matches[2].rm_so < 0 || matches[2].rm_eo > (regoff_t)url_len ||
  43          matches[3].rm_so < 0 || matches[3].rm_eo > (regoff_t)url_len) {
  44          regfree(&regex);
  45          return -1;
  46      }
  47  
  48      if (matches[1].rm_eo <= matches[1].rm_so ||
  49          matches[2].rm_eo <= matches[2].rm_so ||
      ... (82 more lines)
```

Round 5, where the rule appeared:

```c
  29  static int safe_copy_substring(const char *url, size_t url_len, regoff_t start, regoff_t end,
  30                                  char *dest, size_t dest_size) {
  31      if (start < 0 || end < 0) return -1;
  32      if ((size_t)end > url_len) return -1;
  33      if (end <= start) return -1;
  34  
  35      size_t len = (size_t)(end - start);
  36      if (len >= dest_size) return -1;
  37  
  38      memcpy(dest, url + start, len);
  39      dest[len] = '\0';
  40      return 0;
  41  }
```

## E106  `flawfinder:fgetc`  round 2 -> 3

Program `problem-440` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_sentence` (new function).

Round 2, where the rule was removed:

```c
 109  int main(void)
 110  {
 111      char *sentence = NULL;
 112      size_t capacity = 0U;
 113      size_t length = 0U;
 114      int ch;
 115  
 116      while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
 117          if (length == SIZE_MAX) {
 118              free(sentence);
 119              fputs("Input is too large.\n", stderr);
 120              return EXIT_FAILURE;
 121          }
 122  
 123          if (length + 1U >= capacity) {
 124              size_t new_capacity;
 125              char *new_sentence;
 126  
 127              if (capacity == 0U) {
 128                  new_capacity = 128U;
 129              } else {
 130                  if (capacity > SIZE_MAX / 2U) {
 131                      free(sentence);
 132                      fputs("Input is too large.\n", stderr);
 133                      return EXIT_FAILURE;
 134                  }
 135                  new_capacity = capacity * 2U;
 136              }
 137  
 138              new_sentence = realloc(sentence, new_capacity);
 139              if (new_sentence == NULL) {
 140                  free(sentence);
 141                  fputs("Memory allocation failed.\n", stderr);
 142                  return EXIT_FAILURE;
 143              }
 144  
 145              sentence = new_sentence;
 146              capacity = new_capacity;
 147          }
 148  
      ... (48 more lines)
```

Round 3, where the rule appeared:

```c
 122  static int read_sentence(char **sentence, size_t *length)
 123  {
 124      char *buffer = NULL;
 125      size_t capacity = 0U;
 126      size_t used = 0U;
 127      int ch;
 128  
 129      while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
 130          if (used == SIZE_MAX) {
 131              free(buffer);
 132              fputs("Input is too large.\n", stderr);
 133              return 0;
 134          }
 135  
 136          if (used + 1U >= capacity) {
 137              size_t new_capacity;
 138              char *new_buffer;
 139  
 140              if (capacity == 0U) {
 141                  new_capacity = 128U;
 142              } else {
 143                  if (capacity > SIZE_MAX / 2U) {
 144                      free(buffer);
 145                      fputs("Input is too large.\n", stderr);
 146                      return 0;
 147                  }
 148                  new_capacity = capacity * 2U;
 149              }
 150  
 151              new_buffer = realloc(buffer, new_capacity);
 152              if (new_buffer == NULL) {
 153                  free(buffer);
 154                  fputs("Memory allocation failed.\n", stderr);
 155                  return 0;
 156              }
 157  
 158              buffer = new_buffer;
 159              capacity = new_capacity;
 160          }
 161  
      ... (23 more lines)
```

## E107  `flawfinder:memcpy`  round 4 -> 5

Program `problem-421` (feedback-static+dynamic-r0-t1.0-p1.0); left `concatenate_tuple`, entered `safe_copy` (new function).

Round 4, where the rule was removed:

```c
  21  char *concatenate_tuple(const char **elements, size_t count, const char *delimiter)
  22  {
  23      size_t total_length;
  24      size_t delimiter_length;
  25      size_t i;
  26      size_t *lengths;
  27      size_t remaining;
  28      char *result;
  29      char *position;
  30      int valid;
  31  
  32      if (elements == NULL || delimiter == NULL || count == 0) {
  33          return NULL;
  34      }
  35  
  36      for (i = 0; i < count; i++) {
  37          if (elements[i] == NULL) {
  38              return NULL;
  39          }
  40      }
  41  
  42      if (count > SIZE_MAX / sizeof(size_t)) {
  43          return NULL;
  44      }
  45  
  46      lengths = malloc(count * sizeof(size_t));
  47      if (lengths == NULL) {
  48          return NULL;
  49      }
  50  
  51      delimiter_length = bounded_length(delimiter, MAX_STRING_LENGTH, &valid);
  52      if (!valid) {
  53          free(lengths);
  54          return NULL;
  55      }
  56  
  57      total_length = 0;
  58  
  59      for (i = 0; i < count; i++) {
  60          lengths[i] = bounded_length(elements[i], MAX_STRING_LENGTH, &valid);
      ... (57 more lines)
```

Round 5, where the rule appeared:

```c
  21  static int safe_copy(char *destination, size_t destination_remaining,
  22                       const char *source, size_t source_length)
  23  {
  24      if (destination == NULL || source == NULL) {
  25          return 0;
  26      }
  27      if (source_length > destination_remaining) {
  28          return 0;
  29      }
  30      memcpy(destination, source, source_length);
  31      return 1;
  32  }
```

## E108  `flawfinder:memcpy`  round 1 -> 2

Program `problem-310-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `string_to_tuple`, entered `safe_strdup` (new function).

Round 1, where the rule was removed:

```c
  11  Tuple string_to_tuple(const char *str) {
  12      Tuple tuple;
  13      tuple.items = NULL;
  14      tuple.count = 0;
  15      
  16      if (str == NULL) {
  17          return tuple;
  18      }
  19      
  20      size_t len = strlen(str);
  21      if (len == 0) {
  22          return tuple;
  23      }
  24      
  25      char *copy = malloc(len + 1);
  26      if (copy == NULL) {
  27          return tuple;
  28      }
  29      memcpy(copy, str, len + 1);
  30      
  31      size_t capacity = 8;
  32      tuple.items = malloc(capacity * sizeof(char *));
  33      if (tuple.items == NULL) {
  34          free(copy);
  35          return tuple;
  36      }
  37      
  38      char *start = copy;
  39      char *p = copy;
  40      int in_quotes = 0;
  41      size_t remaining;
  42      
  43      while (*p) {
  44          if (*p == '"') {
  45              in_quotes = !in_quotes;
  46              remaining = strlen(p + 1) + 1;
  47              memmove(p, p + 1, remaining);
  48          } else if (*p == ',' && !in_quotes) {
  49              *p = '\0';
  50              while (isspace((unsigned char)*start)) start++;
      ... (88 more lines)
```

Round 2, where the rule appeared:

```c
  22  static char *safe_strdup(const char *str, size_t max_len) {
  23      if (str == NULL) {
  24          return NULL;
  25      }
  26      size_t len = safe_strlen(str, max_len);
  27      char *copy = malloc(len + 1);
  28      if (copy == NULL) {
  29          return NULL;
  30      }
  31      memcpy(copy, str, len);
  32      copy[len] = '\0';
  33      return copy;
  34  }
```

## E109  `flawfinder:char`  round 1 -> 2

Program `problem-621` (feedback-static+dynamic-r0-t1.0-p1.0); left `increment_numeric_values`, entered `main`.

Round 1, where the rule was removed:

```c
   6  char* increment_numeric_values(const char* str, int k) {
   7      if (str == NULL) {
   8          return NULL;
   9      }
  10  
  11      size_t len = strlen(str);
  12      size_t new_len = len * 2 + 1;
  13      char* result = (char*)malloc(new_len);
  14      if (result == NULL) {
  15          return NULL;
  16      }
  17  
  18      size_t res_idx = 0;
  19      size_t i = 0;
  20  
  21      while (i < len) {
  22          if (isdigit((unsigned char)str[i])) {
  23              long val = 0;
  24              while (i < len && isdigit((unsigned char)str[i])) {
  25                  val = val * 10 + (str[i] - '0');
  26                  i++;
  27              }
  28              val += k;
  29              
  30              char num_str[32];
  31              int written = snprintf(num_str, sizeof(num_str), "%ld", val);
  32              if (written > 0 && (size_t)written < sizeof(num_str)) {
  33                  if (res_idx + written >= new_len) {
  34                      new_len = (res_idx + written) * 2 + 1;
  35                      char* temp = (char*)realloc(result, new_len);
  36                      if (temp == NULL) {
  37                          free(result);
  38                          return NULL;
  39                      }
  40                      result = temp;
  41                  }
  42                  memcpy(result + res_idx, num_str, written);
  43                  res_idx += written;
  44              }
  45          } else {
      ... (16 more lines)
```

Round 2, where the rule appeared:

```c
  68  int main(void) {
  69      char buffer[4096];
  70      int k;
  71  
  72      if (scanf("%d ", &k) == 1) {
  73          if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
  74              size_t len = 0;
  75              while (buffer[len] != '\0') {
  76                  len++;
  77              }
  78              if (len > 0 && buffer[len - 1] == '\n') {
  79                  buffer[len - 1] = '\0';
  80              }
  81              
  82              char* output = increment_numeric_values(buffer, k);
  83              if (output != NULL) {
  84                  printf("['");
  85                  size_t out_len = 0;
  86                  while (output[out_len] != '\0') out_len++;
  87                  
  88                  int first = 1;
  89                  size_t i = 0;
  90                  while (i < out_len) {
  91                      while (i < out_len && isspace((unsigned char)output[i])) {
  92                          i++;
  93                      }
  94                      if (i >= out_len) break;
  95                      
  96                      if (!first) {
  97                          printf("', '");
  98                      }
  99                      first = 0;
 100                      
 101                      while (i < out_len && !isspace((unsigned char)output[i])) {
 102                          putchar(output[i]);
 103                          i++;
 104                      }
 105                  }
 106                  printf("']\n");
 107                  free(output);
      ... (6 more lines)
```

## E110  `cppcheck:knownConditionTrueFalse`  round 3 -> 4

Program `problem-526-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `capitalize_first_last`, entered `capitalize` (new function).

Round 3, where the rule was removed:

```c
   5  void capitalize_first_last(char *str) {
   6      if (str == NULL || *str == '\0') return;
   7      size_t len = strnlen(str, 256);
   8      for (size_t i = 0; i < len; ++i) {
   9          if (i == 0 || (i > 0 && str[i - 1] == ' ') || (i < len - 1 && str[i + 1] == ' ')) {
  10              str[i] = toupper((unsigned char)str[i]);
  11          }
  12          if (i > 0 && (i == len - 1 || str[i + 1] == ' ')) {
  13              str[i] = toupper((unsigned char)str[i]);
  14          }
  15      }
  16  }
```

Round 4, where the rule appeared:

```c
   5  void capitalize(char *str) {
   6      if (str == NULL || *str == '\0') return;
   7      size_t len = strnlen(str, 256);
   8      for (size_t i = 0; i < len; ++i) {
   9          if (i == 0 || (i > 0 && str[i - 1] == ' ')) {
  10              str[i] = toupper((unsigned char)str[i]);
  11          }
  12          if (i > 0 && (i == len - 1 || str[i + 1] == ' ')) {
  13              str[i] = toupper((unsigned char)str[i]);
  14          }
  15      }
  16  }
```

## E111  `flawfinder:strlen`  round 2 -> 3

Program `problem-30-s3` (feedback-static-r0-t1.0-p1.0); left `countSubstringsWithSameStartAndEnd`, entered `main`.

Round 2, where the rule was removed:

```c
   4  int countSubstringsWithSameStartAndEnd(const char *str) {
   5      int count = 0;
   6      size_t len = strlen(str);
   7      for (size_t i = 0; i < len; ++i) {
   8          for (size_t j = i + 1; j <= len; ++j) {
   9              if (str[i] == str[j - 1]) {
  10                  count++;
  11              }
  12          }
  13      }
  14      return count;
  15  }
```

Round 3, where the rule appeared:

```c
  16  int main() {
  17      const char *exampleString = "abcab";
  18      size_t exampleStringLength = strlen(exampleString);
  19      int result = countSubstringsWithSameStartAndEnd(exampleString, exampleStringLength);
  20      printf("Number of substrings: %d\n", result);
  21      return 0;
  22  }
```

## E112  `flawfinder:char`  round 0 -> 1

Program `problem-397-s3` (feedback-static-r0-t1.0-p1.0); left `main`, entered `read_and_parse_int` (new function).

Round 0, where the rule was removed:

```c
  39  int main(void) {
  40      char input[64];
  41      int a, b, c;
  42  
  43      if (fgets(input, sizeof(input), stdin) == NULL) {
  44          fprintf(stderr, "Error reading input\n");
  45          return EXIT_FAILURE;
  46      }
  47      if (parse_int(input, &a) != 0) {
  48          fprintf(stderr, "Invalid input for first number\n");
  49          return EXIT_FAILURE;
  50      }
  51  
  52      if (fgets(input, sizeof(input), stdin) == NULL) {
  53          fprintf(stderr, "Error reading input\n");
  54          return EXIT_FAILURE;
  55      }
  56      if (parse_int(input, &b) != 0) {
  57          fprintf(stderr, "Invalid input for second number\n");
  58          return EXIT_FAILURE;
  59      }
  60  
  61      if (fgets(input, sizeof(input), stdin) == NULL) {
  62          fprintf(stderr, "Error reading input\n");
  63          return EXIT_FAILURE;
  64      }
  65      if (parse_int(input, &c) != 0) {
  66          fprintf(stderr, "Invalid input for third number\n");
  67          return EXIT_FAILURE;
  68      }
  69  
  70      printf("%d\n", median_of_three(a, b, c));
  71  
  72      return EXIT_SUCCESS;
  73  }
```

Round 1, where the rule appeared:

```c
  59  int read_and_parse_int(int *out) {
  60      char input[INPUT_BUFFER_SIZE];
  61      size_t len;
  62  
  63      if (fgets(input, sizeof(input), stdin) == NULL) {
  64          return -1;
  65      }
  66  
  67      len = strlen(input);
  68      if (len > 0 && input[len - 1] != '\n' && !feof(stdin)) {
  69          int ch;
  70          while ((ch = getchar()) != '\n' && ch != EOF);
  71          return -1;
  72      }
  73  
  74      if (parse_int(input, out) != 0) {
  75          return -1;
  76      }
  77  
  78      return 0;
  79  }
```

## E113  `flawfinder:memcpy`  round 1 -> 2

Program `problem-450` (feedback-static-r0-t1.0-p1.0); left `extract_strings_by_size`, entered `duplicate_bounded` (new function).

Round 1, where the rule was removed:

```c
  17  char **extract_strings_by_size(const char **list, size_t list_count, size_t target_size, size_t *result_count)
  18  {
  19      char **result = NULL;
  20      size_t count = 0;
  21      size_t i;
  22  
  23      if (result_count == NULL) {
  24          return NULL;
  25      }
  26  
  27      *result_count = 0;
  28  
  29      if (list == NULL || list_count == 0 || target_size >= MAX_STRING_LENGTH) {
  30          return NULL;
  31      }
  32  
  33      for (i = 0; i < list_count; i++) {
  34          if (list[i] != NULL && bounded_length(list[i]) == target_size) {
  35              count++;
  36          }
  37      }
  38  
  39      if (count == 0) {
  40          return NULL;
  41      }
  42  
  43      if (count > SIZE_MAX / sizeof(char *)) {
  44          return NULL;
  45      }
  46  
  47      result = malloc(count * sizeof(char *));
  48      if (result == NULL) {
  49          return NULL;
  50      }
  51  
  52      count = 0;
  53      for (i = 0; i < list_count; i++) {
  54          if (list[i] != NULL) {
  55              size_t len = bounded_length(list[i]);
  56              if (len == target_size) {
      ... (15 more lines)
```

Round 2, where the rule appeared:

```c
  18  static char *duplicate_bounded(const char *s, size_t len)
  19  {
  20      char *copy;
  21  
  22      if (s == NULL || len >= MAX_STRING_LENGTH) {
  23          return NULL;
  24      }
  25  
  26      if (len > SIZE_MAX - 1) {
  27          return NULL;
  28      }
  29  
  30      copy = malloc(len + 1);
  31      if (copy == NULL) {
  32          return NULL;
  33      }
  34  
  35      memcpy(copy, s, len);
  36      copy[len] = '\0';
  37      return copy;
  38  }
```

## E114  `flawfinder:strlen`  round 4 -> 5

Program `problem-708-s3` (feedback-static-r0-t1.0-p1.0); left `string_to_list`, entered `main`.

Round 4, where the rule was removed:

```c
  84  static int string_to_list(const char *input, char delimiter,
  85                            StringNode **result)
  86  {
  87      StringNode *head = NULL;
  88      StringNode *tail = NULL;
  89      const char *token_start;
  90      const char *cursor;
  91      size_t input_length;
  92      size_t remaining;
  93  
  94      if (input == NULL || result == NULL || delimiter == '\0') {
  95          errno = EINVAL;
  96          return -1;
  97      }
  98  
  99      *result = NULL;
 100      input_length = strlen(input);
 101      token_start = input;
 102      cursor = input;
 103      remaining = input_length;
 104  
 105      for (;;) {
 106          if (*cursor == delimiter || *cursor == '\0') {
 107              size_t length = (size_t)(cursor - token_start);
 108  
 109              if (append_node(&head, &tail, token_start, remaining,
 110                              length) != 0) {
 111                  free_list(head);
 112                  return -1;
 113              }
 114  
 115              if (*cursor == '\0') {
 116                  break;
 117              }
 118  
 119              ++cursor;
 120              --remaining;
 121              token_start = cursor;
 122              continue;
 123          }
      ... (8 more lines)
```

Round 5, where the rule appeared:

```c
 129  int main(int argc, char *argv[])
 130  {
 131      StringNode *list = NULL;
 132      char delimiter = ',';
 133      size_t input_length;
 134  
 135      if (argc < 2 || argc > 3) {
 136          fprintf(stderr, "Usage: %s <string> [delimiter]\n", argv[0]);
 137          return EXIT_FAILURE;
 138      }
 139  
 140      if (argc == 3) {
 141          if (argv[2][0] == '\0' || argv[2][1] != '\0') {
 142              fputs("Delimiter must be exactly one non-null character.\n",
 143                    stderr);
 144              return EXIT_FAILURE;
 145          }
 146  
 147          delimiter = argv[2][0];
 148      }
 149  
 150      input_length = strlen(argv[1]);
 151  
 152      if (string_to_list_n(argv[1], input_length, delimiter, &list) != 0) {
 153          perror("string_to_list_n");
 154          return EXIT_FAILURE;
 155      }
 156  
 157      if (print_list(list) != 0) {
 158          perror("print_list");
 159          free_list(list);
 160          return EXIT_FAILURE;
 161      }
 162  
 163      free_list(list);
 164  
 165      if (fflush(stdout) == EOF) {
 166          perror("stdout");
 167          return EXIT_FAILURE;
 168      }
      ... (3 more lines)
```

## E115  `flawfinder:strlen`  round 2 -> 3

Program `problem-917-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `find_sequences`, entered `main`.

Round 2, where the rule was removed:

```c
   7  char** find_sequences(const char* str, int* count) {
   8      *count = 0;
   9      size_t capacity = 10;
  10      char** results = malloc(capacity * sizeof(char*));
  11      if (!results) {
  12          return NULL;
  13      }
  14  
  15      size_t len = str ? strlen(str) : 0;
  16  
  17      size_t i = 0;
  18  
  19      while (i < len) {
  20          if (isupper((unsigned char)str[i])) {
  21              size_t start = i;
  22              i++;
  23              while (i < len && islower((unsigned char)str[i])) {
  24                  i++;
  25              }
  26              if (i > start) {
  27                  if ((size_t)*count >= capacity) {
  28                      size_t new_capacity = capacity * 2;
  29                      if (new_capacity <= capacity) {
  30                          for (int j = 0; j < *count; j++) {
  31                              free(results[j]);
  32                          }
  33                          free(results);
  34                          *count = 0;
  35                          return NULL;
  36                      }
  37                      char** temp = realloc(results, new_capacity * sizeof(char*));
  38                      if (!temp) {
  39                          for (int j = 0; j < *count; j++) {
  40                              free(results[j]);
  41                          }
  42                          free(results);
  43                          *count = 0;
  44                          return NULL;
  45                      }
  46                      results = temp;
      ... (24 more lines)
```

Round 3, where the rule appeared:

```c
  70  int main(void) {
  71      const char* text = "This is a Test string with Multiple valid Sequences like Abc and Xyz.";
  72      size_t count = 0;
  73      size_t len = strlen(text);
  74      char** sequences = find_sequences(text, len, &count);
  75  
  76      if (!sequences) {
  77          fprintf(stderr, "Memory allocation failed\n");
  78          return 1;
  79      }
  80  
  81      for (size_t i = 0; i < count; i++) {
  82          printf("%s\n", sequences[i]);
  83          free(sequences[i]);
  84      }
  85      free(sequences);
  86  
  87      return 0;
  88  }
```

## E116  `flawfinder:memcpy`  round 1 -> 2

Program `problem-740-s3` (feedback-static-r0-t1.0-p1.0); left `tuple_to_dict`, entered `safe_strdup` (new function).

Round 1, where the rule was removed:

```c
  15  Dictionary tuple_to_dict(char **tuple, size_t tuple_size) {
  16      Dictionary dict;
  17      dict.size = tuple_size / 2;
  18      dict.entries = NULL;
  19  
  20      if (dict.size == 0 || tuple == NULL) {
  21          return dict;
  22      }
  23  
  24      dict.entries = (DictEntry *)malloc(dict.size * sizeof(DictEntry));
  25      if (dict.entries == NULL) {
  26          dict.size = 0;
  27          return dict;
  28      }
  29  
  30      for (size_t i = 0; i < dict.size; i++) {
  31          dict.entries[i].key = NULL;
  32          dict.entries[i].value = NULL;
  33  
  34          if (tuple[i * 2] == NULL) {
  35              for (size_t j = 0; j < i; j++) {
  36                  free(dict.entries[j].key);
  37                  free(dict.entries[j].value);
  38              }
  39              free(dict.entries);
  40              dict.entries = NULL;
  41              dict.size = 0;
  42              return dict;
  43          }
  44  
  45          size_t key_len = strlen(tuple[i * 2]);
  46          dict.entries[i].key = (char *)malloc(key_len + 1);
  47          if (dict.entries[i].key == NULL) {
  48              for (size_t j = 0; j < i; j++) {
  49                  free(dict.entries[j].key);
  50                  free(dict.entries[j].value);
  51              }
  52              free(dict.entries);
  53              dict.entries = NULL;
  54              dict.size = 0;
      ... (34 more lines)
```

Round 2, where the rule appeared:

```c
  27  static char *safe_strdup(const char *str, size_t max_len) {
  28      if (str == NULL) {
  29          return NULL;
  30      }
  31      size_t len = safe_strlen(str, max_len);
  32      char *copy = (char *)malloc(len + 1);
  33      if (copy == NULL) {
  34          return NULL;
  35      }
  36      if (len > 0) {
  37          memcpy(copy, str, len);
  38      }
  39      copy[len] = '\0';
  40      return copy;
  41  }
```

## E117  `flawfinder:char`  round 0 -> 1

Program `problem-407-s2` (feedback-static-r0-t1.0-p1.0); left `next_bigger`, entered `main`.

Round 0, where the rule was removed:

```c
   7  static int next_bigger(uint64_t number, uint64_t *result)
   8  {
   9      char digits[32];
  10      int length;
  11      int pivot;
  12      int successor;
  13      int left;
  14      int right;
  15      char temporary;
  16      char *end;
  17      uintmax_t value;
  18  
  19      if (result == NULL) {
  20          return 0;
  21      }
  22  
  23      length = snprintf(digits, sizeof(digits), "%" PRIu64, number);
  24      if (length < 0 || (size_t)length >= sizeof(digits)) {
  25          return 0;
  26      }
  27  
  28      pivot = length - 2;
  29      while (pivot >= 0 && digits[pivot] >= digits[pivot + 1]) {
  30          --pivot;
  31      }
  32  
  33      if (pivot < 0) {
  34          return 0;
  35      }
  36  
  37      successor = length - 1;
  38      while (digits[successor] <= digits[pivot]) {
  39          --successor;
  40      }
  41  
  42      temporary = digits[pivot];
  43      digits[pivot] = digits[successor];
  44      digits[successor] = temporary;
  45  
  46      left = pivot + 1;
      ... (20 more lines)
```

Round 1, where the rule appeared:

```c
  72  int main(void)
  73  {
  74      char input[128];
  75      char *end;
  76      uintmax_t value;
  77      uint64_t number;
  78      uint64_t result;
  79  
  80      if (fgets(input, sizeof input, stdin) == NULL) {
  81          fputs("Invalid input\n", stderr);
  82          return EXIT_FAILURE;
  83      }
  84  
  85      errno = 0;
  86      end = NULL;
  87      value = strtoumax(input, &end, 10);
  88  
  89      if (errno == ERANGE || end == input || value > UINT64_MAX) {
  90          fputs("Invalid input\n", stderr);
  91          return EXIT_FAILURE;
  92      }
  93  
  94      while (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n' ||
  95             *end == '\f' || *end == '\v') {
  96          ++end;
  97      }
  98  
  99      if (*end != '\0') {
 100          fputs("Invalid input\n", stderr);
 101          return EXIT_FAILURE;
 102      }
 103  
 104      number = (uint64_t)value;
 105  
 106      if (!next_bigger(number, &result)) {
 107          puts("No bigger number can be formed");
 108          return EXIT_SUCCESS;
 109      }
 110  
 111      printf("%" PRIu64 "\n", result);
      ... (2 more lines)
```

## E118  `flawfinder:strlen`  round 0 -> 1

Program `problem-747-s3` (feedback-static-r0-t1.0-p1.0); left `lcs_of_3`, entered `main`.

Round 0, where the rule was removed:

```c
  11  char* lcs_of_3(const char* X, const char* Y, const char* Z) {
  12      int m = strlen(X);
  13      int n = strlen(Y);
  14      int o = strlen(Z);
  15  
  16      int*** dp = (int***)malloc((m + 1) * sizeof(int**));
  17      if (dp == NULL) return NULL;
  18  
  19      for (int i = 0; i <= m; i++) {
  20          dp[i] = (int**)malloc((n + 1) * sizeof(int*));
  21          if (dp[i] == NULL) {
  22              for (int k = 0; k < i; k++) free(dp[k]);
  23              free(dp);
  24              return NULL;
  25          }
  26          for (int j = 0; j <= n; j++) {
  27              dp[i][j] = (int*)malloc((o + 1) * sizeof(int));
  28              if (dp[i][j] == NULL) {
  29                  for (int k = 0; k <= n; k++) {
  30                      if (k < j) free(dp[i][k]);
  31                  }
  32                  free(dp[i]);
  33                  for (int k = 0; k < i; k++) {
  34                      for (int l = 0; l <= n; l++) free(dp[k][l]);
  35                      free(dp[k]);
  36                  }
  37                  free(dp);
  38                  return NULL;
  39              }
  40          }
  41      }
  42  
  43      for (int i = 0; i <= m; i++) {
  44          for (int j = 0; j <= n; j++) {
  45              for (int k = 0; k <= o; k++) {
  46                  if (i == 0 || j == 0 || k == 0) {
  47                      dp[i][j][k] = 0;
  48                  } else if (X[i - 1] == Y[j - 1] && Y[j - 1] == Z[k - 1]) {
  49                      dp[i][j][k] = dp[i - 1][j - 1][k - 1] + 1;
  50                  } else {
      ... (43 more lines)
```

Round 1, where the rule appeared:

```c
  93  int main() {
  94      const char* X = "AGGT12";
  95      const char* Y = "GXT1X2B";
  96      const char* Z = "GTT12B";
  97  
  98      char* result = lcs_of_3(X, Y, Z, strlen(X), strlen(Y), strlen(Z));
  99      if (result != NULL) {
 100          printf("Longest Common Subsequence: %s\n", result);
 101          free(result);
 102      } else {
 103          printf("Failed to allocate memory for LCS.\n");
 104      }
 105  
 106      return 0;
 107  }
```

## E119  `flawfinder:char`  round 1 -> 2

Program `problem-639-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `sum_valid_name_lengths`.

Round 1, where the rule was removed:

```c
  24  int main(void) {
  25      char names[MAX_NAMES][NAME_LENGTH];
  26      int count = 0;
  27      
  28      printf("Enter number of names: ");
  29      if (scanf("%d", &count) != 1) {
  30          printf("Invalid input\n");
  31          return EXIT_FAILURE;
  32      }
  33      
  34      int c;
  35      while ((c = getchar()) != '\n' && c != EOF);
  36      
  37      if (count <= 0 || count > MAX_NAMES) {
  38          printf("Invalid count\n");
  39          return EXIT_FAILURE;
  40      }
  41      
  42      for (int i = 0; i < count; i++) {
  43          printf("Enter name %d: ", i + 1);
  44          if (fgets(names[i], NAME_LENGTH, stdin) == NULL) {
  45              printf("Error reading input\n");
  46              return EXIT_FAILURE;
  47          }
  48          
  49          size_t len = strnlen(names[i], NAME_LENGTH);
  50          if (len > 0 && names[i][len - 1] == '\n') {
  51              names[i][len - 1] = '\0';
  52          } else if (len == NAME_LENGTH - 1 && names[i][len - 1] != '\n') {
  53              names[i][NAME_LENGTH - 1] = '\0';
  54              int ch;
  55              while ((ch = getchar()) != '\n' && ch != EOF);
  56          }
  57      }
  58      
  59      int result = sum_valid_name_lengths(names, count);
  60      printf("Sum of lengths of valid names: %d\n", result);
  61      
  62      return EXIT_SUCCESS;
  63  }
```

Round 2, where the rule appeared:

```c
   9  int sum_valid_name_lengths(char (*names)[NAME_LENGTH], int count) {
  10      int total = 0;
  11      
  12      for (int i = 0; i < count; i++) {
  13          if (names[i][0] != '\0' && !islower((unsigned char)names[i][0])) {
  14              size_t len = strnlen(names[i], NAME_LENGTH);
  15              if (len < NAME_LENGTH) {
  16                  total += (int)len;
  17              }
  18          }
  19      }
  20      
  21      return total;
  22  }
```

## E120  `flawfinder:char`  round 2 -> 3

Program `problem-932` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `remove_duplicate_words`.

Round 2, where the rule was removed:

```c
  34  int main(void) {
  35      char *input[] = {"apple", "banana", "apple", "cherry", "banana", "date", "apple"};
  36      int input_count = sizeof(input) / sizeof(input[0]);
  37      char *output[MAX_WORDS] = {NULL};
  38      int output_max = MAX_WORDS;
  39  
  40      if (input_count > output_max) {
  41          fprintf(stderr, "Input exceeds maximum allowed size\n");
  42          return 1;
  43      }
  44  
  45      int output_count = remove_duplicate_words(input, input_count, output, output_max);
  46  
  47      for (int i = 0; i < output_count; i++) {
  48          if (output[i] != NULL) {
  49              printf("%s\n", output[i]);
  50          }
  51      }
  52  
  53      return 0;
  54  }
```

Round 3, where the rule appeared:

```c
  17  int remove_duplicate_words(char *input[], int input_count, char *output[], int output_max) {
  18      int output_count = 0;
  19      for (int i = 0; i < input_count; i++) {
  20          if (input[i] == NULL) {
  21              continue;
  22          }
  23          if (output_count >= output_max) {
  24              break;
  25          }
  26          if (!is_duplicate((const char *const *)output, output_count, input[i])) {
  27              output[output_count] = input[i];
  28              output_count++;
  29          }
  30      }
  31      return output_count;
  32  }
```

## E121  `flawfinder:strlen`  round 0 -> 1

Program `problem-30` (feedback-static+dynamic-r0-t1.0-p1.0); left `countSubstringsWithSameStartEnd`, entered `main`.

Round 0, where the rule was removed:

```c
   4  int countSubstringsWithSameStartEnd(const char *str) {
   5      int count = 0;
   6      int n = strlen(str);
   7      
   8      for (int i = 0; i < n; ++i) {
   9          for (int j = i + 1; j < n; ++j) {
  10              if (str[i] == str[j]) {
  11                  count++;
  12              }
  13          }
  14      }
  15  
  16      return count;
  17  }
```

Round 1, where the rule appeared:

```c
  16  int main() {
  17      const char *input = "abcab";
  18      size_t input_len = strlen(input);
  19      int result = countSubstringsWithSameStartEnd(input, input_len);
  20      printf("Number of substrings starting and ending with the same character: %d\n", result);
  21      return 0;
  22  }
```

## E122  `flawfinder:char`  round 3 -> 4

Program `problem-21` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_int` (new function).

Round 3, where the rule was removed:

```c
  20  int main(void) {
  21      int n, m;
  22      char *endptr;
  23      char buffer[256];
  24      long val;
  25  
  26      printf("Enter the number (n): ");
  27      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  28          fprintf(stderr, "Error reading input\n");
  29          return EXIT_FAILURE;
  30      }
  31      
  32      if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
  33          fprintf(stderr, "Input too long\n");
  34          return EXIT_FAILURE;
  35      }
  36  
  37      errno = 0;
  38      val = strtol(buffer, &endptr, 10);
  39      if (errno != 0 || endptr == buffer || *endptr != '\n' || val > INT_MAX || val < INT_MIN) {
  40          fprintf(stderr, "Invalid input for n\n");
  41          return EXIT_FAILURE;
  42      }
  43      n = (int)val;
  44  
  45      printf("Enter the number of multiples (m): ");
  46      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  47          fprintf(stderr, "Error reading input\n");
  48          return EXIT_FAILURE;
  49      }
  50      
  51      if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
  52          fprintf(stderr, "Input too long\n");
  53          return EXIT_FAILURE;
  54      }
  55  
  56      errno = 0;
  57      val = strtol(buffer, &endptr, 10);
  58      if (errno != 0 || endptr == buffer || *endptr != '\n' || val > INT_MAX || val < INT_MIN) {
  59          fprintf(stderr, "Invalid input for m\n");
      ... (13 more lines)
```

Round 4, where the rule appeared:

```c
  20  int read_int(const char *prompt, int *out) {
  21      char buffer[256];
  22      char *endptr;
  23      long val;
  24  
  25      printf("%s", prompt);
  26      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  27          fprintf(stderr, "Error reading input\n");
  28          return 0;
  29      }
  30  
  31      if (strchr(buffer, '\n') == NULL && !feof(stdin)) {
  32          fprintf(stderr, "Input too long\n");
  33          return 0;
  34      }
  35  
  36      errno = 0;
  37      val = strtol(buffer, &endptr, 10);
  38      if (errno != 0 || endptr == buffer || *endptr != '\n' || val > INT_MAX || val < INT_MIN) {
  39          fprintf(stderr, "Invalid input\n");
  40          return 0;
  41      }
  42  
  43      *out = (int)val;
  44      return 1;
  45  }
```

## E123  `flawfinder:strlen`  round 1 -> 2

Program `problem-118-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `stringToList`.

Round 1, where the rule was removed:

```c
  71  int main(void) {
  72      char buffer[1024];
  73  
  74      if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
  75          return 0;
  76      }
  77  
  78      size_t len = strlen(buffer);
  79      if (len > 0 && buffer[len - 1] == '\n') {
  80          buffer[len - 1] = '\0';
  81      }
  82  
  83      Node* list = stringToList(buffer);
  84      
  85      if (list != NULL) {
  86          printList(list);
  87          freeList(list);
  88      } else {
  89          printf("[]\n");
  90      }
  91  
  92      return 0;
  93  }
```

Round 2, where the rule appeared:

```c
  10  Node* stringToList(char* str) {
  11      if (str == NULL) {
  12          return NULL;
  13      }
  14  
  15      Node* head = NULL;
  16      Node* tail = NULL;
  17  
  18      char* token = strtok(str, " \t\n\r");
  19      while (token != NULL) {
  20          Node* newNode = (Node*)malloc(sizeof(Node));
  21          if (newNode == NULL) {
  22              Node* current = head;
  23              while (current != NULL) {
  24                  Node* next = current->next;
  25                  free(current->data);
  26                  free(current);
  27                  current = next;
  28              }
  29              return NULL;
  30          }
  31          
  32          size_t token_len = strlen(token);
  33          newNode->data = (char*)malloc(token_len + 1);
  34          if (newNode->data != NULL) {
  35              strcpy(newNode->data, token);
  36          }
  37          newNode->next = NULL;
  38  
  39          if (head == NULL) {
  40              head = newNode;
  41              tail = newNode;
  42          } else {
  43              tail->next = newNode;
  44              tail = newNode;
  45          }
  46          token = strtok(NULL, " \t\n\r");
  47      }
  48  
  49      return head;
      ... (1 more lines)
```

## E124  `flawfinder:getchar`  round 2 -> 3

Program `problem-877-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `discard_remaining_input` (new function).

Round 2, where the rule was removed:

```c
  41  int main(void)
  42  {
  43      char buffer[BUFFER_SIZE];
  44      size_t len;
  45      int ch;
  46  
  47      memset(buffer, 0, sizeof(buffer));
  48  
  49      if (fgets(buffer, (int)sizeof(buffer), stdin) == NULL) {
  50          fprintf(stderr, "Error reading input\n");
  51          return EXIT_FAILURE;
  52      }
  53  
  54      buffer[sizeof(buffer) - 1] = '\0';
  55  
  56      len = strnlen(buffer, sizeof(buffer));
  57      if (len > 0 && buffer[len - 1] == '\n') {
  58          buffer[len - 1] = '\0';
  59      } else if (len == sizeof(buffer) - 1) {
  60          do {
  61              ch = getchar();
  62          } while (ch != '\n' && ch != EOF);
  63      }
  64  
  65      if (sort_string(buffer, sizeof(buffer)) != 0) {
  66          fprintf(stderr, "Error sorting string\n");
  67          return EXIT_FAILURE;
  68      }
  69  
  70      if (printf("%s\n", buffer) < 0) {
  71          return EXIT_FAILURE;
  72      }
  73  
  74      return EXIT_SUCCESS;
  75  }
```

Round 3, where the rule appeared:

```c
  41  static void discard_remaining_input(void)
  42  {
  43      int ch;
  44  
  45      do {
  46          ch = getchar();
  47      } while (ch != '\n' && ch != EOF);
  48  }
```

## E125  `flawfinder:getchar`  round 4 -> 5

Program `problem-364-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `discard_remaining_input` (new function).

Round 4, where the rule was removed:

```c
 105  int main(void)
 106  {
 107      char buffer[MAX_INPUT_SIZE];
 108      long result;
 109      size_t len;
 110  
 111      memset(buffer, 0, sizeof(buffer));
 112  
 113      printf("Enter a binary string: ");
 114  
 115      if (fgets(buffer, (int)sizeof(buffer), stdin) == NULL)
 116      {
 117          fprintf(stderr, "Error reading input.\n");
 118          return EXIT_FAILURE;
 119      }
 120  
 121      buffer[sizeof(buffer) - 1U] = '\0';
 122  
 123      len = bounded_length(buffer, sizeof(buffer));
 124  
 125      if ((len > 0U) && (buffer[len - 1U] == '\n'))
 126      {
 127          buffer[len - 1U] = '\0';
 128      }
 129      else if (len == (sizeof(buffer) - 1U))
 130      {
 131          int ch;
 132  
 133          while (((ch = getchar()) != '\n') && (ch != EOF))
 134          {
 135          }
 136  
 137          fprintf(stderr, "Input too long: maximum length is %u characters.\n",
 138                  MAX_INPUT_SIZE - 2U);
 139          return EXIT_FAILURE;
 140      }
 141  
 142      if (is_binary_string(buffer, sizeof(buffer)) == 0)
 143      {
 144          fprintf(stderr, "Invalid input: string must be non-empty and contain only '0' and '1'.\n");
      ... (15 more lines)
```

Round 5, where the rule appeared:

```c
 105  static void discard_remaining_input(void)
 106  {
 107      int ch;
 108      size_t guard = 0U;
 109  
 110      do
 111      {
 112          ch = getchar();
 113          guard++;
 114      } while ((ch != '\n') && (ch != EOF) && (guard < (size_t)1000000U));
 115  }
```

## E126  `flawfinder:strlen`  round 1 -> 2

Program `problem-440-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `find_adverbs`, entered `main`.

Round 1, where the rule was removed:

```c
  21  int find_adverbs(const char *sentence, AdverbInfo *adverbs, size_t max_adverbs) {
  22      if (sentence == NULL || adverbs == NULL || max_adverbs == 0) {
  23          return -1;
  24      }
  25  
  26      size_t count = 0;
  27      size_t pos = 0;
  28      size_t word_start = 0;
  29      size_t word_len = 0;
  30      size_t sentence_len = strlen(sentence);
  31  
  32      while (pos <= sentence_len) {
  33          if (pos == sentence_len || isspace((unsigned char)sentence[pos])) {
  34              if (word_len > 0) {
  35                  if (word_len >= MAX_WORD_LENGTH) {
  36                      return -1;
  37                  }
  38  
  39                  if (is_adverb(sentence + word_start, word_len)) {
  40                      if (count >= max_adverbs) {
  41                          return -1;
  42                      }
  43                      memcpy(adverbs[count].word, sentence + word_start, word_len);
  44                      adverbs[count].word[word_len] = '\0';
  45                      adverbs[count].position = word_start;
  46                      count++;
  47                  }
  48              }
  49              word_start = pos + 1;
  50              word_len = 0;
  51          } else {
  52              word_len++;
  53          }
  54          pos++;
  55      }
  56  
  57      return (int)count;
  58  }
```

Round 2, where the rule appeared:

```c
  61  int main(void) {
  62      const char sentence[] = "She quickly ran to the store and carefully picked the items. He slowly walked home.";
  63      size_t sentence_len = strlen(sentence);
  64      AdverbInfo *adverbs = malloc(MAX_ADVERBS * sizeof(AdverbInfo));
  65  
  66      if (adverbs == NULL) {
  67          fprintf(stderr, "Memory allocation failed\n");
  68          return EXIT_FAILURE;
  69      }
  70  
  71      int count = find_adverbs(sentence, sentence_len, adverbs, MAX_ADVERBS);
  72  
  73      if (count < 0) {
  74          fprintf(stderr, "Error processing sentence\n");
  75          free(adverbs);
  76          return EXIT_FAILURE;
  77      }
  78  
  79      printf("Found %d adverb(s):\n", count);
  80      for (int i = 0; i < count; i++) {
  81          printf("Adverb: %s, Position: %zu\n", adverbs[i].word, adverbs[i].position);
  82      }
  83  
  84      free(adverbs);
  85      return EXIT_SUCCESS;
  86  }
```

## E127  `flawfinder:strlen`  round 2 -> 3

Program `problem-946-s3` (feedback-static-r0-t1.0-p1.0); left `copy_string`, entered `main, process_text`.

Round 2, where the rule was removed:

```c
  25  static char *copy_string(const char *s)
  26  {
  27      size_t len;
  28      char *p;
  29      if (s == NULL) {
  30          return NULL;
  31      }
  32      len = strlen(s);
  33      if (len == SIZE_MAX) {
  34          return NULL;
  35      }
  36      p = malloc(len + 1);
  37      if (p == NULL) {
  38          return NULL;
  39      }
  40      if (len > 0) {
  41          memcpy(p, s, len);
  42      }
  43      p[len] = '\0';
  44      return p;
  45  }
```

Round 3, where the rule appeared:

```c
 194  int main(void)
 195  {
 196      const char *text = "The quick brown fox jumps over the lazy dog. The dog was not amused by the fox.";
 197      WordCount *wc = NULL;
 198      size_t n = 0;
 199      int result;
 200  
 201      result = process_text(text, strlen(text), &wc, &n);
 202      if (result != 0) {
 203          fprintf(stderr, "Error: failed to process text.\n");
 204          return EXIT_FAILURE;
 205      }
 206  
 207      print_most_common(wc, n);
 208      free_word_counts(wc, n);
 209  
 210      return EXIT_SUCCESS;
 211  }
 106  static int process_text(const char *text, size_t text_len, WordCount **wc_out, size_t *n_out)
 107  {
 108      WordCount *wc = NULL;
 109      size_t n = 0;
 110      size_t cap = 0;
 111      char *text_copy = NULL;
 112      char *token = NULL;
 113      char *saveptr = NULL;
 114      const char *delim = " \t\n\r.,;:!?\"'()[]{}<>/-";
 115  
 116      if (text == NULL || wc_out == NULL || n_out == NULL) {
 117          return -1;
 118      }
 119  
 120      text_copy = copy_string(text, text_len + 1);
 121      if (text_copy == NULL) {
 122          return -1;
 123      }
 124  
 125      token = strtok_r(text_copy, delim, &saveptr);
 126      while (token != NULL) {
 127          int idx;
 128          lowercase_string(token);
 129          idx = find_word_index(wc, n, token);
 130          if (idx >= 0) {
 131              wc[idx].count++;
 132          } else {
 133              if (add_word(&wc, &n, &cap, token, strlen(token) + 1) != 0) {
 134                  free(text_copy);
 135                  free_word_counts(wc, n);
 136                  return -1;
 137              }
 138          }
 139          token = strtok_r(NULL, delim, &saveptr);
 140      }
 141  
 142      free(text_copy);
 143      *wc_out = wc;
 144      *n_out = n;
 145      return 0;
      ... (1 more lines)
```

## E128  `flawfinder:memcpy`  round 4 -> 5

Program `problem-933-s3` (feedback-static-r0-t1.0-p1.0); left `camel_to_snake`, entered `safe_copy` (new function).

Round 4, where the rule was removed:

```c
  11  char *camel_to_snake(const char *input)
  12  {
  13      regex_t regex;
  14      regmatch_t match;
  15      char *output = NULL;
  16      size_t input_len;
  17      size_t out_capacity;
  18      size_t out_len = 0;
  19      size_t offset = 0;
  20      int ret;
  21  
  22      if (input == NULL) {
  23          return NULL;
  24      }
  25  
  26      input_len = strnlen(input, MAX_INPUT_LEN);
  27      if (input_len >= MAX_INPUT_LEN) {
  28          return NULL;
  29      }
  30  
  31      out_capacity = (input_len * 2U) + 1U;
  32  
  33      output = malloc(out_capacity);
  34      if (output == NULL) {
  35          return NULL;
  36      }
  37  
  38      ret = regcomp(&regex, "[A-Z]", REG_EXTENDED);
  39      if (ret != 0) {
  40          free(output);
  41          return NULL;
  42      }
  43  
  44      while (offset < input_len) {
  45          ret = regexec(&regex, input + offset, 1, &match, 0);
  46          if (ret == REG_NOMATCH) {
  47              size_t remaining = input_len - offset;
  48              if (remaining > (out_capacity - 1U) ||
  49                  out_len > ((out_capacity - 1U) - remaining) ||
  50                  (out_len + remaining) >= out_capacity) {
      ... (69 more lines)
```

Round 5, where the rule appeared:

```c
  11  static int safe_copy(char *dest, size_t dest_capacity, size_t dest_len,
  12                       const char *src, size_t src_len)
  13  {
  14      if (dest == NULL || src == NULL) {
  15          return -1;
  16      }
  17      if (src_len > (dest_capacity - 1U) ||
  18          dest_len > ((dest_capacity - 1U) - src_len)) {
  19          return -1;
  20      }
  21      memcpy(dest + dest_len, src, src_len);
  22      return 0;
  23  }
```

## E129  `flawfinder:fgetc`  round 2 -> 3

Program `problem-643-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `read_line` (new function).

Round 2, where the rule was removed:

```c
  28  int main(void)
  29  {
  30      char word[WORD_CAPACITY];
  31  
  32      if (fgets(word, sizeof word, stdin) == NULL) {
  33          return ferror(stdin) ? 1 : 0;
  34      }
  35  
  36      size_t length = 0U;
  37      while (length < sizeof word && word[length] != '\0' &&
  38             word[length] != '\n' && word[length] != '\r') {
  39          ++length;
  40      }
  41  
  42      if (word[length] == '\r') {
  43          if (length + 1U < sizeof word &&
  44              word[length + 1U] != '\n' &&
  45              word[length + 1U] != '\0') {
  46              return 1;
  47          }
  48      } else if (word[length] == '\0') {
  49          if (length == sizeof word - 1U) {
  50              int ch = fgetc(stdin);
  51  
  52              if (ch != '\n' && ch != EOF) {
  53                  while (ch != '\n' && ch != EOF) {
  54                      ch = fgetc(stdin);
  55                  }
  56                  return 1;
  57              }
  58  
  59              if (ferror(stdin)) {
  60                  return 1;
  61              }
  62          }
  63      }
  64  
  65      word[length] = '\0';
  66  
  67      if (printf("%s\n", matches_word(word, length) ? "match" : "no match") < 0) {
      ... (5 more lines)
```

Round 3, where the rule appeared:

```c
  27  static int read_line(char **line, size_t *length)
  28  {
  29      size_t capacity = 128U;
  30      size_t used = 0U;
  31      char *buffer = malloc(capacity);
  32  
  33      if (buffer == NULL) {
  34          return -1;
  35      }
  36  
  37      for (;;) {
  38          int ch = fgetc(stdin);
  39  
  40          if (ch == EOF) {
  41              if (ferror(stdin)) {
  42                  free(buffer);
  43                  return -1;
  44              }
  45              break;
  46          }
  47  
  48          if (ch == '\n') {
  49              break;
  50          }
  51  
  52          if (ch == '\r') {
  53              int next = fgetc(stdin);
  54  
  55              if (next != '\n' && next != EOF) {
  56                  if (ungetc(next, stdin) == EOF) {
  57                      free(buffer);
  58                      return -1;
  59                  }
  60              } else if (next == EOF && ferror(stdin)) {
  61                  free(buffer);
  62                  return -1;
  63              }
  64              break;
  65          }
  66  
      ... (31 more lines)
```

## E130  `flawfinder:memcpy`  round 0 -> 1

Program `problem-301-s3` (feedback-static-r0-t1.0-p1.0); left `dict_create`, entered `dict_strdup_safe` (new function).

Round 0, where the rule was removed:

```c
  14  static Dict *dict_create(const char *key, const char *value)
  15  {
  16      Dict *d = malloc(sizeof(Dict));
  17      if (d == NULL) {
  18          return NULL;
  19      }
  20      d->key = NULL;
  21      d->value = NULL;
  22      d->children = NULL;
  23      d->child_count = 0;
  24  
  25      if (key != NULL) {
  26          size_t len = strlen(key) + 1;
  27          d->key = malloc(len);
  28          if (d->key == NULL) {
  29              free(d);
  30              return NULL;
  31          }
  32          memcpy(d->key, key, len);
  33      }
  34  
  35      if (value != NULL) {
  36          size_t len = strlen(value) + 1;
  37          d->value = malloc(len);
  38          if (d->value == NULL) {
  39              free(d->key);
  40              free(d);
  41              return NULL;
  42          }
  43          memcpy(d->value, value, len);
  44      }
  45  
  46      return d;
  47  }
```

Round 1, where the rule appeared:

```c
  16  static char *dict_strdup_safe(const char *src)
  17  {
  18      size_t len;
  19      char *dst;
  20  
  21      if (src == NULL) {
  22          return NULL;
  23      }
  24  
  25      len = strnlen(src, DICT_MAX_STR_LEN);
  26      if (len >= DICT_MAX_STR_LEN) {
  27          return NULL;
  28      }
  29  
  30      dst = malloc(len + 1);
  31      if (dst == NULL) {
  32          return NULL;
  33      }
  34  
  35      memcpy(dst, src, len);
  36      dst[len] = '\0';
  37      return dst;
  38  }
```

## E131  `flawfinder:fopen`  round 3 -> 4

Program `problem-13-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `main`, entered `open_regular_file` (new function).

Round 3, where the rule was removed:

```c
 211  int main(int argc, char const *const argv[])
 212  {
 213      FILE *fp;
 214      WordTable table;
 215      int status = EXIT_SUCCESS;
 216  
 217      if (argc != 2 || argv[1] == NULL) {
 218          fprintf(stderr, "Usage: %s <dictionary-file>\n",
 219                  (argc > 0 && argv[0] != NULL) ? argv[0] : "wordcount");
 220          return EXIT_FAILURE;
 221      }
 222  
 223      fp = fopen(argv[1], "r");
 224      if (fp == NULL) {
 225          fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
 226          return EXIT_FAILURE;
 227      }
 228  
 229      if (word_table_init(&table) != 0) {
 230          fprintf(stderr, "Error: memory allocation failed\n");
 231          if (fclose(fp) != 0) {
 232              fprintf(stderr, "Error: failed to close file\n");
 233          }
 234          return EXIT_FAILURE;
 235      }
 236  
 237      if (count_words(fp, &table) != 0) {
 238          fprintf(stderr, "Error: failed while reading words\n");
 239          status = EXIT_FAILURE;
 240      } else {
 241          if (table.size > 0) {
 242              qsort(table.entries, table.size, sizeof(WordEntry),
 243                    compare_entries);
 244          }
 245          print_top_words(&table, TOP_COUNT);
 246      }
 247  
 248      word_table_free(&table);
 249  
 250      if (fclose(fp) != 0) {
      ... (6 more lines)
```

Round 4, where the rule appeared:

```c
 210  static FILE *open_regular_file(const char *path)
 211  {
 212      FILE *fp;
 213      struct stat st;
 214      int fd;
 215  
 216      if (path == NULL) {
 217          return NULL;
 218      }
 219      fp = fopen(path, "r");
 220      if (fp == NULL) {
 221          return NULL;
 222      }
 223      fd = fileno(fp);
 224      if (fd < 0 || fstat(fd, &st) != 0 || !S_ISREG(st.st_mode)) {
 225          fclose(fp);
 226          return NULL;
 227      }
 228      return fp;
 229  }
```

## E132  `flawfinder:strlen`  round 3 -> 4

Program `problem-37-s3` (feedback-static-r0-t1.0-p1.0); left `main`, entered `duplicate_string`.

Round 3, where the rule was removed:

```c
 120  int main(int argc, char *argv[])
 121  {
 122      Item *items;
 123      size_t count;
 124      size_t initialized;
 125      size_t i;
 126  
 127      if (argc < 2 || argv == NULL) {
 128          const char *program =
 129              argc > 0 && argv != NULL && argv[0] != NULL
 130                  ? argv[0]
 131                  : "program";
 132  
 133          fprintf(stderr, "Usage: %s item...\n", program);
 134          return EXIT_FAILURE;
 135      }
 136  
 137      count = (size_t)(argc - 1);
 138  
 139      if (count > SIZE_MAX / sizeof(*items)) {
 140          fprintf(stderr, "Input is too large\n");
 141          return EXIT_FAILURE;
 142      }
 143  
 144      items = calloc(count, sizeof(*items));
 145      if (items == NULL) {
 146          perror("calloc");
 147          return EXIT_FAILURE;
 148      }
 149  
 150      initialized = 0U;
 151  
 152      for (i = 0U; i < count; ++i) {
 153          const char *argument = argv[i + 1U];
 154          size_t length;
 155          int value;
 156  
 157          if (argument == NULL) {
 158              fprintf(stderr, "Invalid null argument\n");
 159              free_items(items, initialized);
      ... (49 more lines)
```

Round 4, where the rule appeared:

```c
  21  static char *duplicate_string(const char *source)
  22  {
  23      size_t length;
  24      char *copy;
  25  
  26      if (source == NULL) {
  27          return NULL;
  28      }
  29  
  30      length = strlen(source);
  31  
  32      if (length == SIZE_MAX) {
  33          return NULL;
  34      }
  35  
  36      copy = malloc(length + 1U);
  37      if (copy == NULL) {
  38          return NULL;
  39      }
  40  
  41      if (length > 0U) {
  42          memcpy(copy, source, length);
  43      }
  44  
  45      copy[length] = '\0';
  46      return copy;
  47  }
```

## E133  `flawfinder:strlen`  round 0 -> 1

Program `problem-684-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_char_in_repeated_string`, entered `main`.

Round 0, where the rule was removed:

```c
   6  size_t count_char_in_repeated_string(const char *str, char c, size_t n) {
   7      if (str == NULL) {
   8          return 0;
   9      }
  10  
  11      size_t len = strlen(str);
  12  
  13      if (len == 0) {
  14          return 0;
  15      }
  16  
  17      size_t count_single = 0;
  18      for (size_t i = 0; i < len; i++) {
  19          if (str[i] == c) {
  20              count_single++;
  21          }
  22      }
  23  
  24      if (n <= (SIZE_MAX / len)) {
  25          size_t full_repeats = n / len;
  26          size_t remainder = n % len;
  27  
  28          size_t total_count = count_single * full_repeats;
  29  
  30          for (size_t i = 0; i < remainder; i++) {
  31              if (str[i] == c) {
  32                  total_count++;
  33              }
  34          }
  35  
  36          return total_count;
  37      } else {
  38          size_t i = 0;
  39          size_t total_count = 0;
  40          while (i < n) {
  41              for (size_t j = 0; j < len && i < n; j++) {
  42                  if (str[j] == c) {
  43                      total_count++;
  44                  }
  45                  i++;
      ... (5 more lines)
```

Round 1, where the rule appeared:

```c
  50  int main(void) {
  51      const char *str = "abc";
  52      char c = 'a';
  53      size_t n = 10;
  54      size_t len = strlen(str);
  55  
  56      size_t count = count_char_in_repeated_string(str, len, c, n);
  57      printf("%zu\n", count);
  58  
  59      return 0;
  60  }
```

## E134  `flawfinder:strlen`  round 0 -> 1

Program `problem-757-s3` (feedback-static-r0-t1.0-p1.0); left `reverse_string`, entered `count_reverse_pairs`.

Round 0, where the rule was removed:

```c
   5  char *reverse_string(const char *str) {
   6      if (str == NULL) {
   7          return NULL;
   8      }
   9      size_t len = strlen(str);
  10      char *rev = (char *)malloc(len + 1);
  11      if (rev == NULL) {
  12          return NULL;
  13      }
  14      for (size_t i = 0; i < len; i++) {
  15          rev[i] = str[len - 1 - i];
  16      }
  17      rev[len] = '\0';
  18      return rev;
  19  }
```

Round 1, where the rule appeared:

```c
  23  int count_reverse_pairs(char **strings, int count) {
  24      if (strings == NULL || count <= 0) {
  25          return 0;
  26      }
  27      int pair_count = 0;
  28      for (int i = 0; i < count; i++) {
  29          if (strings[i] != NULL) {
  30              size_t len = strlen(strings[i]);
  31              char *rev = reverse_string(strings[i], len);
  32              if (rev != NULL) {
  33                  for (int j = i + 1; j < count; j++) {
  34                      if (strings[j] != NULL && strcmp(strings[i], strings[j]) != 0 && strcmp(rev, strings[j]) == 0) {
  35                          pair_count++;
  36                      }
  37                  }
  38                  free(rev);
  39              }
  40          }
  41      }
  42      return pair_count;
  43  }
```

## E135  `flawfinder:strlen`  round 4 -> 5

Program `problem-202-s2` (feedback-static-r0-t1.0-p1.0); left `remove_even_characters`, entered `main`.

Round 4, where the rule was removed:

```c
   4  void remove_even_characters(char *str, size_t len) {
   5      char result[len];
   6      int j = 0;
   7      for (int i = 0; i < len && str[i] != '\0'; i++) {
   8          if ((i + 1) % 2 != 0) {
   9              if (j < (len - 1)) {
  10                  result[j++] = str[i];
  11              }
  12          }
  13      }
  14      result[j] = '\0';
  15      memmove(str, result, strlen(result) + 1);
  16  }
```

Round 5, where the rule appeared:

```c
  14  int main() {
  15      char str[50];
  16      printf("Enter a string: ");
  17      if (fgets(str, sizeof(str), stdin) != NULL) {
  18          str[strcspn(str, "\n")] = '\0';
  19          size_t len = strlen(str);
  20          remove_even_characters(str, len);
  21          printf("String after removing even characters: %s\n", str);
  22      }
  23      return 0;
  24  }
```

## E136  `flawfinder:char`  round 2 -> 3

Program `problem-928-s2` (feedback-static-r0-t1.0-p1.0); left `convert_date_format, main`, entered `parse_date_component` (new function).

Round 2, where the rule was removed:

```c
  11  int convert_date_format(const char *input, char *output, size_t output_size) {
  12      if (input == NULL || output == NULL) {
  13          return -1;
  14      }
  15      
  16      size_t len = strnlen(input, DATE_LENGTH);
  17      if (len != 10) {
  18          return -1;
  19      }
  20      
  21      if (input[4] != '-' || input[7] != '-') {
  22          return -1;
  23      }
  24      
  25      for (int i = 0; i < 10; i++) {
  26          if (i == 4 || i == 7) {
  27              continue;
  28          }
  29          if (input[i] < '0' || input[i] > '9') {
  30              return -1;
  31          }
  32      }
  33      
  34      if (output_size < DATE_LENGTH) {
  35          return -1;
  36      }
  37      
  38      char year_str[YEAR_LEN + 1];
  39      char month_str[MONTH_LEN + 1];
  40      char day_str[DAY_LEN + 1];
  41      
  42      memset(year_str, 0, sizeof(year_str));
  43      memset(month_str, 0, sizeof(month_str));
  44      memset(day_str, 0, sizeof(day_str));
  45      
  46      memcpy(year_str, input, YEAR_LEN);
  47      year_str[YEAR_LEN] = '\0';
  48      memcpy(month_str, input + 5, MONTH_LEN);
  49      month_str[MONTH_LEN] = '\0';
  50      memcpy(day_str, input + 8, DAY_LEN);
      ... (35 more lines)
  87  int main(void) {
  88      char input_date[DATE_LENGTH * 2];
  89      char output_date[DATE_LENGTH];
  90      
  91      printf("Enter date (yyyy-mm-dd): ");
  92      if (fgets(input_date, sizeof(input_date), stdin) == NULL) {
  93          fprintf(stderr, "Error reading input\n");
  94          return 1;
  95      }
  96      
  97      input_date[strcspn(input_date, "\n")] = '\0';
  98      
  99      if (convert_date_format(input_date, output_date, sizeof(output_date)) != 0) {
 100          fprintf(stderr, "Invalid date format\n");
 101          return 1;
 102      }
 103      
 104      printf("Converted date: %s\n", output_date);
 105      
 106      return 0;
 107  }
```

Round 3, where the rule appeared:

```c
  39  static int parse_date_component(const char *str, size_t len, long *value) {
  40      char buffer[5] = {0};
  41      
  42      if (len >= sizeof(buffer)) {
  43          return -1;
  44      }
  45      
  46      memcpy(buffer, str, len);
  47      buffer[len] = '\0';
  48      
  49      char *endptr;
  50      errno = 0;
  51      *value = strtol(buffer, &endptr, 10);
  52      
  53      if (errno != 0 || *endptr != '\0') {
  54          return -1;
  55      }
  56      
  57      return 0;
  58  }
```

## E137  `flawfinder:strlen`  round 2 -> 3

Program `problem-684` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_char_in_repeated_string`, entered `main`.

Round 2, where the rule was removed:

```c
   4  int count_char_in_repeated_string(const char *str, int repeat_count, char c) {
   5      size_t len = 0;
   6      if (str != NULL) {
   7          len = strlen(str);
   8      }
   9      if (len == 0 || repeat_count <= 0) {
  10          return 0;
  11      }
  12      int total_length = (int)(len * repeat_count);
  13      int count = 0;
  14  
  15      for (int i = 0; i < total_length; ++i) {
  16          if (str[i % len] == c) {
  17              ++count;
  18          }
  19      }
  20  
  21      return count;
  22  }
```

Round 3, where the rule appeared:

```c
  20  int main() {
  21      const char *input_str = "hello";
  22      int repeat_times = 3;
  23      char target_char = 'l';
  24      size_t input_len = strlen(input_str);
  25  
  26      int result = count_char_in_repeated_string(input_str, repeat_times, target_char, input_len);
  27      printf("Count of '%c' in the repeated string: %d\n", target_char, result);
  28  
  29      return 0;
  30  }
```

## E138  `flawfinder:strlen`  round 0 -> 1

Program `problem-30-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `count_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  int count_substrings(const char *str) {
   6      if (str == NULL) {
   7          return 0;
   8      }
   9      size_t len = strlen(str);
  10      if (len == 0) {
  11          return 0;
  12      }
  13      int count = 0;
  14      for (size_t i = 0; i < len; i++) {
  15          for (size_t j = i; j < len; j++) {
  16              if (str[i] == str[j]) {
  17                  count++;
  18              }
  19          }
  20      }
  21      return count;
  22  }
```

Round 1, where the rule appeared:

```c
  20  int main(int argc, char * const argv[]) {
  21      if (argc != 2) {
  22          fprintf(stderr, "Usage: %s <string>\n", argv[0]);
  23          return EXIT_FAILURE;
  24      }
  25      size_t len = strlen(argv[1]);
  26      int result = count_substrings(argv[1], len);
  27      printf("%d\n", result);
  28      return EXIT_SUCCESS;
  29  }
```

## E139  `flawfinder:strlen`  round 0 -> 1

Program `problem-238-s2` (feedback-static-r0-t1.0-p1.0); left `count_non_empty_substrings`, entered `main`.

Round 0, where the rule was removed:

```c
   5  size_t count_non_empty_substrings(const char *str) {
   6      size_t len = strlen(str);
   7      return len * (len + 1) / 2;
   8  }
```

Round 1, where the rule appeared:

```c
   9  int main() {
  10      const char *test_str = "abc";
  11      size_t len = strlen(test_str);
  12      size_t count = count_non_empty_substrings(test_str, len);
  13      printf("Number of non-empty substrings: %zu\n", count);
  14      return 0;
  15  }
```

## E140  `flawfinder:strlen`  round 1 -> 2

Program `problem-897` (feedback-static-r0-t1.0-p1.0); left `is_word_in_sentence`, entered `safe_is_word_in_sentence` (new function).

Round 1, where the rule was removed:

```c
   6  bool is_word_in_sentence(const char *sentence, const char *word) {
   7      size_t sentence_len = strlen(sentence);
   8      size_t word_len = strlen(word);
   9      if (word_len > sentence_len) return false;
  10  
  11      for (size_t i = 0; i <= sentence_len - word_len; i++) {
  12          if (strncmp(sentence + i, word, word_len) == 0) {
  13              if (i == 0 || !isalpha((unsigned char)sentence[i - 1])) {
  14                  if (i + word_len == sentence_len || !isalpha((unsigned char)sentence[i + word_len])) {
  15                      return true;
  16                  }
  17              }
  18          }
  19      }
  20      return false;
  21  }
```

Round 2, where the rule appeared:

```c
  20  bool safe_is_word_in_sentence(const char *sentence, const char *word) {
  21      size_t sentence_len = sentence ? strlen(sentence) : 0;
  22      size_t word_len = word ? strlen(word) : 0;
  23      return is_word_in_sentence(sentence, sentence_len, word, word_len);
  24  }
```

## E141  `flawfinder:memcpy`  round 1 -> 2

Program `problem-140-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `add_to_singles, create_tuple, extract_single_occurrences`, entered `safe_memcpy` (new function).

Round 1, where the rule was removed:

```c
  52  static int add_to_singles(SingleElements *singles, const Tuple *tuple) {
  53      if (singles->count >= singles->capacity) {
  54          size_t new_capacity = singles->capacity == 0 ? 8 : singles->capacity * 2;
  55          if (new_capacity > SIZE_MAX / sizeof(Tuple)) {
  56              return -1;
  57          }
  58          Tuple *new_elements = realloc(singles->elements, new_capacity * sizeof(Tuple));
  59          if (new_elements == NULL) {
  60              return -1;
  61          }
  62          singles->elements = new_elements;
  63          singles->capacity = new_capacity;
  64      }
  65      
  66      if (tuple->size > 0 && tuple->size > SIZE_MAX / sizeof(int)) {
  67          return -1;
  68      }
  69      
  70      singles->elements[singles->count].data = malloc(tuple->size * sizeof(int));
  71      if (singles->elements[singles->count].data == NULL && tuple->size > 0) {
  72          return -1;
  73      }
  74      
  75      if (tuple->size > 0) {
  76          memcpy(singles->elements[singles->count].data, tuple->data, tuple->size * sizeof(int));
  77      }
  78      singles->elements[singles->count].size = tuple->size;
  79      singles->count++;
  80      
  81      return 0;
  82  }
 158  int create_tuple(Tuple *tuple, const int *data, size_t size) {
 159      if (tuple == NULL || (data == NULL && size > 0)) {
 160          return -1;
 161      }
 162      
 163      if (size > 0 && size > SIZE_MAX / sizeof(int)) {
 164          return -1;
 165      }
 166      
 167      tuple->data = malloc(size * sizeof(int));
 168      if (tuple->data == NULL && size > 0) {
 169          return -1;
 170      }
 171      
 172      if (size > 0) {
 173          memcpy(tuple->data, data, size * sizeof(int));
 174      }
 175      tuple->size = size;
 176      
 177      return 0;
 178  }
  84  SingleElements extract_single_occurrences(const TupleList *list) {
  85      SingleElements result = {NULL, 0, 0};
  86      
  87      if (list == NULL || list->tuples == NULL || list->count == 0) {
  88          return result;
  89      }
  90      
  91      if (list->count > SIZE_MAX / sizeof(Tuple)) {
  92          return result;
  93      }
  94      
  95      Tuple *sorted = malloc(list->count * sizeof(Tuple));
  96      if (sorted == NULL) {
  97          return result;
  98      }
  99      
 100      memcpy(sorted, list->tuples, list->count * sizeof(Tuple));
 101      qsort(sorted, list->count, sizeof(Tuple), compare_tuples);
 102      
 103      size_t i = 0;
 104      while (i < list->count) {
 105          size_t j = i;
 106          while (j < list->count && tuple_equals(&sorted[i], &sorted[j])) {
 107              j++;
 108          }
 109          
 110          if (j - i == 1) {
 111              if (add_to_singles(&result, &sorted[i]) != 0) {
 112                  for (size_t k = 0; k < result.count; k++) {
 113                      free(result.elements[k].data);
 114                  }
 115                  free(result.elements);
 116                  free(sorted);
 117                  result.elements = NULL;
 118                  result.count = 0;
 119                  result.capacity = 0;
 120                  return result;
 121              }
 122          }
 123          
      ... (6 more lines)
```

Round 2, where the rule appeared:

```c
  54  static int safe_memcpy(void *dest, size_t dest_size, const void *src, size_t src_size) {
  55      if (dest == NULL || src == NULL) {
  56          return -1;
  57      }
  58      
  59      if (src_size > dest_size) {
  60          return -1;
  61      }
  62      
  63      memcpy(dest, src, src_size);
  64      return 0;
  65  }
```

## E142  `flawfinder:strlen`  round 1 -> 2

Program `problem-500-s3` (feedback-static-r0-t1.0-p1.0); left `concatenate_list`, entered `main`.

Round 1, where the rule was removed:

```c
  26  char* concatenate_list(struct Node *head) {
  27      if (head == NULL) {
  28          char *empty_str = malloc(1);
  29          if (empty_str != NULL) {
  30              empty_str[0] = '\0';
  31          }
  32          return empty_str;
  33      }
  34  
  35      size_t total_length = 0;
  36      struct Node *current = head;
  37      while (current != NULL) {
  38          if (current->data != NULL) {
  39              total_length += strlen(current->data);
  40          }
  41          current = current->next;
  42      }
  43  
  44      char *result = malloc(total_length + 1);
  45      if (result == NULL) {
  46          return NULL;
  47      }
  48  
  49      char *dest = result;
  50      current = head;
  51      while (current != NULL) {
  52          if (current->data != NULL) {
  53              size_t len = strlen(current->data);
  54              memcpy(dest, current->data, len);
  55              dest += len;
  56          }
  57          current = current->next;
  58      }
  59      *dest = '\0';
  60  
  61      return result;
  62  }
```

Round 2, where the rule appeared:

```c
  69  int main(void) {
  70      struct Node *head = malloc(sizeof(struct Node));
  71      if (head == NULL) {
  72          return 1;
  73      }
  74      
  75      struct Node *second = malloc(sizeof(struct Node));
  76      if (second == NULL) {
  77          free(head);
  78          return 1;
  79      }
  80  
  81      struct Node *third = malloc(sizeof(struct Node));
  82      if (third == NULL) {
  83          free(head);
  84          free(second);
  85          return 1;
  86      }
  87  
  88      head->data = strdup("Hello, ");
  89      head->data_len = head->data ? strlen(head->data) : 0;
  90      head->next = second;
  91  
  92      second->data = strdup("world");
  93      second->data_len = second->data ? strlen(second->data) : 0;
  94      second->next = third;
  95  
  96      third->data = strdup("!");
  97      third->data_len = third->data ? strlen(third->data) : 0;
  98      third->next = NULL;
  99  
 100      if (head->data == NULL || second->data == NULL || third->data == NULL) {
 101          free_list(head);
 102          return 1;
 103      }
 104  
 105      char *concatenated = concatenate_list(head);
 106      if (concatenated != NULL) {
 107          printf("%s\n", concatenated);
 108          free(concatenated);
      ... (6 more lines)
```

## E143  `flawfinder:strlen`  round 1 -> 2

Program `problem-285-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `match_pattern`, entered `main`.

Round 1, where the rule was removed:

```c
   4  int match_pattern(const char *str) {
   5      if (str == NULL) {
   6          return 0;
   7      }
   8      
   9      size_t len = strlen(str);
  10      size_t i = 0;
  11      
  12      while (i < len) {
  13          if (str[i] == 'a') {
  14              size_t b_count = 0;
  15              size_t j = i + 1;
  16              
  17              while (j < len && str[j] == 'b') {
  18                  b_count++;
  19                  j++;
  20              }
  21              
  22              if (b_count >= 2 && b_count <= 3) {
  23                  return 1;
  24              }
  25          }
  26          i++;
  27      }
  28      
  29      return 0;
  30  }
```

Round 2, where the rule appeared:

```c
  40  int main(void) {
  41      const char *test1 = "abb";
  42      const char *test2 = "abbb";
  43      const char *test3 = "ab";
  44      const char *test4 = "abbbb";
  45      const char *test5 = "xyzabbxyz";
  46      
  47      printf("test1: %d\n", match_pattern(test1, strlen(test1)));
  48      printf("test2: %d\n", match_pattern(test2, strlen(test2)));
  49      printf("test3: %d\n", match_pattern(test3, strlen(test3)));
  50      printf("test4: %d\n", match_pattern(test4, strlen(test4)));
  51      printf("test5: %d\n", match_pattern(test5, strlen(test5)));
  52      
  53      return 0;
  54  }
```

## E144  `flawfinder:strlen`  round 0 -> 1

Program `problem-480` (feedback-static+dynamic-r0-t1.0-p1.0); left `find_max_occurring_char`, entered `main`.

Round 0, where the rule was removed:

```c
   4  char find_max_occurring_char(const char *str) {
   5      if (str == NULL || strlen(str) == 0) {
   6          return '\0';
   7      }
   8  
   9      int count[256] = {0};
  10      size_t len = strlen(str);
  11  
  12      for (size_t i = 0; i < len; i++) {
  13          unsigned char c = str[i];
  14          count[c]++;
  15      }
  16  
  17      int max_count = -1;
  18      char max_char = '\0';
  19  
  20      for (size_t i = 0; i < len; i++) {
  21          unsigned char c = str[i];
  22          if (count[c] > max_count) {
  23              max_count = count[c];
  24              max_char = str[i];
  25          }
  26      }
  27  
  28      return max_char;
  29  }
```

Round 1, where the rule appeared:

```c
  30  int main(void) {
  31      const char *str = "experience";
  32      size_t len = strlen(str);
  33      char result = find_max_occurring_char(str, len);
  34  
  35      if (result != '\0') {
  36          printf("Maximum occurring character: %c\n", result);
  37      } else {
  38          printf("String is empty or NULL.\n");
  39      }
  40  
  41      return 0;
  42  }
```

## E145  `flawfinder:strlen`  round 1 -> 2

Program `problem-693-s3` (feedback-static+dynamic-r0-t1.0-p1.0); left `remove_multiple_spaces`, entered `main`.

Round 1, where the rule was removed:

```c
   6  int remove_multiple_spaces(char **str) {
   7      if (str == NULL || *str == NULL) {
   8          return -1;
   9      }
  10  
  11      regex_t regex;
  12      int reti;
  13  
  14      reti = regcomp(&regex, "  +", REG_EXTENDED);
  15      if (reti) {
  16          return -1;
  17      }
  18  
  19      regmatch_t pmatch[1];
  20      char *current = *str;
  21      
  22      size_t new_len = strlen(current);
  23      char *new_str = malloc(new_len + 1);
  24  
  25      if (new_str == NULL) {
  26          regfree(&regex);
  27          return -1;
  28      }
  29  
  30      char *dest = new_str;
  31  
  32      while (regexec(&regex, current, 1, pmatch, 0) == 0) {
  33          size_t copy_len = pmatch[0].rm_so;
  34          
  35          memcpy(dest, current, copy_len);
  36          dest += copy_len;
  37          
  38          *dest = ' ';
  39          dest++;
  40          
  41          current += pmatch[0].rm_eo;
  42      }
  43  
  44      strcpy(dest, current);
  45  
      ... (6 more lines)
```

Round 2, where the rule appeared:

```c
  44  int main(void) {
  45      const char *initial = "This   is  a    test string   with multiple     spaces.";
  46      char *text = malloc(strlen(initial) + 1);
  47      if (text == NULL) {
  48          return 1;
  49      }
  50      
  51      size_t k;
  52      for (k = 0; initial[k] != '\0'; k++) {
  53          text[k] = initial[k];
  54      }
  55      text[k] = '\0';
  56  
  57      printf("Original: '%s'\n", text);
  58  
  59      if (remove_multiple_spaces(&text) == 0) {
  60          printf("Modified: '%s'\n", text);
  61      } else {
  62          printf("Error processing string.\n");
  63      }
  64  
  65      free(text);
  66      return 0;
  67  }
```

## E146  `flawfinder:strlen`  round 0 -> 1

Program `problem-640` (feedback-static-r0-t1.0-p1.0); left `remove_parenthesis`, entered `main`.

Round 0, where the rule was removed:

```c
   5  char *remove_parenthesis(const char *str) {
   6      if (str == NULL) {
   7          return NULL;
   8      }
   9  
  10      size_t len = strlen(str);
  11      char *result = (char *)malloc(len + 1);
  12      if (result == NULL) {
  13          return NULL;
  14      }
  15  
  16      size_t j = 0;
  17      int depth = 0;
  18  
  19      for (size_t i = 0; i < len; i++) {
  20          if (str[i] == '(') {
  21              depth++;
  22          } else if (str[i] == ')') {
  23              if (depth > 0) {
  24                  depth--;
  25              } else {
  26                  result[j++] = str[i];
  27              }
  28          } else {
  29              if (depth == 0) {
  30                  result[j++] = str[i];
  31              }
  32          }
  33      }
  34  
  35      result[j] = '\0';
  36  
  37      char *shrunk = (char *)realloc(result, j + 1);
  38      if (shrunk != NULL) {
  39          result = shrunk;
  40      }
  41  
  42      return result;
  43  }
```

Round 1, where the rule appeared:

```c
  44  int main(void) {
  45      const char *input1 = "Hello (World) this (is a) test!";
  46      char *output1 = remove_parenthesis(input1, strlen(input1));
  47  
  48      if (output1 != NULL) {
  49          printf("%s\n", output1);
  50          free(output1);
  51      }
  52  
  53      const char *input2 = "(Nested (Parentheses)) Outside";
  54      char *output2 = remove_parenthesis(input2, strlen(input2));
  55  
  56      if (output2 != NULL) {
  57          printf("%s\n", output2);
  58          free(output2);
  59      }
  60  
  61      return 0;
  62  }
```

## E147  `flawfinder:strlen`  round 0 -> 1

Program `problem-580-s2` (feedback-static-r0-t1.0-p1.0); left `element_create_string`, entered `main`.

Round 0, where the rule was removed:

```c
  64  Element *element_create_string(const char *str) {
  65      if (str == NULL) {
  66          return NULL;
  67      }
  68      Element *e = malloc(sizeof(Element));
  69      if (e == NULL) {
  70          return NULL;
  71      }
  72      e->type = TYPE_STRING;
  73      e->data.str_val = malloc(strlen(str) + 1);
  74      if (e->data.str_val == NULL) {
  75          free(e);
  76          return NULL;
  77      }
  78      strcpy(e->data.str_val, str);
  79      return e;
  80  }
```

Round 1, where the rule appeared:

```c
 209  int main(void) {
 210      Tuple *outer = tuple_create(5);
 211      if (outer == NULL) {
 212          fprintf(stderr, "Failed to create outer tuple\n");
 213          return EXIT_FAILURE;
 214      }
 215  
 216      Tuple *inner1 = tuple_create(3);
 217      if (inner1 == NULL) {
 218          fprintf(stderr, "Failed to create inner tuple 1\n");
 219          tuple_free(outer);
 220          return EXIT_FAILURE;
 221      }
 222  
 223      Tuple *inner2 = tuple_create(3);
 224      if (inner2 == NULL) {
 225          fprintf(stderr, "Failed to create inner tuple 2\n");
 226          tuple_free(inner1);
 227          tuple_free(outer);
 228          return EXIT_FAILURE;
 229      }
 230  
 231      if (tuple_add_element(inner1, element_create_int(1)) != 0 ||
 232          tuple_add_element(inner1, element_create_int(2)) != 0 ||
 233          tuple_add_element(inner1, element_create_int(3)) != 0) {
 234          fprintf(stderr, "Failed to add elements to inner tuple 1\n");
 235          tuple_free(inner2);
 236          tuple_free(inner1);
 237          tuple_free(outer);
 238          return EXIT_FAILURE;
 239      }
 240  
 241      if (tuple_add_element(inner2, element_create_int(4)) != 0 ||
 242          tuple_add_element(inner2, element_create_string("hello", strlen("hello"))) != 0 ||
 243          tuple_add_element(inner2, element_create_int(6)) != 0) {
 244          fprintf(stderr, "Failed to add elements to inner tuple 2\n");
 245          tuple_free(inner2);
 246          tuple_free(inner1);
 247          tuple_free(outer);
 248          return EXIT_FAILURE;
      ... (33 more lines)
```

## E148  `flawfinder:char`  round 0 -> 1

Program `problem-456-s2` (feedback-static-r0-t1.0-p1.0); left `main`, entered `reverse_strings`.

Round 0, where the rule was removed:

```c
  25  int main(void) {
  26      char strings[][50] = {
  27          "hello",
  28          "world",
  29          "example",
  30          "programming"
  31      };
  32      size_t count = sizeof(strings) / sizeof(strings[0]);
  33      char *list[count];
  34      
  35      for (size_t i = 0; i < count; i++) {
  36          list[i] = strings[i];
  37      }
  38      
  39      reverse_strings(list, count);
  40      
  41      for (size_t i = 0; i < count; i++) {
  42          printf("%s\n", list[i]);
  43      }
  44      
  45      return 0;
  46  }
```

Round 1, where the rule appeared:

```c
  25  void reverse_strings(char (*list)[50], size_t count) {
  26      if (list == NULL || count == 0) {
  27          return;
  28      }
  29      for (size_t i = 0; i < count; i++) {
  30          reverse_string(list[i], 50);
  31      }
  32  }
```

## E149  `flawfinder:memcpy`  round 0 -> 1

Program `problem-368-s2` (feedback-static+dynamic-r0-t1.0-p1.0); left `repeat_tuple`, entered `main`.

Round 0, where the rule was removed:

```c
  10  Tuple repeat_tuple(const Tuple *tuple, size_t n) {
  11      Tuple result;
  12      result.size = tuple->size * n;
  13      result.elements = (int *)malloc(result.size * sizeof(int));
  14      
  15      if (result.elements == NULL) {
  16          result.size = 0;
  17          return result;
  18      }
  19      
  20      for (size_t i = 0; i < n; i++) {
  21          memcpy(result.elements + (i * tuple->size), 
  22                 tuple->elements, 
  23                 tuple->size * sizeof(int));
  24      }
  25      
  26      return result;
  27  }
```

Round 1, where the rule appeared:

```c
  54  int main(void) {
  55      int arr[] = {1, 2, 3};
  56      int *heap_arr = (int *)malloc(3 * sizeof(int));
  57      if (heap_arr == NULL) {
  58          fprintf(stderr, "Memory allocation failed\n");
  59          return 1;
  60      }
  61      memcpy(heap_arr, arr, 3 * sizeof(int));
  62      
  63      Tuple original;
  64      original.elements = heap_arr;
  65      original.size = 3;
  66      
  67      size_t n = 4;
  68      Tuple repeated = repeat_tuple(&original, n);
  69      
  70      if (repeated.elements == NULL) {
  71          fprintf(stderr, "Memory allocation failed\n");
  72          free_tuple(&original);
  73          return 1;
  74      }
  75      
  76      printf("Repeated tuple: ");
  77      for (size_t i = 0; i < repeated.size; i++) {
  78          printf("%d ", repeated.elements[i]);
  79      }
  80      printf("\n");
  81      
  82      free_tuple(&repeated);
  83      free_tuple(&original);
  84      
  85      return 0;
  86  }
```

## E150  `flawfinder:strlen`  round 0 -> 1

Program `problem-864-s2` (feedback-static-r0-t1.0-p1.0); left `is_palindrome`, entered `main`.

Round 0, where the rule was removed:

```c
   6  bool is_palindrome(const char *str) {
   7      if (str == NULL) {
   8          return false;
   9      }
  10      size_t len = strlen(str);
  11      if (len == 0) {
  12          return true;
  13      }
  14      size_t left = 0;
  15      size_t right = len - 1;
  16      while (left < right) {
  17          if (str[left] != str[right]) {
  18              return false;
  19          }
  20          left++;
  21          right--;
  22      }
  23      return true;
  24  }
```

Round 1, where the rule appeared:

```c
  53  int main(void) {
  54      char *strings[] = {"radar", "hello", "level", "world", "madam", "c", "", "noon"};
  55      size_t count = sizeof(strings) / sizeof(strings[0]);
  56      size_t lengths[count];
  57      for (size_t i = 0; i < count; i++) {
  58          lengths[i] = strlen(strings[i]);
  59      }
  60      size_t result_count = 0;
  61      char **palindromes = find_palindromes(strings, lengths, count, &result_count);
  62      if (palindromes != NULL) {
  63          for (size_t i = 0; i < result_count; i++) {
  64              printf("%s\n", palindromes[i]);
  65          }
  66          free(palindromes);
  67      }
  68      return 0;
  69  }
```

