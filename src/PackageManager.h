// ##########################################################################
// #                                                                        #
// #                CLOUDCOMPARE PLUGIN: PythonRuntime                       #
// #                                                                        #
// #  This program is free software; you can redistribute it and/or modify  #
// #  it under the terms of the GNU General Public License as published by  #
// #  the Free Software Foundation; version 2 of the License.               #
// #                                                                        #
// #  This program is distributed in the hope that it will be useful,       #
// #  but WITHOUT ANY WARRANTY; without even the implied warranty of        #
// #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
// #  GNU General Public License for more details.                          #
// #                                                                        #
// #                   COPYRIGHT: Thomas Montaigu                           #
// #                                                                        #
// ##########################################################################

#ifndef PYTHON_PLUGIN_PACKAGE_MANAGER_H
#define PYTHON_PLUGIN_PACKAGE_MANAGER_H

#include <ui_PackageManager.h>

class QProcess;
class CommandOutputDialog;
class PythonConfig;

/// Widget that shows an interface allowing the user
/// to see the list of currently installed packages in the
/// current environment.
/// It also allows to install / uninstall packages.
///
/// It works by wrapping pip using QProcess.
class PackageManager final : public QWidget
{
    Q_OBJECT
  public:
    explicit PackageManager(const PythonConfig &config, QWidget *parent = nullptr);
    ~PackageManager() noexcept override;

  private: // Methods
    void refreshInstalledPackagesList();
    void handleInstallPackage();
    void handleUninstallPackage();
    void handleSelectionChanged();
    void handleSearch();

    void executeCommand(const QStringList &arguments);
    void setBusy(bool isBusy);

  private: // Members
    Ui_PackageManager *m_ui;
    QProcess *m_pythonProcess;
    CommandOutputDialog *m_outputDialog;
};

#endif // PYTHON_PLUGIN_PACKAGE_MANAGER_H
