
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

const char FL_GAUGE_VERSION[]="V0.0.7";

/* Modified and expanded by TeamPrii at sourceforge.net. */

/****************************************************************
*                       Fl_Gauge.cxx
*
* Instrumentation style guage widget.
* Useable as general meter, tacho, speedo etc
*
* Author: Michael Pearce
*
* Started: 2 August 2004
*
* Copyright: Copyright 2004 Michael Pearce
*            All Rights Reserved
*
* Licence: LGPL with exceptions same as FLTK license Agreement
*          http://www.fltk.org/COPYING.php
*
* This widget based inpart on the work of the FLTK project
* http://www.fltk.org/
*
*****************************************************************
*                     VERSION INFORMATION
*****************************************************************
* V0.0.6 - 6 August 2004
* Added Green Zone
* Added Red Line Mode
* Altered how callback/beep functions work - now on setting value
*****************************************************************
* V0.0.5 - 5 August 2004
* Fixed drawing values in correct place when min > 0
*****************************************************************
* V0.0.4 - 5 August 2004
* Changed defaults so additional values not shown.
*****************************************************************
* V0.0.3 - 4 August 2004
* Fixed drawing problems when -ve values used on scales.
*****************************************************************
* V0.0.2 - 4 August 2004
* Add values for marks to guage dial
*****************************************************************
* V0.0.1 - 3 August 2004
*  Finish round gauge functionality.
*****************************************************************
* V0.0.0 - 2 August 2004
*  Start of Project - Work on Round Guage
****************************************************************/



#include <stdlib.h>
#include <stdio.h>
#include <math.h>
//#include <sys/time.h>
#include "Fl_Gauge.H"
#include "Util.h"
//#include <float.h>

// #define PI 3.14159265
const double RadPerDeg = 3.1415926536/180;
const bool bDebug = false;

/******************************************************************************
*                              Fl_Gauge - default to simplest number only no graphics
******************************************************************************/
Fl_Gauge :: Fl_Gauge(int X,int Y,int W,int H,const char *l): Fl_Widget(X,Y,W,H,l){

 valueRunFirstTime = true; // initialize whether value() is run for the first time
 value2RunFirstTime = true; // initialize whether value2() is run for the first time

 box(FL_NO_BOX);


 color(FL_BLACK);

 dialcolor(FL_BLACK/*fl_rgb_color(32)*/);
 DialColor0 = dialcolor();
 DialColorG = DialColor0;
 DialColorR = DialColor0;//FL_RED;
 framecolor(FL_DARK_BLUE);
// FrameColor0 = framecolor();
// FrameColorG = FrameColor0;
// FrameColorR = FL_DARK_YELLOW;
 pointercolor(my_fl_light_gray);//FL_GREEN/*fl_rgb_color(160)/*FL_WHITE*/);
 PointerColor0 = pointercolor();
 PointerColorG = PointerColor0;//my_fl_gray;//FL_BLACK;
 PointerColorR = PointerColor0;//my_fl_gray;//FL_RED;
 redlinecolor(FL_DARK_RED);//FL_DARK_BLUE);
 RedLineColor0 = redlinecolor();
 RedLineColorG = RedLineColor0;//FL_DARK_GREEN;
 RedLineColorR = FL_RED;//FL_YELLOW;
 greenzonecolor(my_fl_dark_blue);//FL_DARK_BLUE);
 GreenZoneColor0 = greenzonecolor();
 GreenZoneColorG = FL_DARK_GREEN;
 GreenZoneColorR = GreenZoneColor0;//FL_RED;
 textcolor(my_fl_gray/*fl_rgb_color(160)FL_WHITE*/); //color of gauge scales
 TextColor0 = textcolor();
 TextColorG = FL_GREEN;
 TextColorR = FL_YELLOW;
 ValueColor = FL_WHITE;
 V2Color = FL_BLUE;

 bEverInRedZone = false;

 min(0.0);  //Range of gauge
 max(100.0);

 step(10.0);                /* OFF */
 stepdiv(2.0);              /* OFF */

 scalefontsize(12);
 fontface(FL_HELVETICA/*FL_COURIER*/);
 valuefontsize(28);
 minmaxfontsize(12);

 RedLineStart=90;
 RedLineMode=FL_GAUGE_RL_CBIN;//FL_GAUGE_RL_CBIN;//FL_GAUGE_RL_OFF;
 RL_cb=NULL;

//    temp_gauge->greenzonemode(FL_GAUGE_GZ_CBIN);
//    temp_gauge->redlinemode(FL_GAUGE_RL_CBIN);
 GZ_cb=NULL;
 GreenZoneMode=FL_GAUGE_GZ_CBIN;//FL_GAUGE_GZ_CBIN;//FL_GAUGE_GZ_OFF;
 GreenZoneStart=0;
 GreenZoneEnd=10;

//temp_gauge->v2mode(FL_GAUGE_V2_V1);
 V2Mode=FL_GAUGE_V2_OFF;//FL_GAUGE_V2_V1;//FL_GAUGE_V2_V1;
 V2Real=5;
 V2Decimal=1;

 ValueReal=5;
 ValueDecimal=1;

 ScaleReal=3;
 ScaleDecimal=0;

 OdoMode=FL_GAUGE_ODO_OFF;
 OdoReal=5;
 OdoDecimal=1;
 odoincsize(0.1);

 align(FL_ALIGN_TOP);

 Value = 0.0; // value of gauge
// ValueMax = -DBL_MAX; // Range of read values
// ValueMin = DBL_MAX;
 ValueMax = Value; // Range of read values
 ValueMin = Value;
 ValueStart = Value;
 ValueEnd = Value;
 ValueDisplay = Value;
 Value2Display = Value;
 ValueDisplayDelta = 0.0;
 Value2 = 0.0;
 Value1 = 0.0;


 type(FL_GAUGE_SQUARE);

}


/******************************************************************************
*                            ~Fl_Gauge
******************************************************************************/
Fl_Gauge :: ~Fl_Gauge(void){
}

/******************************************************************************
*                              version
******************************************************************************/
const char* Fl_Gauge::  version(void){
 return(FL_GAUGE_VERSION);
}

