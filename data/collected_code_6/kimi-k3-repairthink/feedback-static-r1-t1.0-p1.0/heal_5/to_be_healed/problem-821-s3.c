/* Possible weaknesses found:
 *  syntax error: keyword 'if' is not allowed in global scope [syntaxError]
 *  expected identifier or '('
 *  expected identifier or '(' before 'if'
 */
if (src_len >= dst_size) {
    free(copy);
    return NULL;
}
/* Possible weaknesses found:
 *  type specifier missing, defaults to 'int'; ISO C99 and later do not support implicit int [-Wimplicit-int]
 *  type defaults to 'int' in declaration of 'memcpy' [-Wimplicit-int]
 *  parameter names (without types) in function declaration [-Wdeclaration-missing-parameter-type]
 * Flawfinder memcpy: Does not check for buffer overflows when copying to destination (CWE-120). Make sure destination can always hold the source data. (risk 2, buffer)
 *  a parameter list without types is only allowed in a function definition
 */
memcpy(copy, str, src_len);