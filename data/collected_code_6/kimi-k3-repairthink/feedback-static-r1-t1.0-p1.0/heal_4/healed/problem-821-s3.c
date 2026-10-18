if (src_len >= dst_size) {
    free(copy);
    return NULL;
}
memcpy(copy, str, src_len);