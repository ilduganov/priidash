
/**********************************************************************************************
    This file is part of PriiDash(TM), an enhanced instrument panel and
    data logger for the Prius and more.
    Copyright (C) 2011 by TeamPrii at sourceforge.net

    PriiDash(TM) is provided "AS IS" under the terms of
    the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version, WITH ADDITIONAL TERMS in
    paragraphs below.

    This program is distributed in the hope that it will be useful,
    but Without Any Warranty; without even the implied warranty of
    Merchantability or Fitness For A Particular Purpose.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    ADDITIONAL TERMS

    IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, DEATH; INJURY; COLLISION AND/OR DAMAGE TO VEHICLES;
    LIABILITY FOR ACCIDENTS AND/OR TRAFFIC VIOLATIONS; PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE OR
    INABILITY TO USE THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    THE AUTHOR RESERVES ALL RIGHTS OF OWNERSHIP OF THE NAME OF THE SOFTWARE, PriiDash(TM).
    ALL MODIFIED VERSIONS MUST BE CLEARLY MARKED AS BEING DIFFERENT FROM THE ORIGINAL,
    AND MUST CARRY ACKNOWLEDGEMENT TO THE ORIGINAL AUTHOR, TeamPrii at sourceforge.net.
    ALL ADVERTISING MATERIALS MENTIONING FEATURES OR USE OF THIS SOFTWARE
    MUST DISPLAY THE FOLLOWING ACKNOWLEDGEMENT:
    This product includes the software PriiDash(TM) developed by
    TeamPrii at sourceforge.net.

    To contact the author: look up TeamPrii at sourceforge.net.
    Project home page: http://priidash.sourceforge.net/
************************************************************************************************/

// OBD.cpp --- communication between OBDUSB and computer
#include "OS.h"
#include <stdio.h>
#include <fcntl.h>   /* File control definitions */
#ifdef VC
#include <windows.h>
HANDLE hCOM; // shared by all functions and only one COM port is used during one session
 int write(int fd, const char * ptr, int numbytes){//mimic UNIX function //<unistd.h>
	 DWORD nBytesWritten;
	 if (WriteFile(hCOM,ptr,numbytes,&nBytesWritten,NULL)){
		 return(nBytesWritten); // success
	 } else {return(-1);} // fail
 }
 int read(int fd, void * ptr, int numbytes){//mimic UNIX function //<unistd.h>
	 DWORD nBytesRead;
	 if (ReadFile(hCOM,ptr,numbytes,&nBytesRead,NULL)){
		 return(nBytesRead); // success
	 } else {return(-1);} // fail
 }
#else
#include <unistd.h>  /* UNIX standard function definitions */
#include <termios.h> /* POSIX terminal control definitions */
#endif
#include <string.h>  /* String function definitions */
#include "Dash.h"
#include "Util.h"
#include "CANmsg.h"
#include "OBD.h"
#include "LogMsg.h"
//#define DEBUGtiming
//#define DEBUGreading
//#define DEBUGreadATMA
//#define DEBUGpauseATMA
//#define DEBUGsendcmd

int openport(const char *portname) {
    int fd; /* File descriptor for the port */ // not used for Win32, just to mimic UNIX syntax
#ifdef VC
	// http://msdn.microsoft.com/en-us/library/aa363201
	hCOM = CreateFile( portname,                    
					GENERIC_READ | GENERIC_WRITE,
                    0,    //  must be opened with exclusive-access
                    NULL, //  default security attributes
                    OPEN_EXISTING, //  must use OPEN_EXISTING
                    0,    //  not overlapped I/O
                    NULL  //  hTemplate must be NULL for comm devices
					);
	if (hCOM == INVALID_HANDLE_VALUE){ // error opening port; abort
		fd = -1;
	} else {fd = 1;}
#else
    struct termios options;

    fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);
