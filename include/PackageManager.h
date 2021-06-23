#ifndef PYTHON_PLUGIN_PACKAGE_MANAGER_H
#define PYTHON_PLUGIN_PACKAGE_MANAGER_H

#include <ui_PackageManager.h>

class QProcess;
class CommandOutputDialog;
class PythonConfigPaths;

class PackageManager final : public QWidget
{
    Q_OBJECT
  public:
    explicit PackageManager(const PythonConfigPaths &config, QWidget *parent = nullptr);

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
