#include "file_io.h"

#include <setjmp.h>
#include <stdlib.h>

DATA_FLAG DataCheck(const char* dir) {
    FILE* current_file;
    int flag;
    current_file = fopen(dir, "r");
    if (current_file == NULL) /*file non exists*/
        return DATA_NON_EXIST;
    else {
        flag = fgetc(current_file);
        fclose(current_file);
        if (flag == EOF) /*file empty*/
            return DATA_NON_EXIST;
        else /*file exists*/
            return DATA_PASSED;
    }
}

DATA_FLAG DataBlockParseStream(FILE* _file, char** dst, int item_size) {
    char ch;
    int item_cnt, char_cnt;
    /*data format:  [""""...""], <> is a block, ""is an item*/
    if ((ch = fgetc(_file)) == '<') { /*start of a block*/
        item_cnt = 0;
        while (item_cnt < item_size)
            if (!fscanf(_file, "%*1[\"]%[^\"<>]", dst[item_cnt]))
                break;
            else
                item_cnt++;
        if ((ch = fgetc(_file)) == '>' &&
            item_cnt == item_size) { /*end of a block*/
            return DATA_PASSED;
        } else
            return DATA_BROKEN;
    } else if (ch == EOF) /*file reaches end*/
        return DATA_EOF;
    else
        return DATA_BROKEN;
}

DATA_FLAG DataBlockAppendStream(FILE* _file, char** src, int item_size) {
    int item_cnt;
    if (fputc('<', _file) == EOF) return DATA_BROKEN;
    item_cnt = 0;
    while (item_cnt < item_size) {
        fprintf(_file, "\"%s", src[item_cnt++]);
    }
    if (fputc('>', _file) == EOF) return DATA_BROKEN;
    return DATA_PASSED;
}

DATA_FLAG DataBlockParse(const char* dir, const fpos_t begin, const fpos_t end,
                         char** dst, const int item_size) {
    FILE* current_file;
    char ch;
    int item_cnt, char_cnt, item_flag;
    current_file = fopen(dir, "r");
    if(current_file == NULL) return DATA_UNOPENED;
    fsetpos(current_file, &begin);
    /*data format:  [""""...""], <> is a block, ""is an item*/
    if ((ch = fgetc(current_file)) == '<') {
        item_cnt = 0;
        while (item_cnt < item_size)
            if (!fscanf(current_file, "%*1[\"]%[^\"<>]", dst[item_cnt]))
                break;
            else
                item_cnt++;
        /*read finished*/
        if ((ch = fgetc(current_file)) == '>' && item_cnt == item_size) {
            /*read successfully*/
            fclose(current_file);
            return DATA_PASSED;
        }
    }
    fclose(current_file);
    return DATA_BROKEN;
}

DATA_FLAG DataBlockLocate(const char* dir, int item_index, const char* text,
                          fpos_t* begin, fpos_t* end) {
    FILE* current_file;
    char ch;
    int len, char_cnt, item_cnt;
    jmp_buf data_corrupted_buf;
    current_file = fopen(dir, "r");
    if(current_file == NULL) return DATA_UNOPENED;
    len = strlen(text);
    if (setjmp(data_corrupted_buf)) {
        /*data corrupted*/
        fclose(current_file);
        return DATA_BROKEN;
    }
    /*data format:  [""""...""], <> is a block, ""is an item*/
    while (1) {
        fgetpos(current_file, begin);
        /*start of a block*/
        if ((ch = fgetc(current_file)) == '<') {
            item_cnt = 0;
            char_cnt = 0;
            /*skip former items*/
            if ((ch = fgetc(current_file)) == '"') {
                while (item_cnt < item_index)
                    if (!fscanf(current_file, "%*[^\"<>]%1[\"]", &ch))
                        longjmp(data_corrupted_buf, 1);
                    else
                        item_cnt++;
                /*reached the target item, compare them*/
                while ((ch = fgetc(current_file)) != '"' && ch != '<' &&
                       ch != '>' && ch != EOF) {
                    if (ch != text[char_cnt])
                        break;
                    else
                        char_cnt++;
                }
                if ((ch == '\"' || ch == '>') && char_cnt == len) {
                    while (ch != '>') {
                        if (!fscanf(current_file, "%*[^\"<>]%1[\">]", &ch))
                            longjmp(data_corrupted_buf, 2);
                    }
                    if (ch == '>') {
                        /*check block format*/
                        fgetpos(current_file, end);
                        fclose(current_file);
                        /*compare and check passed*/
                        return DATA_PASSED;
                    }
                } else {
                    while (ch != '>') {
                        if (!fscanf(current_file, "%*[^\"<>]%1[\">]", &ch))
                            longjmp(data_corrupted_buf, 2);
                    }
                    if (ch == '>') continue;
                }
            }
        } else if (ch == EOF) {
            /*file non found*/
            fclose(current_file);
            return DATA_NON_EXIST;
        }
        longjmp(data_corrupted_buf, 3);
    }
}

