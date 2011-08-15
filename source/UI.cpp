
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

#include "OS.h"
#include <stdio.h>
#ifdef VC
#include <windows.h>
extern HANDLE hCOM;
int close(int fd){ // mimic UNIX function close() // #include <unistd.h>
	// fd is not used, assume the HANDLE hCOM is shared by all functions and only one COM port is used during one session
	if(CloseHandle(hCOM)){ // close COM port
	return(0); // success
	} else {
		printf("Failed to close COM port!\n");
		return(-1); // fail
	}
}
#else
#include <unistd.h>  /* UNIX standard function definitions */
#endif
#include <math.h>
#include "UI.h"
#include "Dash.h"
#include "Util.h"
#include "CANmsg.h"
#include "OBD.h"
#include "LogMsg.h"
#include "icon.h"
//#define DEBUGtick
//#define DEBUGcsvtime

UIstateType UIstate = Stopped; // Controlled by the Go button in Dash.fl -> Dash.cxx & Dash.h
UIrunType UIruntype = Normal; // Controlled by the run type choice in Dash.fl -> Dash.cxx & Dash.h

char LicText[]="\n"
"    PriiDash(TM)  Copyright (C) 2011 by TeamPrii at sourceforge.net\n"
"    This program is provided \"AS IS\" under the GNU General Public License,\n"
"    WITH ADDITIONAL TERMS in paragraphs below.\n"
"\n"
"    IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY\n"
"    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n"
"    (INCLUDING, BUT NOT LIMITED TO, DEATH; INJURY; COLLISION AND/OR DAMAGE TO VEHICLES;\n"
"    LIABILITY FOR ACCIDENTS AND/OR TRAFFIC VIOLATIONS; PROCUREMENT OF SUBSTITUTE GOODS OR\n"
"    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND\n"
"    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
"    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE OR\n"
"    INABILITY TO USE THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n"
"\n"
"    THE AUTHOR RESERVES ALL RIGHTS OF OWNERSHIP OF THE NAME OF THE SOFTWARE, PriiDash(TM).\n"
"    ALL MODIFIED VERSIONS MUST BE CLEARLY MARKED AS BEING DIFFERENT FROM THE ORIGINAL,\n"
"    AND MUST CARRY ACKNOWLEDGEMENT TO THE ORIGINAL AUTHOR, TeamPrii at sourceforge.net. \n"
"    ALL ADVERTISING MATERIALS MENTIONING FEATURES OR USE OF THIS SOFTWARE\n"
"    MUST DISPLAY THE FOLLOWING ACKNOWLEDGEMENT:\n"
"    This product includes the software PriiDash(TM) developed by \n"
"    TeamPrii at sourceforge.net.\n";


void init_filenames(){ // initialize log and csv filenames
    
	        //o->value("/media/Home/OBDlog/test.txt");
        char str[1024],str2[16]; getdatetimestr(str2);
		#ifdef VC
		sprintf(str,"f:\\OBDLog\\log%s.txt",str2);
		#else
        sprintf(str,"/cygdrive/f/OBDLog/log%s.txt",str2);
		#endif
        log_filename->value(str);
		#ifdef VC
		sprintf(str,"f:\\OBDLog\\log%s.csv",str2);
		#else
        sprintf(str,"/cygdrive/f/OBDLog/log%s.csv",str2);
		#endif
        csv_filename->value(str);
}

void setUIstate(UIstateType UIst) {
    switch (UIst) {
        case Stopped:{
            go_button->deactivate();
            go_button->copy_label("Stopping");
            printf("UI stopping\n");
        }
            break;
        case Running:{
            settings_group->deactivate();
            run_type_choice->deactivate();
            go_button->deactivate();
            go_button->copy_label("Starting");
            printf("UI starting\n");

            init_filenames();
        }
            break;
    }
}

