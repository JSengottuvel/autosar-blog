/*
nvm_writeall_example.c

Sample code related to blog entry :
https://autosar.jayakumar.de/blog/2020/06/id_32/

MIT License

Copyright (c) 2020 Jayakumar Sengottuvel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Warning: source code is not compilable due to dependecies.*/

typedef struct SpeedLimiterType {
    uint8 defaultLimit;
    uint8 maxLimit;
} SpeedLimiterType;

void Swc_SpeedLimter_NvmUpdateRam(uint8 defaultLimit, uint8 maxLimit) {
    /* Get reference to RAM block*/
    SpeedLimiterType* pRamData = Rte_Pim_SpeedLimter();
    /* Update RAM block content with the values from user */
    pRamData->defaultLimit = defaultLimit;
    pRamData->maxLimit = maxLimit;
    /* Mark the NvRam status CHANGED */
    Rte_Call_PS_PIM_SpeedLimiter_SetRamBlockStatus(TRUE);
}
