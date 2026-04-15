#pragma once
// ***************************************************************
//  zmpc_matrix   version:  1.0   -  date:  2025/10/11
//  -------------------------------------------------------------
//  Yongming Wang(wangym@gmail.com)
//  -------------------------------------------------------------
//  This file is a part of project libmpc.
//  Copyright (C) 2025 - All Rights Reserved
// ***************************************************************
//
// ***************************************************************
#include <zce/zce_config.h>
#include "zmpc_proto.h"

namespace zmpc {
class ZmpcMatrixModifier {
    zmpc::IDModelMatrix& matrix_;

  public:
    ZmpcMatrixModifier(zmpc::IDModelMatrix& matrix);

    void insertMVColumnIntoMatrix(int mvIndex, int cvCount);

    void removeMVColumnFromMatrix(int mvIndex, int cvCount);

    void swapMVColumnsInMatrix(int index1, int index2, int cvCount);

    void insertDVColumnIntoMatrix(int dvIndex, int mvCount, int cvCount);

    void removeDVColumnFromMatrix(int dvIndex, int mvCount, int cvCount);

    void swapDVColumnsInMatrix(int index1, int index2, int mvCount, int cvCount);

    void insertCVRowIntoMatrix(int cvIndex, int totalInputs);

    void removeCVRowFromMatrix(int cvIndex, int totalInputs);
    
    void swapCVRowsInMatrix(int index1, int index2, int totalInputs);
};
}  // namespace zmpc
