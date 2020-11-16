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

#ifndef CLOUDCOMPAREPROJECTS_UTILITIES_H
#define CLOUDCOMPAREPROJECTS_UTILITIES_H

#include <QString>

/// Returns a newly allocated array (null terminated) from a QString
inline wchar_t *QStringToWcharArray(const QString &string)
{
    auto *wcharArray = new wchar_t[string.size() + 1];
    int len = string.toWCharArray(wcharArray);
    Q_ASSERT(len <= string.size());
    wcharArray[len] = '\0';
    return wcharArray;
}
#endif // CLOUDCOMPAREPROJECTS_UTILITIES_H