static void simulategauges(Gauges *gs, car_data *cd){
    static double wT = 100, dwT = 1, rpm = 0, drpm = 10, ign = 0, dign = 0.1, speed = 30, dspeed = .2; // fake car data
    static double soc = 10, dsoc = 5, amp = 0, damp = .2, hvV = 200, dhvV = 0.1;
    static double auxV = 12, dauxV = 0.1, inv1T = 0, dinv1T = 1, inv2T = 0, dinv2T = 2;
    static double t1 = gettime();
    static double fuel = 11;

    wait(0.005);
    //Fl::lock();
    (gs->wT_G)->value(wT);
    (gs->iceT_G)->value2(wT);// + 10);    //(gs->wT_G)->redraw();
    (gs->iceT_G)->value(wT + 10);   //(gs->iceT_G)->redraw();
    (gs->rpm_G)->value(rpm);        //(gs->rpm_G)->redraw();
    gs->rpm2_G->value(rpm+50);
    gs->rpm_G->value2(rpm+50);
    (gs->psd_G)->value(rpm);
    (gs->psd_G)->value2(rpm/2);
    (gs->psd_G)->value1(2.3*rpm);
    gs->mg1rpm_G->value(2.3*rpm);
    gs->mg2rpm_G->value(rpm/2);
    (gs->ign_G)->value(ign);        //(gs->ign_G)->redraw();
    (gs->speed_G)->value(speed);    //(gs->speed_G)->redraw();
    (gs->soc_G)->value(soc);        //(gs->soc_G)->redraw();
    (gs->amp_G)->value(amp);        //(gs->amp_G)->redraw();
    (gs->hvV_G)->value(hvV);        //(gs->hvV_G)->redraw();
    (gs->auxV_G)->value(auxV);      //(gs->auxV_G)->redraw();
    (gs->inv1T_G)->value(inv1T);   
    //(gs->inv1T_G)->value2(inv2T);   //(gs->inv1T_G)->redraw();
    (gs->inv2T_G)->value(inv2T);    //(gs->inv2T_G)->redraw();
    gs->mg1T_G->value(inv1T);
    gs->mg2T_G->value(inv2T);
    gs->cnvT_G->value(inv1T);
    gs->blC_G->value(inv2T);
    gs->buC_G->value(inv1T);
    gs->catT1_G->value(inv1T*10);
    gs->catT2_G->value(inv2T*10);
    gs->oaT_G->value(inv1T);
    gs->intakeT_G->value(inv2T);
    gs->atm_G->value(wT);
    gs->fuel_G->value(fuel);

    cd->speed = speed;
    cd->lps = 1000;
    cd->rpm = rpm;

    //Fl::check();//XRequest.152: RenderBadPicture BadIDChoice
    //Fl::flush();
    //Fl::unlock();
    fflush(stdout);
    wT += dwT; if (wT > 240) { dwT = -1; } else if (wT < 90) { dwT = 1; }
    rpm += drpm; if (rpm > 2700) { drpm = -10; } else if (rpm <= 0) { drpm = 10; }
    ign += dign; if (ign > 25) { dign = -0.1; } else if (ign<-15) { dign = 0.1; }
    speed += dspeed; if (speed > 50) { dspeed = -.192; } else if (speed < 31) { dspeed = .192; }
    soc += dsoc; if (soc > 90) { dsoc = -.5; } else if (soc < 10) { dsoc = .5; }
    amp += damp; if (amp > 100) { damp = -.42; } else if (amp<-100) { damp = .42; }
    hvV += dhvV; if (hvV > 210) { dhvV = -.1; } else if (hvV < 190) { dhvV = .1; }
    auxV += dauxV; if (auxV > 15) { dauxV = -.1; } else if (auxV < 11) { dauxV = .1; }
    inv1T += dinv1T; if (inv1T > 105) { dinv1T = -1; } else if (inv1T<-11) { dinv1T = 2; }
    inv2T += dinv2T; if (inv2T > 105) { dinv2T = -2; } else if (inv2T<-11) { dinv2T = 1; }
    fuel -= 0.05; if(fuel<=0){fuel = 11;}


    double t2 = gettime();
    printf("Timing: simulate one iteration = %10d (us), current time=%8.6lf\n",int(((t2 - t1)*1000000)),t2);
    /*printf("iceT(%d,%d),rpm(%d,%d,%d),Timing: simulate one iteration = %10d (us), current time=%8.6lf\n",
            gs->iceT_G->redlinemode(),gs->iceT_G->dialcolor(),
            gs->rpm_G->redlinemode(),gs->rpm_G->dialcolor(),gs->rpm_G->dialcolorR(),
            int(((t2 - t1)*1000000)),t2);*/
    t1 = t2;
    //printf(" %5.1f\n", wT);

}


static void idle_print(){
    static int i=0;
    switch (i){
        case 0:{i=1;printf("xxx");}break;
        case 1:{i=2;printf("\b\b\b");}break;
        case 2:{i=3;printf("ooo");}break;
        case 3:{i=0;printf("\b\b\b");}break;
    }
}

void showLic(){

    printf("showLic() started.\n"); fflush(stdout);
    wait(0.8); // wait till other windows have shown
    if (!about_window) {make_about_window();}
    
    //about_window->fullscreen();
    Fl::lock(); seticonfixed(about_window); about_window->show(); Fl::flush(); Fl::unlock();
    while (!QuitShowLic){wait(0.5);}
    printf("showLic() ended.\n"); fflush(stdout);
}

