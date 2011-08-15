
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

/*****************************************
*            Fl_Gauge.H
*
* Header file for Fl_Gauge.cxx
* See Fl_Gauge.cxx for details
*****************************************/
#ifndef _Fl_Gauge_H_
#define _Fl_Gauge_H_

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Widget.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <cmath>
//inline double abs(double v){
//    if (v>0){return(v);}
//    else {return(-v);}
//}

const Fl_Color my_fl_dark_gray=fl_color_cube(64*(FL_NUM_RED-1)/255, 64*(FL_NUM_GREEN-1)/255, 64*(FL_NUM_BLUE-1)/255);
const Fl_Color my_fl_gray=fl_color_cube(128*(FL_NUM_RED-1)/255, 128*(FL_NUM_GREEN-1)/255, 128*(FL_NUM_BLUE-1)/255);
const Fl_Color my_fl_light_gray=fl_color_cube(192*(FL_NUM_RED-1)/255, 192*(FL_NUM_GREEN-1)/255, 192*(FL_NUM_BLUE-1)/255);
const Fl_Color my_fl_dark_blue=fl_color_cube(0, 0, 64*(FL_NUM_BLUE-1)/255);



/* Drawing Types */ // all types have value, min, and max shown as numbers like a digital clock/watch
#define FL_GAUGE_ROUND       0  // round gauge with needle and scales
#define FL_GAUGE_SQUARE      1  // only numbers, numbers change color according to pointercolor
#define FL_GAUGE_LINEAR      2  // horizontal triangular-fill gauge
#define FL_GAUGE_SQRT      3  // horizontal triangular-fill gauge with sqrt() scale
#define FL_GAUGE_DIGITAL     4  // horizontal bar gauge with needle, no scales yet
#define FL_GAUGE_VERTICAL   5 // vertical bar gauge like a thermometer
#define FL_GAUGE_PSD   6 // Prius power splitting device
#define FL_GAUGE_SQUARE_CLOCK      7  // only numbers, assume number is in seconds and convert to hh:mm:ss
#define FL_GAUGE_XY    8   // X-Y for acceleration sensor
#define FL_GAUGE_SPEED 9    // only numbers, numbers change color according to pointercolor, smaller font for decimal part

/* Green Zone settings */
#define FL_GAUGE_GZ_OFF    0      /* Turn it OFF */
#define FL_GAUGE_GZ_ON     1      /* Display it  */
#define FL_GAUGE_GZ_BEEP   2      /* Display and Beep if OUT of zone         */
#define FL_GAUGE_GZ_CBIN   3      /* Display and use callback if IN zone     */
#define FL_GAUGE_GZ_CBOUT  4      /* Display and use callback if OUT of zone */


/* Red Line settings */
#define FL_GAUGE_RL_OFF    0      /* Turn it OFF */
#define FL_GAUGE_RL_ON     1      /* Display it  */
#define FL_GAUGE_RL_BEEP   2      /* Display and Beep if OUT of zone         */
#define FL_GAUGE_RL_CBIN   3      /* Display and use callback if IN zone     */
#define FL_GAUGE_RL_CBOUT  4      /* Display and use callback if OUT of zone */


/* Value 2 Modes  */
#define FL_GAUGE_V2_OFF 0      /* No Value 2 display                   */
#define FL_GAUGE_V2_ON   1      /* Value 2 Displayed                    */
#define FL_GAUGE_V2_V1   2      /* Value 2 displayed BUT Equals Value 1 */
#define FL_GAUGE_V2_TRIP 3      /* Value 2 displayed works as Trip Meter*/

/* Odometer Modes */
#define FL_GAUGE_ODO_OFF 0      /* Odometer OFF */
#define FL_GAUGE_ODO_ON  1      /* Odometer ON  */



class FL_EXPORT Fl_Gauge: public Fl_Widget
{

 protected:

  virtual void draw();

  void value2str(char *str, double v);
  /* Drawing functions for different modes */
  void gauge_square(void);
  void gauge_square_clock(void);
  void gauge_round(void);
  void gauge_linear(void);
  void gauge_sqrt(void);
  void gauge_digital(void);
  void gauge_vertical(void);
  void gauge_psd(void);
  void gauge_xy(void);
  void gauge_speed(void);

  /* Variables etc */
  double Value;
  double ValueMax; // range of values read during the lifetime of the gauge
  double ValueMin;
  double ValueStart; // starting and ending value
  double ValueEnd;
  double ValueDisplay; // currently displayed value
  double Value2Display; // currently displayed value2
  double ValueDisplayDelta; // if Value (or Value2 (if V2Mode)) changes more than ValueDisplayDelta then update the displayed value via redraw()
  double Value2;
  double Value1; // for gauge_psd() Value = ICE RPM, Value1 = MG1 RPM, Value2 = MG2 RPM
  double Odometer;
  double OdoIncSize;

