
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


#ifndef _CANDECODE_H
#define	_CANDECODE_H

//#include <algorithm> // does not have min()?
#include "Fl_Gauge.H"
#include "Dash.h"

template <class T> const T& min ( const T& a, const T& b ) {
  return (a<b)?a:b;     // or: return comp(a,b)?a:b; for the comp version
}
template <class T> const T& max ( const T& a, const T& b ) {
  return (b<a)?a:b;     // or: return comp(b,a)?a:b; for the comp version
}

const int oaTsSize = 64;

class car_data { // object containing car data, function to decode from CAN message to get the car data, function to write to csv file
private:
    double odo_t; // time of previous ODO update via update_odo()
    double lpsTotal; // ~ total fuel used so far, to calculate mean MPG
    double odo0; // initial ODO reading
public:
    double amp, ampMin, ampMax;
    int hvV, amphvVcount;
    int rpm, rpmMin, rpmMax, rpmcount;
    int rpm2, rpm2count;
    double speed, speedMin, speedMax, speedFR, speedFL, speedRR, speedRL;
    int speedcount, speedFcount, speedRcount;
    int maxdischrgA, maxchrgA, blC, buC;
    double soc, dsc;
    int soccount;
    double ign;
    int igncount;
    double wT, iceT, wT2;
    int wTcount, iceTcount, wT2count;
    double oaT, intakeT, oaTs[oaTsSize]/*array to store oaT every mileinterval miles*/;
    int oaTcount, joaTs/*index for oaTs*/;
    int mg1T, mg2T, inv1T, inv2T, cnvT, inv1Tcount, inv2Tcount, cnvTcount;
    int mg1rpm, mg2rpm,mg1rpmcount, mg2rpmcount;
    double auxV;
    int catT1, catT2, atm, auxVcount;
    int catT1count, catT2count, atmcount;
    int tick, linecountAll, linecountDiscard, tick_timeDelta;
    double timeDelta;
    int timeDeltacount;
    double mpg, lps, dtcdist, odo, mpgmean, trip;
    int mpgcount, dtcdistcount;
    double fuel, load; // fuel gauge in gallons, ICE load in %
    int fuelcount, loadcount;
    int aLR, aLRcount, aFB, aFBcount;
    int shiftcount, lightcount, doorcount;
    int brakecount, brake; // 0~127
    int iceNm, iceNm2, kW2count;
    double icekW, icekW2, eff, eff2;
    double gaspdl, gaspdlMin, gaspdlMax;
    int gaspdlcount;
    int hvBRcount, hvBVcount, hvBTcount;
    double hvAirT, hvFanV, hvBVmin, hvBVmax;
    int hvdsc, hvBR[16]; // only index going from 1 ~ 14 being used
    double hvBV[16], hvBT[4]; // only 1~14 and 1~3 used
    double mg1Nm, mg2Nm;
    int evapVSV, evapP;
    double dhvBV;
    int mfrpm, mfload, cyn1, cyn2, cyn3, cyn4, cyn5, cyn6, cyn7, cyn8;

    car_data() {
        for(int i=0; i<oaTsSize; i++){oaTs[i]=-100;}
        joaTs = 0;
        amp = 0; ampMin = 0; ampMax = 0;
        hvV = 0; amphvVcount = 0;
        rpm = 0; rpmMin = 0; rpmMax = 0; rpmcount = 0;
        rpm2 = 0; rpm2count = 0;
        speed = 0; speedMin = 0; speedMax = 0; speedcount = 0; speedFR=0;speedFL=0;speedRR=0;speedRL=0;speedFcount=0;speedRcount=0;
        maxdischrgA = 0; maxchrgA = 0; blC = 0; buC = 0;
        soc = 0; dsc = 0;
        soccount = 0;
        ign = 0;
        igncount = 0;
        wT = 0; iceT = 0; wT2 = 0;
        wTcount = 0; iceTcount = 0; wT2count = 0;
        oaT = 0; intakeT = 0;
        oaTcount = 0;
        mg1T = 0; mg2T = 0; inv1T = 0; inv2T = 0; cnvT = 0; inv1Tcount = 0; inv2Tcount = 0; cnvTcount = 0;
        mg1rpm = 0; mg2rpm = 0; mg1rpmcount = 0; mg2rpmcount = 0;
        auxV = 0;
        catT1 = 0; catT2 = 0; atm = 0; auxVcount = 0;
        catT1count = 0; catT2count = 0; atmcount = 0;
        tick = 0; linecountAll = 0; linecountDiscard = 0; tick_timeDelta = 0;
        timeDelta = 0;
        timeDeltacount = 0;
        mpg = 0; lps = 0; dtcdist = 0; //odo=0;
        mpgcount = 0; dtcdistcount = 0;
        fuel = 0; load =0;
        fuelcount = 0; loadcount = 0;
        aLR=0; aLRcount=0; aFB=0; aFBcount=0;
        shiftcount=0; lightcount=0, doorcount=0;
        brakecount=0; brake=0;
        iceNm=0; iceNm2=0; kW2count=0;
        icekW=0; icekW2=0; eff=0; eff2=0;
        gaspdl=0; gaspdlMin=0; gaspdlMax=0;
        gaspdlcount=0;
        hvBRcount=0; hvBVcount=0; hvBTcount=0;
        hvAirT=0; hvFanV=0; hvBVmin=0; hvBVmax=0;
        hvdsc=0;
        for (int i=0;i<16;i++){hvBR[i]=0;hvBV[i]=0;}
        for (int i=0;i<4;i++){hvBT[i]=0;}
        mg1Nm=0; mg2Nm=0;
        evapVSV=0; evapP=0;
        dhvBV=0;
        mfrpm=0; mfload=0; cyn1=0; cyn2=0; cyn3=0; cyn4=0; cyn5=0; cyn6=0; cyn7=0; cyn8=0;
        }

