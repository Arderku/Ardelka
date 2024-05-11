// ConfigurationManager.h
#pragma once

#include "Config.h"

class ConfigurationManager {
public:
    ConfigurationManager();
    void loadConfiguration(const std::string& configFilePath);
    const EngineSettings& getCurrentSettings() const;
    void applySettings(const EngineSettings& settings);

private:
    EngineSettings m_Settings;
};


