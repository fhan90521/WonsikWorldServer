#pragma once
#define DIAGONOL_DELTA_SECTOR 5
#define NON_DIAGONOL_DELTA_SECTOR 3
const int dXdY[9][2] = { {-1, 1}, { 0, 1} , { 1, 1},
						 {-1, 0}, { 0, 0} , { 1, 0},
						 {-1,-1}, { 0, -1}, { 1, -1}};

const int addRR[3] = { 2,5,8 };
const int removeRR[3] = { 0,3,6 };

const int addRD[5] = {2,5,8,6,7};
const int removeRD[5] = { 0,3,6,1,2 };

const int addDD[3] = { 6,7,8 };
const int removeDD[3] = { 0,1,2 };

const int addLD[5] = { 0,3,6,7,8 };
const int removeLD[5] = { 0,1,2,5,8 };

const int addLL[3] = { 0,3,6 };
const int removeLL[3] = { 2,5,8 };

const int addLU[5] = {0,3,6,1,2};
const int removeLU[5] = { 2,5,8,6,7 };

const int addUU[3] = { 0,1,2 };
const int removeUU[3] = { 6,7,8 };

const int addRU[5] = { 0,1,2,5,8 };
const int removeRU[5] = { 0,3,6,7,8 };

