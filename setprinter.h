/* Defines */

//#define _DEBUG_

/* Prototypes */
readregistry(void);
keygen(void);
configuser(void);
LoadDefaultPrinter(void);
/* Constants */
const ProgVer = "Terminal Server Default Printer Configurator V.001\n\nBy Michael Letterle (c)2003";
const ProgName = "setprinter";
const char *KeyFileConst = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\n      Device = DELETE\n\nHKEY_CURRENT_USER\\Printers\n      DeviceOld = DELETE\n\nHKEY_CURRENT_USER\\Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows\n      Device = \"%s,%s\"\n\nHKEY_CURRENT_USER\\Printers\n      DeviceOld = \"%s,%s\"";
const char *USAGE = "%s\n\nUsage: %s \n This program will go through the registry and create a key file\n for every printer found therein, it will put the files in the same directory\n from where the program was run and will overwrite any files\n of the same *.key name\n \n Key files are in the format of <printername>.key\n \n Options:\n \n -v: Dispaly Version Information\n -h: Display this help\n -g: Generate Keyfiles for all Printers\n -c: Configure Default Printer for a user\n -l Load default printer for logged in user\n -i: Create a new inifile from scratch";
const char *inifile = "setprinter.ini";
const char *INIHEADER = "#setprinter V.001#\n";
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