/******************************************************************************
*                                 check_zone
******************************************************************************/
void Fl_Gauge:: check_zone(void){
 double Hi,Lo;
 bool bJustEnteredRedZone=false;

/*    timeval tv;
    double mytime, mytime0;

if (bDebug){
  gettimeofday(&tv, NULL);
    mytime0 = (double) (tv.tv_sec) + 0.000001 * (double) tv.tv_usec;
} */

 if(GreenZoneMode){
// check if within gauge range first! avoid using function form otherwise risk infinite loop since e.g. greenzonestart(val) calls check_zone()
  if(GreenZoneStart < Min) {GreenZoneStart=Min;}
  else if(GreenZoneStart > Max) {GreenZoneStart=Max;}
  if(GreenZoneEnd < Min) {GreenZoneEnd=Min;}
  else if(GreenZoneEnd > Max) {GreenZoneEnd=Max;}

  /* Check Green Zone Polarity */
  if(GreenZoneStart > GreenZoneEnd){
   Hi=GreenZoneStart; Lo=GreenZoneEnd;
   GreenZoneStart=Lo;GreenZoneEnd=Hi;
  }

  /* Check for Red line overlap */
  if(RedLineStart < GreenZoneEnd) {RedLineStart=GreenZoneEnd;}
 }

 if(RedLineMode){
  /* Check Red Line Range */
  if(RedLineStart < Min) {RedLineStart=Min;}
  else if(RedLineStart > Max) {RedLineStart=Max;}
 }

 /* Check for REDLINE Beep / Callback mode */
 switch(RedLineMode)
 {
  default:
  case FL_GAUGE_RL_OFF:
  case FL_GAUGE_RL_ON:
   break;

  case FL_GAUGE_RL_BEEP:
   if(Value >= RedLineStart) fl_beep(FL_BEEP_ERROR);
   break;

  case FL_GAUGE_RL_CBIN: // inside red zone
//   if(value() >= redlinestart()){if(RL_cb !=NULL)(RL_cb)(0);}
   if(Value >= RedLineStart){
       textcolor(TextColorR);
       dialcolor(DialColorR);
       greenzonecolor(GreenZoneColorR);
       redlinecolor(RedLineColorR);
       pointercolor(PointerColorR);
       redlinemode(FL_GAUGE_RL_CBOUT);
       bEverInRedZone = true;
       bJustEnteredRedZone = true;
   }
   break;

  case FL_GAUGE_RL_CBOUT:
//   if(value() < redlinestart()){if(RL_cb !=NULL)(RL_cb)(0);}
   if(Value < RedLineStart){
       textcolor(TextColor0);
       dialcolor(DialColor0);
       greenzonecolor(GreenZoneColor0);
       if (bEverInRedZone) {redlinecolor(FL_DARK_RED);}
       else {redlinecolor(RedLineColor0);}
       pointercolor(PointerColor0);
       redlinemode(FL_GAUGE_RL_CBIN);
    }
   break;
 }

 /* Check for Greenline Beep / Callback mode */
 Lo=greenzonestart(); Hi=greenzoneend();
 switch(GreenZoneMode)
 {
  default:
  case FL_GAUGE_GZ_OFF:
  case FL_GAUGE_GZ_ON:
   break;

  case FL_GAUGE_GZ_BEEP:
   if(Value >= Hi || Value <= Lo) fl_beep(FL_BEEP_ERROR);
   break;

  case FL_GAUGE_GZ_CBIN:
//   if(value() <= Hi && value() >= Lo){if(GZ_cb !=NULL)(GZ_cb)(0);}
   if(Value <= Hi && Value >= Lo){
       textcolor(TextColorG);
       dialcolor(DialColorG);
       greenzonecolor(GreenZoneColorG);
       redlinecolor(RedLineColorG);
       pointercolor(PointerColorG);
       greenzonemode(FL_GAUGE_GZ_CBOUT);
   }
   break;

  case FL_GAUGE_GZ_CBOUT:
//   if(value() > Hi || value() < Lo){if(GZ_cb !=NULL)(GZ_cb)(0);}
   if(Value > Hi || Value < Lo){
       if (bJustEnteredRedZone==false){
            textcolor(TextColor0);
            dialcolor(DialColor0);
            greenzonecolor(GreenZoneColor0);
            if (bEverInRedZone) {redlinecolor(FL_DARK_RED);}
            else {redlinecolor(RedLineColor0);}
            pointercolor(PointerColor0);
       }
       greenzonemode(FL_GAUGE_GZ_CBIN);
   }
   break;
 }

    /* if (bDebug){
    gettimeofday(&tv, NULL);
    mytime = (double)(tv.tv_sec) + 0.000001 * (double)tv.tv_usec;
    //printf("Timing check_zone(): mytime0=%20.6f, mytime=%20.6f, delta=%10.6f\n",mytime0,mytime,mytime-mytime0);
    printf("Timing check_zone(): delta=%10.6f\n",mytime-mytime0);
    } */
 
}

/******************************************************************************
 *                                 draw
 ******************************************************************************/
void Fl_Gauge::draw() {
    int X, Y, W, H;
    //timeval tv;
    double mytime, mytime0;

    X = x(); Y = y(); W = w(); H = h();
    
    if (bDebug) {
        //gettimeofday(&tv, NULL);
        //mytime0 = (double) (tv.tv_sec) + 0.000001 * (double) tv.tv_usec;
        mytime0 = gettime();
    }

    fl_push_clip(x(), y(), w(), h()); //Limit all drawing to a rectangular region; later put the drawings back using fl_pop_clip. This rectangle is measured in pixels and is unaffected by the current transformation matrix.

//        fl_push_matrix(); //Save and restore the current transformation. The maximum depth of the stack is 4.
//        fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
//        fl_scale(1, -1);
//        fl_translate(-X - W / 2 + .5, -Y - H / 2 + .5);


    switch (type()) {
        default:
        case FL_GAUGE_ROUND: gauge_round(); break;
        case FL_GAUGE_SQUARE: gauge_square(); break;
        case FL_GAUGE_SQUARE_CLOCK: gauge_square_clock(); break;
        case FL_GAUGE_LINEAR: gauge_linear(); break;
        case FL_GAUGE_SQRT: gauge_sqrt(); break;
        case FL_GAUGE_DIGITAL: gauge_digital(); break;
        case FL_GAUGE_VERTICAL: gauge_vertical(); break;
        case FL_GAUGE_PSD: gauge_psd(); break;
        case FL_GAUGE_XY: gauge_xy(); break;
        case FL_GAUGE_SPEED: gauge_speed(); break;
    }

//    fl_pop_matrix();

    fl_pop_clip();

    //check_zone();   // Done when value changed

    draw_label(); /* Draw Label Last so it is on the top */

    if (bDebug) {
        //gettimeofday(&tv, NULL);
        //mytime = (double) (tv.tv_sec) + 0.000001 * (double) tv.tv_usec;
        mytime = gettime();
        printf("Timing draw() type %2d: delta=%10d (us)\n", type(), int((mytime - mytime0)*1000000.0));
    }

}