  double Min; // range of gauge scale
  double Max;
  double Range; // = Max - Min
  double Step;
  double StepDiv;
  double RedLineStart;
  double GreenZoneStart;
  double GreenZoneEnd;

  int Type;                   /* Type of Guage */
  int RedLineMode;
  int GreenZoneMode;
  int V2Mode;                 /* Value 2 Mode  */
  int ScaleReal; //significant digits for scales
  int ScaleDecimal;
  int ValueReal; // significant digits for value
  int ValueDecimal;
  int V2Real;
  int V2Decimal;
  int OdoMode;                /* Odometer Mode */
  int OdoReal;                /* Odometer Real Number Places */
  int OdoDecimal;             /* Odometer Decimal Places */
  bool bEverInRedZone;

  Fl_Color DialColor; // These colors will change depending on GreenZone or RedLine status
  Fl_Color FrameColor;
  Fl_Color PointerColor;
  Fl_Color RedLineColor;
  Fl_Color TextColor; // color for scale texts
  Fl_Color OdoColor;
  Fl_Color ValueColor; // color for values
  Fl_Color V2Color;
  Fl_Color GreenZoneColor;
  Fl_Color DialColor0; // Original colors outside of GreenZone and RedZone
//  Fl_Color FrameColor0;
  Fl_Color PointerColor0;
  Fl_Color RedLineColor0;
  Fl_Color TextColor0;
//  Fl_Color OdoColor0;
//  Fl_Color V2Color0;
  Fl_Color GreenZoneColor0;
  Fl_Color DialColorG; // Colors when in GreenZone
//  Fl_Color FrameColorG;
  Fl_Color PointerColorG;
  Fl_Color RedLineColorG;
  Fl_Color TextColorG;
//  Fl_Color OdoColorG;
//  Fl_Color V2ColorG;
  Fl_Color GreenZoneColorG;
  Fl_Color DialColorR; // Colors when in RedZone
//  Fl_Color FrameColorR;
  Fl_Color PointerColorR;
  Fl_Color RedLineColorR;
  Fl_Color TextColorR;
//  Fl_Color OdoColorR;
//  Fl_Color V2ColorR;
  Fl_Color GreenZoneColorR;

  int ScaleFontSize; // font size for scale texts
  int FontFace;
  int ValueFontSize; // font size for value texts
  int MinMaxFontSize; // font size for min and max value texts


  void (*RL_cb)(void*);
  void (*GZ_cb)(void*);

  void check_zone(void);

  bool valueRunFirstTime; // to tell if value(input) is run the first time then initialize ValueMin & ValueMax & ValueDisplay
  bool value2RunFirstTime; // to tell if value2(input) is run the first time then initialize Value2Display

 public: //===============================================

  Fl_Gauge(int ,int ,int ,int ,const char * = 0);
  ~Fl_Gauge(void);

  const char* version(void);                         /* return widget version */

  Fl_Color dialcolor(void){return (DialColor);}      /* colour of the Dial    */
  void dialcolor(Fl_Color C){DialColor=C;}
  Fl_Color dialcolor0(void){return (DialColor);}      /* colour of the Dial    */
  void dialcolor0(Fl_Color C){DialColor=C;}
  Fl_Color dialcolorG(void){return (DialColorG);}      /* colour of the Dial when in green zone   */
  void dialcolorG(Fl_Color C){DialColorG=C;}
  Fl_Color dialcolorR(void){return (DialColorR);}      /* colour of the Dial when in red zone   */
  void dialcolorR(Fl_Color C){DialColorR=C;}

  Fl_Color framecolor(void){return (FrameColor);}      /* colour of the Frame    */
  void framecolor(Fl_Color C){FrameColor=C;}

  Fl_Color pointercolor(void){return(PointerColor);} /* Colour of the pointer */
  void pointercolor(Fl_Color C){PointerColor=C;}
  Fl_Color pointercolor0(void){return(PointerColor0);} /* Colour of the pointer */
  void pointercolor0(Fl_Color C){PointerColor0=C;}
  Fl_Color pointercolorG(void){return(PointerColorG);} /* Colour of the pointer */
  void pointercolorG(Fl_Color C){PointerColorG=C;}
  Fl_Color pointercolorR(void){return(PointerColorR);} /* Colour of the pointer */
  void pointercolorR(Fl_Color C){PointerColorR=C;}
  void pointercolorall(Fl_Color C){PointerColor=C;PointerColor0=C;PointerColorG=C;PointerColorR=C;}

