/*
Stole from https://github.com/MrColdbird/procfw/blob/master/Installer/main.c
Modified it a bit :grin:
*/


// include psp stuff
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspiofilemgr.h>

// calloc, malloc, relloc
#include <stdlib.h>
#include <string.h>

static u8 g_buf[64*1024] __attribute__((aligned(64)));

int copy_file(const char *src, const char *dst)
{
	SceUID fd = -1, fdw = -1;
	int ret;

	ret = sceIoOpen(src, PSP_O_RDONLY, 0777);

	if (ret < 0) {
		goto error;
	}

	fd = ret;

	ret = sceIoOpen(dst, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);

	if (ret < 0) {
		goto error;
	}

	fdw = ret;
	ret = sizeof(g_buf);
	ret = sceIoRead(fd, g_buf, ret);

	while (ret > 0) {
		ret = sceIoWrite(fdw, g_buf, ret);

		if (ret < 0) {
			goto error;
		}

		ret = sceIoRead(fd, g_buf, ret);
	}

	if (ret < 0) {
		goto error;
	}

	sceIoClose(fd);
	sceIoClose(fdw);

	return 0;

error:
	sceIoClose(fd);
	sceIoClose(fdw);
	return ret;
}