void Fl_Gauge::value2str(char *str, double v){ // convert values to strings to be drawn on various gauges
    char sfmt[16];
    if (ValueDecimal > 0) {
        sprintf(sfmt, "%%%d.0%dlf", ValueReal, ValueDecimal); // "%%#%d.0%dlf"
        sprintf(str, sfmt, v);
    } else {
        sprintf(sfmt, "%%%dd", ValueReal); // "%%#%dd" // if no decimal digits then convert to int expression
        sprintf(str, sfmt, (int) floor(0.5 + v));
    }
    //        sprintf(tmps, sfmt, Value);
    //  printf("sfmt = %s\n tmps = %s\n",sfmt,tmps);

}

/******************************************************************************
 *                             gauge_round i.e. draw round style gauge
 ******************************************************************************/
void Fl_Gauge::gauge_round(void) {
    int X, Y, W, H, tx, ty, tw, th;
    int A, B;
    double RL, V;
    char tmps[32], sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */

    fl_color(FrameColor); /* Change to frame colour    */
    fl_line_style(FL_SOLID, 3, 0);
    fl_arc(X + 1, Y + 1, W - 2, H - 2, 0, 360); /* Draw Outer Ring           */
    // fl_color(fl_lighter(framecolor()));        /* Change to frame colour    */
    // fl_line_style(FL_SOLID, 1, 0);
    // fl_arc(X+1,Y+1,W-2,H-2,0,360);             /* Draw Outer Ring           */
    fl_line_style(FL_SOLID, 1, 0);

    /* Draw GreenZone Area */
    if (GreenZoneMode != FL_GAUGE_GZ_OFF) {
        fl_color(GreenZoneColor);
        A = (W / 2) / 3;
        B = A * 2;
        RL = 270 - (((GreenZoneStart - Min) / Range) * 270);
        V = 270 - (((GreenZoneEnd - Min) / Range) * 270);
        fl_pie(X + A, Y + A, W - B, H - B, V - 45, RL - 45); /* Draw Green Zone */
    }

    /* Draw Redline Area */
    if (RedLineMode != FL_GAUGE_RL_OFF && RedLineStart < Max) {
        fl_color(RedLineColor);
        A = (W / 2) / 3;
        B = A * 2;
        RL = 270 - (((RedLineStart - Min) / Range) * 270);
        fl_pie(X + A, Y + A, W - B, H - B, -45, RL - 45); /* Draw Red Zone */
    }

    /* Draw Value marks etc */
    fl_color(TextColor);

    A = (W / 2) / 3; /* Calc Placement of 270 Ring */
    B = A * 2;
    fl_arc(X + A, Y + A, W - B, H - B, -45, 225); /* Draw 270 degree Ring       */

    /* Draw smallest Divisions */
    if (StepDiv > 0 && StepDiv < Step) {
        fl_push_matrix(); //Save and restore the current transformation. The maximum depth of the stack is 4.
        fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
        fl_scale(W - 1, H - 1);
        for (V = Min; V <= Max; V += StepDiv) {
            RL =  -(((V - Min) / Range)*270);

            fl_push_matrix(); //Save and restore the current transformation. The maximum depth of the stack is 4.
//            fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
//            fl_scale(W - 1, H - 1);
            fl_rotate(RL);

            fl_begin_line();
            fl_vertex(-0.22, 0.22);
            fl_vertex(-0.24, 0.24);
            fl_end_line();

            fl_pop_matrix();
        }
        fl_pop_matrix();
    }

    /* Draw Large Divisions */
    if (Step > 0 && Step < Range) {
        fl_push_matrix();
        fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
        fl_scale(W - 1, H - 1);
        for (V = Min; V <= Max; V += Step) {
            RL =  -(((V - Min) / Range)*270);

            fl_push_matrix();
//            fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
//            fl_scale(W - 1, H - 1);
            fl_rotate(RL);

            fl_begin_line();
            fl_vertex(-0.21, 0.21);
            fl_vertex(-0.25, 0.25);
            fl_end_line();

            fl_pop_matrix();
        }
        fl_pop_matrix();
    }

    /* Draw the Guage text (scale) mark Values */
    RL = Min;
    sprintf(sfmt, "%%0%d.0%dlf", ScaleReal, ScaleDecimal);

    fl_font(FontFace, ScaleFontSize);
    fl_color(textcolor());

    A = (W / 3) + 5;
    B = (H / 3) + 5;

    for (V = 270 + 45; V >= 45; V -= (Step / Range * 270)) {
        tx = (int) (sin(V * RadPerDeg) * A) + X + (W / 2);
        ty = (int) (cos(V * RadPerDeg) * B) + Y + (H / 2);

        sprintf(tmps, sfmt, RL);

        tw = th = 0;
        fl_measure(tmps, tw, th, 1);

        if (Min < 0 && Max >= 0) {
            if (RL < ((Max + Min) / 2))tx -= tw;
            else if (RL == ((Max + Min) / 2))tx -= (tw / 2);
        } else {
            if (RL < (Range / 2) + Min)tx -= tw;
            else if (RL == (Range / 2) + Min)tx -= (tw / 2);
        }

        if (ty > (Y + (H / 2))) ty += (th / 2);

        fl_draw(tmps, tx, ty);
        RL += Step;
    }

    /* Draw Pointer */
    V = Value;
    if (V > Max){V = Max;}
    else if (V < Min){V = Min;}
    RL =   - (((V - Min) / Range)*270);
    /* This Matrix modified from FL_Dial */
    fl_push_matrix();
    fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
    fl_scale(W - 1, H - 1);
    fl_rotate(RL);

    fl_color(PointerColor);
    fl_begin_polygon();
    fl_vertex(0.025, -0.025);
    fl_vertex(0.025, 0.025);
    fl_vertex(-0.25, 0.25);
    fl_vertex(-0.025, -0.025);
    fl_end_polygon();
    //fl_color(dialcolor());
    fl_color(TextColor);
    fl_begin_loop();
    fl_vertex(0.025, -0.025);
    fl_vertex(0.025, 0.025);
    fl_vertex(-0.25, 0.25);
    fl_vertex(-0.025, -0.025);
    fl_end_loop();
    fl_pop_matrix();

    /* Draw pointer Centre */ // no aspect ratio scaling yet
    fl_color(framecolor()); /* Change to frame colour    */
    A = (W / 2)-(((W / 2) / 5) / 5); /* Calc Placement of upper left corner */
    B = A * 2;  // lower right corner
    fl_pie(X + A, Y + A, W - B, H - B, 0, 360); /* Draw centre Ring           */
//    fl_point(X + A, Y + A);
    fl_color(TextColor);
    fl_arc(X + A, Y + A, W - B, H - B, 0, 360); /* Draw centre Ring           */

        /* Always Draw Value Text */ // "%%0%d.0%dlf"
        value2str(tmps, Value);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = W * 2 / 3;
        th = H / 5;
        tx = X + (W - tw) / 2;
        ty = Y + 3 * th;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(fl_darker(ValueColor));
        fl_font(FontFace, MinMaxFontSize);
// draw max and min values
        value2str(tmps, ValueMin);
        tw = W / 2;
        ty += th;
        tx -= tw / 2;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        value2str(tmps, ValueMax);
        tx += tw;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);


    if (V2Mode != 0) {
        switch (V2Mode) {
            default:
            case FL_GAUGE_V2_OFF: /* No Value 2 display                   */
                V = 0;
                break;

            case FL_GAUGE_V2_ON: /* Value 2 Displayed                    */
            case FL_GAUGE_V2_TRIP: /* Value 2 displayed works as Trip Meter*/
                V = Value2;
                break;

            case FL_GAUGE_V2_V1: /* Value 2 displayed BUT Equals Value 1 */
                V = Value;
                break;
        }
        if (V2Mode == FL_GAUGE_V2_ON) {

            /* Draw Pointer */
            V = Value2;
            if (V > Max) {V = Max;}
            else if (V < Min) {V = Min;}
            RL = -(((V - Min) / Range)*270);
            /* This Matrix modified from FL_Dial */
            fl_push_matrix();
            fl_translate(X + W / 2 - .5, Y + H / 2 - .5);
            fl_scale(W - 1, H - 1);
            fl_rotate(RL);

            fl_color(V2Color);
            fl_begin_polygon();
            fl_vertex(-0.32, 0.32);
            fl_vertex(-0.28, 0.34);
            fl_vertex(-0.24, 0.24);
            fl_vertex(-0.34, 0.28);
            fl_end_polygon();
            //fl_color(dialcolor());
            fl_color(TextColor);
            fl_begin_loop();
            fl_vertex(-0.32, 0.32);
            fl_vertex(-0.28, 0.34);
            fl_vertex(-0.24, 0.24);
            fl_vertex(-0.34, 0.28);
            fl_vertex(-0.32, 0.32);
            fl_vertex(-0.24, 0.24);
            fl_end_loop();
            fl_pop_matrix();
        }
    }


    /* Draw Odometer Text */
    if (OdoMode != 0) {
        V = Odometer;
        /* Draw Value2 Text */
        sprintf(sfmt, "%%0%d.0%dlf", OdoReal, OdoDecimal);
        sprintf(tmps, sfmt, V);
        fl_color(OdoColor);
        fl_font(FontFace, ValueFontSize);
        fl_measure(tmps, tw, th, 1);
        tx = X + (W / 2)-(tw / 2);
        ty = Y + H - (th * 2);
        fl_draw(tmps, tx, ty);
    }

}// end of void Fl_Gauge::gauge_round(void)