DATA_FLAG DataBlockRewrite(const char* dir, const fpos_t* begin, fpos_t* end,
                           char** src, int item_size) {
    FILE* target_file;
    FILE* tmp_file;
    char* current_buffer;
    char* tmp_dir;
    char ch;
    int rename_flag, item_cnt;
    target_file = fopen(dir, "r");
    if(target_file == NULL) return DATA_UNOPENED;
    /*tmp file create*/
    tmp_dir = (char*)malloc(sizeof(char) * strlen(dir) + sizeof(".tmp"));
    strcpy(tmp_dir, dir);
    strcat(tmp_dir, ".tmp");
    tmp_file = fopen(tmp_dir, "a");
    /*transfer data to tmp*/
    while (ftell(target_file) != *begin) fputc(fgetc(target_file), tmp_file);
    /*write new data block*/
    fputc('<', tmp_file);
    item_cnt = 0;
    while (item_cnt < item_size) {
        fprintf(tmp_file, "\"%s", src[item_cnt++]);
    }
    fputc('>', tmp_file);
    /*transfer data to tmp*/
    fsetpos(target_file, end);
    fgetpos(tmp_file, end); /*update block end pos*/
    while ((ch = fgetc(target_file)) != EOF) fputc(ch, tmp_file);
    /*close file stream*/
    fclose(tmp_file);
    fclose(target_file);
    remove(dir);
    rename_flag = rename(tmp_dir, dir);
    free(tmp_dir);
    if (!rename_flag)
        return DATA_PASSED;
    else
        return DATA_OCCUPIED;
}

DATA_FLAG DataBlockAppend(const char* dir, char** src, int item_size) {
    FILE* current_file;
    char* current_buffer;
    int item_cnt;
    current_file = fopen(dir, "a");
    if(current_file == NULL) return DATA_UNOPENED;
    fputc('<', current_file);
    item_cnt = 0;
    /*append the block data to the file*/
    while (item_cnt < item_size) {
        fprintf(current_file, "\"%s", src[item_cnt++]);
    }
    fputc('>', current_file);
    fclose(current_file);
    return DATA_PASSED;
}

DATA_FLAG DataBlockDelete(const char* dir, const fpos_t begin,
                          const fpos_t end) {
    FILE* target_file;
    FILE* tmp_file;
    char* tmp_dir;
    char ch;
    int rename_flag;
    target_file = fopen(dir, "r");
    if(target_file == NULL) return DATA_UNOPENED;
    /*tmp file create*/
    tmp_dir = (char*)malloc(sizeof(char) * strlen(dir) + sizeof(".tmp"));
    strcpy(tmp_dir, dir);
    strcat(tmp_dir, ".tmp");
    tmp_file = fopen(tmp_dir, "a");
    /*transfer data to tmp*/
    while (ftell(target_file) != begin) fputc(fgetc(target_file), tmp_file);
    /*transfer data to tmp*/
    fsetpos(target_file, &end);
    while ((ch = fgetc(target_file)) != EOF) fputc(ch, tmp_file);
    /*close file stream*/
    fclose(tmp_file);
    fclose(target_file);
    remove(dir);
    rename_flag = rename(tmp_dir, dir);
    free(tmp_dir);
    if (!rename_flag)
        return DATA_PASSED;
    else
        return DATA_OCCUPIED;
}
