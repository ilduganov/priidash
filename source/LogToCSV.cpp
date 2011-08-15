
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

// LogToCSV.cpp --- Extract data from log file and save to csv file
#include <stdio.h>
#include "Util.h"
#include "Dash.h"
#include "CANmsg.h"
#include "icon.h"
//#define LOADvsIGN //special case: pull out ICE load vs. IGN

int logtocsv(const char *logfilename, const char *csvfilename){ // Extract data from log file and save to csv file
    FILE *fplog, *fpcsv;
    car_data cd;
    const int maxLineLength = 1024;
    char str[maxLineLength + 1], *fgetsout;
    int tick_log = 0;
    Gauges gs; // Gauges window class
    double miletick, mileinterval = km2mile * 3; //save additional oaT every mileinterval (miles) (maybe off by one km2mile unit)
    bool miletickset = false;

    
    Fl::lock(); seticonfixed(gs.gauges_window); gs.show();  Fl::flush(); Fl::unlock();// show Gauges window - not fully working yet - only shows correct ending state but no "simulation" of the trip log
    //gs.show(); // doesn't work either
    //Fl::lock(); gs.show(); Fl:wait(0); Fl::unlock(); // doesn't work either

    printf("Extract data from log file and save to csv file ... ");
    double starttime = gettime();

    fplog = fopen(logfilename, "r"); if(fplog == NULL){printf(" can not open %s \n",logfilename);return (-1);}
    fpcsv = fopen(csvfilename, "w"); if(fpcsv == NULL){printf(" can not open %s \n",csvfilename);fclose(fplog);return (-1);}

    cd.csv_title(fpcsv); // write header line to csv file i.e. name for each column

#ifdef LOADvsIGN
    double load0=cd.load, ign0=cd.ign;
#endif

    do {
        fgetsout = fgets(str, maxLineLength, fplog);
        //cd.decode(str, NULL); // decode one line of CAN message in the log file
        cd.decode(str, &gs); // decode one line of CAN message in the log file
        
#ifdef LOADvsIGN
        if(cd.load>70 &&(cd.load!=load0 ||cd.ign!=ign0)){
            load0=cd.load; ign0=cd.ign;
#else
        if (cd.tick >= tick_log) {
            tick_log += 80;//20;//80;
#endif
            cd.csv_data(fpcsv); // save one line of data into csv file
            //cd.reset_minmax(); // reset interval min max (the min max during the interval between csv lines) absorb into csv_data()
                                    if (miletickset){
                                        if(cd.dtcdist>miletick+mileinterval){
                                            //printf("dtcdist = %lf, miletick = %lf ",cd.dtcdist,miletick);
                                            miletick=cd.dtcdist; cd.csv_oaTs();
                                            //printf("-> %lf\n",miletick);
                                        }
                                    }
                                    else if (cd.dtcdistcount>0){miletickset=true; miletick=cd.dtcdist;}
        }
    } while (fgetsout != NULL);

    //cd.csv_end(fpcsv, &gs); // write ending line to csv file i.e. statistics

    fclose(fpcsv);
    fclose(fplog);
    printf(" done in %lf (sec)\n",gettime()-starttime);
    return(0);
}