/******************************************************************************
*                            gauge_square - numbers only, no graphs
******************************************************************************/
void Fl_Gauge:: gauge_square(void){
    int X, Y, W, H, tx, ty, tw, th;
    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();
    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, DialColor); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, FrameColor); /* Draw Outer border           */

    /* Always Draw Value Text */ // gauge_square()
    value2str(tmps, Value);
    fl_font(FontFace, ValueFontSize);
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    tw = (W * 2) / 3;
    th = (H * 5) / 6;
    tx = X + (W - tw) / 2;
    ty = Y + H - th;
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    fl_color(PointerColor);
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

    fl_color(fl_darker(ValueColor));
    fl_font(FontFace, MinMaxFontSize);
    value2str(tmps, ValueMin);
    tw = W / 3;
    th = H/4;
    ty = Y;
    tx = X;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
    value2str(tmps, ValueMax);
    tx = X + W - tw;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
} // end of Fl_Gauge:: gauge_square(void)

/******************************************************************************
*                            gauge_speed - numbers only, no graphs, smaller font for decimals
******************************************************************************/
void Fl_Gauge:: gauge_speed(void){
    int X, Y, W, H, tx, ty, tw, th;
    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();
    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, DialColor); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, FrameColor); /* Draw Outer border           */

    /* Always Draw Value Text */ // gauge_speed()
    //value2str(tmps, Value);
    sprintf(tmps,"%2d",(int)floor(Value));
    fl_font(FontFace, ValueFontSize);
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    tw = (W * 2) / 3;
    th = (H * 5) / 6;
    tx = X + (W - tw) / 2;
    ty = Y + H - th;
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    fl_color(PointerColor);
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

    sprintf(tmps,"%d",(int)floor(Value*10-floor(Value)*10));
    fl_font(FontFace, floor(ValueFontSize/2.5));
    tx += tw;
    ty += th/3;
    tw /= 3;
    th /= 3;
    //tx = X + (W - tw) / 2;
    //ty = Y + H - th;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_LEFT);
    //printf("tmps=%s\n",tmps);

    fl_color(fl_darker(ValueColor));
    fl_font(FontFace, MinMaxFontSize);
    value2str(tmps, ValueMin);
    tw = W / 3;
    th = H/4;
    ty = Y;
    tx = X;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
    value2str(tmps, ValueMax);
    tx = X + W - tw;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
} // end of Fl_Gauge:: gauge_speed(void)

/******************************************************************************
*                            gauge_square_clock - numbers only, assume seconds -> hh:mm:ss
******************************************************************************/
void Fl_Gauge:: gauge_square_clock(void){
    int X, Y, W, H, tx, ty, tw, th;
    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();
    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, DialColor); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, FrameColor); /* Draw Outer border           */

    /* Always Draw Value Text */ // gauge_square()
    //value2str(tmps, Value);
    int mm = floor(Value/60);
    double ss = Value - mm*60;
    int hh = floor(mm/60);
    mm -= hh*60;
    sprintf(tmps,"%2d:%2d:%04.1lf",hh,mm,ss);
    fl_font(FontFace, ValueFontSize);
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    tw = (W * 2) / 3;
    th = (H * 5) / 6;
    tx = X + (W - tw) / 2;
    ty = Y + H - th;
    // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
    fl_color(PointerColor);
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

    fl_color(fl_darker(ValueColor));
    fl_font(FontFace, MinMaxFontSize);
    value2str(tmps, ValueMin);
    tw = W / 3;
    th = H/4;
    ty = Y;
    tx = X;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
    value2str(tmps, ValueMax);
    tx = X + W - tw;
    fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
} // end of Fl_Gauge:: gauge_square_clock(void){


