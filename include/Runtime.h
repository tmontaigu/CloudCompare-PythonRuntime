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
//#                   COPYRIGHT: Thomas Montaigu                           #
//#                                                                        #
//##########################################################################

#ifndef CLOUDCOMPAREPROJECTS_RUNTIME_H
#define CLOUDCOMPAREPROJECTS_RUNTIME_H

class ccGUIPythonInstance;
class ccCommandLineInterface;

Q_DECL_EXPORT ccGUIPythonInstance *GetInstance();
Q_DECL_EXPORT ccCommandLineInterface *GetCmdLineInstance();

#endif // CLOUDCOMPAREPROJECTS_RUNTIME_H