#endif
    if (fd == -1) {
        printf("openport: Unable to open %s\n",portname );  // Could not open the port.
    } else {
#ifdef VC
		DCB dcb={0};
		if(GetCommState(hCOM,&dcb)){// get current DCB in use
			if(BuildCommDCB("baud=2000000 parity=N data=8 stop=1",&dcb)){
			} else {printf("Failed to set DCB for COM port!\n");}
		} else { printf("Failed to get DCB for COM port!\n");}
		COMMTIMEOUTS CommTimeouts;
		CommTimeouts.ReadIntervalTimeout = MAXDWORD;
		CommTimeouts.ReadTotalTimeoutMultiplier = MAXDWORD;
		CommTimeouts.ReadTotalTimeoutConstant = 1;
		CommTimeouts.WriteTotalTimeoutMultiplier = 0;
		CommTimeouts.WriteTotalTimeoutConstant = 0;
		if (SetCommTimeouts(hCOM, &CommTimeouts)){// set timeout
		} else { printf("Failed to set timeout for COM port!\n"); }
#else
        fcntl(fd, F_SETFL, 0);

        //http://www.easysw.com/~mike/serial/serial.html
        tcgetattr(fd, &options);// Set options for the port ---- may need more work

        //No parity (8N1):
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;

        cfsetispeed(&options, B1000000);
        cfsetospeed(&options, B1000000);

//        options.c_cflag &= ~CNEW_RTSCTS; //disable hardware flow control:

        options.c_cflag |= (CLOCAL | CREAD); //The c_cflag member contains two options that should always be enabled, CLOCAL and CREAD. These will ensure that your program does not become the 'owner' of the port subject to sporatic job control and hangup signals, and also that the serial interface driver will read incoming data bytes.
        options.c_lflag &= !(ICANON | ECHO | ECHOE | ISIG); //Choosing Raw Input
        options.c_oflag &= !(OPOST);//Choosing Raw Output
        options.c_cc[VMIN] = 0; // minimum num of char to read
        options.c_cc[VTIME] = 1; // timeout in 1/10 sec

        tcsetattr(fd, TCSANOW, &options);
#endif
    }
    return (fd);
}

void sendcmd(const char *cmd, int fd, FILE *fplog) {
#ifdef DEBUGsendcmd
    fprintf(fplog, "***write() %s command...", cmd);
#endif
    int n = write(fd, cmd, strlen(cmd));
    if (n < 0) {
        fprintf(fplog, "\n%s %s failed!\n",ID_sendcmderror_Str, cmd);
    }
#ifdef DEBUGsendcmd
    else {
        fprintf(fplog, " done!\n");
    }
#endif
}

void sendsettingscmd(const char *cmd, int fd, FILE *fplog) { // send OBD command, read reply and log to file but no car data extraction
#ifdef DEBUGtiming
    char str[1024];
    double mytime0 = gettime();
//    logtime(mytime0, fplog); mytime0 = gettime();
#endif
    sendcmd(cmd, fd, fplog);
    readandlogdata(fd, fplog);
    //car_data cd;
    //readquerydata(fd,fplog,&cd,NULL);
#ifdef DEBUGtiming
    sprintf(str,"sendsettingscmd(): End of %s.",cmd);
    logtime(mytime0, fplog, str);
#endif
}

int sendquerycmd(const char *cmd, int fd, FILE *fplog, car_data *cd, Gauges *gs) {// send, read, extract data and log messages in request/reply mode
	int n;
#ifdef DEBUGtiming
    char str[1024];
    double mytime0 = gettime();
//    logtime(mytime0, fplog); mytime0 = gettime();
#endif

    sendcmd(cmd, fd, fplog);
    n=readquerydata(fd, fplog, cd, gs);

#ifdef DEBUGtiming
    sprintf(str,"sendquerycmd(): End of %s.",cmd);
    logtime(mytime0, fplog, str);
#endif
	return(n);
}

