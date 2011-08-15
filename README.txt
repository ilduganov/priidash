
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

===============================================================================

SAFETY - READ ME FIRST!

Drive safely and responsibly.

Keep your eyes on the road at all times while driving.

===============================================================================

INTRO

PriiDash(TM) is an enhanced instrument panel and data logger for the Prius and more. Obtain and display real time CAN bus data on a car PC through the OBD port via an ELM327 compatible OBD-USB device.

===============================================================================

HARDWARE AND SOFTWARE SETUP - PLEASE READ THOROUGHLY!

    * Hardware: We need a laptop or netbook, and an ELM327 compatible OBD-USB device. Connect the hardware and set the baud rate to 2M bps. Just as an example and without any implied endorsement or promotion, I am currently using an Eee PC 1001PXD-EU17 10.1-inch netbook with Intel Atom N455 CPU, 1 GB RAM and Win7 Starter (as out of box without any upgrade), and an OBDLink device.
    * Operating System: The software assumes Linux system so it is slightly time consuming to set it up on a Windows machine. Linux users need to recompile from the source and link with X11, FLTK and Boost_thread libraries. Windows users may do the same or use the precompiled "priidash.exe" file and set up directories and the USB-serial port as instructed below. Windows users please download and install Cygwin with the following components:
         1. devel/cmake
         2. devel/gcc ("gcc-core" and "gcc-g++")
         3. devel/make
         4. devel/gdb
         5. devel/subversion (just "subversion")
         6. X11/xinit
         7. X11/libfltk-devel
         8. X11/libfltk1.1-gdi
         9. lib/boost
      The Cygwin installer will suggest some other dependent components. Please keep them selected. Then it will go to the web to download and install all the selected components. On my laptop Cygwin (with the components listed above) takes about 700 MB of disk space.
    * Directories and Ports: The precompiled "priidash.exe" file needs a text file "ODO.txt" (containing the current odometer reading) in the same directory, assumes the USB-serial port is COM7, and saves the log data in the path "f:\OBDLog\". So to use the precompiled "priidash.exe" file directly we need to create the file "ODO.txt" with the current odometer reading and put it in the same directory as the "priidash.exe" file, go to the Device Manager to set the USB-serial port to COM7, and go to the Disk Management to either create a partition "f:" or change the drive letter of a disk to "f:", and then create a directory "OBDLog" under it. Once these are set the program needs to be executed from a cygwin xterm, not from a Windows cmd.exe window.
    * Disk Space: The data logs are plain text and csv files and can take a lot of disk space, about 160 MB per hour of driving. It is probably wise to use a solid state device as opposed to a hard disk unless it's shock proof. For example I use an 8 GB HDSC card and set its drive letter to "f:".
    * Usage: To run the program, first open an X terminal (Start | All Programs | Cygwin-X | XWin Server). Change directory (cd) to where the "priidash.exe" file is and type "./priidash". If the program is run for the first time, then an "About" window will appear, displaying the license agreement. Please read the license agreement carefully and click the "Accept License" button. At a later time the "About" window can be opened by clicking the "About" button in the "Main Control" window. Click the "Go" button in either the "Main Control" window or the "Gauges" window to start data logging and display. Click the same button (text on the button should have changed to "Stop") to stop data logging and display. Click the "x" (close window) button at the upper right corner of the "Main Control" window to quit the program.

===============================================================================

ACKNOWLEDGEMENTS

    * Gary Briggs, author of obdgpslogger, who introduced me to FLTK and cygwin.
    * Michael Pearce, who started the Fl_Gauge program, upon which I have expanded functionality and added many new gauges.
    * Flyer at scantool.net, who came up with a brilliant method to toggle between passive and active CAN message handling.
    * Attila Vass, who pioneered in cracking the Prius CAN codes.
    * Planetaire at priuschat.com, who provided many updated Prius CAN codes since Vass' work.
    * The PriusChat forum community, that inspired my interest in the Prius technology.
    * Many other forums that provided answers to various questions during the development of the software.

===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================


===============================================================================



