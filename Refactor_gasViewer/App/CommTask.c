#include "commtask.h"
#include "string.h"


//通信任务
/* USER CODE END Header_COMM_Task */
void COMM_Task(void const * argument)
{
  /* USER CODE BEGIN GUI_Task */
//	UART1_Interrupt_Check();
  /* Infinite loop */
  for(;;)
  {
		if (g_ModbusEntry.RxStatus)
    {
      /* 1. 处理接收的数据（示例：回发数据） */
			Send_Modbus(g_ModbusEntry.RxBuf,g_ModbusEntry.RxCount);
      /* 2. 重置接收状态，准备下一次接收 */
      g_ModbusEntry.RxStatus = 0;
			g_ModbusEntry.RxCount=0;
      memset(g_ModbusEntry.RxBuf, 0, RX_BUF_SIZE);
    }
    osDelay(10);
  }
  /* USER CODE END GUI_Task */
}
