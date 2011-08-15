
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


#include <stdio.h>
#include <FL/x.H>
#include <FL/Fl_Draw.H>  
#include <FL/Fl_XPM_Image.H>
#include "icon.h"
#include "favicon.xpm"
//#include "favicon24x24.xpm"

void seticonfromfile(Fl_Window *window, const char *filename){
    Fl_XPM_Image icon(filename) ;

    seticon(window, &icon);
}


void seticonfixed(Fl_Window *window){
//    printf("set icon fixed\n");
    Fl_Pixmap icon(favicon_xpm);
    //Fl_Pixmap icon(favicon24x24_xpm);
    seticon(window, &icon);
}

void seticon(Fl_Window *window, Fl_Pixmap *icon) {
//    printf("set icon\n");

    Fl_Window *dummyWindow = new Fl_Window(50, 50);
    dummyWindow->show();        /* needed to define a graphics context */
    dummyWindow->make_current();
    int maxDim = icon->w();
    if(icon->h() > maxDim)
        maxDim = icon->h();
    Pixmap pixmap = fl_create_offscreen(maxDim, maxDim);
    fl_begin_offscreen(pixmap);
    fl_color(FL_BACKGROUND_COLOR);
    fl_rectf(0, 0, maxDim, maxDim);
    icon->draw((maxDim - icon->w())/2, (maxDim - icon->h())/2);
    fl_end_offscreen();
    dummyWindow->hide();
    delete dummyWindow;
    window->icon((char *)pixmap);
}

