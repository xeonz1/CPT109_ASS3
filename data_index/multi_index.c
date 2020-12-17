#include "multi_index.h"

#include <setjmp.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "index_tree.h"

void UploadNewDataBlockToCache(pMultiIndexCache db, pDataBlockContainer key) {
    int cnt;
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node;
    pIndexTree cur_index_tree;
    for (cnt = 0; cnt < key->item_number; cnt++) {
        /*general*/
        tmp_id = CreateTmpIndexTreeId(NULL, -1, cnt);
        mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
        if (mapping_tree_node != NULL) {
            cur_index_tree = GET_INDEX(mapping_tree_node);
            LoadIndexToIndexTree(cur_index_tree, key);
        }
        /*filtered*/
        tmp_id = CreateTmpIndexTreeId(key->item[cnt], cnt, -1);
        mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
        while (mapping_tree_node != NULL) {
            cur_index_tree = GET_INDEX(mapping_tree_node);
            LoadIndexToIndexTree(cur_index_tree, key);
            mapping_tree_node = GetNextMappingOf(
                &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
        }
    }
    AppendDataBlockContainer(key, &(db->data_list_tail));
    db->data_list_tail = key;
    if (db->log_enable) CacheWriteLog(db, 'a', key);
}

void RemoveDataBlockFromCache(pMultiIndexCache db, pDataBlockContainer key) {
    int cnt;
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node;
    pIndexTree cur_index_tree;
    for (cnt = 0; cnt < key->item_number; cnt++) {
        /*general*/
        tmp_id = CreateTmpIndexTreeId(NULL, -1, cnt);
        mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
        if (mapping_tree_node != NULL) {
            cur_index_tree = GET_INDEX(mapping_tree_node);
            UnloadIndexFromIndexTree(cur_index_tree, key);
        }
        /*filtered*/
        tmp_id = CreateTmpIndexTreeId(key->item[cnt], cnt, -1);
        mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
        while (mapping_tree_node != NULL) {
            cur_index_tree = GET_INDEX(mapping_tree_node);
            UnloadIndexFromIndexTree(cur_index_tree, key);
            mapping_tree_node = GetNextMappingOf(
                &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
        }
    }
    if (db->log_enable) CacheWriteLog(db, 'd', key);
    if (key == db->data_list_tail) db->data_list_tail = key->last;
    DestructDataBlockContainer(key);
}
pDataBlockContainer SearchDataBlockInCache(pMultiIndexCache db,
                                           const int id_index, char* id,
                                           const int item_index,
                                           const char* key) {
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node;
    pIndexTree cur_index_tree;
    pTreeNode index_node;
    tmp_id = CreateTmpIndexTreeId(id, id_index, item_index);
    mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
    if (mapping_tree_node != NULL) {
        cur_index_tree = GET_INDEX(mapping_tree_node);
        index_node = FindIndexOfKeyInIndexTree(cur_index_tree, key);
        if (index_node != NULL) return GET_DATA(index_node);
    } else
        return NULL;
}

pIndexTree GetDataOfId(pMultiIndexCache db, const int id_index, char* id,
                      int ordered_item_index) {
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node;
    tmp_id = CreateTmpIndexTreeId(id, id_index, ordered_item_index);
    mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
    if (mapping_tree_node != NULL) {
        return GET_INDEX(mapping_tree_node);
    } else
        return NULL;
}

void ClearDataOfIdInCache(pMultiIndexCache db, int id, char* text) {
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node, last_mapping, cur, tmp;
    char flag;
    pIndexTree index_tree;
    tmp_id = CreateTmpIndexTreeId(text, id, -1);
    mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
    if (mapping_tree_node != NULL) {
        /*remove the maps*/
        do {
            last_mapping = mapping_tree_node;
            mapping_tree_node = GetNextMappingOf(
                &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
            if (mapping_tree_node != NULL)
                RemoveMap(db->id_to_index_tree_mapping, last_mapping);
            else /*get last mapping*/
                break;
        } while (1);
        index_tree = GET_INDEX(last_mapping);
        UnloadMap(db->id_to_index_tree_mapping, last_mapping);
        /*remove all the data on the index tree*/
        if (db->log_enable) {
            db->log_enable = 0;
            flag = 1;
        } else /*close log temporarily*/
            flag = 0;

        cur = IndexTreeBegin(index_tree);
        if (cur->succ != NULL) {
            while (cur->succ != NULL) {
                cur = cur->succ;
                RemoveDataBlockFromCache(db, cur->pred->key);
            }
            RemoveDataBlockFromCache(db, cur->key);
        } else {
            RemoveDataBlockFromCache(db, cur->key);
        }
        DestructIndexTree(index_tree);

        /*if the log was open, reopen it now*/
        if (flag) db->log_enable = 1;
        if (db->log_enable) CacheWriteLog(db, 'c', id, text);
    }
}

void UpdateDataBlockInCache(pMultiIndexCache db, pDataBlockContainer new_key,
                            pDataBlockContainer old_key, int changed_index) {
    int cnt;
    TmpIndexTreeId tmp_id;
    pTreeNode mapping_tree_node;
    pIndexTree cur_index_tree;
    for (cnt = 0; cnt < old_key->item_number; cnt++) {
        /*enumerate possible id index*/
        /*general*/
        tmp_id = CreateTmpIndexTreeId(NULL, -1, cnt);
        mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
        /*general dont care about id, enumerated cnt is the item index they
         * care about*/
        if (mapping_tree_node != NULL) {
            cur_index_tree = GET_INDEX(mapping_tree_node);
            if (mapping_tree_node != NULL) {
                if ((changed_index & (1 << cnt))) {
                    UnloadIndexFromIndexTree(cur_index_tree, old_key);
                    LoadIndexToIndexTree(cur_index_tree, new_key);
                } else {
                    /*if they do not care about that changed item, just redirect
                     * them to the new data block container*/
                    RedirectIndexTo(cur_index_tree, old_key, new_key);
                }
            }
        }

        if (!(changed_index & (1 << cnt))) {
            /*if it was not changed*/
            /*then it is still in those trees of this id*/
            tmp_id = CreateTmpIndexTreeId(old_key->item[cnt], cnt, -1);
            mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
            while (mapping_tree_node != NULL) {
                cur_index_tree = GET_INDEX(mapping_tree_node);
                /*for each index tree of this id, check if their indexed item
                 * has been changed*/
                if ((changed_index & (1 << cur_index_tree->item_index))) {
                    UnloadIndexFromIndexTree(cur_index_tree, old_key);
                    LoadIndexToIndexTree(cur_index_tree, new_key);
                } else {
                    /*if they do not care about that changed item, just redirect
                     * them to the new data block container*/
                    RedirectIndexTo(cur_index_tree, old_key, new_key);
                }
                /*do the same to another index tree of this is id*/
                mapping_tree_node = GetNextMappingOf(
                    &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
            }
        } else {
            /*if it was changed, then it is either removed from original place
             * or added to the new place*/
            tmp_id = CreateTmpIndexTreeId(old_key->item[cnt], cnt, -1);
            mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
            /*remove them from old ones*/
            while (mapping_tree_node != NULL) {
                cur_index_tree = GET_INDEX(mapping_tree_node);
                UnloadIndexFromIndexTree(cur_index_tree, old_key);
                mapping_tree_node = GetNextMappingOf(
                    &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
            }
            /*add them to new ones*/
            tmp_id = CreateTmpIndexTreeId(new_key->item[cnt], cnt, -1);
            mapping_tree_node = GetMapOf(&tmp_id, db->id_to_index_tree_mapping);
            /*remove them from old ones*/
            while (mapping_tree_node != NULL) {
                cur_index_tree = GET_INDEX(mapping_tree_node);
                LoadIndexToIndexTree(cur_index_tree, new_key);
                mapping_tree_node = GetNextMappingOf(
                    &tmp_id, db->id_to_index_tree_mapping, mapping_tree_node);
            }
        }
    }
    if (db->log_enable) CacheWriteLog(db, 'u', new_key, old_key, changed_index);
    if (old_key == db->data_list_tail) db->data_list_tail = new_key;
    ReplaceDataBlockContainer(old_key, new_key);
    DestructDataBlockContainer(old_key);
}

pMultiIndexCache CreateNewCache(const char* dir, int item_number) {
    pMultiIndexCache db;
    db = (pMultiIndexCache)malloc(sizeof(sMultiIndexCache));
    db->dir = (char*)malloc(MAXIMUM_CHAR_LENGTH(strlen(dir)));
    strcpy(db->dir, dir);
    db->log_dir =
        (char*)malloc(MAXIMUM_CHAR_LENGTH(strlen(db->dir) + strlen(".log")));
    strcpy(db->log_dir, dir);
    strcat(db->log_dir, ".log");
    db->id_to_index_tree_mapping =
        CreateMapContainer(GENERATED_MAP_CMP_LIST(IndexTreeIdCompare),
                           DestructIndexTreeId, DestructIndexTree);
    db->data_list_tail = NULL;
    db->data_item_size = item_number;
    db->log = NULL;
    db->log_enable = 0;
    return db;
}

void CreateNewIndex(pMultiIndexCache db, char* id, int id_index, int item_index,
                    PriorityCmp maintain_cmp, PriorityCmp query_cmp) {
    pIndexTree new_index;
    pIndexTreeId new_index_tree_id;
    new_index_tree_id = CreateNewIndexTreeId(id, id_index, item_index);
    new_index = CreateIndexTree(item_index, maintain_cmp, query_cmp);
    AddNewMap(new_index_tree_id, new_index, db->id_to_index_tree_mapping);
}

void CloseCache(pMultiIndexCache db) {
    FILE* data;
    DestructMapContainer(db->id_to_index_tree_mapping);
    /*write all data into file*/
    data = fopen(db->dir, "w");
    while (db->data_list_tail != NULL) {
        DataBlockAppendStream(data, db->data_list_tail->item,
                              db->data_item_size);

        if (db->data_list_tail->last != NULL) {
            db->data_list_tail = db->data_list_tail->last;
            DestructDataBlockContainer(db->data_list_tail->next);
        } else {
            DestructDataBlockContainer(db->data_list_tail);
            db->data_list_tail = NULL;
        }
    }
    fclose(data);
    fclose(db->log);
    remove(db->log_dir);
    free(db->dir);
    free(db->log_dir);
    free(db);
}

void InitializeCache(pMultiIndexCache db,
                     ConstructDataBlockContainer constructor) {
    char ch;
    char log_op;
    pDataBlockContainer buffer, buffer2, current;
    jmp_buf corrupted_jmp;
    FILE* data;
    int update_list, corrupted_flag, cnt;
    /*1. read all the data*/
    if (DataCheck(db->dir) == DATA_PASSED) {
        data = fopen(db->dir, "r");
        while (1) {
            buffer = constructor();
            if (DataBlockParseStream(data, buffer->item, buffer->item_number) ==
                DATA_PASSED)
                UploadNewDataBlockToCache(db, buffer);
            else {
                DestructDataBlockContainer(buffer);
                break;
            }
        }
        fclose(data);
    }
    /*2. check log*/
    switch (corrupted_flag = setjmp(corrupted_jmp)) {
        case 0:
            break;
            /*data corrupted*/
        default: /*exception*/
            printf(
                "[Cache]: the log file %s corrupted. Please manually check! "
                "Its "
                "content will not be excuted.\n",
                db->log_dir);
            switch (corrupted_flag) {
                case 1:
                    printf(
                        "[Cache]: Initialization Error: log begin symbol lost "
                        "position: %d\n",
                        ftell(db->log));
                    break;
                case 2:
                    printf(
                        "[Cache]: Initialization Error: log operation content "
                        "corrupted: %d\n",
                        ftell(db->log));
                    break;
                case 3:
                    printf(
                        "[Cache]: Initialization Error: log end symbol lost "
                        "position: "
                        "%d\n",
                        ftell(db->log));
                    break;
                case 4:
                    printf(
                        "[Cache]: Initialization Error: unknown log operation "
                        "%c at "
                        "position: "
                        "%d\n",
                        log_op, ftell(db->log));
                    break;
            }
            fclose(db->log);
            return;
    }
    /*log recover*/
    if (DataCheck(db->log_dir) == DATA_PASSED) { /*log file exist*/
        db->log = fopen(db->log_dir, "r");
        while (fgetc(db->log) != EOF) { /*read the log file one by one*/
            fseek(db->log, -1, SEEK_CUR);
            if (fgetc(db->log) != '<') longjmp(corrupted_jmp, 1);
            if (fscanf(db->log, "%1c", &log_op)) {
                if (fgetc(db->log) != ':') longjmp(corrupted_jmp, 1);
                switch (log_op) {
                    case 'c': /*clear id*/
                        buffer = constructor();
                        if (!fscanf(db->log, "%d", &cnt))
                            longjmp(corrupted_jmp, 2);
                        if (fgetc(db->log) != ',') longjmp(corrupted_jmp, 2);
                        if (!fscanf(db->log, "%[^>]",
                                    buffer->item[cnt])) { /*this can still lead
                                                             to overflow*/
                            DestructDataBlockContainer(buffer);
                            longjmp(corrupted_jmp, 2);
                        }
                        ClearDataOfIdInCache(db, cnt, buffer->item[cnt]);
                        break;
                    case 'a':
                    case 'd':
                    case 'u':
                        buffer = constructor();
                        if (DataBlockParseStream(db->log, buffer->item,
                                                 buffer->item_number) ==
                            DATA_BROKEN) {
                            DestructDataBlockContainer(buffer);
                            longjmp(corrupted_jmp, 2);
                        }
                        switch (log_op) {
                            case 'a': /*add*/
                                UploadNewDataBlockToCache(db, buffer);
                                break;
                            case 'd': /*delete*/
                                RemoveDataBlockFromCache(db, buffer);
                                break;
                            case 'u': /*update*/
                                buffer2 = constructor();
                                if (DataBlockParseStream(
                                        db->log, buffer2->item,
                                        buffer2->item_number) == DATA_BROKEN) {
                                    DestructDataBlockContainer(buffer2);
                                    longjmp(corrupted_jmp, 2);
                                }
                                if (fgetc(db->log) != ',')
                                    longjmp(corrupted_jmp, 2);
                                if (!fscanf(db->log, "%d", &update_list))
                                    longjmp(corrupted_jmp, 2);
                                UpdateDataBlockInCache(db, buffer, buffer2,
                                                       update_list);
                                break;
                        }
                        break;
                    default:
                        longjmp(corrupted_jmp, 4);
                }
                /*check end*/
                if ((ch = fgetc(db->log)) != '>') longjmp(corrupted_jmp, 3);
            } else { /*cannot read operator*/
                longjmp(corrupted_jmp, 1);
            }
        }
        /*log file checked*/
        fclose(db->log);
        data = fopen(db->dir, "w");
        /*write*/
        current = db->data_list_tail;
        while (current != NULL) {
            DataBlockAppendStream(data, current->item, db->data_item_size);
            current = current->last;
        }
        fclose(data);
    }
    db->log = fopen(db->log_dir, "w");
    db->log_enable = 1;
}

void CacheWriteLog(pMultiIndexCache db, char op, ...) {
    pDataBlockContainer bf1, bf2;
    va_list args;
    int item_cnt, id_index;
    char* id;
    fprintf(db->log, "<%c:", op);
    if (op == 'a') { /*add data*/
        va_start(args, 1);
        bf1 = va_arg(args, pDataBlockContainer);
        DataBlockAppendStream(db->log, bf1->item, bf1->item_number);
        fputc('>', db->log);
        va_end(args);
        fflush(db->log);
    } else if (op == 'd') { /*delete data*/
        va_start(args, 1);
        bf1 = va_arg(args, pDataBlockContainer);
        DataBlockAppendStream(db->log, bf1->item, bf1->item_number);
        fputc('>', db->log);
        va_end(args);
        fflush(db->log);
    } else if (op == 'u') { /*update*/
        va_start(args, 2);
        bf1 = va_arg(args, pDataBlockContainer);
        bf2 = va_arg(args, pDataBlockContainer);
        DataBlockAppendStream(db->log, bf1->item, bf1->item_number);
        DataBlockAppendStream(db->log, bf2->item, bf2->item_number);
        fprintf(db->log, ",%d", va_arg(args, int));
        fputc('>', db->log);
        va_end(args);
        fflush(db->log);
    } else if (op == 'c') { /*clear operation*/
        va_start(args, 2);
        id_index = va_arg(args, int);
        id = va_arg(args, char*);
        fprintf(db->log, "%d,%s>", id_index, id);
        va_end(args);
        fflush(db->log);
    }
}