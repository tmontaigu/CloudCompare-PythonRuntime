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

#include <ccMainAppInterface.h>

class Q_DECL_EXPORT ccPythonInstance {
public:
	explicit ccPythonInstance(ccMainAppInterface *app);

	bool haveSelection() const { return m_app->haveSelection(); }
	bool haveOneSelection() const { return m_app->haveOneSelection(); }
	const ccHObject::Container& getSelectedEntities() const { return m_app->getSelectedEntities(); }
	void setSelectedInDB(ccHObject* obj, bool selected) { m_app->setSelectedInDB(obj, selected); }
	ccHObject* dbRootObject() { return m_app->dbRootObject(); }
	void redrawAll(bool only2D = false) { return m_app->redrawAll(only2D); }
	void refreshAll(bool only2D = false) { return m_app->refreshAll(only2D); }
	void enableAll() { return m_app->enableAll(); }
	void disableAll() { return m_app->disableAll(); }
	void updateUI() { return m_app->updateUI(); }
	void freezeUI(bool state) { return m_app->freezeUI(state); }

	ccHObject *loadFile(const char* filename);

private:
	ccMainAppInterface* m_app;
};

Q_DECL_EXPORT ccPythonInstance* GetInstance();

Q_DECL_EXPORT void PrintMessage(const char *message);

Q_DECL_EXPORT void PrintWarning(const char *message);

Q_DECL_EXPORT void PrintError(const char *message);

#endif //CLOUDCOMPAREPROJECTS_EXPOSED_H