void runUI() {// main loop started in a separate thread from the main control window
    FILE *fplog = NULL, *fpcsv = NULL;
    int fd=-1; /* File descriptor for the port */
    double mytime0=gettime(), querytick1, querytick2;
    const double querytickinterval = 0.2; // query PID data every querytickinterval (sec)
    double atmastoptick; // set timeout after sending '\r' to stop ATMA
    double csvtick1, csvtick2;
    const double csvtickinterval = 1.0; // save csv data every csvtickinterval (sec)
    double miletick;
    const double mileinterval = km2mile * 3; //save additional oaT every mileinterval (miles) (maybe off by one km2mile unit)
    bool miletickset = false;
    Gauges gs; // Gauges window class
    car_data cd; // car data class
    int n=0, ntot=0, partialmsglen=0;
    const int maxlen = 1024;
    char str[maxlen+1], msg[maxlen], clockstr[20];
    char logbuffer[64*BUFSIZ];

    printf("runUI() started.\n"); fflush(stdout);

    
    Fl::lock(); seticonfixed(gs.gauges_window); gs.show(); /*aw.show();*/ Fl::unlock();// show Gauges window
    

    while (!QuitProgram) {

        n=0; ntot=0; partialmsglen=0;

        switch (UIstate) {
            case Running:{
                mytime0=gettime();
                //Fl::lock(); gs.show(); Fl::unlock();// show Gauges window
                settings_group->deactivate();
                run_type_choice->deactivate();

                fpcsv = fopen(csv_filename->value(), "a");
                if (fpcsv==NULL) {printf("!!! Error open file \"%s\" !!!\n",csv_filename->value());}
                fplog = fopen(log_filename->value(), "a");
                if (fplog==NULL) {printf("!!! Error open file \"%s\" !!!\n",log_filename->value());}
                setbuf(fplog,logbuffer);
                logtime(mytime0, fplog, "Beginning of runUI(), UIstate==Running, log & csv files opened.");

                switch(UIruntype){
                    case Simulation: {
                        printf("Running in simulation mode.\n");
                        wait(0.5);
                    }
                    break;
                    case Normal: {
                        printf("Running in normal mode.\n");
                        //if (fd != -1){ close(fd); }
                        fd = openport(OBDUSB_portname->value());// open and set up serial port
                        if (fd == -1){ 
                            printf("!!! Error open port %s -> UI stopping.\n",OBDUSB_portname->value());
                            UIstate = Stopped;
                            goto End_Running;
                        }
                        initOBD(fd, fplog);// initialize OBDUSB device
                        csvtick2 = floor(gettime()-mytime0)+mytime0; // initialize starting time of csv data writing
                        cd.csv_title(fpcsv);
                    }
                    break;
                }
                printf("*\n");fflush(stdout);
                go_button->copy_label("Stop");
                go_button->activate();
                gs.go_button->copy_label("Stop");

                cd.init_odo_avgmpg();

                querytick2 = floor(gettime()-mytime0)+mytime0; // initialize starting time of loop of alternating request/reply & ATMA
                
                do {// loop of alternating between OBD query and ATMA
                    querytick1 = logtime(mytime0, fplog, "Set querytick1 at start of loop of alternating OBD query & ATMA.");
                    do{querytick2 += querytickinterval;}while(querytick1>=querytick2);
#ifdef DEBUGtick
                        fprintf(fplog,"\n%s querytick1 = %8.6lf, querytick2 = %8.6lf\n",ID_querytick_Str,querytick1,querytick2);
                        printf("\rQuerying                   ");fflush(stdout);
#endif
                    cd.update_odo_avgmpg(&gs);

                    getdatetimeclockstr(clockstr);
                    gs.clock_G->value(clockstr);
                    gs.clock_G->redraw();// need redraw because it is not Fl_Gauge, just text output
                    //gs.speed_G->value(floor(cd.speed));
                    //sprintf(speeddecstr,"%d",(int)floor(cd.speed*10-floor(cd.speed)*10));
                    //gs.speeddec_G->value(speeddecstr);
                    //gs.speeddec_G->redraw();// need redraw because it is not Fl_Gauge, just text output

                    if (UIruntype==Normal){
                        queryOBD(fd, fplog, &cd, &gs); // query OBD data
                        /*csvtick1 = gettime();
                        if (csvtick1>csvtick2){
                            cd.timeDelta = logtime(mytime0,fplog)-mytime0;
                            cd.csv_data(fpcsv);
                            do{csvtick2 += csvtickinterval;} while (csvtick1>csvtick2);
                        }*/
                        //setupATMA(fd, fplog); // set up ATMA
                    }
                    n=0; ntot=0; // initialize counter for readATMAdata()
                    logtime(mytime0, fplog, "queryOBD() done. Entering loop of ATMA.");
#ifdef DEBUGtick
                        printf("\rATMAloop                   ",ntot);fflush(stdout);
#endif
                    do {// loop of ATMA
                        switch (UIruntype){
                            case Simulation:{  simulategauges(&gs, &cd);/*printf(".");fflush(stdout); */
                                cd.timeDelta = 60*(gettime() - mytime0); // simulate faster time evolution
                                gs.timeDelta_G->value(cd.timeDelta);
                                }
                            break;
                            case Normal:{
                                n = readATMAdata(str, maxlen, &partialmsglen, fd, fplog, &cd, &gs);
                                if(n>0){ntot += n;}
                                csvtick1 = gettime();
#ifdef DEBUGtick
                                printf("\rCheching CSV time tick         ",ntot);fflush(stdout);
                        fprintf(fplog,"\n%s csvtick1 = %8.6lf, csvtick2 = %8.6lf\n",ID_csvtick_Str,csvtick1,csvtick2);
#endif
                                if (csvtick1>csvtick2){
#ifdef DEBUGcsvtime
                                    cd.timeDelta=logtime(mytime0,fplog,"Saving csv line.")-mytime0;
#else
                                    cd.timeDelta=gettime()-mytime0;
#endif
                                    gs.timeDelta_G->value(cd.timeDelta);
                                    cd.csv_data(fpcsv); // save one line of csv data
                                    if (miletickset){
                                        if(cd.dtcdist>miletick+mileinterval){
                                            miletick=cd.dtcdist; cd.csv_oaTs();
                                        }
                                    }
                                    else if (cd.dtcdistcount>0){miletickset=true; miletick=cd.dtcdist;}
#ifdef DEBUGcsvtime
                                    logtime(mytime0,fplog,"Saved csv line.");
#endif
                                    do{csvtick2 += csvtickinterval;} while (csvtick1>csvtick2);
#ifdef DEBUGtick
                        fprintf(fplog,"%s csvtick1 = %8.6lf, csvtick2 = %8.6lf\n",ID_csvtick_Str,csvtick1,csvtick2);
                        printf("\b\b\b\b\b\b\b\b Done",ntot);fflush(stdout);
#endif
                                }
                            }
                            break;
                        }
                        querytick1 = gettime();
#ifdef DEBUGtick
                        fprintf(fplog,"\n%s querytick1 = %8.6lf, querytick2 = %8.6lf\n",ID_querytick_Str,querytick1,querytick2);
                        printf("\r%8d                   ",ntot);fflush(stdout);
#endif
                    } while (querytick1 < querytick2); // loop of ATMA
#ifdef DEBUGtick
                        printf("\rSendingr                   ",ntot);fflush(stdout);
#endif

                    if (UIruntype==Normal){
                        logtime(mytime0, fplog, "End of ATMA loop. Sending \\r.");
                        sendcmd("\r", fd, fplog); //to stop ATMA
                        atmastoptick = querytickinterval + gettime();
                        do { // read remaining data
                            n = readATMAdata(str, maxlen, &partialmsglen, fd, fplog, &cd, &gs);
                            if(n>0){ntot += n;}
                        } while (n>0 && (partialmsglen==0 || str[partialmsglen-1]!='>') && (gettime()<atmastoptick) );
                    }
                    
                    sprintf(msg,"%s ATMA read total %d bytes after one iteration of loop of alternating OBD query & ATMA",ID_ATMAread_Str,ntot);
                    logtime(mytime0,fplog,msg);
                    //        sendcmd("ATCS\r", fd, fplog);readdata(fd, fplog);//readdata(fd, fplog);

                }  while (UIstate == Running); // End of loop of alternating between OBD query and ATMA

                logtime(mytime0, fplog, "End of UIstate==Running. Closing OBD port and files.");

                if (UIruntype==Normal){ close(fd); fd = -1; cd.csv_end(fpcsv, &gs);}
//cd.csv_end(fpcsv, &gs); // only for debugging

End_Running: // if can not open port (fd) then jump here
                                
                fclose(fplog); fplog = NULL;
                fclose(fpcsv); fpcsv = NULL;

            } // End of case Running:
            break;

            case Stopped:{
                /*printf("x");*/ idle_print(); fflush(stdout); wait(0.5); // need this wait otherwise crashes for some reason
                //if (fplog != NULL) {fclose(fplog); fplog = NULL;}

                //Fl::lock(); // crashed probably because the main window is in another thread
                go_button->copy_label("Go");
                go_button->activate();
                go_button->redraw();//Fl::flush();//Fl::unlock();
                gs.go_button->copy_label("Go");

                settings_group->activate();
                run_type_choice->activate();
                /*switch(UIruntype){
                    case Simulation: {
                    }
                    break;
                    case Normal: {
                        if (fd != -1){ close(fd); fd = -1;}
                    }
                    break;
                }*/
            } // End of case Stopped:
            break;

        } // End of switch (UIstate)

    } // End of while (!QuitProgram)

    printf("runUI() ended\n");
    return;
}