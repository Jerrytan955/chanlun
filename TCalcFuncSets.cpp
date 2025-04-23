// TCalcFuncSets.cpp
#include "stdafx.h"
#include "TCalcFuncSets.h"
#include <vector>

// 自动绑定注册函数（空实现，用于编号 9999）
extern "C" __declspec(dllexport)
void AutoBindInit(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
    // 不执行任何逻辑，仅保留编号
}

// ✅ TestPlugin1：输出包含处理后的 HIGH
extern "C" __declspec(dllexport)
void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
    if (DataLen < 3) return;

    pfOUT[0] = pfINa[0];
    pfOUT[1] = pfINa[1];

    for (int i = 2; i < DataLen; ++i)
    {
        float h2 = pfOUT[i - 2];
        float h1 = pfOUT[i - 1];
        float h = pfINa[i];

        float l2 = pfINb[i - 2];
        float l1 = pfINb[i - 1];
        float l = pfINb[i];

        if (h1 > h2 && l1 > l2 && h <= h1 && l >= l1)
        {
            pfOUT[i] = (h > h1) ? h : h1;
        }
        else if (h1 < h2 && l1 < l2 && h >= h1 && l <= l1)
        {
            pfOUT[i] = (h < h1) ? h : h1;
        }
        else
        {
            pfOUT[i] = h;
        }
    }
}

// ✅ TestPlugin2：输出包含处理后的 LOW
extern "C" __declspec(dllexport)
void TestPlugin2(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
    if (DataLen < 3) return;

    pfOUT[0] = pfINb[0];
    pfOUT[1] = pfINb[1];

    for (int i = 2; i < DataLen; ++i)
    {
        float h2 = pfINa[i - 2];
        float h1 = pfINa[i - 1];
        float h = pfINa[i];

        float l2 = pfOUT[i - 2];
        float l1 = pfOUT[i - 1];
        float l = pfINb[i];

        if (h1 > h2 && l1 > l2 && h <= h1 && l >= l1)
        {
            pfOUT[i] = (l > l1) ? l : l1;
        }
        else if (h1 < h2 && l1 < l2 && h >= h1 && l <= l1)
        {
            pfOUT[i] = (l < l1) ? l : l1;
        }
        else
        {
            pfOUT[i] = l;
        }
    }
}

// ✅ TestPlugin3：输出是否包含标记（1为包含K线）
extern "C" __declspec(dllexport)
void TestPlugin3(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
    if (DataLen < 3) return;

    pfOUT[0] = 0;
    pfOUT[1] = 0;

    for (int i = 2; i < DataLen; ++i)
    {
        float h2 = pfINa[i - 2];
        float h1 = pfINa[i - 1];
        float h = pfINa[i];

        float l2 = pfINb[i - 2];
        float l1 = pfINb[i - 1];
        float l = pfINb[i];

        if ((h1 > h2 && l1 > l2 && h <= h1 && l >= l1) ||
            (h1 < h2 && l1 < l2 && h >= h1 && l <= l1))
        {
            pfOUT[i] = 1.0f;
        }
        else
        {
            pfOUT[i] = 0.0f;
        }
    }
}

// ✅ 函数绑定表：保持不动
PluginTCalcFuncInfo g_CalcFuncSets[] = {
    {1,    (pPluginFUNC)&TestPlugin1},
    {2,    (pPluginFUNC)&TestPlugin2},
    {3,    (pPluginFUNC)&TestPlugin3},
    {9999, (pPluginFUNC)&AutoBindInit},
    {0, NULL}
};

// ✅ 通达信注册函数不动
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
    if (*pFun == NULL) {
        *pFun = g_CalcFuncSets;
        return TRUE;
    }
    return FALSE;
}
