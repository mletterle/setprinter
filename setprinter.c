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




/*********************************************************************************
**                                                                              **
** Program:  SetPrinter V1.0                                                    **
**                                                                              **
** Purpose:  To make setting default printers on a per user basis in Windows NT **
**           easier via a simple selection process rather then registry entry   **
**                                                                              **
** Author:  Michael Letterle <letterle@prokrams.com>                            **
**                                                                              **
** Comments:  It's ugly code, I'm just relearning C again, but it seems to work,**
**            it needs alot more error handling code, someday I'll get to it    **
**                                                                              **
**                                                                              **
**********************************************************************************/


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
        case 'h': printf(USAGE,ProgVer,ProgName,inifile);exit(0); /* Display Help and exit with 0 (USAGE defined in setprinter.h) */
        case 'v': printf("%s\n",ProgVer);exit(1); /* Display Version and exit with 1 */
        case 'g': readregistry();keygen();exit(1); /* Generate regini key files and exit with 1, I don't know why we would even do this anymore, this program kinda makes it obsolete */
        case 'c': readregistry();configuser();exit(1); /* Let's configure a default printer */
        case 'l': LoadDefaultPrinter();exit(1); /* Let's load the default printer from the ini file into the registry */
        case 'i': CreateIniFile();exit(0); /* Do we really need it when it's so easy to do manually? */
        default : printf("%s\nrun %s -h for help\n",ProgVer,ProgName);exit(1); /* Hey those options don't make sense, look at the help */
        };

    };
};







return 0;
};

/*****************************************************************
 *                                                               *
 *     readregistry() function                                   *
 *                                                               *
 *     This function is used for a couple of the options.        *
 *     All it basically does is open up the Devices key and      *
 *     Get the names and spoolers of all the printers and loads  *
 *     them into the printers and spoolers arrays.               *
 *     It also get's the local users on the system and loads them*
 *     into the users array                                      *
 *****************************************************************/


int readregistry()
{


    RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Devices",0,KEY_ALL_ACCESS,&hKey); /* Open the Devices key to the hKey handle */
    RegQueryInfoKey(hKey,NULL,NULL,NULL,NULL,NULL,NULL,&NumofValue,&MaxValueLength,NULL,NULL,NULL); /* all we really want to know is how many printers and how long is the longest name */

    while (RegEnumValue(hKey,keyindex,&PrinterName,&lpcbValueName,NULL,NULL,&Spooler,&lpcbData) != ERROR_NO_MORE_ITEMS ) /* Sometimes reading the code is all you need */
    {

        strcpy(printer[keyindex], PrinterName); /* Copy the printername from the registry into the array */
        strcpy(spooler[keyindex], Spooler); /* Copy the spooler name from the registry into the array */
        keyindex=keyindex+1; /* Increment index number */
        lpcbValueName = MaxValueLength+1; /* Reset lpcbValueName has RegEnumValue changes it's contents when read */
    };
    RegCloseKey(hKey);
    NumOfPrinters=keyindex;
    z=0;
blah = 1;
    while (blah) /* this is where we load the usernames on the system into an array */
    {
        blah = NetQueryDisplayInformation(NULL,1,z,1,1024,&MaxValueLength,&buf) ;
        ndu = (NET_DISPLAY_USER *)buf;
        wcstombs(username[y],ndu->usri1_name,sizeof(username)); /* Took me a while to figure this part out */
        z=z+1;
        y=y+1;
    };
    strcpy(username[y],"setprinter's Default Printer");
    y=y+1;
    NumOfUsers=y;
    z=0;
    return 0;
};

/*******************************************************************
 *                                                                 *
 *     keygen() function                                           *
 *                                                                 *
 *     This is the function that creates the printername.key files *
 *     it would be a really useful function if this program didn't *
 *     basically make key files obsolete, it is included though for*
 *     historical purposes and because it could still be useful    *
 *******************************************************************/



int keygen()
{
printf("Generating KeyFiles\n");
while (z != keyindex)
    {

        tempname = strtok(printer[z],"\\\/ "); /* Next few lines basically just take out the \ / and spaces from printernames so the keyfiles can be written */
        strcpy(printer[z],tempname);
        while ( tempname != NULL)
        {
            tempname = strtok(NULL,"\\\/ ");
            if (tempname != NULL) { strcat(printer[z],tempname);};
        };
        stpcpy(mainfile, printer[z]); /* Here we make the filename the name of the printer with all the bad characters taken out */
        strcat(mainfile,".key");
        printf("Writing %s\n",mainfile);
        if ((fptr = fopen(mainfile,"w")) == NULL)
            {
                printf("\nCannot open %s, for writing.\n", mainfile);
                break;
            };

        fprintf(fptr,KeyFileConst,printer[z],spooler[z],printer[z],spooler[z]); /* Watch the magic as the program creates the keyfile contents */
        fclose(fptr);
        z = z+1;
    };
printf("\n");
return 0;
};