/******************************************************************************
*                              gauge_linear - horizontal triangular fill gauge
******************************************************************************/
void Fl_Gauge:: gauge_linear(void){
    int X, Y, W, H, gx, gy, gw, gh, tx, ty, tw, th, x1, x2, y1, y2;
    int A, B;
    double V;
    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */


    gx = X+1; gy = Y+1; gw = W-2; gh = H/2; // Set up position and range of "triangular fill" gauge
    fl_line_style(FL_SOLID, 3, 0);
    fl_draw_box(FL_BORDER_FRAME, gx, gy, gw, gh, FrameColor); /* Draw Outer border of "horizontal bar" gauge           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, gx-1, gy-1, gw+2, gh+2, TextColor); /* Draw Outer border of "horizontal bar" gauge           */

    gx +=2; gy +=2; gw -=4; gh -=4; // Set up position and range of "triangular fill" gauge again to accomodate border


    /* Draw GreenZone Area */
    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
        A = ((GreenZoneStart - Min)* gw) / Range;
        B = ((GreenZoneEnd - GreenZoneStart)* gw) / Range;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
    }

    /* Draw Redline Area */
    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
        A = ((RedLineStart - Min)* gw) / Range;
        B = ((Max - RedLineStart)* gw) / Range;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
   }


    /* Draw Pointer i.e. triangle fill in this case */ // gauge_linear()
    V = Value;
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    A = gx + ((V - Min)* gw) / Range;
    if (Max>0){
        if (Min<0){
            x1 = gx + ((-Min) * gw) / Range; // corresponding to V = 0; the (0,0) point
            y1 = gy - ((-Max) * gh) / Range;
            B = gy - ((V - Max)* gh) / Range;

            fl_color(PointerColor);
            fl_polygon(A,B, A,2*y1-B, x1,y1);
            fl_color(TextColor);
            fl_loop(A,B, A,2*y1-B, x1,y1);
        } else { // Max>0, Min>=0; make it a trapezoid
            x1 = gx; // corresponding to V = Min; the minimum value
            y1 = gy + gh;
            y2 = gy + gh/2;

            B = gy - ((V - Max)* gh/2) / Range;

            fl_color(PointerColor);
            fl_polygon(A,B, A,y1, x1,y1, x1,y2);
            fl_color(TextColor);
            fl_loop(A,B, A,y1, x1,y1, x1,y2);
        }
    }
        /* Draw Value marks after triangle/trapezoid fill so it shows in front of the triangle/trapezoid */
    fl_color(TextColor);
    y1 = gy; y2 = gy+gh;
    for (V=Min; V<=Max; V+=Step){
        x1 = gx + (V-Min)*gw/Range;
        fl_line(x1,y1,x1,y2);
    }
    y1 = gy + gh/3; y2 = gy + gh*2/3;
    for (V=Min; V<=Max; V+=StepDiv){
        x1 = gx + (V-Min)*gw/Range;
        fl_line(x1,y1,x1,y2);
    }


        /* Always Draw Value Text */ // gauge_linear()
        value2str(tmps, Value);
        fl_color(PointerColor);//ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = W / 2;
        th = H - gh;
        tx = X + (W - tw) / 2;
        ty = Y + gh;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(fl_darker(ValueColor));
        fl_font(FontFace, MinMaxFontSize);
        value2str(tmps, ValueMin);
        tw = W / 3;
        //th /= 2;
        //ty += th;
        tx = X;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        value2str(tmps, ValueMax);
        tx = X+tw*2;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

} // end of void Fl_Gauge:: gauge_linear(void)


/******************************************************************************
*                              gauge_sqrt - horizontal triangular fill gauge
******************************************************************************/
void Fl_Gauge:: gauge_sqrt(void){
    int X, Y, W, H, gx, gy, gw, gh, tx, ty, tw, th, x1, x2, y1, y2;
    int A, B;
    double V;
    const double sqrtMin=mysqrt(Min), sqrtMax=mysqrt(Max), sqrtRange=sqrtMax-sqrtMin;
    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */

    gx = X+1; gy = Y+1; gw = W-2; gh = H/2; // Set up position and range of "triangular fill" gauge
    fl_line_style(FL_SOLID, 3, 0);
    fl_draw_box(FL_BORDER_FRAME, gx, gy, gw, gh, FrameColor); /* Draw Outer border of "horizontal bar" gauge           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, gx-1, gy-1, gw+2, gh+2, TextColor); /* Draw Outer border of "horizontal bar" gauge           */

    gx +=2; gy +=2; gw -=4; gh -=4; // Set up position and range of "triangular fill" gauge again to accomodate border


    /* Draw GreenZone Area */
    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
        A = (mysqrt(GreenZoneStart) - sqrtMin)* gw / sqrtRange;
        B = (mysqrt(GreenZoneEnd) - mysqrt(GreenZoneStart))* gw / sqrtRange;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
    }

    /* Draw Redline Area */
    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
        A = (mysqrt(RedLineStart) - sqrtMin) * gw / sqrtRange;
        B = (sqrtMax - mysqrt(RedLineStart)) * gw / sqrtRange;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
   }


    /* Draw Pointer i.e. triangle fill in this case */ // gauge_sqrt()
    V = Value;
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    A = gx + ((mysqrt(V) - sqrtMin)* gw) / sqrtRange;
    B = gy - ((mysqrt(V) - sqrtMax)* gh) / sqrtRange;
    if (Max>0){
        if (Min<0) {
            x1 = gx + ((-sqrtMin) * gw) / sqrtRange; // corresponding to V = 0; the (0,0) point
            y1 = gy - ((-sqrtMax) * gh) / sqrtRange;

            fl_color(PointerColor);
            fl_polygon(A,B, A,2*y1-B, x1,y1);
            fl_color(TextColor);
            fl_loop(A,B, A,2*y1-B, x1,y1);
        }
    }
    
   /* Draw Value marks after triangle fill so it shows above the triangle */
    fl_color(TextColor);
    y1 = gy; y2 = gy+gh;
    for (V=Min; V<=Max; V+=Step){
        x1 = gx + (mysqrt(V)-sqrtMin)*gw/sqrtRange;
        fl_line(x1,y1,x1,y2);
        //printf("V=%lf, x1=%d\n",V,x1);
    }
    y1 = gy + gh/3; y2 = gy + gh*2/3;
    for (V=Min; V<=Max; V+=StepDiv){
        x1 = gx + (mysqrt(V)-sqrtMin)*gw/sqrtRange;
        fl_line(x1,y1,x1,y2);
    }
// end of frawing graphics for gauge_sqrt()

        /* Always Draw Value Text */ // gauge_sqrt()
        value2str(tmps, Value);
        fl_color(PointerColor);//ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = W / 2;
        th = H / 3;
        tx = X + (W - tw) / 2;
        ty = Y + int(th*1.2);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(fl_darker(ValueColor));
        fl_font(FontFace, MinMaxFontSize);
        value2str(tmps, ValueMin);
        tw = W / 3;
        ty += int(th*0.8);
        tx = X;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        value2str(tmps, ValueMax);
        tx = X+tw*2;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

} // end of void Fl_Gauge:: gauge_sqrt(void)


