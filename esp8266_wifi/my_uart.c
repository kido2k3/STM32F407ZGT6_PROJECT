

#include "my_uart.h"
#define BUFFER_MAX_SIZE 20
#define TEMP_CMD        "!TEMP:"
uint8_t buf_uart[BUFFER_MAX_SIZE];
uint8_t idx = 0;
int done_receive = 0;
void get_data(uint8_t msg) {
  buf_uart[idx++] = msg;
  if(buf_uart[idx - 1] == '#'){
    done_receive = 1;
    idx = 0;
  }
  if (idx > BUFFER_MAX_SIZE - 1) {
    idx = 0;
  }
}



uint8_t compile_cmd(double *cmd_data) {
  if (done_receive) {
    done_receive = 0;
    if(strstr((char*)buf_uart, TEMP_CMD)){
      *cmd_data = atof(buf_uart + strlen(TEMP_CMD));
      return 1;
    }
  }
  return 0;
}