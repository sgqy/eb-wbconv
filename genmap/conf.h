
#ifndef CONF_H
#define CONF_H

#define FIND_SUCCESS   0
#define FIND_FALLBACK -1

#define FILE_OPEN_ERR 10
#define MEMORY_ERR    11

#define DATA_INPUT_ERR 8
#define UCS2_INPUT_ERR 9

#define ENABLE_GBK_CONV  1
#define DISABLE_GBK_CONV 0

// Safe malloc
#define mem_s(x) (x+128)


extern void wcpc(const int scp, const void* src, const int srcl,
    const int tcp, void* tag, int& tagl,
    const char dft, bool& islost);

#endif