int readandlogdata(int fd, FILE *fplog) {// read and log messages in log file
    int n=0, ntot=0;
#ifdef DEBUGreading
    int j=0;
#endif
    const int maxLineLength = 1024, max_ntot = 2048;
    char str[maxLineLength + 1];

    do {
        //wait(0.000005);// give the port some time to collect data
        //printf("try to read ... ");
        n = read(fd, str, maxLineLength);
        //printf("read() retured %d\n", n);
        if (n > 0) {
#ifdef DEBUGreading
            j++;
#endif
            ntot += n;
            str[n] = '\0';
            //printf("I have read \n%s\n", str);
            //printf("length of read str is %d\n", strlen(str));
            fprintf(fplog, "%s", str);
        }
    } while ((n > 0) && (str[n-1]!='>') && (ntot<max_ntot) );
    //    fprintf(fplog, "\n***total length of read is %d\n\n", ntot);
#ifdef DEBUGreading
    fprintf(fplog, "readandlogdata(): # of reads= %d\n", j);
#endif
    //fflush(fplog);
    return ntot;
}

int readquerydata(int fd, FILE *fplog, car_data *cd, Gauges *gs) {// read, extract data and log messages in request/reply mode
    int n=0, ntot=0;
#ifdef DEBUGreading
    int j=0;
#endif
    const int maxLineLength = 1024, max_ntot = 20480;
    char str[maxLineLength + 1], *readpt=str;
    bool gotCompleteMsg;

    do {
        //wait(0.000005);// give the port some time to collect data
        //printf("try to read ... ");
        n = read(fd, readpt, maxLineLength);
        //printf("read() retured %d\n", n);
        if (n > 0) {
#ifdef DEBUGreading
            j++;
#endif
            ntot += n;
            readpt[n] = '\0';
            //printf("I have read \n%s\n", str);
            //printf("length of read str is %d\n", strlen(str));
            fprintf(fplog, "%s", readpt);
            readpt += n;
        }
    } while ((n > 0) && (*(readpt-1)!='>') && (ntot<max_ntot) );
    //    fprintf(fplog, "\n***total length of read is %d\n\n", ntot);
#ifdef DEBUGreading
    fprintf(fplog, "readquerydata(): # of reads= %d\n", j);
#endif
    //fflush(fplog);

    readpt = str;
    gotCompleteMsg = false;
    do {
        n = strcspn(readpt, "\n");
        if (readpt[n] == '\n'){
            gotCompleteMsg = true;
            readpt[n] = '\0';
            cd->decode(readpt, gs);
            readpt += (n+1);
        } else{
            gotCompleteMsg = false;
        }
    } while ((n>0) && gotCompleteMsg);

    return ntot;
}

