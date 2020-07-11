#ifndef CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H
#define CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H

#include <ccMainAppInterface.h>


class Q_DECL_EXPORT ccGUIPythonInstance {
public:
	explicit ccGUIPythonInstance(ccMainAppInterface *app);

	bool haveSelection() const { return m_app->haveSelection(); }

	bool haveOneSelection() const { return m_app->haveOneSelection(); }

	const ccHObject::Container &getSelectedEntities() const { return m_app->getSelectedEntities(); }

	void setSelectedInDB(ccHObject *obj, bool selected) { m_app->setSelectedInDB(obj, selected); }

	ccHObject *dbRootObject() { return m_app->dbRootObject(); }

	void addToDB(ccHObject *obj,
	             bool updateZoom = false,
	             bool autoExpandDBTree = true,
	             bool checkDimensions = false,
	             bool autoRedraw = true) {
		m_app->addToDB(obj, updateZoom, autoExpandDBTree, checkDimensions, autoRedraw);
	}

	void redrawAll(bool only2D = false) { return m_app->redrawAll(only2D); }

	void refreshAll(bool only2D = false) { return m_app->refreshAll(only2D); }

	void enableAll() { return m_app->enableAll(); }

	void disableAll() { return m_app->disableAll(); }

	void updateUI() { return m_app->updateUI(); }

	void freezeUI(bool state) { return m_app->freezeUI(state); }

	ccHObject *loadFile(const char *filename);

private:
	ccMainAppInterface *m_app;
};

#endif //CLOUDCOMPAREPROJECTS_CCGUIPYTHONINSTANCE_H
