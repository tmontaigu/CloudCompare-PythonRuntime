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


#ifndef CLOUDCOMPAREPROJECTS_EXPOSED_H
#define CLOUDCOMPAREPROJECTS_EXPOSED_H

Q_DECL_EXPORT ccHObject::Container GetSelectedEntities();

Q_DECL_EXPORT ccHObject *GetDBRoot();

Q_DECL_EXPORT void SetSelectedInDB(ccHObject* obj, bool selected);

Q_DECL_EXPORT ccHObject *LoadFile(const char *filename);

Q_DECL_EXPORT void RemoveFromDB(ccHObject *obj);

Q_DECL_EXPORT void PrintMessage(const char *message);

Q_DECL_EXPORT void PrintWarning(const char *message);

Q_DECL_EXPORT void PrintError(const char *message);

#endif //CLOUDCOMPAREPROJECTS_EXPOSED_H

