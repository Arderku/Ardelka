
#include "ConfigurationManager.h"
#include <fstream>
#include <stdexcept>

ConfigurationManager::ConfigurationManager() {
    // Initialize with default settings
    m_Settings = {{"Default Title", 800, 600, false}};
}


void ConfigurationManager::loadConfiguration(const std::string& configFilePath) {
    std::ifstream configFile(configFilePath);
    if (!configFile) {
        throw std::runtime_error("Failed to open config file");
    }


    // EngineSettings newSettings = parseConfigFile(configFile);

    // m_Settings = newSettings;
}

const EngineSettings& ConfigurationManager::getCurrentSettings() const {
    return m_Settings;
}

void ConfigurationManager::applySettings(const EngineSettings& settings) {
    m_Settings = settings;

    // TODO: Apply the settings to your engine

}


