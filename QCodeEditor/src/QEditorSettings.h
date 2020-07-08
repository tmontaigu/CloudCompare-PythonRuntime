//
// Created by t.montaigu on 08/07/2020.
//

#ifndef CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H
#define CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H

#include <ui_QEditorSettings.h>

class QEditorSettings : public QWidget, public Ui::QEditorSettings
{
	Q_OBJECT
public:
	QEditorSettings();

	int fontSize() const;

public :
	Q_SIGNALS:
			void settingsChanged();

protected:
	void connectSignals();
};


#endif //CLOUDCOMPAREPROJECTS_QEDITORSETTINGS_H
