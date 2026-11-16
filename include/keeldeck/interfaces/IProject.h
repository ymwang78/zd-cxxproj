#pragma once

#include <QString>
#include <QWidget>
#include <QIcon>
#include <QJsonObject>
#include <memory>

class ProjectHost;

/**
 * @brief Abstract interface for all project types
 *
 * This interface defines the contract that all APC/RTO project types must implement.
 * It enables a plugin-based architecture where different project types (TaijiMPC,
 * Linear Programming, etc.) can be seamlessly integrated into the xApc platform.
 */
class IProject {
  public:
    virtual ~IProject() = default;

    // === Metadata ===
    /**
     * @brief Get the display name of this project type
     * @return Project type name (e.g., "Taiji MPC", "LP Optimizer")
     */
    virtual QString getProjectTypeName() const = 0;

    /**
     * @brief Get the icon for this project type (used in project tree)
     * @return Icon for the project type
     */
    virtual QIcon getProjectTypeIcon() const = 0;

    /**
     * @brief Get a brief description of this project type
     * @return Description text
     */
    virtual QString getProjectTypeDescription() const = 0;

    /**
     * @brief Get the file extension for this project type
     * @return File extension (e.g., "tmpc", "lpopt")
     */
    virtual QString getProjectFileExtension() const = 0;

    // === Project Lifecycle ===

    /**
     * @brief Load project from file
     * @param filePath Full path to project file
     * @return true if successful
     */
    virtual bool loadFromFile(const QString& filePath) = 0;

    /**
     * @brief Save project to file
     * @param filePath Full path to save to (empty = use current file)
     * @return true if successful
     */
    virtual bool saveToFile(const QString& filePath = QString()) = 0;

    /**
     * @brief connect project
     * @return true if successful
     */
    virtual bool connectProject() = 0;

    /**
     * @brief Get the raw project name without host or endpoint prefix.
     * @return Project name
     */
    virtual QString getProjectName() const = 0;

    /**
     * @brief Get a stable project GUID when the project type provides one.
     * @return Stable project GUID, or empty when unavailable
     */
    virtual QString getProjectGuid() const { return QString(); }

    /**
     * @brief Get the key used for project-scoped UI state inside a ProjectHost.
     * @return Project GUID when available; otherwise project name
     */
    virtual QString getProjectUiKey() const {
        const QString guid = getProjectGuid().trimmed();
        return guid.isEmpty() ? getProjectName().trimmed() : guid;
    }

    /**
     * @brief Get project display name
     * @return Display name (combines host name/endpoint and project name if available)
     */
    virtual QString getProjectDisplayName() const { return getProjectName(); }

    // === UI Creation (Core of Plugin Architecture) ===
    /**
     * @brief Create the main configuration/operation widget for this project type
     *
     * This is the heart of the plugin system. Each project type returns its own
     * custom widget that will be displayed in the main application area.
     *
     * @param parent Parent widget
     * @return Main widget for this project type (ownership transferred to caller)
     */
    virtual QWidget* createMainWidget(QWidget* parent = nullptr) = 0;

    /**
     * @brief Create a properties widget for project settings
     * @param parent Parent widget
     * @return Properties widget (ownership transferred to caller)
     */
    virtual QWidget* createPropertiesWidget(QWidget* parent = nullptr) = 0;

    // === Project Operations ===
    /**
     * @brief Verify project configuration
     * @return Verification result with errors/warnings
     */
    virtual bool verifyConfiguration(QStringList* errors = nullptr,
                                     QStringList* warnings = nullptr) = 0;

    /**
     * @brief Start the main operation (testing, optimization, etc.)
     * @return true if started successfully
     */
    virtual bool startOperation() = 0;

    /**
     * @brief Stop the current operation
     * @return true if stopped successfully
     */
    virtual bool stopOperation() = 0;

    /**
     * @brief Check if operation is currently running
     * @return true if running
     */
    virtual bool isOperationRunning() const = 0;

    // === Data Import/Export ===
    /**
     * @brief Get supported import file formats
     * @return List of file filters (e.g., "CSV Files (*.csv)")
     */
    virtual QStringList getSupportedImportFormats() const = 0;

    /**
     * @brief Get supported export file formats
     * @return List of file filters
     */
    virtual QStringList getSupportedExportFormats() const = 0;

    /**
     * @brief Import data from file
     * @param filePath Path to data file
     * @param format Format identifier
     * @return true if successful
     */
    virtual bool importData(const QString& filePath, const QString& format) = 0;

    /**
     * @brief Export data to file
     * @param filePath Path to export file
     * @param format Format identifier
     * @return true if successful
     */
    virtual bool exportData(const QString& filePath, const QString& format) = 0;

    // === Project Information ===
    /**
     * @brief Get project statistics as key-value pairs
     * @return Statistics map (e.g., "Signal Count" -> "25")
     */
    virtual QVariantMap getProjectStatistics() const = 0;

    // === Plugin Integration Callbacks ===
    /**
     * @brief Called when project is activated in the main window
     */
    virtual void onProjectActivated() {}

    /**
     * @brief Called when project is deactivated in the main window
     */
    virtual void onProjectDeactivated() {}

    virtual ProjectHost* getProjectHost() const = 0;

    /**
     * @brief Check if the project can be verified
     * @return true if verification is allowed
     */
    virtual bool canVerify() const { return false; }

    /**
     * @brief Called when the user triggers "verify current project"
     *
     * The core calls this on the current project (after canVerify() passed) instead of
     * broadcasting a main-window signal, so plugins never need MainWindow.h.
     */
    virtual void onVerifyRequested() {}

    /**
     * @brief Check if this project supports in-place renaming.
     */
    virtual bool canRenameProject() const { return false; }

    /**
     * @brief Update in-memory name and file path metadata after a project rename.
     *
     * For file-backed local projects, this may also be called when canRenameProject() is false
     * to inform the project instance of the renamed file path on disk.
     *
     * @param newName New display/instance name of the project.
     * @param newFilePath New absolute file path on disk, if file-backed.
     * @return true if the project accepted the rename, false otherwise.
     */
    virtual bool renameProject(const QString& newName, const QString& newFilePath = QString()) {
        Q_UNUSED(newName);
        Q_UNUSED(newFilePath);
        return false;
    }
};

/**
 * @brief Smart pointer type for IProject
 */
using IProjectPtr = std::unique_ptr<IProject>;
