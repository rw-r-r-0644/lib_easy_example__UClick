#include "lib_easy.h"


int vpadError = -1;
VPADData vpad;

int screen_buf0_size = 0;
int screen_buf1_size = 0;

void flipBuffers() {
  // Flush the cache
  DCFlushRange(screenBuffer, screen_buf0_size);
  DCFlushRange((screenBuffer + screen_buf0_size), screen_buf1_size);
  // Flip buffers
  OSScreenFlipBuffersEx(0);
  OSScreenFlipBuffersEx(1);
}

void ucls() {
  for(int i=0;i<2;i++) {
    OSScreenClearBufferEx(0, 0);
    OSScreenClearBufferEx(1, 0);
    flipBuffers();
  }
  curr_line=0;
}

void ScreenInit() {
  //Init screen and screen buffers
  OSScreenInit();	
  screen_buf0_size = OSScreenGetBufferSizeEx(0);
  screen_buf1_size = OSScreenGetBufferSizeEx(1);
  screenBuffer = MEM1_alloc(screen_buf0_size + screen_buf1_size, 0x40);
  OSScreenSetBufferEx(0, screenBuffer);
  OSScreenSetBufferEx(1, (screenBuffer + screen_buf0_size));
  OSScreenEnableEx(0, 1);
  OSScreenEnableEx(1, 1);
  ucls(); //Clear screens
}

void uprintf(const char* format, ...) {
  char buff[255];
  va_list argptr;
  va_start(argptr, format);
  vsnprintf(buff, 255, format, argptr);
  va_end(argptr);
  char tmp_c[1]; //Just because OSScreenPutFontEx only accept char* :P (todo: find a better solution)
  int curr_x;
  int curr_line_tmp;
  for(int i=0; i<2; i++) {	//Print on both Buffers
    curr_line_tmp=curr_line;
    curr_x=0;
    for(size_t ii=0; ii<strlen(buff); ii++) {
      if(buff[ii]!='\n') { //HACKY! Check for carrige returns 
        tmp_c[0]=buff[ii];
        OSScreenPutFontEx(0, curr_x, curr_line_tmp, tmp_c);  //That is printed to TV
        OSScreenPutFontEx(1, curr_x, curr_line_tmp, tmp_c);  //That is printed on GamePad
        curr_x++; //Next char
      } else {
        curr_x=0;
        curr_line_tmp++;
        if(curr_line_tmp==19) { //Out of gamepad screen
          ucls(); //Clear Screen
          curr_line_tmp=0; //Reset curr_line
        }
      }
    }
    flipBuffers();
  }
  curr_line=curr_line_tmp;
}

int64_t uGetTime() {
	return OSGetTime()/SECS_TO_TICKS(1);
}

void updatePressedButtons() {
	VPADRead(0, &vpad, 1, &vpadError);
	buttons_hold = vpad.btns_h;
}

int isPressed(int button) {
	if(buttons_hold&button) return 1;
	else return 0;
}

void uInit() {
  //--Initialize every function pointer-- (byebye FindExport :D)
  InitOSFunctionPointers();
  InitSocketFunctionPointers();
  InitACPFunctionPointers();
  InitAocFunctionPointers();
  InitAXFunctionPointers();
  InitCurlFunctionPointers();
  InitFSFunctionPointers();
  InitGX2FunctionPointers();
  InitPadScoreFunctionPointers();
  InitSysFunctionPointers();
  InitSysHIDFunctionPointers();
  InitVPadFunctionPointers();

  memoryInitialize();				//You probably shouldn't care about this for now :P
  VPADInit();						//Init GamePad input library (needed for getting gamepad input)
  ScreenInit();					//Init OSScreen (all the complex stuff is in easyfunctions.h :P )
}

void uDeInit() {
  MEM1_free(screenBuffer);
  screenBuffer = NULL;
  memoryRelease();
}

