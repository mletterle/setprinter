/*********************************************************************************
**  Copyright (c) 2003 Michael Letterle <letterle@prokrams.com>                 **
**  All rights reserved.                                                        **
**                                                                              **
**  Redistribution and use in source and binary forms, with or without          **
**  modification, are permitted provided that the following conditions          **
**  are met:                                                                    **
**  1. Redistributions of source code must retain the above copyright           **
**     notice, this list of conditions and the following disclaimer.            **
**  2. Redistributions in binary form must reproduce the above copyright        **
**     notice, this list of conditions and the following disclaimer in the      **
**     documentation and/or other materials provided with the distribution.     **
**  3. The name of the author may not be used to endorse or promote products    **
**     derived from this software without specific prior written permission.    **
**                                                                              **
**  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR        **
**  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES   **
**  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.     **
**  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,            **
**  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT    **
**  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   **
**  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY       **
**  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT         **
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF    **
**  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           **
**********************************************************************************/


/* Defines */

//#define _DEBUG_

/* Prototypes */
readregistry(void);
keygen(void);
configuser(void);
LoadDefaultPrinter(void);
CreateIniFile();
/* Constants */
const ProgVer = "Windows Default Printer Configurator V1.0\nBuild 030312\n\nBy Michael Letterle (c)2003";
const ProgName = "setprinter";
const char *KeyFileConst = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\n      Device = DELETE\n\nHKEY_CURRENT_USER\\Printers\n      DeviceOld = DELETE\n\nHKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\n      Device = \"%s,%s\"\n\nHKEY_CURRENT_USER\\Printers\n      DeviceOld = \"%s,%s\"";
const char *USAGE = "%s\n\nUsage: %s [options] \n This program is useful for setting default printers on Windows NT 4.0 TSE\n and Windows 2000\n \n Options:\n \n -v: Display Version Information\n\n -h: Display this help\n\n -g: Generate Keyfiles for all Printers.\n     Key files will be in the format of <printername>.key.\n     The program will write the file in the current directory and will\n     overwrite files of the same name in that directory (this option is\n     discouraged as the program itself makes key files obsolete)\n\n -c: Configure Default Printer for a user\n\n -l: Load default printer for logged in user from %s\n\n -i: Create a new inifile (creates it in setprinter.bak)\n\n";
const char *inifile = "setprinter.ini";
const char *INIHEADER = "#setprinter V1.0#\n";
const char *SPACE = " ";

/* Global Variables */
int z=0;
int y=0;
char *printer[128][255];
char *spooler[128][255];
char *username[128][128];
FILE *fptr;
HKEY hKey;
char keyvalue[80];
int *NumofValue;
int *MaxValueLength;
DWORD *lpcbData = 80;
int keyindex = 0;
DWORD *lpcbValueName = 80;
char *queryvalue[80];
char *PrinterName[80];
char *tempname;
char *Spooler[80];
char *mainfile[128];
char *option;
int *buf;
NET_DISPLAY_USER *ndu;
int blah;
int NumOfUsers;
char *inputS[50];
int inputI;
char SelectedUsername[128][128];
char SelectedPrinter[128][255];
int NumOfPrinters;
char iniline[255];
long pos;
char *origline[255];
int isconfig = 0;