/******************************************************
 *                                                    *
 *      configuser() function                         *
 *                                                    *
 *    This is kind of a magical function, this is were*
 *    we create the ini file entries.  I hope someday *
 *    to be able to make the selection proccess less  *
 *    of a hack, but it seems to work well enough.    *
 ******************************************************/

int configuser()
{
    z=0;
    while (z != NumOfUsers)
        {
         printf("%i:  %s\n",z+1,username[z]); /* I'd really like to find a better way to do this, somekind of select screen or at least have some pageing functionality */
         z=z+1;
     };
     printf("Select Number of User You Wish To Configure:");
     gets(inputS); /* Next few parts are a bad hack, but it works */
     inputI = atoi(inputS); /* convert the string to an interger so we can just drop it into the array */
     inputI = inputI-1; /* Since we made it pretty for the user (No 0) we need to subtract 1 */
     strcpy(SelectedUsername,username[inputI]); /* That's what we picked! */
     #ifdef _DEBUG_
     printf("inputI = %d\nUsername[inputI] = %s\nSelectedUsername = %s\n",inputI,username[inputI],SelectedUsername);
     #endif
    z=0;
    while ( z != NumOfPrinters ) /* Ugh, this part is has bad as the Users routine, sometime i'll do this better */
    {
        printf("%i:  %s\n",z+1,printer[z]);
        z=z+1;
    };
    
    printf("Select Number Of Printer To Be Default For %s:", SelectedUsername);
    gets(inputS);
    inputI = atoi(inputS);
    inputI = inputI-1;
    strcpy(SelectedPrinter,strtok(printer[inputI],"\n"));
   // strcat(SelectedPrinter,",");
   // strcat(SelectedPrinter,spooler[inputI]);
    #ifdef _DEBUG_
    printf("\nSelected Printer: \"%s\"\n",SelectedPrinter);
    #endif
        if ((fptr = fopen(inifile,"r+")) == NULL)
        {
            printf("\nCannot open %s for reading, please correct and try again.\nIf file does not exist it can be created with: %s -i", inifile);
            exit(1);
        };
        fgets(iniline,sizeof(iniline),fptr); /* Read First Line of inifile */
		pos = ftell(fptr);
        if ( !strcmp(iniline, INIHEADER)) /* is it a valid ini file? */
            {
	              z=1;
	            while ( fgets(iniline,sizeof(iniline),fptr) != NULL ) /* If it's a valid ini file let's go through it line by line */
            {
            
	        strcpy(origline,iniline);
	        tempname=strtok(iniline,"="); /* since the equal sign is our field separtor we'll look at the first token (the username) */
            #ifdef _DEBUG_
            printf("tempname = %s\n",tempname);
            #endif
            if (!strcmpi(tempname,SelectedUsername)){ /* If the token equals the username then it must have been set up already, print a message and exit */
                
            	printf("User %s already has a configured printer\nDelete line in setprinter.ini and try again\n",SelectedUsername);
                printf("Would you like to configure another user? [y/n]:");
        		gets(inputS);
        		if(!strcmpi(inputS,"n")){fclose(fptr);exit(0);} else {configuser();};};
                         
	             
	                
            };
            
             
            /* Alright, user isn't previously configured or we decided to reconfigure them, let's move on */
            
            #ifdef _DEBUG_
            printf("Moving on\nValue of pos=%ld\n",pos);
            #endif
          
            if ((fptr = fopen(inifile,"a")) == NULL){printf("\nCannot open %s for updating, please correct and try again.\n");};
            
            
           
            
            #ifdef _DEBUG_
           
            printf("%s=%s\n",SelectedUsername,SelectedPrinter);
            #endif
            fprintf(fptr,"%s=%s\n",SelectedUsername,SelectedPrinter); /* Woo hoo, write that ini file */
            fclose(fptr);
            printf("Would you like to configure another user? [y/n]:");
        	gets(inputS);
        	if(!strcmpi(inputS,"n")){exit(0);} else {configuser();};
            }
            
        else
            {/* If the ini file isn't valid (ie the Header isn't there) spit out an error message */
                printf("%s does not appear to be a valid ini file, this can be corrected with %s -i\n", inifile, ProgName);
            };
        fclose(fptr);
        
        
return 0;
};

