
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

// CANmsg.cpp Extract parameters from input message string and update dash gauges if provided

#include "OS.h"
#include <stdio.h>
#include <string.h>
#include "Util.h"
#include "Dash.h"
#include "CANmsg.h"
#include "LogMsg.h"


void car_data::csv_end(FILE *fpcsv, Gauges *gs){
        double timeInterval = gs->timeDelta_G->valueend() - gs->timeDelta_G->valuestart();
        char datetimestr[128];
        getdatetimestr(datetimestr);

        csv_title(fpcsv);// repeat the names again
        fprintf(fpcsv, "%d,%d,%d,%8.6lf,%d,%d,-,<-same,-,-,%d,-,%d,%d,%d,<-same,<-same,"
                "-,%d,-,%d,<-same,"
                "<-same,<-same,<-same,<-same,%d,%d,%d,%d,%d,<-same,%d,%d,<-same,<-same,%d,"
                "%d,%d,%d,%d,-,%d,%d,%d,"
                "%d,<-same,%d,<-same,%d,same as ICE T count,ditto,%d,ditto,<-same,%d,<-same,<-same\n",
                linecountAll, tick, tick_timeDelta, timeDelta, auxVcount, amphvVcount, rpmcount, rpm2count,mg1rpmcount,mg2rpmcount,
                speedcount, soccount,
                igncount, iceTcount, wTcount, wT2count, oaTcount, inv1Tcount, inv2Tcount, cnvTcount,
                catT1count, catT2count,mpgcount,dtcdistcount,loadcount,aLRcount,aFBcount,
                speedFcount,speedRcount,brakecount,kW2count,gaspdlcount);
        fprintf(fpcsv, "%.2lf,%.2lf,%.2lf,%8.6lf,%.2lf,%.2lf,-,<-same,-,-,%.2lf,-,%.2lf,%.2lf,%.2lf,<-same,<-same,"
                "-,%.2lf,-,%.2lf,<-same,"
                "<-same,<-same,<-same,<-same,%.2lf,%.2lf,%.2lf,%.2lf,%.2lf,<-same,%.2lf,%.2lf,<-same,<-same,%.2lf,"
                "%.2lf,%.2lf,%.2lf,%.2lf,-,%.2lf,%.2lf,%.2lf,"
                "%.2lf,<-same,%.2lf,<-same,%.2lf,same as ICE T,ditto,%.2lf,ditto,<-same,%.2lf,<-same,<-same\n",
                linecountAll/timeInterval, tick/timeInterval, tick_timeDelta/timeInterval, timeInterval, auxVcount/timeInterval, amphvVcount/timeInterval, rpmcount/timeInterval, rpm2count/timeInterval,mg1rpmcount/timeInterval,mg2rpmcount/timeInterval,
                speedcount/timeInterval, soccount/timeInterval,
                igncount/timeInterval, iceTcount/timeInterval, wTcount/timeInterval, wT2count/timeInterval, oaTcount/timeInterval, inv1Tcount/timeInterval, inv2Tcount/timeInterval, cnvTcount/timeInterval,
                catT1count/timeInterval, catT2count/timeInterval,mpgcount/timeInterval,dtcdistcount/timeInterval,loadcount/timeInterval,aLRcount/timeInterval,aFBcount/timeInterval,
                speedFcount/timeInterval, speedRcount/timeInterval, brakecount/timeInterval, kW2count/timeInterval, gaspdlcount/timeInterval);
        fprintf(fpcsv, "\nline/sec,%lf,tick,%d,line_count,%d,discarded_line_count,%d\n",
        linecountAll/timeInterval,tick, linecountAll, linecountDiscard);

        fprintf(fpcsv, "Min,-,-,%.1lf,%5.3lf,%.1lf,-,%.1lf,-,-,%.1lf,-,%.1lf,%.1lf,%.1lf,-,-,"
                "-,%.1lf,-,%d,%d,"
                "%.1lf,%.1lf,%.1f,%.1f,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1f,%.1f,%.1f,%.1f,%.1f,"
                "%.1f,%.1f,%.1f,%.1f,%3.2f,%.1lf,"
                "%.1lf,%.1lf\n",
                gs->timeDelta_G->valuemin(),
                gs->auxV_G->valuemin(), gs->hvV_G->valuemin(), gs->amp_G->valuemin(), gs->rpm_G->valuemin(), gs->rpm2_G->valuemin(), gs->mg1rpm_G->valuemin(), gs->mg2rpm_G->valuemin(),
                gs->speed_G->valuemin(), maxdischrgA, maxchrgA,
                gs->dsc_G->valuemin(), gs->soc_G->valuemin(), gs->blC_G->valuemin(), gs->buC_G->valuemin(), gs->ign_G->valuemin(), gs->iceT_G->valuemin(), gs->wT_G->valuemin(), gs->wT2_G->valuemin(), gs->oaT_G->valuemin(), gs->intakeT_G->valuemin(), gs->inv1T_G->valuemin(), gs->inv2T_G->valuemin(), gs->mg1T_G->valuemin(), gs->mg2T_G->valuemin(), gs->cnvT_G->valuemin(),
                gs->catT1_G->valuemin(), gs->catT2_G->valuemin(), gs->mpg_G->valuemin(), gs->dtcdist_G->valuemin(), gs->odo_G->valuemin(), gs->load_G->valuemin(),
                gs->aLR_G->valuemin(), gs->acc_G->valuemin());
        fprintf(fpcsv, "Max,-,-,%.1lf,%5.3lf,%.1lf,-,%.1lf,-,-,%.1lf,-,%.1lf,%.1lf,%.1lf,-,-,"
                "-,%.1lf,-,%d,%d,"
                "%.1lf,%.1lf,%.1f,%.1f,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1f,%.1f,%.1f,%.1f,%.1f,"
                "%.1f,%.1f,%.1f,%.1f,%3.2f,%.1lf,"
                "%.1lf,%.1lf\n",
                gs->timeDelta_G->valuemax(),
                gs->auxV_G->valuemax(), gs->hvV_G->valuemax(), gs->amp_G->valuemax(), gs->rpm_G->valuemax(), gs->rpm2_G->valuemax(), gs->mg1rpm_G->valuemax(), gs->mg2rpm_G->valuemax(),
                gs->speed_G->valuemax(), maxdischrgA, maxchrgA,
                gs->dsc_G->valuemax(), gs->soc_G->valuemax(), gs->blC_G->valuemax(), gs->buC_G->valuemax(), gs->ign_G->valuemax(), gs->iceT_G->valuemax(), gs->wT_G->valuemax(), gs->wT2_G->valuemax(), gs->oaT_G->valuemax(), gs->intakeT_G->valuemax(), gs->inv1T_G->valuemax(), gs->inv2T_G->valuemax(), gs->mg1T_G->valuemax(), gs->mg2T_G->valuemax(), gs->cnvT_G->valuemax(),
                gs->catT1_G->valuemax(), gs->catT2_G->valuemax(), gs->mpg_G->valuemax(), gs->dtcdist_G->valuemax(), gs->odo_G->valuemax(), gs->load_G->valuemax(),
                gs->aLR_G->valuemax(), gs->acc_G->valuemax());
        fprintf(fpcsv,"Date,invTmin,invTmax,"
                "cnvTmax,MG1Tmax,MG2Tmax,"
                "socStart,socEnd,socMin,socMax,wTmax,RPMmax,SpeedMax,"
                "Time,"
                "oaTstart,oaTend,oaTmin,oaTmax,"
                "dscMin,dscMax,blCmax,buCmax,"
                "catT1max,catT2max,DTCdistEnd,ODO,"
                "SpeedMean,"
                "avgMPG,12Vmin,12Vmax,gal,dhvBVmax,"
                "hvFanVmax,"
                "oaTs[%d]\n",joaTs);

        char strtosave[2048], *str=strtosave;
        int n;
        n = sprintf(str,"%c%c%c%c-%c%c-%c%c,%.1lf,%.1lf,"
                "%.1lf,%.1lf,%.1lf,"
                "%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,"
                "%c%c:%c%c,"
                "%.1lf,%.1lf,%.1lf,%.1lf,"
                "%.1lf,%.1lf,%.1lf,%.1lf,"
                "%.1lf,%.1lf,%.1lf,%3.2lf,"
                "%.1lf,"
                "%.1lf,%6.3lf,%6.3lf,%6.3lf,%.1lf,"
                "%.1lf",
                datetimestr[0],datetimestr[1],datetimestr[2],datetimestr[3],datetimestr[4],datetimestr[5],datetimestr[6],datetimestr[7],
                min(gs->inv1T_G->valuemin(), gs->inv2T_G->valuemin()),max(gs->inv1T_G->valuemax(), gs->inv2T_G->valuemax()),
                gs->cnvT_G->valuemax(),gs->mg1T_G->valuemax(),gs->mg2T_G->valuemax(),
                gs->soc_G->valuestart(),gs->soc_G->valueend(),gs->soc_G->valuemin(),gs->soc_G->valuemax(),
                gs->wT2_G->valuemax(),gs->rpm_G->valuemax(),gs->speed_G->valuemax(),
                datetimestr[9],datetimestr[10],datetimestr[11],datetimestr[12],
                gs->oaT_G->valuestart(),gs->oaT_G->valueend(),gs->oaT_G->valuemin(),gs->oaT_G->valuemax(),
                gs->dsc_G->valuemin(),gs->dsc_G->valuemax(),gs->blC_G->valuemax(), gs->buC_G->valuemax(),
                gs->catT1_G->valuemax(), gs->catT2_G->valuemax(), gs->dtcdist_G->valueend(), gs->odo_G->valueend(),
                (gs->odo_G->valueend() - gs->odo_G->valuestart())*3600.0/timeInterval,
                gs->mpgmean_G->valueend(),gs->auxV_G->valuemin(),gs->auxV_G->valuemax(),gs->gal_G->valueend(),gs->dhvBV_G->valuemax(),
                gs->hvFanV_G->valuemax());
        for (int j=0; j<joaTs; j++) {str+=n; n=sprintf(str,",%.1lf",oaTs[j]);}
        str+=n; sprintf(str,"\n");
        fprintf(fpcsv,"%s",strtosave);

        fprintf(fpcsv,"\ntimeDelta start=%lf,end=%lf,odo start=%lf,end=%lf\n",gs->timeDelta_G->valuestart(),gs->timeDelta_G->valueend(),
        gs->odo_G->valuestart(),gs->odo_G->valueend());


        FILE *fp;

        fp = fopen("summary.csv", "a"); //Append summary line of csv to file
        if (fp==NULL) {printf("!!! Error open file \"summary.csv\" !!!\n");}
        fprintf(fp, "%s", strtosave);
        fclose(fp);

        fp = fopen("ODO.txt", "w"); //Save odo to file //KEEP this one for opening and reading previous value
        if (fp==NULL) {printf("!!! Error open file \"ODO.txt\" !!!\n");}
        fprintf(fp, "%lf", odo);
        fclose(fp);

        // save another copy with the log files
        char path[1024], pathname[1024], *ptr;
        const char *ptr0;
        size_t length;

        ptr0 = log_filename->value();
#ifdef VC
        ptr = strrchr(ptr0,'\\');
#else
        ptr = strrchr(ptr0,'/');
#endif
        length = 1 + ptr - ptr0;
        strncpy(path, ptr0, length);
        path[length]='\0';

        sprintf(pathname, "%s%s", path, "summary.csv");
        fp = fopen(pathname, "a"); //Append summary line of csv to file
        if (fp==NULL) {printf("!!! Error open file \"%s\" !!!\n",pathname);}
        fprintf(fp, "%s", strtosave);
        fclose(fp);

        sprintf(pathname, "%s%s", path, "ODO.txt");
        fp = fopen(pathname, "w"); //Save odo to file
        if (fp==NULL) {printf("!!! Error open file \"%s\" !!!\n",pathname);}
        fprintf(fp, "%lf", odo);
        fclose(fp);

    }


