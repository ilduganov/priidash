
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

// Util.cpp utilities
#include "OS.h"
#include <stdio.h>
#include <math.h>
#include "Util.h"
#include "LogMsg.h"

#ifdef VC // Visual C
//#define _CRT_SECURE_NO_WARNINGS // put in Property Page > Preprocessor Definitions
#include <time.h>
#include <Windows.h>
struct timezone {
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
// Definition of a gettimeofday function
int gettimeofday(struct timeval *tv, struct timezone *tz)
{
// Define a structure to receive the current Windows filetime
  FILETIME ft;
// Initialize the present time to 0 and the timezone to UTC
  unsigned __int64 tmpres = 0;
  static int tzflag = 0;
 
  if (NULL != tv){
    GetSystemTimeAsFileTime(&ft);
// The GetSystemTimeAsFileTime returns the number of 100 nanosecond 
// intervals since Jan 1, 1601 in a structure. Copy the high bits to 
// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
    tmpres |= ft.dwHighDateTime;
    tmpres <<= 32;
    tmpres |= ft.dwLowDateTime;
// Convert to microseconds by dividing by 10
    tmpres /= 10;
// The Unix epoch starts on Jan 1 1970.  Need to subtract the difference 
// in seconds from Jan 1 1601.
    tmpres -= DELTA_EPOCH_IN_MICROSECS;
// Finally change microseconds to seconds and place in the seconds value. 
// The modulus picks up the microseconds.
    tv->tv_sec = (long)(tmpres / 1000000UL);
    tv->tv_usec = (long)(tmpres % 1000000UL);
  }
  if (NULL != tz){
    if (!tzflag){
      _tzset();
      tzflag++;
    }
// Adjust for the timezone west of Greenwich
      tz->tz_minuteswest = _timezone / 60;
    tz->tz_dsttime = _daylight;
  }
  return 0;
}
#else // Unix
#include <unistd.h>
#include <sys/time.h>
#endif

double mysqrt(double x){
    if (x>=0){return(sqrt(x));}
    else {return(-sqrt(-x));}
}

double gettime(void){
    timeval tv;
    double mytime;

    gettimeofday(&tv, NULL);
    mytime = tv.tv_sec + 0.000001 * tv.tv_usec;
    return mytime;
}

size_t getdatetimestr(char * ptr){
    const size_t maxsize = 16;
    const char format[]="%Y%m%d_%H%M%S";
    time_t rawtime;
    struct tm * timeinfo;
    size_t strlen;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strlen = strftime (ptr, maxsize, format, timeinfo); // If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned. Otherwise, zero is returned and the contents of the array are indeterminate.
    ptr[strlen] = '\0';
    return(strlen);
}

size_t getdatetimeclockstr(char * ptr){
    const size_t maxsize = 20;
    const char format[]="%Y-%m-%d %H:%M:%S";
    time_t rawtime;
    struct tm * timeinfo;
    size_t strlen;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strlen = strftime (ptr, maxsize, format, timeinfo); // If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned. Otherwise, zero is returned and the contents of the array are indeterminate.
    ptr[strlen] = '\0';
    return(strlen);
}

size_t getdatestr(char * ptr){
    const size_t maxsize = 16;
    const char format[]="%Y-%m-%d";
    time_t rawtime;
    struct tm * timeinfo;
    size_t strlen;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strlen = strftime (ptr, maxsize, format, timeinfo); // If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned. Otherwise, zero is returned and the contents of the array are indeterminate.
    ptr[strlen] = '\0';
    return(strlen);
}
size_t gettimestr(char * ptr){
    const size_t maxsize = 16;
    const char format[]="%H:%M:%S";
    time_t rawtime;
    struct tm * timeinfo;
    size_t strlen;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strlen = strftime (ptr, maxsize, format, timeinfo); // If the resulting C string fits in less than maxsize characters including the terminating null-character, the total number of characters copied to ptr (not including the terminating null-character) is returned. Otherwise, zero is returned and the contents of the array are indeterminate.
    ptr[strlen] = '\0';
    return(strlen);
}

void wait(double seconds)
{
#ifdef VC
  //double endwait;
  //endwait = gettime() + seconds;
  //while (gettime() < endwait) {}
	int ms;
	ms = 1000 * seconds;
	Sleep(ms);
#else
    int us = int(seconds*1000000); // convert to microseconds
    usleep(us);
#endif
}

double logtime(const double mytime0, FILE *fplog){
    double mytime;

    mytime = gettime();
//    fprintf(fplog,"\nFFFF gettimeofday()=%20.6f(sec), delta=%12.6f(sec)\n", mytime, mytime - mytime0);
    fprintf(fplog,"\n%s gettimeofday()=%20.6lf(sec), delta=%12.6lf(sec)\n", ID_TimeStamp_Str, mytime, mytime - mytime0);
    //fflush(fplog);
    return mytime;
}

double logtime(const double mytime0, FILE *fplog, const char *str){
    double mytime;

    mytime = gettime();
    fprintf(fplog,"\n%s gettimeofday()=%20.6lf(sec), delta=%12.6lf(sec), %s\n", ID_TimeStamp_Str, mytime, mytime - mytime0,str);
    //fflush(fplog);
    return mytime;
}