int readATMAdata(char *str, const int maxlen, int *partialmsglen, int fd, FILE *fplog, car_data *cd, Gauges *gs){
    // read ATMA messages into string then log them and extract data from them, leave remaining partial message at beginning of str and return its length in *partialmsglen
    char *msgptr, *logptr; // pointer to the beginning of message and the log (the end of log may have partial message)
    int n=0, ntot=0; 
    int processlen = maxlen/2; // read and log this length before extracting data

#ifdef DEBUGpauseATMA
    int j = 0;
    printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\bATMA Reading     ");fflush(stdout);
#endif
#ifdef DEBUGreadATMA
    FILE *fpdbg;
    fpdbg = fopen("/media/Home/OBDlog/debug.txt", "a");
    if (fpdbg==NULL) {printf("!!! Error open file \"/media/Home/OBDlog/debug.txt\" !!!\n");}
#endif

    msgptr = str; // beginning of message is always at beginning of str[]
    logptr = str + *partialmsglen; // if there is partial message at beginning of str then skip logging since it should have been logged previously

    do {
        //wait(0.000005);// don't need to give the port some time to collect data - ATMA is flooded with data
#ifdef DEBUGpauseATMA
    printf("\b\b\b\b\b*****",j);fflush(stdout);
#endif
        n = read(fd, logptr, processlen);
        if (n > 0) {
#ifdef DEBUGpauseATMA
    j++;
    printf("\b\b\b\b\b%5d",j);fflush(stdout);
#endif
            ntot += n;
            logptr[n] = '\0'; // add termination so fprintf know the length to write to log file
#ifdef DEBUGpauseATMA
    printf("\b\b\b\b\b\b\b\b\b\b\b\bSaving....  ");fflush(stdout);
#endif
            fprintf(fplog, "%s", logptr);
#ifdef DEBUGpauseATMA
    printf("\b\b\b\b\b\b: Done");fflush(stdout);
#endif
            logptr += n;
        }
    } while ( (n>0) && (ntot<processlen) && (*(logptr-1)!='>') );
    //    fprintf(fplog, "\n***total length of read is %d\n\n", ntot);
    //    fprintf(fplog, "\n***# of reads= %d\n\n", j);
    //fflush(fplog);

#ifdef DEBUGpauseATMA
    printf("\b\b\b\b\b\b\b\b\b\b\b\bDecoding....");fflush(stdout);
#endif
    bool gotCompleteMsg = false;
    do {
        n = strcspn(msgptr, "\n"); //  number of characters before first "\n" or length of str if not found
        if (msgptr[n]=='\n') {
            gotCompleteMsg = true;
            msgptr[n] = '\0';
            cd->decode(msgptr, gs);
#ifdef DEBUGreadATMA
            fprintf(fpdbg, "%s", msgptr);
#endif
            msgptr += (n + 1); // skip the '\n' as well to the beginning of the next line
        } else{
            gotCompleteMsg = false;
        }
    } while ((n>0) && gotCompleteMsg);

    *partialmsglen = logptr - msgptr;
    if (*partialmsglen > 0){
        memcpy(str, msgptr, *partialmsglen);
        if (*partialmsglen != n){fprintf(fplog,"\n%s wrong algorithm in readATMAdata()!!! *partialmsglen(=%d) != n(=%d)\n",ID_ATMAerror_Str,*partialmsglen,n);}
    }
    else if (*partialmsglen < 0){fprintf(fplog,"\n%s wrong algorithm in readATMAdata()!!! *partialmsglen(=%d) < 0\n",ID_ATMAerror_Str,*partialmsglen);}

#ifdef DEBUGreadATMA
    fclose(fpdbg); // = fopen("/media/Home/OBDlog/debug.txt", "a");
#endif
#ifdef DEBUGpauseATMA
    printf("\b\b\b\b\b\b\b\b\b\b\b\bDone        ");fflush(stdout);
#endif

    return ntot;
}

void initOBD(int fd, FILE *fplog){// initialize OBDUSB device
    sendcmd("\r", fd, fplog); readandlogdata(fd, fplog);// send \r to wake it up
    sendcmd("ATZ\r", fd, fplog); wait(1.0); readandlogdata(fd, fplog);// ATZ reset need extra time lowest 0.8 sec tested OK
                //sendsettingscmd("AT PP 01 SV 00\r", fd, fplog);// set headers ON by default, only need to do once
                //sendsettingscmd("AT PP 01 ON\r", fd, fplog);
    sendsettingscmd("STSN\r", fd, fplog); //show device serial number
    sendsettingscmd("STSLCS\r", fd, fplog); //show PowerSave configuration
    sendsettingscmd("STI\r", fd, fplog); //show firmware version
    sendsettingscmd("STDI\r", fd, fplog); //show device version
    sendsettingscmd("ATPPS\r", fd, fplog); //show programmable parameters list
    sendsettingscmd("ATL1\r", fd, fplog); // turn on line feed
    sendsettingscmd("ATSP6\r", fd, fplog); // set protocol to #6
    sendsettingscmd("ATDPN\r", fd, fplog); // check protocol should be 6
    sendsettingscmd("ATH1\r", fd, fplog); // turn on header
    sendsettingscmd("ATCAF0\r", fd, fplog); // turn off formatting
    //sendsettingscmd("ATR0\r", fd, fplog); // turn off waiting for car to reply for queries
    sendsettingscmd("ATST FF\r", fd, fplog); // set timeout in units of 4 ms; Flyer suggested using FF, try shorter timeout in case Fujitsu goes to sleep erroneously
    sendsettingscmd("ATCM 00000000\r", fd, fplog); // set CAN ID mask
}