/******************************************************************************
*                              gauge_digital - horizontal bar with pointer moving left/right
******************************************************************************/
void Fl_Gauge:: gauge_digital(void) {
    int X, Y, W, H, gx, gy, gw, gh, tx, ty, tw, th;
    int A, B;
    double V;

    char tmps[32], sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */

    gx = X+1; gy = Y+1; gw = W-2; gh = H/3; // Set up position and range of "horizontal bar" gauge
    fl_line_style(FL_SOLID, 3, 0);
    fl_draw_box(FL_BORDER_FRAME, gx, gy, gw, gh, FrameColor); /* Draw Outer border of "horizontal bar" gauge           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, gx-1, gy-1, gw+2, gh+2, TextColor); /* Draw Outer border of "horizontal bar" gauge           */

    gx +=2; gy +=2; gw -=4; gh -=4; // Set up position and range of "horizontal bar" gauge again to accomodate border


    /* Draw GreenZone Area */
    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
        A = ((GreenZoneStart - Min)* gw) / Range;
        B = ((GreenZoneEnd - GreenZoneStart)* gw) / Range;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
    }

    /* Draw Redline Area */
    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
        A = ((RedLineStart - Min)* gw) / Range;
        B = ((Max - RedLineStart)* gw) / Range;
        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
   }

    /* Draw Value marks etc */
//    fl_color(textcolor());

    /* Draw Pointer */ // gauge_digital()
    V = Value;
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    A = gx + ((V - Min)* gw) / Range;
    B = gy + gh/2;
    fl_color(PointerColor);
    fl_polygon(A-8,B, A,B+gh/2, A+8,B, A,B-gh/2);
    fl_color(TextColor);
    fl_loop(A-8,B, A,B+gh/2, A+8,B, A,B-gh/2);

    /* Draw pointer Centre */
//    fl_color(TextColor); // no need if already selected
    fl_loop(A-1,B-gh/2, A-1,B+gh/2, A+1,B+gh/2, A+1,B-gh/2);
    fl_color(framecolor()); /* Change to frame colour    */
    fl_line(A,B-gh/2, A,B+gh/2);


        /* Always Draw Value Text */ // gauge_digital()
        value2str(tmps, Value);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = (W * 2) / 3;
        th = H / 3;
        tx = X + (W - tw) / 2;
        ty = Y + int(th*1.2);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(fl_darker(ValueColor));
        fl_font(FontFace, MinMaxFontSize);
        value2str(tmps, ValueMin);
        tw = W / 3;
        ty += int(th*0.8);
        tx = X;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        value2str(tmps, ValueMax);
        tx = X+tw*2;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

    if (V2Mode != 0) {
        switch (V2Mode) {
            default:
            case FL_GAUGE_V2_OFF: /* No Value 2 display                   */
                V = 0;
                break;

            case FL_GAUGE_V2_ON: /* Value 2 Displayed                    */
            case FL_GAUGE_V2_TRIP: /* Value 2 displayed works as Trip Meter*/
                V = Value2;
                break;

            case FL_GAUGE_V2_V1: /* Value 2 displayed BUT Equals Value 1 */
                V = Value;
                break;
        }

    
// Draw V2 pointer
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    A = gx + ((V - Min)* gw) / Range;
    B = gy + gh/2;
    fl_color(V2Color);
    fl_polygon(A,B, A+8,B+gh/2, A-8,B+gh/2);
    fl_polygon(A,B, A+8,B-gh/2, A-8,B-gh/2);
    fl_color(TextColor);
    fl_loop(A,B, A+8,B+gh/2, A-8,B+gh/2);
    fl_loop(A,B, A+8,B-gh/2, A-8,B-gh/2);

    /* Draw pointer Centre */
//    fl_color(TextColor); // no need if already selected
    fl_loop(A-1,B-gh/2, A-1,B+gh/2, A+1,B+gh/2, A+1,B-gh/2);
    fl_color(framecolor()); /* Change to frame colour    */
    fl_line(A,B-gh/2, A,B+gh/2);

    }

    /* Draw Odometer Text */
    if (OdoMode != 0) {
        V = Odometer;
        /* Draw Value2 Text */
        sprintf(sfmt, "%%0%d.0%dlf", OdoReal, OdoDecimal);
        sprintf(tmps, sfmt, V);
        fl_color(OdoColor);
        fl_font(FontFace, ValueFontSize);
        fl_measure(tmps, tw, th, 1);
        tx = X + (W / 2)-(tw / 2);
        ty = Y + H - (th * 2);
        fl_draw(tmps, tx, ty);
    }

} // end of void Fl_Gauge:: gauge_digital(void)


/******************************************************************************
*                              gauge_xy - 2D x-y pointer for acceleration sensor
******************************************************************************/
void Fl_Gauge:: gauge_xy(void) {
    int X, Y, W, H, gx, gy, gw, gh, x0, y0, tx, ty, tw, th;
    int A, B;
    double V;

    char tmps[32], sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */
//    fl_line_style(FL_SOLID, 1, 0);
//    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */

    gx = X+1; gy = Y+1; gw = W-2; gh = H-2; // Set up position and range of X-Y gauge
    fl_color(FrameColor);
    fl_line_style(FL_SOLID, 3, 0);
    fl_arc(gx, gy, gw, gh, 0,360); /* Draw Outer border of X-Y gauge          */
    fl_color(TextColor);
    fl_line_style(FL_SOLID, 1, 0);
    fl_arc(gx-1, gy-1, gw+2, gh+2, 0,360); /* Draw Outer border of X-Y gauge          */

//    gx +=2; gy +=2; gw -=4; gh -=4; // Set up position and range of X-Y gauge again to accomodate border

    gw /=2; gh /=2; // change to half width and half height
    x0 = gx+gw; y0 = gy + gh; // center of X-Y, i.e. {0,0}

    /* Draw GreenZone Area */
//    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
//        A = ((GreenZoneStart - Min)* gw) / Range;
//        B = ((GreenZoneEnd - GreenZoneStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
//    }

    /* Draw Redline Area */
//    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
//        A = ((RedLineStart - Min)* gw) / Range;
//        B = ((Max - RedLineStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
//   }

    /* Draw Value marks etc */ // Remember for X-Y gauge Max == -Min == Range/2
    fl_color(TextColor);
    fl_line_style(FL_SOLID, 1, 0); //Because of how line styles are implemented on WIN32 systems, you must set the line style after setting the drawing color. If you set the color after the line style you will lose the line style settings!
    fl_line(x0,y0-gh,x0,y0+gh);
    fl_line(x0-gw,y0,x0+gw,y0);
    if (Step > 0 && Step < Max) {
        A = gw*Step/Max; B = gh*Step/Max;
        for (V=1; (V*Step)<Max; V++){
            fl_arc(x0-V*A,y0-V*B,2*V*A,2*V*B,0,360);
        }
    }
    fl_line_style(FL_DASH, 1, 0);
    if (StepDiv > 0 && StepDiv < Step) {
        A = gw*StepDiv/Max; B = gh*StepDiv/Max;
        for (V=1; (V*StepDiv)<Max; V++){
            fl_arc(x0-V*A,y0-V*B,2*V*A,2*V*B,0,360);
        }
    }
    fl_line_style(FL_SOLID, 1, 0);

    /* Draw Pointer */ // gauge_xy()
    V = Value2;
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    A = x0 + (V * gw) / Max;
    V = Value;
    if (V > Max){V = Max;}
        else if (V < Min){V = Min;}
    B = y0 + (V * gh) / Max;

    fl_color(PointerColor);
    fl_arc(A-3,B-3, 7,7, 0, 360);
    fl_color(fl_lighter(ValueColor)); /* Change to frame colour    */
    fl_line(A-5,B, A+5,B);
    fl_line(A,B-5, A,B+5);

    /* Always Draw Value Text */ // gauge_xy()
        value2str(tmps, Value);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = (W * 2) / 3;
        th = H / 3;
        tx = X + (W - tw) / 2;
        ty = Y + H-th;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(fl_darker(ValueColor));
        fl_font(FontFace, MinMaxFontSize);
        value2str(tmps, ValueMin);
        tw = W / 3;
        //ty += int(th*0.8);
        tx = X;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        value2str(tmps, ValueMax);
        tx = X+tw*2;
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);


} // end of void Fl_Gauge:: gauge_xy(void)



