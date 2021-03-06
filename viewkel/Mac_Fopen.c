/*******************************************************

Copyright (C) 1995 Greg Landrum
All rights reserved

This file is part of yaehmop.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

********************************************************************/
/*************************
  Copyright 1999, Greg Landrum

        this has the files for doing file opening/choosing on the Macintosh

        created by greg Landrum  January 22, 1996

*************************/
#include "viewkel.h"
#include "Mac_Fopen.h"
#include <StandardFile.h>

extern FILE *fopen_mac ( short vRefNum , long parID , char * fileName , char * mode,
                                                                                                char action );

FILE *choose_mac_file(char *file_name,char action)
{
        StandardFileReply reply;
        SFTypeList                type_list;
        short num_types;
        FILE *the_file;

        /* start out by using StandardGetFile to get the file name and stuff */
        num_types = -1;
        type_list[0] = 'YhMp';
        StandardGetFile(nil, num_types, type_list, &reply);
        if (!reply.sfGood)
        {
                strcpy(file_name,"User Cancelled");
                return 0;
        }


        /* now open the file */
        strcpy(file_name,(char *)reply.sfFile.name);
        PtoCstr((unsigned char *)file_name);
        the_file = fopen_mac(reply.sfFile.vRefNum,reply.sfFile.parID,
                                                                                         file_name,(char *)"r",action);

        return the_file;
}


FILE *
fopen_mac ( short vRefNum , long parID , char * fileName , char * mode,
                                                char action )
{
        short oldVol ;
        short aVol ;
        long aDir , aProc ;
        FILE * ret = NULL ;

        /* change to the proper disk and directory */
  if ( GetVol ( NULL , & oldVol ) ) {
     return NULL ;
  }
  if ( GetWDInfo ( oldVol , & aVol , & aDir , & aProc ) ) {
     return NULL  ;
  }
  if ( HSetVol ( NULL , vRefNum , parID ) ) {
     return NULL ;
  }
                if( action == MAC_FOPEN_OPEN_CD || action == MAC_FOPEN_OPEN_NOCD ){
            ret = fopen ( fileName , mode ) ;
           } else{
            ret = 0;
           }

                /* this doesn't seem to work the way I want it to... */
    if( action == MAC_FOPEN_OPEN_NOCD || action == MAC_FOPEN_NOOPEN_NOCD ){
             if ( HSetVol ( NULL, aVol , aDir ) ) {
                /* an error we can't currently handle */
            }
            if ( SetVol ( NULL, oldVol ) ) {
        /* an error we can't currently handle */
            }
    }
    return ret ;
}