void queryOBD(int fd, FILE *fplog, car_data *cd, Gauges *gs){// query for data i.e. request/reply mode
    // alternate between groups to reduce gap between ATMA sessions and query some parameters more often than others
    //static bool gp1 = true;
    static int gp1 = 4, gp2 = 99, gp3 = 1;

    switch(gp1){
        case 1:{gp1=3;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("03010E04\r", fd, fplog);/*query IGN & load*/}break;
        // does not work case 2:{gp1=3;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("05010E0421CD\r", fd, fplog);/*query IGN & load & torque*/}break;
        //case 1:{gp1=2;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("02010E\r", fd, fplog);/*query IGN*/}break;
        //case 2:{gp1=3;/*sendsettingscmd("ATSH7E0\r", fd, fplog);*/sendcmd("020104\r", fd, fplog); /*query ICE load*/}break;
        case 3:{gp1=4;/*sendsettingscmd("ATSH7E0\r", fd, fplog);*/sendcmd("0221CD\r", fd, fplog); /*query ICE torque (N-m)*/}break;
        case 4:{gp1=1;
            switch(gp2){
                case 1:{gp2=4;sendsettingscmd("ATSH7E2\r", fd, fplog);sendcmd("0421C3C442\r", fd, fplog);/*query inverter & MG T & 12V*/}break;
                //case 1:{gp2=2;sendsettingscmd("ATSH7E2\r", fd, fplog);sendcmd("0221C3\r", fd, fplog);/*query inverter & MG T*/}break;
                //case 2:{gp2=4;sendsettingscmd("ATSH7E2\r", fd, fplog);sendcmd("0221C4\r", fd, fplog); /*query converter T*/}break;
                //case 3:{gp2=4;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("020105\r", fd, fplog);/*query ICE coolant T*/}break;
                case 4:{gp2=6;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("05213C3E2E32\r", fd, fplog);/*query catT1&2 EVAP*/}break;
                //case 4:{gp2=6;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("03213C3E\r", fd, fplog);/*query catT1*/}break;
                //case 5:{gp2=6;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("02213E\r", fd, fplog);/*query catT2*/}break;
                case 6:{gp2=99;sendsettingscmd("ATSH7E3\r", fd, fplog);sendcmd("0421d0cecf\r", fd, fplog);/*query HV battery*/}break;
                //case 6:{gp2=7;sendsettingscmd("ATSH7E3\r", fd, fplog);sendcmd("0221d0\r", fd, fplog);/*query HV battery*/}break;
                //case 7:{gp2=8;sendsettingscmd("ATSH7E3\r", fd, fplog);sendcmd("0221ce\r", fd, fplog);/*query HV battery*/}break;
                //case 8:{gp2=9;sendsettingscmd("ATSH7E3\r", fd, fplog);sendcmd("0221cf\r", fd, fplog);/*query HV battery*/}break;
                //case 9:{gp2=10;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("02212E\r", fd, fplog);/*query EVAP purge VSV*/}break;
                //case 10:{gp2=99;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("022132\r", fd, fplog);/*query EVAP vapor pressure*/}break;
                case 99:{gp2=1;
                    switch (gp3){
                        case 1:{gp3=2;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("020105\r", fd, fplog);/*query ICE coolant T*/}break;
                        case 2:{gp3=3;sendsettingscmd("ATSH7E0\r", fd, fplog);sendcmd("032133F5\r", fd, fplog);/*query atm, misfire*/}break;
                        case 3:{gp3=1;sendsettingscmd("ATSH7E3\r", fd, fplog);sendcmd("022131\r", fd, fplog);/*DTC clear run dist*/}break;
                        //case 1:{gp3=2;sendsettingscmd("ATSH7E2\r", fd, fplog);sendcmd("022142\r", fd, fplog); /*query 12V*/}break;
                        default:{gp3=1;}break;
                    }
                }break;
                default:{gp2=1;}break;
            }
        }break;
        default:{gp1=1;}break;
    }
//    sendsettingscmd("ATCAF1\r", fd, fplog); // turn on formatting
//    sendsettingscmd("ATSH7E2\r", fd, fplog); // set header
//    sendquerycmd("21C3\r", fd, fplog, cd, gs); //query inverter & MG T
//    sendquerycmd("21C4\r", fd, fplog, cd, gs); //query converter T
//    sendcmd("0221C3\r", fd, fplog); /*query inverter & MG T*/
//    sendcmd("0221C4\r", fd, fplog); /*query converter T*/
//    if (gp1) {
//            sendsettingscmd("ATSH7E2\r", fd, fplog); // set header

//        sendquerycmd("2142\r", fd, fplog, cd, gs); //query 12V
//        sendcmd("022142\r", fd, fplog); /*query 12V*/

        //sendsettingscmd("ATSH7E0\r", fd, fplog); //set header
        //sendquerycmd("2133\r", fd, fplog, cd, gs); //query atm
        //sendquerycmd("213C\r", fd, fplog, cd, gs); //query catT1
        //sendquerycmd("213E\r", fd, fplog, cd, gs); //query catT2
//        gp1 = false;
//    } else {
        //sendquerycmd("ATRV\r",fd, fplog, cd, gs);
        //sendsettingscmd("ATSH7E0\r", fd, fplog); //set header
//        sendsettingscmd("ATSH7E0\r", fd, fplog); //set header
//        sendquerycmd("2133\r", fd, fplog, cd, gs); //query atm
//        sendquerycmd("213C\r", fd, fplog, cd, gs); //query catT1
//        sendquerycmd("213E\r", fd, fplog, cd, gs); //query catT2
//        switch (gp2){
//            case 1:{sendcmd("022133\r", fd, fplog);/*sendquerycmd("2133\r", fd, fplog, cd, gs);*/ gp2=2;}break;
//            case 2:{sendcmd("02213C\r", fd, fplog);/*sendquerycmd("213C\r", fd, fplog, cd, gs);*/ gp2=3;}break;
//            case 3:{sendcmd("02213E\r", fd, fplog);/*sendquerycmd("213E\r", fd, fplog, cd, gs);*/ gp2=1;}break;
//        };
        //sendcmd("2133\r", fd, fplog);
        //sendcmd("213C\r", fd, fplog);
        //sendcmd("213E\r", fd, fplog);

 //       gp1 = true;
//    }
}
                    //    sendcmd("ATCAF1\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("ATSH7E2\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("21C3\r", fd, fplog);readdata(fd, fplog);//readdata(fd, fplog);
                    //    sendcmd("21C4\r", fd, fplog);readdata(fd, fplog);//readdata(fd, fplog);
                    //    sendcmd("2142\r", fd, fplog);readdata(fd, fplog);
                    //    sendcmd("ATRV\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("ATSH7E0\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("2133\r", fd, fplog);readdata(fd, fplog);
                    //    sendcmd("213C\r", fd, fplog);readdata(fd, fplog);
                    //    sendcmd("213E\r", fd, fplog);readdata(fd, fplog);

void setupATMA(int fd, FILE *fplog){// set up ATMA
    //sendsettingscmd("ATL1\r", fd, fplog); // turn on line feed
//    sendsettingscmd("ATCAF0\r", fd, fplog); // turn off formatting
//    sendsettingscmd("ATAR\r", fd, fplog); // turn off filters
//    sendcmd("ATL1\r", fd, fplog); // turn on line feed
//    sendcmd("ATCAF0\r", fd, fplog); // turn off formatting
//    sendcmd("ATAR\r", fd, fplog); // turn off filters
    sendcmd("ATMA\r", fd, fplog); // start ATMA
}

                    //    sendcmd("ATL1\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("ATCAF0\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("ATAR\r", fd, fplog);readdataquick(fd, fplog);
                    //    sendcmd("ATMA\r", fd, fplog);
