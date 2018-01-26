//
//  utils.h
//  vsys
//
//  Created by 薯条 on 17/12/24.
//  Copyright © 2017年 薯条. All rights reserved.
//

#ifndef __vsys_utils__
#define __vsys_utils__

#define SIREN_NEW_AR2(type, row, column) (type **)__dx_new_array2(sizeof(type), row, column);
#define SIREN_DEL_AR2(pData) __dx_delete_array2(pData)

#define __dx_delete_array2(pData) if(pData) delete[] pData[0]; pData[0] = nullptr; delete[] pData; pData = nullptr;

static inline char** __dx_new_array2(int width, int row, int column) {
    if(row == 0 || column == 0) return nullptr;
    char** pData = new char*[row];
    pData[0] = new char[width * row * column];
    memset(pData[0], 0, width * row * column);
    for(int i= 1; i < row; i++) {
        pData[i] = pData[i * width * column];
    }
    return pData;
}
#endif /* __vsys_utils__ */
