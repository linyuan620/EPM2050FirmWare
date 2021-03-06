#include "global.h"

typedef struct                  // 汉字字模数据结构
{
    unsigned char Index[2];               // 汉字内码索引
    const unsigned char Msk[24];                       // 点阵码数据
} typFNT_GB16;



extern uint8 const hz12k1[24];    //主
extern uint8 const hz12k2[24];//菜
extern uint8 const hz12k3[24];//单
extern uint8 const hz12k4[24];//选
extern uint8 const hz12k5[24];//择
extern uint8 const hz12k6[24];//项
extern uint8 const hz12k7[24];//！
extern uint8 const hz12k8[24];//1
extern uint8 const hz12k9[24];//2
extern uint8 const hz12k10[24];//3
extern uint8 const hz12k11[24];//4
extern uint8 const hz12k12[24];//5
extern uint8 const hz12k13[24];//6
extern uint8 const hz12k14[24];//7
extern uint8 const hz12k15[24];//8
extern uint8 const hz12k16[24];//9
extern uint8 const hz12k17[24];//0
extern uint8 const hz12k18[24];//右指示
extern uint8 const hz12k19[24];//左
extern uint8 const hz12k20[24];//上
extern uint8 const hz12k21[24];//下
extern uint8 const hz12k22[24];//...
extern uint8 const hz12k23[24];//:
extern uint8 const hz12k24[24];//.
extern uint8 const hz12k25[24];//。
extern uint8 const hz12k25[24];
extern uint8 const hz12k26[24];
extern uint8 const hz12k27[24];
extern uint8 const hz12k28[24];
extern uint8 const hz12k29[24];
extern uint8 const hz12k30[24];
extern uint8 const hz12k31[24];
extern uint8 const hz12k32[24];
extern uint8 const hz12k33[24];
extern uint8 const hz12k34[24];
extern uint8 const hz12k35[24];
extern uint8 const hz12k36[24];
extern uint8 const hz12k37[24];
extern uint8 const hz12k38[24];
extern uint8 const hz12k39[24];
extern uint8 const hz12k40[24];
extern uint8 const hz12k41[24];
extern uint8 const hz12k42[24];
extern uint8 const hz12k43[24];
extern uint8 const hz12k44[24];

extern uint8 const hzk1212k1[][24];
extern uint8 const hzk126k1[][12];
extern uint8 const hzk2412k1[][36];
extern uint8 const hzk168k1[][16];
extern uint8 const hzk1212k2[][24];

//extern uint8 const hzk1616k1[][32];
extern const uint8 hanzi1212[][24];
extern const uint8 hanzi816[][16];
extern typFNT_GB16  GB_16[];
extern const uint8 hanzi2412[][36];
extern const uint8 hanzi1616[][32];
extern const uint8 	hanzi2424[][36];