int car_data::decode(char *str, Gauges *gs) {
    char int12bit[4]; // to collect ascii representation of 12 bit integer
    int CANID, data[16];
    /*    const int IDtick = 0x020, IDice = 0x039, IDav = 0x03B, IDrpm = 0x3C8, IDspeed = 0x3CA,
                IDbat = 0x3CB, IDign = 0x526, IDwt = 0x52C, IDoat = 0x3CF,
                IDinv = 0x7EA, IDcat = 0x7E8, IDtimeDelta = 0xFFFF;
     */
    static int ECU=0, PID=0, multilinecount=0, tmp1=0; // for multiframe messages

    linecountAll++;
    CANID = 0;
    sscanf(str, "%x", &CANID); //printf("CANID= 0x%x\n",CANID);
    bool foundData = true; // later if don't find matching CAN ID then set it to false
    switch (CANID) {
        case 0x020://tick
            tick++;
            break;
        case 0x022://aLR left-right acceleration
            if (strlen(str) >= 28) {
                aLRcount++;
                sscanf(str + 4, "%x %x", data, data+1);
                aLR = data[0]*256 + data[1] - 0x200;
                gs->acc_G->value2(aLR);
                gs->aLR_G->value(aLR);
            }
            break;
        case 0x023://aFB front-back acceleration
            if (strlen(str) >= 25) {
                aFBcount++;
                sscanf(str + 4, "%x %x", data, data+1);
                aFB = data[0]*256 + data[1] - 0x200;
                gs->acc_G->value(aFB);
            }
            break;
        case 0x030://Brakes
            if (strlen(str) >= 28){
                brakecount++;
                sscanf(str + 16, "%x", data);
                brake = data[0];
            }
            break;
        case 0x039://ICE T & torque
            if (strlen(str) >= 16) {
                iceTcount++;
                sscanf(str + 4, "%x %x %x", data, data + 1, data + 2);
                iceT = (data[0] < 128 ? data[0] : data[0] - 256)*1.8 + 32;
                //gs->wT_G->value2(iceT);
                gs->iceT_G->value(iceT);
                iceNm = data[2] * 2;
                icekW = ((double)iceNm) * ((double)rpm)/60.0 * RadPerRev /1000;
                gs->kW_G->value(icekW);
                if (lps>85){eff = ((double)iceNm) * efffac / (lps-85);}
                else {eff=0;}
                gs->eff_G->value(eff);
            }
            break;
        case 0x03B://Hv battery Amp & V
            if (strlen(str) >= 19) {
                amphvVcount++;
                int12bit[0] = str[5]; // str[6] is a space character
                int12bit[1] = str[7];
                int12bit[2] = str[8];
                sscanf(int12bit, "%3x", data);
                amp = (data[0]<2048 ? data[0] : data[0]-4096); //12 bit signed int
                amp *= 0.1;
                updateminmax(amp, &(ampMin), &(ampMax));
                sscanf(str + 10, "%x %x", data, data + 1);
                hvV = data[0]*256 + data[1];
                gs->amp_G->value(amp);
                gs->hvV_G->value(hvV);
            }
            break;
        case 0x348://RPM2
            if (strlen(str) >= 22) {
                rpm2count++;
                sscanf(str + 10, "%x", data);
                rpm2 = (data[0]*256)/10;
                gs->rpm2_G->value(rpm2);
                gs->rpm_G->value2(rpm2);
            }
            break;
        case 0x3C8://RPM
            if (strlen(str) >= 22) {
                rpmcount++;
                sscanf(str + 10, "%x", data);
                rpm = data[0]*32;
                updateminmax(rpm, &(rpmMin), &(rpmMax));
                gs->rpm_G->value(rpm);
                gs->psd_G->value(rpm);
                if (rpm<200){lps=0; 
                icekW = 0; gs->kW_G ->value(icekW); eff = 0; gs->eff_G->value(eff);
                /*icekW2 =0; gs->kW2_G->value(icekW2);*/eff2= 0; gs->eff2_G->value(eff2);}
                if (speed>0 && rpm<200){mpg = 999.9; gs->mpg_G->value(mpg);}
            }
            break;
        case 0x3CA://speed - reading very jagged, change to average of the rear wheels
            if (strlen(str) >= 19) {
                speedcount++;
                sscanf(str + 10, "%x", data);
                //speed = data[0]*km2mile; // mile/hr
            }
            break;
        case 0x0B1://speed: Front wheels
            if (strlen(str) >= 22) {
                speedFcount++;
                sscanf(str + 4, "%x %x %x %x", data,data+1,data+2,data+3);
                speedFR = (data[0]*256+data[1])*km2mile/100.0; // mile/hr
                speedFL = (data[2]*256+data[3])*km2mile/100.0; // mile/hr
                //gs->speedFR_G->value(speedFR);
                //gs->speedFL_G->value(speedFL);
            }
            break;
        case 0x0B3://speed: Rear wheels
            if (strlen(str) >= 22) {
                speedRcount++;
                sscanf(str + 4, "%x %x %x %x", data,data+1,data+2,data+3);
                speedRR = (data[0]*256+data[1])*km2mile/100.0; // mile/hr
                speedRL = (data[2]*256+data[3])*km2mile/100.0; // mile/hr
                //gs->speedRR_G->value(speedRR);
                //gs->speedRL_G->value(speedRL);
                speed = 0.5*(speedRR+speedRL); // mile/hr
                updateminmax(speed, &(speedMin), &(speedMax));
                gs->speed_G->value(speed);
                if (speed==0.0){mpg = 0; gs->mpg_G->value(mpg);}
            }
            break;
        case 0x244://gas pedal
            if (strlen(str) >= 28) {
                sscanf(str+22, "%x", data);
                gaspdl = 0.5*data[0]; // change range from 0~200 to 0~100
                updateminmax(gaspdl, &(gaspdlMin), &(gaspdlMax));
            }
            break;
        case 0x3CB:// HV battery: SOC, dsc, Ts, max charge/discharge Amps
            if (strlen(str) >= 25) {
                soccount++;
                sscanf(str + 4, "%x %x %x %x %x %x", 
                        &(maxdischrgA), &(maxchrgA), data, data + 1, &(buC), &(blC)); // swapped buC & blC 20110114
                dsc = data[0]*0.5;
                soc = data[1]*0.5;
                if (buC>127){buC -= 256;}
                if (blC>127){blC -= 256;}
                gs->dsc_G->value(dsc);
                gs->soc_G->value(soc);
                gs->blC_G->value(blC);
                gs->buC_G->value(buC);
            }
            break;
        case 0x520:// MPG
            if (strlen(str) >= 14) {
                mpgcount++;
                sscanf(str + 7, "%x %x", data, data+1);
                lps = data[0]*256 + data[1]; // something like liter per unit time
                if (speed==0.0){mpg = 0;} // if speed is 0 then set MPG to 0
                else { if (rpm<200 || lps == 0) {mpg = 999.9;} // set max MPG if speed>0 but no fuel is used
                else { mpg = 378.541*speed*3600/rpm/lps; }}
                gs->mpg_G->value(mpg);
                if (lps <= 85){icekW = 0; gs->kW_G->value(icekW); eff = 0; eff2 = 0;} // icekW2 may not be zero here since it includes torque from MG1/2
                else {eff = ((double)iceNm) * efffac / (lps-85);
                    eff2 = ((double)iceNm2) * efffac / (lps-85);}
                gs->eff_G->value(eff);
                gs->eff2_G->value(eff2);
            }
            break;
        case 0x52C:// wT ICE coolant T "water T"
            if (strlen(str) >= 10) {
                wTcount++;
                sscanf(str + 7, "%x", data);
                wT = data[0]*0.9 + 32;
                gs->wT_G->value(wT);
                gs->iceT_G->value2(wT);
            }
            break;
        case 0x3CF:// OAT (outside air T) & intake T
            if (strlen(str) >= 19) {
                oaTcount++;
                sscanf(str + 7, "%x %x", data, data + 1);
                oaT = (data[0] < 128 ? data[0] : data[0] - 256)*1.8 + 32; //signed byte int
                intakeT = (data[1] < 128 ? data[1] : data[1] - 256)*1.8 + 32;
                gs->oaT_G->value(oaT);
                gs->intakeT_G->value(intakeT);
            }
            break;
        case 0x540:// shift lever
            if (strlen(str) >= 16) {
                shiftcount++;
                sscanf(str + 7, "%x", data);
                switch(data[0]){
                    case 0x00:{gs->shift_G->value("B");}break;
                    case 0x10:{gs->shift_G->value("D");}break;
                    case 0x20:{gs->shift_G->value("N");}break;
                    case 0x40:{gs->shift_G->value("R");}break;
                    case 0x80:{gs->shift_G->value("P");}break;
                    default:{gs->shift_G->value("?");}break;
                }
                gs->shift_G->redraw();
            }
            break;
        case 0x57F:// light
            if (strlen(str) >= 25) {
                lightcount++;
                sscanf(str + 7, "%x", data);
                switch(data[0]){
                    case 0x00:{gs->light_G->value(".");}break; //off
                    case 0x10:{gs->light_G->value("p");}break; //parking light
                    case 0x30:{gs->light_G->value("L");}break; //low beam
                    case 0x38:{gs->light_G->value("H");}break; //high beam
                    default:{gs->light_G->value("?");}break;
                }
                gs->light_G->redraw();
            }
            break;
        case 0x5B6:// door
            if (strlen(str) >= 13) {
                doorcount++;
                sscanf(str + 10, "%x", data);
                switch(data[0]){
                    case 0x00:{gs->door_G->value(".");}break; //closed
                    case 0x04:{gs->door_G->value("R");}break; //Rear
                    case 0x40:{gs->door_G->value("P");}break; //Passenger
                    case 0x80:{gs->door_G->value("D");}break; //Driver
                    default:{gs->door_G->value("?");}break;
                }
                gs->door_G->redraw();
            }
            break;
        case 0x5A4:// fuel gauge
            if (strlen(str) >= 10) {
                fuelcount++;
                sscanf(str + 7, "%x", data);
                fuel = data[0]*0.264172052; // guessing it is in liters -> convert from liter to gallon
                gs->fuel_G->value(fuel);
            }
            break;
        case 0x7EA: // Hybrid Vehicle ECU: mixture of multi and single frame data!
            if (strlen(str) >= 28) {
                if (ECU!=0x7EA){ // reset multiframe variables since previous multiframe may have been interrupted
                    ECU=0x7EA; PID=0; multilinecount=0;
                }
                sscanf(str + 4, "%x %x %x %x %x %x %x %x", data, data + 1, data + 2, data + 3,data+4,data+5,data+6,data+7);
                if (PID == 0) {
                    if (data[2] == 0x61 && data[3] == 0xC3) { // 1st multiframe // now 3 requests combined C3, C4, 42
//                        7EA 10 27 61 C3 3F FF 0F A0   <------ 1st
//                        7EA 21 00 00 3F FF 0F A0 00   <------ 2nd
//                        7EA 22 00 00 00 00 00 00 80
//                        7EA 23 8C 41 62 00 80 00 55
//                        7EA 24 54 58 5B 6D 00 80 73
//                        7EA 25 74 4C 46 01 80 00 00
                        PID = 0x61C3;
                        mg2rpmcount++;
                        //sscanf(str+16,"%x %x",data,data+1);
                        mg2rpm = data[4]*256+data[5]-0x3FFF;
                        gs->mg2rpm_G->value(mg2rpm);
                        gs->psd_G->value2(mg2rpm);
                        mg2Nm = 0.125*(data[6]*256+data[7]-0x0FA0);
//                    } else if (data[2] == 0x61 && data[3] == 0xC4) { // 1st multiframe
//                        PID = 0x61C4;
//                    } else if (data[1] == 0x61 && data[2] == 0x42) { // single frame
//                        auxVcount++;
//                        //sscanf(str + 13, "%x %x", data, data + 1);
//                        auxV = (data[3]*256 + data[4])*0.001;
//                        gs->auxV_G->value(auxV);
                    }
                } else if (PID == 0x61C3) { // 2nd multiframe and after
                    multilinecount++;
                    if (data[0] == 0x21){
                        mg1rpmcount++;
                        //sscanf(str + 13, "%x %x", data, data + 1);
                        mg1rpm = data[3]*256+data[4]-0x3FFF;
                        gs->mg1rpm_G->value(mg1rpm);
                        gs->psd_G->value1(mg1rpm);
                        mg1Nm = 0.125*(data[5]*256+data[6]-0x0FA0);
                    } else if (data[0] == 0x23) {
                        inv1Tcount++;
                        //sscanf(str + 25, "%x", data);
                        inv1T = data[7] - 50; // deg C
                        gs->inv1T_G->value(inv1T);
                    } else if (data[0] == 0x24) {
                        inv2Tcount++;
                        inv2T = data[1] - 50; // deg C
                        mg1T = data[2] - 50; // deg C
                        mg2T = data[3] - 50; // deg C
                        gs->inv2T_G->value(inv2T);
                        gs->mg1T_G->value(mg1T);
                        gs->mg2T_G->value(mg2T);
                    } else if (data[0] == 0x25){
                        if (data[6]!=0xC4){// should be 0xC4, otherwise abandon following frames
                            PID = 0; // reset PID; frames after will be ignored just like other unrecognized frames
                            multilinecount = 0;
                            }
                    } else if (data[0] == 0x26){
                        cnvTcount++;
                        cnvT = data[5] - 50; //degC
                        gs->cnvT_G->value(cnvT);
                    } else if (data[0] == 0x28){
                        if (data[1] == 0x42) {
                            auxVcount++;
                            //sscanf(str + 13, "%x %x", data, data + 1);
                            auxV = (data[2]*256 + data[3])*0.001;
                            gs->auxV_G->value(auxV);
                        }
                        PID = 0; // reset PID after this multiframe data extraction is done; frames after will be ignored just like other unrecognized frames
                        multilinecount = 0;
                    } else if (multilinecount > 7) { // received all 9 frames -> reset PID & multilinecount; just in case
                        PID = 0;
                        multilinecount = 0;
                    }
//                } else if (PID == 0x61C4) { // 2nd multiframe and after
//                    7EA 10 11 61 C4 04 40 00 6C  // 1st
//                    7EA 21 48 56 B3 06 04 80 80  // 2nd
//                    7EA 22 FF 20 00 00 00 00 00
//                    multilinecount++;
//                    if (data[0] == 0x21) {
//                        cnvTcount++;
//                        cnvT = data[2] - 50; //degC
//                        gs->cnvT_G->value(cnvT);
//                        PID = 0; // reset PID after this multiframe data extraction is done; frames after will be ignored just like other unrecognized frames
//                        multilinecount = 0;
//                    } else if (multilinecount > 1) { // received all 3 frames -> reset PID & multilinecount; just in case
//                        PID = 0;
//                        multilinecount = 0;
//                    }
                }
            }
            break;
        case 0x7E8:// wT2, IGN, load, CAT T, atm, Nm
            if (strlen(str) >= 28) {
                if (ECU!=0x7E8){ // reset multiframe variables since previous multiframe may have been interrupted
                    ECU=0x7E8; PID=0; multilinecount=0;
                }
                sscanf(str + 4, "%x %x %x %x %x %x %x %x", data, data + 1, data + 2, data + 3,data+4,data+5,data+6,data+7);
                if (PID == 0) {
                    if (data[2] == 0x61 && data[3] == 0x3C) { // 1st multiframe // now 4 requests combined 3C, 3E, 2E, 32
                        PID = 0x613C;
                        catT1count++;
                        catT1 = (int) ((data[4]*256 + data[5])*0.1 - 40); // deg C
                        gs->catT1_G->value(catT1);
                        if (data[6] == 0x3E) {
                            catT2count++;
                            tmp1 = data[7]; // save for next frame to calculate catT2
                        } else {// should be 0x3E, otherwise abandon following frames
                            PID = 0; // reset PID; frames after will be ignored just like other unrecognized frames
                            multilinecount = 0;
                            }
                    } else if (data[1] == 0x41 && data[2] == 0x05) {// engine coolant T
                        wT2count++;
                        wT2 = (data[3] - 40)*1.8 + 32;
                        gs->wT2_G->value(wT2);
                    } else if (data[2] == 0x61 && data[3] == 0xCD) { // torque Nm in 1st frame of multiframe
//                    7E8 10 14 61 CD 00 00 00 80
//                    7E8 21 07 FF 6D 1E 00 78 70
//                    7E8 22 65 B3 16 B3 33 00 00
                        kW2count++;
                        //sscanf(str + 25, "%x", data);
                        iceNm2 = data[7]-0x80;
                        icekW2 = ((double) iceNm2) * ((double) rpm) / 60.0 * RadPerRev / 1000;
                        gs->kW2_G->value(icekW2);
                        if (lps>85){eff2 = ((double)iceNm2) * efffac / (lps-85);}
                        else {eff2=0;}
                        gs->eff2_G->value(eff2);
//                } else if (data[0] == 0x41 && data[1] == 0x04) {
//                    loadcount++;
//                    load = (data[2])*100.0/255.0;
//                    gs->load_G->value(load);
                    } else if (data[1] == 0x41 && data[2] == 0x0E) {// 2 requests combined
                        igncount++;
                        ign = (data[3])*0.5 - 64;
                        gs->ign_G->value(ign);
                        if (data[4] == 0x04) {
                            loadcount++;
                            load = (data[5])*100.0 / 255.0;
                            gs->load_G->value(load);
                        }
//                } else if (data[0] == 0x61 && data[1] == 0x3C) {// 2 requests combined
//                    catT1count++;
//                    catT1 = (int)((data[2]*256 + data[3])*0.1 - 40);// deg C
//                    gs->catT1_G->value(catT1);
//                    if (data[4]==0x3E){// if catT1 & catT2 requests are combined in the same line of command: 03213C3E
//                        catT2count++;
//                        catT2 = (int) ((data[5]*256 + data[6])*0.1 - 40); // deg C
//                        gs->catT2_G->value(catT2);
//                    }
//                } else if (data[0] == 0x61 && data[1] == 0x3E) {// should be obsolete now catT1 & 2 are requested together
//                    catT2count++;
//                    catT2 = (int)((data[2]*256 + data[3])*0.1 - 40);// deg C
//                    gs->catT2_G->value(catT2);
                    } else if (data[2] == 0x61 && data[3] == 0x33) { // 1st multiframe // now 2 requests combined 33(atm), F5(misfire)
                        PID = 0x6133;
                        atmcount++;
                        atm = data[4]; // KPa
                        gs->atm_G->value(atm);
                        if (data[5] == 0xF5) {
                            mfrpm = 25*data[6]; // misfire RPM
                            mfload = data[7]; // misfire load
                        } else {// should be 0xF5, otherwise abandon following frames
                            PID = 0; // reset PID; frames after will be ignored just like other unrecognized frames
                            multilinecount = 0;
                            }
//                    } else if (data[1] == 0x61 && data[2] == 0x33) {
//                        atmcount++;
//                        atm = data[3]; // KPa
//                        gs->atm_G->value(atm);
                    }
                } else if (PID == 0x613C){
                    multilinecount++;
                    if (data[0]==0x21) {
                        catT2 = (int) ((tmp1 * 256 + data[1])*0.1 - 40); // deg C
                        gs->catT2_G->value(catT2);
                        if (data[2]==0x2E){/*EVAP purge VSV*/
                            evapVSV = data[3];
                        }
                        if (data[4]==0x32){/*EVAP vapor pressure*/
                            evapP = 256 * data[5] + data[6];
                        }
                        PID = 0;
                        multilinecount = 0;
                    } else if (multilinecount>0){ // all 2 frames received
                        PID = 0;
                        multilinecount = 0;
                    }

                } else if (PID == 0x6133){
                    multilinecount++;
                    if (data[0]==0x21) {
                        cyn1 = data[1];
                        cyn2 = data[2];
                        cyn3 = data[3];
                        cyn4 = data[4];
                        cyn5 = data[5];
                        cyn6 = data[6];
                        cyn7 = data[7];
                    } else if (data[0]==0x22) {
                        cyn8 = data[1];
                        PID = 0;
                        multilinecount = 0;
                    } else if (multilinecount>1){ // all 3 frames received
                        PID = 0;
                        multilinecount = 0;
                    }

                }
            }
            break;
        case 0x7EB:// DTC clear run dist, HV battery (multi frame)
            if (strlen(str) >= 28) {
                if (ECU!=0x7EB){ // reset multiframe variables since previous multiframe may have been interrupted
                    ECU=0x7EB; PID=0; multilinecount=0;
                }
                sscanf(str + 4, "%x %x %x %x %x %x %x %x", data, data + 1, data + 2, data + 3,data+4,data+5,data+6,data+7);
                if (PID==0){
                    if (data[1] == 0x61 && data[2] == 0x31) {
                        dtcdistcount++;
                        dtcdist = (data[3]*256 + data[4])*km2mile; // mile
                        gs->dtcdist_G->value(dtcdist);
                    } else if (data[2]==0x61 && data[3]==0xD0){// now three requests combined D0, CE, CF
                        PID=0x61D0;
                        hvBRcount++;
//                    } else if (data[2]==0x61 && data[3]==0xCE){
//                        PID=0x61CE;
//                        hvBVcount++;
//                        tmp1=data[7];// save data[7] for next frame to calculate hvBV[1]
//                    } else if (data[2]==0x61 && data[3]==0xCF){
//                        PID=0x61CF;
//                        hvBTcount++;
//                        hvAirT = 0.16 * data[4];//deg C, extra precision in data[5] but just overkill
//                        hvFanV = 0.2 * (data[6]-0x80);
                    }
                } else if (PID==0x61D0){
                    multilinecount++;
                    if (data[0]==0x21){
                        hvBVmin=0.01*(256*data[6]+data[7]-0x8000);
                    } else if (data[0]==0x22){
                        hvBVmax=0.01*(256*data[2]+data[3]-0x8000);
                        if (hvBVmin>0){
                            dhvBV = 100.0 * ((hvBVmax/hvBVmin)-1);
                        } else {dhvBV = -1;} // something wrong, hvBVmin should be > 0
                        gs->dhvBV_G->value(dhvBV);
                        hvBR[1]=data[5];
                        hvBR[2]=data[6];
                        hvBR[3]=data[7];
                    } else if (data[0]==0x23){
                        hvBR[4]=data[1];
                        hvBR[5]=data[2];
                        hvBR[6]=data[3];
                        hvBR[7]=data[4];
                        hvBR[8]=data[5];
                        hvBR[9]=data[6];
                        hvBR[10]=data[7];
                    } else if (data[0]==0x24){
                        hvBR[11]=data[1];
                        hvBR[12]=data[2];
                        hvBR[13]=data[3];
                        hvBR[14]=data[4];
                        if (data[5] != 0xCE){ // should be 0xCE, otherwise abandon following frames
                            PID = 0;
                            multilinecount = 0;
                        } else {hvBVcount++;}
                    } else if (data[0]==0x25){
                        hvBV[1]=0.01*(256*data[2]+data[3]-0x8000);
                        hvBV[2]=0.01*(256*data[4]+data[5]-0x8000);
                        hvBV[3]=0.01*(256*data[6]+data[7]-0x8000);
                    } else if (data[0]==0x26){
                        hvBV[4]=0.01*(256*data[1]+data[2]-0x8000);
                        hvBV[5]=0.01*(256*data[3]+data[4]-0x8000);
                        hvBV[6]=0.01*(256*data[5]+data[6]-0x8000);
                        tmp1=data[7];// save data[7] for next frame to calculate hvBV[7]
                    } else if (data[0]==0x27){
                        hvBV[7]=0.01*(256*tmp1+data[1]-0x8000);
                        hvBV[8]=0.01*(256*data[2]+data[3]-0x8000);
                        hvBV[9]=0.01*(256*data[4]+data[5]-0x8000);
                        hvBV[10]=0.01*(256*data[6]+data[7]-0x8000);
                    } else if (data[0]==0x28){
                        hvBV[11]=0.01*(256*data[1]+data[2]-0x8000);
                        hvBV[12]=0.01*(256*data[3]+data[4]-0x8000);
                        hvBV[13]=0.01*(256*data[5]+data[6]-0x8000);
                        tmp1=data[7];// save data[7] for next frame to calculate hvBV[14]
                    } else if (data[0]==0x29){
                        hvBV[14]=0.01*(256*tmp1+data[1]-0x8000);
                        if (data[2] == 0xCF) {
                            hvBTcount++;
                            //not right hvAirT = 0.16 * data[3]; //deg C, extra precision in data[4] but just overkill
                            hvAirT = 5.0 + 2.0 * (data[3]-128) + data[4]/128.0; //deg C, guessing
                            hvFanV = 0.2 * (data[5] - 0x80);
                            gs->hvFanV_G->value(hvFanV);
                        } else { // if not 0xCF then abandon following frames
                            PID = 0;
                            multilinecount = 0;
                        }
                    } else if (data[0]==0x2A){
                        hvdsc = data[2];
                        hvBT[1]= 5.0 + 2.0 * (data[6]-128) + data[7]/128.0; //deg C, guessing
                    } else if (data[0]==0x2B){
                        hvBT[2]= 5.0 + 2.0 * (data[1]-128) + data[2]/128.0; //deg C, guessing
                        hvBT[3]= 5.0 + 2.0 * (data[3]-128) + data[4]/128.0; //deg C, guessing
                        PID = 0;
                        multilinecount = 0;
                    } else if (multilinecount>10){ // all 12 frames received
                        PID = 0;
                        multilinecount = 0;
                    }
//                } else if (PID==0x61CE){
//                    multilinecount++;
//                    if (data[0]==0x21){
//                        hvBV[1]=0.01*(256*tmp1+data[1]-0x8000);
//                        hvBV[2]=0.01*(256*data[2]+data[3]-0x8000);
//                        hvBV[3]=0.01*(256*data[4]+data[5]-0x8000);
//                        hvBV[4]=0.01*(256*data[6]+data[7]-0x8000);
//                    } else if (data[0]==0x22){
//                        hvBV[5]=0.01*(256*data[1]+data[2]-0x8000);
//                        hvBV[6]=0.01*(256*data[3]+data[4]-0x8000);
//                        hvBV[7]=0.01*(256*data[5]+data[6]-0x8000);
//                        tmp1=data[7];// save data[7] for next frame to calculate hvBV[8]
//                    } else if (data[0]==0x23){
//                        hvBV[8]=0.01*(256*tmp1+data[1]-0x8000);
//                        hvBV[9]=0.01*(256*data[2]+data[3]-0x8000);
//                        hvBV[10]=0.01*(256*data[4]+data[5]-0x8000);
//                        hvBV[11]=0.01*(256*data[6]+data[7]-0x8000);
//                    } else if (data[0]==0x24){
//                        hvBV[12]=0.01*(256*data[1]+data[2]-0x8000);
//                        hvBV[13]=0.01*(256*data[3]+data[4]-0x8000);
//                        hvBV[14]=0.01*(256*data[5]+data[6]-0x8000);
//                        PID = 0;
//                        multilinecount = 0;
//                    } else if (multilinecount>3){
//                        PID = 0;
//                        multilinecount = 0;
//                    }
//                } else if (PID==0x61CF){
//                    multilinecount++;
//                    if (data[0]==0x21){
//                        hvdsc = data[3];
//                        hvBT[1]= 0.16 * data[7];//deg C, extra precision in data[1] (next frame) but just overkill
//                    } else if (data[0]==0x22){
//                        hvBT[2]= 0.16 * data[2];//deg C, extra precision in data[3] but just overkill
//                        hvBT[3]= 0.16 * data[4];//deg C, extra precision in data[5] but just overkill
//                        PID = 0;
//                        multilinecount = 0;
//                    } else if (multilinecount>1){
//                        PID = 0;
//                        multilinecount = 0;
//                    }
                }
            }
            break;
        case ID_TimeStamp://0xFFFF:// time stamp in log file
            if (strlen(str) >= 69) {
                timeDeltacount++;
                tick_timeDelta = tick;
                sscanf(str + 53, "%lf", &(timeDelta));
                gs->timeDelta_G->value(timeDelta);
            }
            break;
        default:// no match to any known CAN ID -> discard
            foundData = false;
            linecountDiscard++;
            break;
    } // End of switch (CANID)

    //if (gaugePresent && foundData){Fl::lock();    Fl::flush();    Fl::unlock();}// update Gauges window
    return CANID;
}

