#ifndef __FILE_IO__
#define __FILE_IO__

#include <stdio.h>
#include <string.h>
/*enum type for caller of data operation to be aware of operation state*/
typedef enum {
    DATA_EOF = 0,
    DATA_BROKEN = 0x01,
    DATA_PASSED = 0x02,
    DATA_NON_EXIST = 0x04,
    DATA_UNOPENED = 0x10,
    DATA_OCCUPIED = 0x20
} DATA_FLAG;

/**
 * @brief check the existence of data
 *
 * @param dir
 * @retval DATA_PASSED if check passed
 * @retval DATA_NON_EXIST if check failed
 */
DATA_FLAG DataCheck(const char* dir);

/**
 * @brief parse blocks in stream mode
 * @note It does not control the file, file must be closed outside
 * @param _file target file
 * @param dst target data block buffer
 * @param item_size item count
 * @retval DATA_BROKEN if data corrupted
 * @retval DATA_EOF if data reaches end
 * @retval DATA_PASSED if this time the parse is successful
 */
DATA_FLAG DataBlockParseStream(FILE* _file, char** dst, int item_size);

/**
 * @brief append blocks in stream mode
 * @note not used in this program!
 * @param _file target file
 * @param dst target data block buffer
 * @param item_size item count
 * @return DATA_FLAG
 */
DATA_FLAG DataBlockAppendStream(FILE* _file, char** src, int item_size);

/**
 * @brief parse data block in [begin,end) to buffer
 *
 * @param dir file location
 * @param begin specified pos
 * @param end specified pos
 * @param dst target buffer
 * @param item_size number of items
 * @return DATA_FLAG
 */
DATA_FLAG DataBlockParse(const char* dir, const fpos_t begin, const fpos_t end,
                         char** dst, const int item_size);
/**
 * @brief locate data block and store its location
 *
 * @param dir file location
 * @param item_index  the characteristic item position in a block
 * @param text the characteristic item content
 * @param begin the pos to store begin
 * @param end the pos to store end
 * @retval DATA_BROKEN if data corrupted
 * @retval DATA_NON_EXIST if data not found
 * @retval DATA_PASSED if data found
 */
DATA_FLAG DataBlockLocate(const char* dir, int item_index, const char* text,
                          fpos_t* begin, fpos_t* end);

/**
 * @brief rewrite data block in specified location and update the location
 *
 * @param dir file location
 * @param begin specified pos
 * @param end specified pos
 * @param src target buffer
 * @param item_size block item count
 * @retval DATA_BROKEN if the rewrite failed
 * @retval DATA_PASSED if the rewrite successful
 */
DATA_FLAG DataBlockRewrite(const char* dir, const fpos_t* begin, fpos_t* end,
                           char** src, int item_size);
/**
 * @brief append data block to the file
 *
 * @param dir file location
 * @param src target buffer
 * @param item_size number of data block items
 * @return DATA_FLAG
 */
DATA_FLAG DataBlockAppend(const char* dir, char** src, int item_size);
/**
 * @brief delete specified data block
 *
 * @param dir file location
 * @param begin specified location
 * @param end specified lcoation
 * @return DATA_FLAG
 */
DATA_FLAG DataBlockDelete(const char* dir, const fpos_t begin,
                          const fpos_t end);

#endif /*__FILE_IO_*/