  Fl_Color redlinecolor(void){return(RedLineColor);} /* Colour of Over Limit */
  void redlinecolor(Fl_Color C){RedLineColor=C;}
  Fl_Color redlinecolor0(void){return(RedLineColor0);} /* Colour of Over Limit */
  void redlinecolor0(Fl_Color C){RedLineColor0=C;}
  Fl_Color redlinecolorG(void){return(RedLineColorG);} /* Colour of Over Limit */
  void redlinecolorG(Fl_Color C){RedLineColorG=C;}
  Fl_Color redlinecolorR(void){return(RedLineColorR);} /* Colour of Over Limit */
  void redlinecolorR(Fl_Color C){RedLineColorR=C;}

  Fl_Color textcolor(void){return(TextColor);}       /* colour of gauge scale text */
  void textcolor(Fl_Color C){TextColor=C;}
  Fl_Color textcolor0(void){return(TextColor0);}       /* colour of gauge scale text */
  void textcolor0(Fl_Color C){TextColor0=C;}
  Fl_Color textcolorG(void){return(TextColorG);}       
  void textcolorG(Fl_Color C){TextColorG=C;}
  Fl_Color textcolorR(void){return(TextColorR);}       
  void textcolorR(Fl_Color C){TextColorR=C;}

  Fl_Color odocolor(void){return(OdoColor);}         /* colour of odometer text */
  void odocolor(Fl_Color C){OdoColor=C;}

  Fl_Color valuecolor(void){return(ValueColor);}           /* colour of value 2 text */
  void valuecolor(Fl_Color C){ValueColor=C;}
  Fl_Color v2color(void){return(V2Color);}           /* colour of value 2 text */
  void v2color(Fl_Color C){V2Color=C;}

  Fl_Color greenzonecolor(void){return(GreenZoneColor);} /* Green Zone Color */
  void greenzonecolor(Fl_Color C){GreenZoneColor=C;}
  Fl_Color greenzonecolor0(void){return(GreenZoneColor0);} /* Green Zone Color */
  void greenzonecolor0(Fl_Color C){GreenZoneColor0=C;}
  Fl_Color greenzonecolorG(void){return(GreenZoneColorG);} /* Green Zone Color */
  void greenzonecolorG(Fl_Color C){GreenZoneColorG=C;}
  Fl_Color greenzonecolorR(void){return(GreenZoneColorR);} /* Green Zone Color */
  void greenzonecolorR(Fl_Color C){GreenZoneColorR=C;}

  double valuemin(void){return(ValueMin);}
  double valuemax(void){return(ValueMax);}
  double valuestart(void){return(ValueStart);}
  double valueend(void){return(ValueEnd);}
  void valuedisplaydelta(double v){ValueDisplayDelta = v;}
  double value(void){return(Value);}                /* Main Value           */
  void   value(double v) {
  //    static bool firsttime=true; // does not work
      if (Value != v){ // Only update if value has changed
          Value=v;
          ValueEnd = Value;
          if (valueRunFirstTime){ // initialize starting, min, and max to initial value
              ValueStart = Value;
              ValueMin = Value;
              ValueMax = Value;
              ValueDisplay = Value;
              check_zone();
              Fl::lock(); redraw(); Fl::flush(); Fl::unlock();
              valueRunFirstTime = false;
          } else { // update min and max
              if (Value < ValueMin) {ValueMin = Value;}
              else if (Value > ValueMax) {ValueMax = Value;}
              if (fabs(Value-ValueDisplay)>ValueDisplayDelta){
                  ValueDisplay = Value;
                  check_zone();
                  Fl::lock(); redraw(); Fl::flush(); Fl::unlock();
              }
          }
          //check_zone();
          //Fl::lock(); redraw(); Fl::flush(); Fl::unlock();
      }
  }

  double value2(void) {return(Value2);}              /* Secondary Value      */
  void   value2(double v) {
      if(Value2!=v){
          Value2=v;
          if(V2Mode){
              if (value2RunFirstTime){
                  Value2Display = Value2;
                  Fl::lock(); redraw(); Fl::flush(); Fl::unlock();
                  value2RunFirstTime = false;
              }
              else {
                  if (fabs(Value2-Value2Display)>ValueDisplayDelta){
                      Value2Display = Value2;
                      Fl::lock(); redraw(); Fl::flush(); Fl::unlock();
                  }
              }
          }
      }
  }

  void   value1(double v) {// another secondary value for PSD
      if(Value1!=v){
          Value1=v;
          if(V2Mode){Fl::lock(); redraw(); Fl::flush(); Fl::unlock();}
      }
  }

  double redlinestart(void){return(RedLineStart);}   /* Red Line Start point */
  void   redlinestart(double r){RedLineStart=r; /*check_zone();*/}

