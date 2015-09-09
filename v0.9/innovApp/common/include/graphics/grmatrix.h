#ifndef GRMATRIX_H
#define GRMATRIX_H

#include <innovEngine_types.h>
#include <math/gemath.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MATRIX2D_DIM_X 3
#define MATRIX2D_DIM_Y 2

typedef struct {
  INT32 a[MATRIX2D_DIM_X * MATRIX2D_DIM_Y];
} TMatrix2D;

extern MAPCORE_API
void Matrix2D_LoadMatrix(TMatrix2D *matrix, const TMatrix2D *load);

extern MAPCORE_API
void Matrix2D_MultMatrix(TMatrix2D *matrix, const TMatrix2D *mult);

extern MAPCORE_API
void Matrix2D_LoadIdentity(TMatrix2D *matrix);

extern MAPCORE_API
void Matrix2D_Translate(TMatrix2D *matrix, INT32 x, INT32 y);

extern MAPCORE_API
void Matrix2D_Scale(TMatrix2D *matrix, INT32 x, INT32 y);

extern MAPCORE_API
void Matrix2D_Rotate(TMatrix2D *matrix, UINT8 angle);

extern MAPCORE_API
void Matrix2D_TransformPos(const TMatrix2D *matrix, TPos *pos);

extern MAPCORE_API
BOOL8 Matrix2D_Invert(TMatrix2D *matrix);

#ifdef __cplusplus
}
#endif

#endif // GRMATRIX_H
