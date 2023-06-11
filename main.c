#include <stdio.h>
#include <stdlib.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspctrl.h>
#include <pspsdk.h>
#include <pspiofilemgr.h>
#include "include/kubridge.h"
#include "include/io.h"
#include "include/callback.h"

#define RGB(r, g, b) (0xFF000000 | ((b)<<16) | ((g)<<8) | (r))
#define RED RGB(255,0,0)
#define BLACK RGB(0,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(50,140,255)
#define ORANGE RGB(255,127,0)
#define WHITE RGB(255,255,255)
#define VIOLET RGB(153,153,255)

PSP_MODULE_INFO("spoofix", 0x800, 0, 1);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);

int main(){
    pspDebugScreenInit();
    pspDebugScreenClear();
    SetupCallbacks();

    int version_num = sceKernelDevkitVersion();
	  char *version_chr = (char *)&version_num;

   
    void init_flash(){ // from procfw source code
	    int ret;
   
	    ret = sceIoUnassign("flash0:");

	      while(ret < 0) {
          pspDebugScreenSetTextColor(RED);
          pspDebugScreenPrintf(" failed, retrying\n");
		      ret = sceIoUnassign("flash0:");
          sceKernelDelayThread(5*1000*100);
          pspDebugScreenSetTextColor(WHITE);
	        }

	      sceIoAssign("flash0:", "lflash0:0,0", "flashfat0:", 0, NULL, 0);
        pspDebugScreenSetTextColor(GREEN);
        pspDebugScreenPrintf(" success!\n");
        pspDebugScreenSetTextColor(WHITE);
      }

    void backup_version(){
      int bak; 
      bak = copy_file("flash0:/vsh/etc/version.txt", "/version.txt.backup");

      if (bak < 0){
          pspDebugScreenSetTextColor(RED);
          pspDebugScreenPrintf(" failed!\ncopy_file(); unsuccessful.\n");
          sceKernelDelayThread(3*1000*1000); 
          sceKernelExitGame();
        }
      
      int rmf;
      rmf = sceIoRemove("flash0:/vsh/etc/version.txt");

      if (rmf < 0){
          pspDebugScreenSetTextColor(RED);
          pspDebugScreenPrintf(" failed!\nsceIoRemove(); unsuccessful.\n");
          sceKernelDelayThread(3*1000*1000); 
          sceKernelExitGame();
        }

      pspDebugScreenSetTextColor(GREEN);
      pspDebugScreenPrintf(" success!\n");
      pspDebugScreenSetTextColor(WHITE);
    }

    void check_backup(){
      int read; 
      read = sceIoOpen("/version.txt.backup", PSP_O_RDONLY, 0777);

      if (read >= 0){
          pspDebugScreenSetTextColor(GREEN);
          pspDebugScreenPrintf("version.txt.backup detected,");
          pspDebugScreenSetTextColor(WHITE);
          pspDebugScreenPrintf(" reverting...");

          int del;
          del = sceIoRemove("flash0:/vsh/etc/version.txt");
            if (del < 0){
              pspDebugScreenSetTextColor(RED);
              pspDebugScreenPrintf(" failed!\nsceIoRemove(); unsuccessful.\n");
              sceKernelDelayThread(10*1000*1000); 
              sceKernelExitGame();
            }

          int rvt; 
          rvt = copy_file("/version.txt.backup", "flash0:/vsh/etc/version.txt");
            if (rvt < 0){
              pspDebugScreenSetTextColor(RED);
              pspDebugScreenPrintf(" failed!\ncopy_file(); unsuccessful.\nWARNING: You might get RSOD after exiting, since the system won't be able to locate the flash0:/vsh/etc/version.txt. To fix this, just go to the Recovery Menu by holding R on the boot, turn on USB from there, copy version.txt.backup (which should be inside of this program's folder) to your PC, turn off the USB, rename the version.txt.backup to version.txt on your PC, turn on the USB (flash0) from the Advanced menu, copy the version.txt from your PC to /vsh/etc/, turn off the USB (flash0), then reboot. This should fix the problem.\n Press X to exit");
              SceCtrlData pad;

              while(1) {
		          sceCtrlReadBufferPositive(&pad, 1);
              if (pad.Buttons & PSP_CTRL_CROSS)
			            {
                  sceKernelExitGame();
                }
              }
            }

          pspDebugScreenSetTextColor(GREEN);
          pspDebugScreenPrintf(" success!\n");
          pspDebugScreenSetTextColor(WHITE);
          pspDebugScreenPrintf("Exiting...\n");
          sceKernelDelayThread(3*1000*1000); 
          sceKernelExitGame();
        }
      }

    void read_version(){
      FILE* file2 = fopen("flash0:/vsh/etc/version.txt", "r");
        char arr[13] = {0};
        fread(arr, sizeof(char), 13, file2);
        fclose(file2);
        if (file2 < 0){
          pspDebugScreenSetTextColor(RED);
          pspDebugScreenPrintf(" failed!\nfopen(); unsuccessful.");
          sceKernelDelayThread(3*1000*1000); 
          sceKernelExitGame();
        }
        pspDebugScreenSetTextColor(GREEN);
        pspDebugScreenPrintf(" %s\n", arr);
        pspDebugScreenSetTextColor(WHITE);
    }

    void replace_version(){
      int cop;
      cop = copy_file("/version.txt", "flash0:/vsh/etc/version.txt");

      if (cop < 0){
          pspDebugScreenSetTextColor(RED);
          pspDebugScreenPrintf(" failed!\ncopy_file(); unsuccessful.");
          sceKernelDelayThread(3*1000*1000); 
          sceKernelExitGame();
        }

      pspDebugScreenSetTextColor(GREEN);
      pspDebugScreenPrintf(" success!\n");
      pspDebugScreenSetTextColor(WHITE);
    }


    pspDebugScreenSetTextColor(VIOLET);
    pspDebugScreenPrintf("spoofix v0.1 ~ github.com/rreha/spoofix/ \n\n");
    pspDebugScreenSetTextColor(WHITE);
    pspDebugScreenPrintf("Getting your kernel version...");
    sceKernelDelayThread(5*1000*100); 
    pspDebugScreenSetTextColor(GREEN);
    pspDebugScreenPrintf(" %i.%i%i (0x%08x)\n", version_chr[3], version_chr[2], version_chr[1], version_num);
    pspDebugScreenSetTextColor(WHITE);
    pspDebugScreenPrintf("Initializing flash0 for RW access...");
    sceKernelDelayThread(5*1000*100); 
    init_flash();
    check_backup();
    pspDebugScreenPrintf("Reading your version.txt...");
    sceKernelDelayThread(5*1000*100);
    read_version();
    pspDebugScreenPrintf("Backing up your version.txt...");
    sceKernelDelayThread(5*1000*100);
    backup_version();
    pspDebugScreenPrintf("Copying the unspoofed version.txt...");
    sceKernelDelayThread(5*1000*100);
    replace_version();
    pspDebugScreenPrintf("\nIf you want to revert to your old version.txt, just run the program again.\n\nExiting...");
    sceKernelDelayThread(6*1000*1000); 
    sceKernelExitGame();
}