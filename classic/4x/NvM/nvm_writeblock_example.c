/*
nvm_writeblock_example.c

Sample code related to blog entry :
https://autosar.jayakumar.de/blog/nvm/2020/06/id_12/

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

/* defines for NvM write State machine */
#define SPEEDLIMITER_NVM_WRITE_STATE_IDLE (0u)
#define SPEEDLIMITER_NVM_WRITE_STATE_REQ_WRITE (1u)
#define SPEEDLIMITER_NVM_WRITE_STATE_PENDING (2u)
#define SPEEDLIMITER_NVM_WRITE_STATE_ERROR (3u)

/* state machine variable to hold state of nvm write sequence */
uint8 SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_IDLE;

typedef SpeedLimiterType {
  uint8 defaultLimit;
  uint8 maxLimit;
}
SpeedLimiterType;

void Swc_SpeedLimter_trigger_persistNvm(uint8 defaultLimit, uint8 maxLimit) {
  Std_ReturnType RetVal = E_NOT_OK;
  SpeedLimiterType* pRamData = Rte_Pim_SpeedLimter();
  if ((SPEEDLIMITER_NVM_WRITE_STATE_IDLE == SpeedLimiterNvmWriteState) ||
      SPEEDLIMITER_NVM_WRITE_STATE_ERROR == SpeedLimiterNvmWriteState))
        {
      pRamData->defaultLimit = defaultLimit;
      pRamData->maxLimit = maxLimit;
      SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_REQ_WRITE;
      RetVal = E_OK;
    }
  return RetVal;
}

void Swc_SpeedLimter_persistNvm_runnable_10ms(void) {
  NvM_RequestResultType nvmStatus;
  Std_ReturnType nvmRetVal;
  SpeedLimiterType* pRamData;
  switch (SpeedLimiterNvmWriteState) {
    case SPEEDLIMITER_NVM_WRITE_STATE_IDLE: {
      /* Idle */
    } break;
    case SPEEDLIMITER_NVM_WRITE_STATE_REQ_WRITE: {
      /* Get reference to RAM block */
      pRamData = Rte_Pim_SpeedLimter();
      nvmRetVal = Rte_Call_PS_SpeedLimiter_WriteBlock(pRamData);
      if (E_OK == nvmRetVal) {
        SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_PENDING;
        /* Job accepted */
      } else {
        /* Job failed */
        SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_ERROR;
      }
    } break;
    case SPEEDLIMITER_NVM_WRITE_STATE_PENDING: {
      Rte_Call_PS_SpeedLimiter_GetErrorStatus(&nvmStatus) if (nvmStatus ==
                                                              NVM_REQ_PENDING) {
        /* Wait for the completion for NvM write */
      }
      else if (nvmStatus == NVM_REQ_OK) {
        /* Write successful. Inform application SW-C */
        /*  ... */
        SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_IDLE;
      }
      else {
        /* An error occured. Inform application SW-C */
        /* ....*/
        SpeedLimiterNvmWriteState = SPEEDLIMITER_NVM_WRITE_STATE_ERROR;
      }
    } break;
    case SPEEDLIMITER_NVM_WRITE_STATE_ERROR: {
      /*An error state reached.*/
    } break;
    default: {
      /*Error occured.*/
    }
  }
}