  int redlinemode(void){return(RedLineMode);}        /* Red Line Mode */
  void redlinemode(int m){RedLineMode=m;}

  void redlinecb(void (*cb)(void *)){RL_cb=cb;}     /* Set Alert Callback    */

  double min(void){return(Min);}                     /* Minimum Guage Value */
  void min(double m){Min=m; Range=Max-Min;}

  double max(void){return(Max);}                     /* Maximum Guage Value */
  void max(double m){Max=m; Range=Max-Min;}

  double step(void){return(Step);}                   /* Graduation step size */
  void step(double s){Step=s;}

  double stepdiv(void){return(StepDiv);}             /* Division between above */
  void stepdiv(double s){StepDiv=s;}

  int scalefontsize(void){return(ScaleFontSize);}              /* Font Size for numbers */
  void scalefontsize(int f){ScaleFontSize=f;}
  int valuefontsize(void){return(ValueFontSize);}              /* Font Size for numbers */
  void valuefontsize(int f){ValueFontSize=f;}
  int minmaxfontsize(void){return(MinMaxFontSize);}              /* Font Size for numbers */
  void minmaxfontsize(int f){MinMaxFontSize=f;}

  int fontface(void){return(FontFace);}              /* Font face for numbers */
  void fontface(int f){FontFace=f;}

  double odometer(void){return(Odometer);}           /* Return Odometer value */
  void odometer(double o){Odometer=o;}               /* Set Odometer value    */

  int odomode(void){return(OdoMode);}                /* Return Odometer Mode  */
  void odomode(int o){OdoMode=o;};                   /* Set Odometer Mode     */

  int odoplaces(int &,int &);                        /* Set/get format for odmeter*/

  double odoincsize(void){return(OdoIncSize);}
  void odoincsize(double i){OdoIncSize=i;}

  void odoinc(void){Odometer+=OdoIncSize;if(V2Mode==FL_GAUGE_V2_TRIP)Value2+=OdoIncSize;}
  void ododec(void){Odometer-=OdoIncSize;if(V2Mode==FL_GAUGE_V2_TRIP)Value2-=OdoIncSize;}

  int v2mode(void){return (V2Mode);}                 /* return value 2 Mode   */
  void v2mode(int v){V2Mode=v;}                      /* Set Value 2 Mode      */

  int v2places(int &,int &);
  int valueplaces(int &, int &);
  int scaleplaces(int &, int &);

  int greenzonemode(void){return(GreenZoneMode);}
  void greenzonemode(int m){GreenZoneMode=m;/*check_zone();*/}

  double greenzonestart(void){return (GreenZoneStart);}
  void greenzonestart(double z) {GreenZoneStart=z;/*check_zone();*/}   /* Begining of green zone */

  double greenzoneend(void) {return(GreenZoneEnd);}
  void greenzoneend(double z) {GreenZoneEnd=z;/*check_zone();*/}

  void greenzonecb(void (*cb)(void *)){GZ_cb=cb;}     /* Set GreenZone Callback    */

  
  int type(void){return(Type);}                      /* Type of gauge drawn  */
  void type(int t){
      int rp=2, dp=0;
      Type=t;
      switch (Type){
          case FL_GAUGE_VERTICAL:{
              max(65);min(10);step(10);stepdiv(5);rp=2; dp=0;valueplaces(rp,dp);
              redlinestart(45);greenzoneend(20);greenzonestart(10);valuecolor(labelcolor());scalefontsize(10);
              textcolor(my_fl_light_gray);textcolor0(my_fl_light_gray);textcolorG(FL_DARK_CYAN);textcolorR(FL_YELLOW);
              valuefontsize(28);
              dialcolor(my_fl_dark_gray);dialcolor0(my_fl_dark_gray);dialcolorG(FL_DARK_BLUE);dialcolorR(FL_RED);
              pointercolor(FL_DARK_RED);pointercolor0(FL_DARK_RED);pointercolorG(FL_DARK_RED);pointercolorR(FL_YELLOW);
          } break;
          case FL_GAUGE_XY:{
              min(-20);max(20);step(10);stepdiv(5);
              redlinemode(FL_GAUGE_RL_OFF);greenzonemode(FL_GAUGE_GZ_OFF);
              pointercolor(FL_GREEN);valuecolor(labelcolor());
              v2mode(FL_GAUGE_V2_ON);
          } break;
          case FL_GAUGE_SQUARE:{
              //pointercolor(labelcolor()); //not working because need to put in fl file otherwise labelcolor() is not set yet and default to black
          } break;
          default:{
          } break;
      }
  }


};



/******* END OF FILE ********/
#endif