/****************************************************
 *                                                  *
 *  LoadDefaultPrinter() function                   *
 *                                                  *
 *  This function works very much like the config   *
 *  user function except that the user doesn't make *
 *  any choices it just snarfs the username from the*
 *  enviroment variable.                            *
 ****************************************************/



int LoadDefaultPrinter()
{
    readregistry();
	strcpy(SelectedUsername,getenv("USERNAME"));/* Copy the %USERNAME% enviroment variable into the SelectedUsername variable */
    #ifdef _DEBUG_
    printf("Your Username: %s\n", SelectedUsername);
    #endif

    if ((fptr = fopen(inifile,"r")) == NULL) /* This was basically cut and pasted from the ConfigUser Function */
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
                    z=0;
                    if (!strcmpi(tempname,SelectedUsername))
                        {
                        isconfig = 1;
	                    strcpy(SelectedPrinter,strtok(NULL,"=\n"));
                        #ifdef _DEBUG_
                        printf("Configured Default Printer:%s\n",SelectedPrinter);
                        #endif 
	                    fclose(fptr);
                        while( strcmpi(SelectedPrinter,printer[z]))
                        {
	                       #ifdef _DEBUG_
	                       printf("Selected Printer: %s, Printer[%d]: %s\n",SelectedPrinter,z,printer[z]);
	                       #endif
	                       z=z+1;
                       };
                       strcat(SelectedPrinter,",");
   					   strcat(SelectedPrinter,spooler[z]);
                       #ifdef _DEBUG_
                       printf("Configured Default Printer,Spooler: %s\n",SelectedPrinter);
                       #endif
                        };
                        /* Look at all that registry Wizardy below, you should see me in my pointy hat */
                        RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows",0,KEY_ALL_ACCESS,&hKey);
                        RegSetValueEx(hKey,"Device",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)));
                        RegCloseKey(hKey);
                        RegOpenKeyEx(HKEY_CURRENT_USER,"Printers",0,KEY_ALL_ACCESS,&hKey);
                        RegSetValueEx(hKey,"DeviceOld",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)));
                        RegCloseKey(hKey);
                    };
                    fclose(fptr);
                    };
                    if (isconfig == 0){
	                    strcpy(SelectedUsername, "setprinter's Default Printer");
	                    if ((fptr = fopen(inifile,"r")) == NULL) /* This was basically cut and pasted from the ConfigUser Function */
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
                    z=0;
                    if (!strcmpi(tempname,SelectedUsername))
                        {
                        isconfig = 1;
	                    strcpy(SelectedPrinter,strtok(NULL,"=\n"));
                        fclose(fptr);
                        
                        #ifdef _DEBUG_
                        printf("Configured Default Printer:%s\n",SelectedPrinter);
                        #endif 
                        
                        while( strcmpi(SelectedPrinter,printer[z]))
                        {
	                       #ifdef _DEBUG_
	                       printf("Selected Printer: %s, Printer[%d]: %s\n",SelectedPrinter,z,printer[z]);
	                       #endif
	                        z=z+1;
                       };
                       strcat(SelectedPrinter,",");
   					   strcat(SelectedPrinter,spooler[z]);
                       #ifdef _DEBUG_
                       printf("Configured Default Printer,Spooler: %s\n",SelectedPrinter);
                       #endif
                        };
                        
                        /* Look at all that registry Wizardy below, you should see me in my pointy hat */
                        RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows",0,KEY_ALL_ACCESS,&hKey);
                        RegSetValueEx(hKey,"Device",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)));
                        RegCloseKey(hKey);
                        RegOpenKeyEx(HKEY_CURRENT_USER,"Printers",0,KEY_ALL_ACCESS,&hKey);
                        RegSetValueEx(hKey,"DeviceOld",0,REG_SZ,SelectedPrinter,(strlen(SelectedPrinter)));
                        RegCloseKey(hKey);
                    };
                    }
                else
                    {
                        printf("%s does not appear to be a valid ini file, this can be corrected with %s -i\n", inifile, ProgName);
                    };
        fclose(fptr);};
return 0;
};

/****************************************************
 *                                                  *
 *  CreateIniFile() function	                    *
 *                                                  *
 *  This function works very much like the config   *
 *  user function except that the user doesn't make *
 *  any choices it just snarfs the username from the*
 *  enviroment variable.                            *
 ****************************************************/
 
int CreateIniFile()
{
	 if((fptr = fopen("setprinter.bak","w")) == NULL)
	 		{
		 		printf("\nCannot open setprinter.bak for writing, you must not have permission");
		 		exit(1);
	 		};
	 			fputs(INIHEADER,fptr);
	 			fclose(fptr);
	 			printf("\nSetPrinter.bak written copy to setprinter.ini to use\n");
return(0);
};
 				