/******************************************************************************
*                              gauge_vertical - vertical thermometer like
******************************************************************************/
void Fl_Gauge:: gauge_vertical(void) {
    int X, Y, W, H, gx, gy, gw, gh, tx, ty, tw, th;
    int A, B, R;
    double V;

    char tmps[32], sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, FL_BLACK);// dialcolor()); /* Draw background           */
    //fl_line_style(FL_SOLID, 1, 0);
    //fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */

    gx = X+1; gy = Y+1; gw = W-2; gh = H*2/3-2; // Set up position and range of graphical region
    //fl_line_style(FL_SOLID, 3, 0);
    fl_draw_box(FL_FLAT_BOX, gx, gy, gw, gh, DialColor); /* Draw background of graphical region        */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, gx-1, gy-1, gw+2, gh+2, TextColor); /* Draw Outer border of graphical region      */

    //gx +=2; gy +=2; gw -=4; gh -=4; // Set up position and range of "horizontal bar" gauge again to accomodate border

    tw = gw/3; th = gh-tw*2; tx = gx+(gw-tw)/2; ty = gy + 4; // set up to draw thermometer region
    fl_draw_box(FL_BORDER_FRAME, tx-1, ty-1, tw+2, th+2+tw, TextColor); /* Draw Outer border of thermometer region      */
    A = tx + tw/2; B = ty+th+tw; // center of thermometer bulb
    R = tw*3/4; //radius of thermometer bulb
    fl_color(TextColor);
    fl_arc(A-R-1,B-R-1,2*R+2,2*R+2,0,360);//outer border of thermometer bulb
    fl_draw_box(FL_FLAT_BOX,tx,ty,tw,th+tw,my_fl_gray); //area of themometer region
    fl_color(PointerColor);
    fl_pie(A-R,B-R,2*R,2*R,0,360);//area of thermometerbulb

//    /* Draw GreenZone Area */
//    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
//        A = ((GreenZoneStart - Min)* gw) / Range;
//        B = ((GreenZoneEnd - GreenZoneStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
//    }
//
//    /* Draw Redline Area */
//    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
//        A = ((RedLineStart - Min)* gw) / Range;
//        B = ((Max - RedLineStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
//   }

    /* Draw Pointer */ // gauge_vertical()
    V = Value;
    if (V > Max){V = Max;} else if (V < Min){V = Min;}
    A = ((V - Min)* th) / Range;
    fl_draw_box(FL_FLAT_BOX,tx,ty+th-A,tw,A+tw,PointerColor);

    // Draw max value read so far
    V = ValueMax;
    if (V > Max){V = Max;} else if (V < Min){V = Min;}
    A = ((V - Min)* th) / Range;
    fl_color(FL_BLACK);
    fl_line(tx,ty+th-A,tx+tw,ty+th-A);
    fl_color(FL_BLACK);
    fl_line(tx,ty+th-A-1,tx+tw,ty+th-A-1);
    fl_line(tx,ty+th-A+1,tx+tw,ty+th-A+1);

    /* Draw Value marks etc */
    fl_color(TextColor);
    if (Step > 0 && Step < Range) {
        for (V = Min; V <= Max; V += Step) {
            A = ty + th - ((V - Min) * th) / Range;
            fl_line(gx + 3, A, tx - 3, A);
            value2str(tmps, V);
            fl_font(FontFace, ScaleFontSize);
            fl_draw(tmps, tx+tw+1, A-ScaleFontSize/2, tx-gx-2, ScaleFontSize, FL_ALIGN_RIGHT);
        }
    }
    if (StepDiv > 0 && StepDiv < Step) {
        for (V = Min; V <= Max; V += StepDiv) {
            A = ty + th - ((V - Min) * th) / Range;
            fl_line((gx+tx)/2, A, tx - 3, A);
        }
    }


        /* Always Draw Value Text */ // gauge_vertical()
        value2str(tmps, Value);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        tw = W;
        th = H / 6;
        tx = X + 1;
        ty = Y + gh;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        fl_color(my_fl_light_gray);
        fl_font(FontFace, MinMaxFontSize);
        ty += th;
        th /= 2;
        tw /= 2;
        tx += tw/2;
        value2str(tmps, ValueMax);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        ty += th;
        value2str(tmps, ValueMin);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
        
} // end of void Fl_Gauge:: gauge_vertical(void)




