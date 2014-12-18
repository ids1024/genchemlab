Welcome to the General Chemistry Lab Simulator! (version 1.0)

Written by Bryan Herger, Georgia Institute of Technology,
herger@chemistry.gatech.edu

This program is released under the terms of the GNU General Public License.
Portions of the source code are copyright by others.  
Please see the files COPYRIGHT.txt and GPL.txt included in this package.

NOTE:  This is NOT a stable version, and certainly has bugs!
       See LICENSE.txt for copyright and limitation of liability.
       Please report bugs to bherger@users.sourceforge.net

GenChemLab is an OpenGL-based application intended to simulate several
common general chemistry exercises. It is meant to be used to help
students prepare for actual lab experience. It could also be used in
cases where laboratory facilites are not accessible, for instance in
K-12 schools or home schooling.

At present, supported experiments include titration, calorimetry, and
freezing point depression.

GenChemLab features an online manual.  Run the program and press F1 (or 
select Help under the Help menu) for instructions.

The HISTORY file lists changes made in each revision.

The GenChemLab web site is http://genchemlab.sourceforge.net/
Please report bugs and suggest features to me at the e-mail listed above,
or use the sourceforge.net bug tracker on the project home page.

Please subscribe to the genchemlab-announce mailing list for
information about future releases.  Send a blank message to
genchemlab-announce-request@lists.sourceforge.net

INSTALLATION
Go into the source directory and type:
./configure
make

Then, as root, type:
make install

The default data and documentation directory is
/usr/local/share/genchemlab, so if you do not "make install", you will
probably not be able to view the help files.  However, all standard
"configure" options are supported for installation to non-standard
locations.

ACKNOWLEDGEMENTS
Many thanks to Gunner Poulsen for the Danish translation and advice on
  internationalization.
