// Copyright 2020, Verizon Media
// Licensed under the terms of MIT. See LICENSE file in project root for terms.

#ifndef DATA_H
#define DATA_H

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QStringLiteral>
#include <cstdlib>
#include <stdexcept>

#include "config/config.h"
#include "config/config_v1.h"
#include "config/no_config.h"
#include "exceptions/fileerror.h"
#include "helpers/constants.h"
#include "helpers/file_helpers.h"

// AppConfig is a singleton construct for accessing the application's configuration.
// singleton design borrowed from:
// https://stackoverflow.com/questions/1008019/c-singleton-design-pattern
class AppConfig {
 public:
  static AppConfig &getInstance() {
    static AppConfig instance;
    return instance;
  }
  AppConfig(AppConfig const &) = delete;
  void operator=(AppConfig const &) = delete;

 private:
  AppConfig() noexcept {
    _config = new NoConfig();

    try {
      readConfig();
    }
    catch (std::exception &e) {
      _config->setErrorText(e.what());
    }
  }

  ~AppConfig() noexcept { delete _config; }

  void readConfig(QString location=Constants::configLocation()) {
    QFile configFile(location);

    if (!configFile.open(QIODevice::ReadOnly)) {
      if (configFile.exists()) {
        throw FileError::mkError("Error reading config file", location.toStdString(),
                                 configFile.error());
      }
      writeDefaultConfig();
      return;
    }
    QByteArray data = configFile.readAll();

    if (configFile.error() != QFile::NoError) {
      throw FileError::mkError("Error reading config file", location.toStdString(),
                               configFile.error());
    }

    _config = ConfigV1::fromJson(data);
    if (_config->errorText() != "") {
      throw std::runtime_error("Unable to parse config file");
    }
  }

  void writeDefaultConfig() noexcept {
    _config = ConfigV1::generateDefaultConfig();
    upgrade();

    try {
      writeConfig();
    }
    catch (FileError &e) {
      Q_UNUSED(e);
      // ignoring error -- best effort approach
    }
  }

 public:
  /// Upgrade changes the underlying configuration from its current version to the latest
  /// possible version. Note that this does NOT change the underlying config file. To update the
  /// actual file, use writeConfig()
  bool upgrade() {
    if (_config == nullptr) {
      return false;
    }
    bool upgraded = false;

    return upgraded;
  }

  /// writeConfig writes out the current internal configuration state to a file.
  /// @throws FileError if there is an issue opening or writing the file
  void writeConfig(QString alternateSavePath="") {
    QString writeLoc = alternateSavePath == "" ? Constants::configLocation() : alternateSavePath;

    auto configContent = _config->toJsonString();
    try {
      FileHelpers::mkdirs(writeLoc, true);  // ensure the path exists
      FileHelpers::writeFile(writeLoc, configContent);
    }
    catch(const FileError &e) {
      // rewrap error for easier identification
      throw FileError::mkError("Error writing config file", writeLoc.toStdString(),
                               e.fileDeviceError);
    }
    return;
  }

 public:
  // accessors
  QString errorText() { return _config->errorText(); }

  ConfigV1 *asConfigV1() {
    if (_config->version() == 1) {
      return static_cast<ConfigV1 *>(_config);
    }
    return nullptr;
  }

  QString evidenceRepo() { return _config->evidenceRepo(); }
  QString captureScreenAreaCmd() { return _config->captureScreenAreaCmd(); }
  QString captureScreenAreaShortcut() { return _config->captureScreenAreaShortcut(); }
  QString captureScreenWindowCmd() { return _config->captureScreenWindowCmd(); }
  QString captureScreenWindowShortcut() { return _config->captureScreenWindowShortcut(); }
  QString captureCodeblockShortcut() { return _config->captureCodeblockShortcut(); }

  // mutators
  void setEvidenceRepo(QString newVal) { _config->setEvidenceRepo(newVal); }
  void setCaptureScreenAreaCmd(QString newVal) { _config->setCaptureScreenAreaCmd(newVal); }
  void setCaptureScreenAreaShortcut(QString newVal) {
    _config->setCaptureScreenAreaShortcut(newVal);
  }
  void setCaptureScreenWindowCmd(QString newVal) { _config->setCaptureScreenWindowCmd(newVal); }
  void setCaptureScreenWindowShortcut(QString newVal) {
    _config->setCaptureScreenWindowShortcut(newVal);
  }
  void setCaptureCodeblockShortcut(QString newVal) { _config->setCaptureCodeblockShortcut(newVal); }

 private:
  Config *_config = nullptr;
};

#endif  // DATA_H