/******************************************************************************
*                              gauge_psd - Prius power splitting device
******************************************************************************/
void Fl_Gauge:: gauge_psd(void) {
    int X, Y, W, H, gx, gy, gw, gh, tx, ty, tw, th;
    int x0, x1, x2, y0, y1, y2;
    double V;

    char tmps[32];//, sfmt[16];

    X = x(); Y = y(); W = w(); H = h();

    /* Draw main Area */

    draw_box(FL_FLAT_BOX/*FL_ROUNDED_BOX/*FL_FLAT_BOX/*FL_OVAL_BOX*/, dialcolor()); /* Draw background           */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, X, Y, W, H, TextColor); /* Draw Outer border           */

    gx = X+1; gy = Y+1; gw = W-2; gh = H*4/5; // Set up position and range of graphical part
    fl_line_style(FL_SOLID, 2, 0);
    fl_draw_box(FL_BORDER_FRAME, gx, gy, gw, gh, FrameColor); /* Draw Outer border of graphical part          */
    fl_line_style(FL_SOLID, 1, 0);
    fl_draw_box(FL_BORDER_FRAME, gx-1, gy-1, gw+2, gh+2, TextColor); /* Draw Outer border of graphical part           */

    gx +=6; gy +=6; gw -=12; gh -=12; // Set up position and range of graphical part again to accomodate border
    gy += gh*Max/Range; // set gy at center (0 RPM);  gy+gh/2=-6500RPM, gy-gh/2=+6500RPM

    /* horizontal location of MG2 (x2), ICE (x0), and MG1 (x1); MG1 = 3.6 ICE - 2.6 MG2 */
    x2 = gx;
    x1 = gx + gw;
    x0 = gx + gw / 3.6;

    /* Draw GreenZone Area */
//    if (greenzonemode() != FL_GAUGE_GZ_OFF) {
//        A = ((GreenZoneStart - Min)* gw) / Range;
//        B = ((GreenZoneEnd - GreenZoneStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, GreenZoneColor); /* Draw Green Zone */
//    }

    /* Draw Redline Area */
//    if (redlinemode() != FL_GAUGE_RL_OFF && RedLineStart < Max) {
//        A = ((RedLineStart - Min)* gw) / Range;
//        B = ((Max - RedLineStart)* gw) / Range;
//        fl_draw_box(FL_FLAT_BOX, gx+A, gy, B, gh, RedLineColor); /* Draw Red Zone */
//   }

    /* Draw Value marks etc */
//    fl_color(textcolor());
    fl_color(FrameColor);
    fl_line_style(FL_SOLID, 5, 0);
    fl_line(x0,gy-gh*1000/Range, x0,gy-gh*4500/Range); // 1000 ~ 4500 RPM for ICE
    fl_line(x1,gy-gh*Max/Range, x1,gy-gh*Min/Range); // -6500 ~ +6500 RPM for MG1
    fl_line(x2,gy, x2,gy-gh*Max/Range); // 0 ~ 6500 RPM for MG2
    fl_line_style(FL_SOLID, 1, 0);
    fl_line(x1,gy, x2,gy);


    /* Draw Pointer */ // gauge_psd()
    V = Value; if (V > Max){V = Max;}else if (V < Min){V = Min;}
    y0 = gy - V * gh/Range;
    V = Value1; if (V > Max){V = Max;}else if (V < Min){V = Min;}
    y1 = gy - V * gh/Range;
    V = Value2; if (V > Max){V = Max;}else if (V < Min){V = Min;}
    y2 = gy - V * gh/Range;

    fl_color(PointerColor);
    fl_polygon(x0-2,y0-2, x0+2,y0-2, x0+2,y0+2, x0-2,y0+2);
    fl_polygon(x1-2,y1-2, x1+2,y1-2, x1+2,y1+2, x1-2,y1+2);
    fl_polygon(x2-2,y2-2, x2+2,y2-2, x2+2,y2+2, x2-2,y2+2);
    fl_color(TextColor);
    fl_loop(x0-2,y0-2, x0+2,y0-2, x0+2,y0+2, x0-2,y0+2);
    fl_loop(x1-2,y1-2, x1+2,y1-2, x1+2,y1+2, x1-2,y1+2);
    fl_loop(x2-2,y2-2, x2+2,y2-2, x2+2,y2+2, x2-2,y2+2);

    /* Draw lines connecting pointer Centre */
    fl_line_style(FL_SOLID, 2, 0);
    fl_line(x0-2,y0, x2+2,y2);
    fl_line(x1-2,y1, x0+2,y0);
    fl_line_style(FL_SOLID, 1, 0);


        /* Always Draw Value Text */ // gauge_psd()
        ty = Y + H*4/5;
        tw = W / 4;
        th = H / 5;

        value2str(tmps, Value);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        
        tx = (x1+x2)/2-tw/2;//x0;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        value2str(tmps, Value1);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        
        tx = x1-tw;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

        value2str(tmps, Value2);
        fl_color(ValueColor);
        fl_font(FontFace, ValueFontSize);
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        
        tx = x2;
        // printf("tx=%d,ty=%d,tw=%d,th=%d,W=%d,H=%d\n",tx,ty,tw,th,W,H);
        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);

//        fl_color(fl_darker(ValueColor));
//        fl_font(FontFace, MinMaxFontSize);
//        //sprintf(tmps, sfmt, ValueMin); // draw max and min values
//        if (ValueDecimal>0){sprintf(tmps, sfmt, ValueMin);}
//        else{sprintf(tmps, sfmt, (int)floor(0.5+ValueMin));}
//        tw = W / 3;
//        ty += int(th*0.8);
//        tx = X;
//        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);
//        //sprintf(tmps, sfmt, ValueMax);
//        if (ValueDecimal>0){sprintf(tmps, sfmt, ValueMax);}
//        else{sprintf(tmps, sfmt, (int)floor(0.5+ValueMax));}
//        tx = X+tw*2;
//        fl_draw(tmps, tx, ty, tw, th, FL_ALIGN_RIGHT);


} // end of void Fl_Gauge:: gauge_psd(void)


/******************************************************************************
*             odoplaces i.e. significant digits of values and scales
******************************************************************************/
int Fl_Gauge:: odoplaces(int &r,int &d){
 if(r==0 & d==0){
  r=OdoReal;
  d=OdoDecimal;
  return(1);
 }
 OdoReal=r;                /* Odometer Real Number Places */
 OdoDecimal=d;             /* Odometer Decimal Places */
 return(0);
}

int Fl_Gauge:: v2places(int &r,int &d){
 if(r==0 & d==0){
  r=V2Real;
  d=V2Decimal;
  return(1);
 }
 V2Real=r;                /* V2 Real Number Places */
 V2Decimal=d;             /* V2 Decimal Places */
 return(0);
}

int Fl_Gauge:: valueplaces(int &r,int &d){
 if(r==0 & d==0){
  r=ValueReal;
  d=ValueDecimal;
  return(1);
 }
 ValueReal=r;                /* value Real Number Places */
 ValueDecimal=d;             /* value Decimal Places */
 return(0);
}

int Fl_Gauge:: scaleplaces(int &r,int &d){
 if(r==0 & d==0){
  r=ScaleReal;
  d=ScaleDecimal;
  return(1);
 }
 ScaleReal=r;                /* value Real Number Places */
 ScaleDecimal=d;             /* value Decimal Places */
 return(0);
}