    void init_odo_avgmpg(){
        FILE *fp;
        fp = fopen("ODO.txt","r"); //Initialize odo from file saved
        if (fp==NULL) {printf("!!! Error open file \"ODO.txt\" !!!\n");}
        fscanf(fp,"%lf",&odo);
        fclose(fp);
        odo_t = gettime(); // Initialize time of "previous update_odo()"
        
        lpsTotal = 0;
        odo0 = odo;
        trip = 0;
        mpgmean = 0;
    }

    void update_odo_avgmpg(Gauges *gs){
        const double fac = 1.022;//1.018; // fudge factor to match car ODO reading
        double t = gettime(); // current time
        double dt = t-odo_t;
        odo += fac * speed * dt / 3600.0 ;
        odo_t = t;
        trip = odo-odo0;

        //instantaneous mpg = 378.541*speed*3600/rpm/lps;
        lpsTotal += lps*rpm*dt/3600.0/378.541;
        if (lpsTotal>0){
            mpgmean = trip*3600.0/lpsTotal;
        }
        else {mpgmean = 999.9;}
        
        gs->odo_G->value(odo);
        gs->trip_G->value(trip);
        gs->mpgmean_G->value(mpgmean);
        gs->gal_G->value(lpsTotal/3600.0); //gallon used
    }

    void reset_minmax() {// reset interval min max (the min max during the interval between csv lines)
        ampMin = amp;
        ampMax = amp;
        rpmMin = rpm;
        rpmMax = rpm;
        speedMin = speed;
        speedMax = speed;
        gaspdlMin = gaspdl;
        gaspdlMax = gaspdl;
    }

    void csv_title(FILE *fpcsv) {
        fprintf(fpcsv, "\n\n\nline,tick,tick_timeDelta,timeDelta,auxV,hvV,ampMin,amp,ampMax,rpmMin,rpm,rpmMax,rpm2,MG1rpm,MG2rpm,"
                "MG1Nm,MG2Nm,speedMin,speed,speedMax,maxdischrgA,maxchrgA,"
                "dsc,soc,blC,buC,ign,iceT,wT,wT2,OAT,intakeT,inv1T,inv2T,mg1T,mg2T,cnvT,catT1,catT2,"
                "mpg,DTCdist,ODO,load,aLR,aFB,"
                "speedFR,speedFL,speedRR,speedRL,brake,ICE kW,ICE eff,kW2,eff2,iceNm2,gaspdl,gaspdlMin,gaspdlMax,"
                "hvAirT,hvFanV,hvBVmin,hvBVmax,dhvBV,hvdsc,evapVSV,evapP,"
                "mfrpm,mfload,cyn1,cyn2,cyn3,cyn4,cyn5,cyn6,cyn7,cyn8,");
        for (int i=1;i<15;i++){fprintf(fpcsv,"hvBV%02d,",i);}
        for (int i=1;i<15;i++){fprintf(fpcsv,"hvBR%02d,",i);}
        for (int i=1;i<4;i++){fprintf(fpcsv,"hvBT%02d,",i);}
        fprintf(fpcsv,"\n");
    }

    void csv_data(FILE *fpcsv) {
        fprintf(fpcsv, "%d,%d,%d,%8.6lf,%5.3lf,%d,%.1lf,%.1lf,%.1lf,%d,%d,%d,%d,%d,%d,"
                "%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%d,%d,"
                "%.1lf,%.1lf,%d,%d,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%.1lf,%d,%d,%d,%d,%d,"
                "%d,%d,%.1lf,%.1lf,%4.2lf,%.1lf,%d,%d,"
                "%.1lf,%.1lf,%.1lf,%.1lf,%d,%.1lf,%.1lf,%.1lf,%.1lf,%d,%.1lf,%.1lf,%.1lf,"
                "%.1lf,%.1lf,%.2lf,%.2lf,%.1lf,%d,%d,%d,"
                "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",
                linecountAll, tick, tick_timeDelta, timeDelta, auxV, hvV, ampMin, amp, ampMax, rpmMin, rpm, rpmMax,rpm2,mg1rpm,mg2rpm,
                mg1Nm,mg2Nm,speedMin, speed, speedMax, maxdischrgA, maxchrgA,
                dsc, soc, blC, buC, ign, iceT, wT, wT2, oaT, intakeT, inv1T, inv2T, mg1T, mg2T, cnvT,
                catT1, catT2, mpg, dtcdist,odo,load,aLR,aFB,
                speedFR,speedFL,speedRR,speedRL,brake,icekW,eff,icekW2,eff2,iceNm2,gaspdl,gaspdlMin,gaspdlMax,
                hvAirT,hvFanV,hvBVmin,hvBVmax,dhvBV,hvdsc,evapVSV,evapP,
                mfrpm,mfload,cyn1,cyn2,cyn3,cyn4,cyn5,cyn6,cyn7,cyn8);
        for (int i=1;i<15;i++){fprintf(fpcsv,"%.1lf,",hvBV[i]);}
        for (int i=1;i<15;i++){fprintf(fpcsv,"%d,",hvBR[i]);}
        for (int i=1;i<4;i++){fprintf(fpcsv,"%.1lf,",hvBT[i]);}
        fprintf(fpcsv,"\n");
        reset_minmax();
    }

    void csv_oaTs(void){
        if (joaTs<oaTsSize) {oaTs[joaTs]=oaT; joaTs++;}
        else {printf("OAT array full!\n");}
    }

    void csv_end(FILE *fpcsv, Gauges *gs);

    int decode(char *str, Gauges *gs);

};


#endif	/* _CANDECODE_H */

