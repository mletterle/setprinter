/* INCLUDES */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <winreg.h>
#include <lm.h>
#include "setprinter.h"


/* Let's get this party started */
int main(int argc, char **argv, char *env)
{
if (argc <= 1) { printf("Try typing %s -h\n",ProgName);exit(1); };
if (argc > 1) /* This check will keep the program from crashing, we only need to check the arguments if there is one */
{
if (argv[1][0] == '-' || argv[1][0] == '/') /* Look for the canonical switch characters */
	{

		switch(argv[1][1]) /* Let's see what's after the switch character */
		{
		case 'h': printf(USAGE,ProgVer,ProgName);exit(0); /* Display Help and exit with 0 (USAGE defined in setprinter.h) */
		case 'v': printf("%s\n",ProgVer);exit(1); /* Display Version and exit with 1 */
		case 'g': readregistry();keygen();exit(1); /* Generate regini key files and exit with 1 */
		case 'c': readregistry();configuser();exit(1);
		case 'l': LoadDefaultPrinter();exit(1);
		case 'i': printf("This function not implemented yet");exit(1);
		default : printf("%s\nrun %s -h for help\n",ProgVer,ProgName);exit(1); /* Hey those options don't make sense, look at the help */
		};

	};
};







return 0;
};

int readregistry()
{


	RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices",0,KEY_ALL_ACCESS,&hKey); /* Open the Devices key to the hKey handle */
	RegQueryInfoKey(hKey,NULL,NULL,NULL,NULL,NULL,NULL,&NumofValue,&MaxValueLength,NULL,NULL,NULL); /* all we really want to know is how many printers and how long is the longest name */

	while (RegEnumValue(hKey,keyindex,&PrinterName,&lpcbValueName,NULL,NULL,&Spooler,&lpcbData) != ERROR_NO_MORE_ITEMS ) /* Sometimes reading the code is all you need */
	{

		strcpy(printer[keyindex], PrinterName);
		strcpy(spooler[keyindex], Spooler);
		keyindex=keyindex+1;
		lpcbValueName = MaxValueLength+1;
	};
	RegCloseKey(hKey);
	NumOfPrinters=keyindex;
	z=0;
blah = 1;
	while (blah)
	{
		blah = NetQueryDisplayInformation(NULL,1,z,1,1024,&MaxValueLength,&buf) ;
		ndu = (NET_DISPLAY_USER *)buf;
		wcstombs(username[y],ndu->usri1_name,sizeof(username)); /* Took me a while to figure this part out */
		z=z+1;
		y=y+1;
	};
	NumOfUsers=y;
	z=0;
	return 0;
};


int keygen()
{
printf("Generating KeyFiles\n");
while (z != keyindex)
	{

		tempname = strtok(printer[z],"\\\/ ");
		strcpy(printer[z],tempname);
		while ( tempname != NULL)
		{
			tempname = strtok(NULL,"\\\/ ");
			if (tempname != NULL) { strcat(printer[z],tempname);};
		};
		stpcpy(mainfile, printer[z]);
		strcat(mainfile,".key");
		printf("Writing %s\n",mainfile);
		if ((fptr = fopen(mainfile,"w")) == NULL)
		    {
				printf("\nCannot open %s, for writing.\n", mainfile);
				break;
			};

		fprintf(fptr,KeyFileConst,printer[z],spooler[z],printer[z],spooler[z]);
		fclose(fptr);
		z = z+1;
	};
printf("\n");
return 0;
};

int configuser()
{
	z=0;
	while (z != NumOfUsers)
		{
		 printf("%i:  %s\n",z+1,username[z]);
		 z=z+1;
	 };
	 printf("Select Number of User You Wish To Configure:");
	 gets(inputS);
	 inputI = atoi(inputS);
	 inputI = inputI-1;
	 strcpy(SelectedUsername,username[inputI]);
	 #ifdef _DEBUG_
	 printf("inputI = %d\nUsername[inputI] = %s\nSelectedUsername = %s\n",inputI,username[inputI],SelectedUsername);
	 #endif
	z=0;
	while ( z != NumOfPrinters )
	{
		printf("%i:  %s\n",z+1,printer[z]);
		z=z+1;
	};
	printf("Select Number Of Printer To Be Default For %s:", SelectedUsername);
	gets(inputS);
	inputI = atoi(inputS);
	inputI = inputI-1;
	strcpy(SelectedPrinter,printer[inputI]);
	strcat(SelectedPrinter,",");
	strcat(SelectedPrinter,spooler[inputI]);
	#ifdef _DEBUG_
	printf("\nSelected Printer,Spooler: %s\n",SelectedPrinter);
	#endif
		if ((fptr = fopen(inifile,"r")) == NULL)
		{
			printf("\nCannot open %s for reading, please correct and try again.\nIf file does not exist it can be created with: %s -i", inifile);
			exit(1);
		};
		fgets(iniline,sizeof(iniline),fptr); /* Read First Line of inifile */

		if ( !strcmp(iniline, INIHEADER)) /* is it a valid ini file? */
			{
			while ( fgets(iniline,sizeof(iniline),fptr) != NULL )
			{
			tempname=strtok(iniline,"=");
			#ifdef _DEBUG_
			printf("tempname = %s\n",tempname);
			#endif
			if (!strcmpi(tempname,SelectedUsername)){
				printf("User %s already has a configured printer, delete line in setprinter.ini or select another username",SelectedUsername);
				fclose(fptr);
				exit(1);
				};
			};
			fclose(fptr);
			if ((fptr = fopen(inifile,"a+")) == NULL){printf("\nCannot open %s for updating, please correct and try again.\n");};
			fprintf(fptr,"%s=%s\n",SelectedUsername,SelectedPrinter);
			fclose(fptr);
			}
		else
			{
				printf("%s does not appear to be a valid ini file, this can be corrected with %s -i\n", inifile, ProgName);
			};
		fclose(fptr);
return 0;
};

int LoadDefaultPrinter()
{
	strcpy(SelectedUsername,getenv("USERNAME"));
 	#ifdef _DEBUG_
 	printf("Your Username: %s", SelectedUsername);
 	#endif

	if ((fptr = fopen(inifile,"r")) == NULL)
			{
				printf("\nCannot open %s for reading, please correct and try again.\nIf file does not exist it can be created with: %s -i", inifile);
				exit(1);
		};
				fgets(iniline,sizeof(iniline),fptr); /* Read First Line of inifile */
		if ( !strcmp(iniline, INIHEADER)) /* is it a valid ini file? */
					{
					while ( fgets(iniline,sizeof(iniline),fptr) != NULL )
					{
					tempname=strtok(iniline,"=");
					#ifdef _DEBUG_
					printf("tempname = %s\n",tempname);
					#endif
					if (!strcmpi(tempname,SelectedUsername))
						{
						strcpy(SelectedPrinter,strtok(NULL,"="));
						fclose(fptr);
						};
						#ifdef _DEBUG_
						printf("Configured Default Printer:%s\n",SelectedPrinter);
						#endif
						RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows",0,KEY_ALL_ACCESS,&hKey);
						RegSetValueEx(hKey,"Device",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)-1));
						RegCloseKey(hKey);
						RegOpenKeyEx(HKEY_CURRENT_USER,"Printers",0,KEY_ALL_ACCESS,&hKey);
						RegSetValueEx(hKey,"DeviceOld",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)-1));
						RegCloseKey(hKey);
					};
					fclose(fptr);
					}
				else
					{
						printf("%s does not appear to be a valid ini file, this can be corrected with %s -i\n", inifile, ProgName);
					};
		fclose(fptr);
return 0;
};