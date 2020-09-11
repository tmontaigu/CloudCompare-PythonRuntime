//##########################################################################
//#                                                                        #
//#                CLOUDCOMPARE PLUGIN: PythonPlugin                       #
//#                                                                        #
//#  This program is free software; you can redistribute it and/or modify  #
//#  it under the terms of the GNU General Public License as published by  #
//#  the Free Software Foundation; version 2 of the License.               #
//#                                                                        #
//#  This program is distributed in the hope that it will be useful,       #
//#  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
//#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
//#  GNU General Public License for more details.                          #
//#                                                                        #
//#                             COPYRIGHT: Thomas Montaigu                 #
//#                                                                        #
//##########################################################################

#pragma once

class ccMainAppInterface;
class ccCommandLineInterface;


namespace Python
{
	void setMainAppInterfaceInstance(ccMainAppInterface *appInterface);
	void unsetMainAppInterfaceInstance();

        size_t clearDB();

	void setCmdLineInterfaceInstance(ccCommandLineInterface *cmdLine);
	void unsetCmdLineInterfaceInstance();
}
