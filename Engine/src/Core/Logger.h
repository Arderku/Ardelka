#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include <memory>

#include <vulkan/vulkan.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#pragma warning(pop)

namespace Ardelka {

    class Logger
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

    // Custom formatter for vk::ArrayWrapper1D<char, 256>
    std::ostream& operator<<(std::ostream& os, const vk::ArrayWrapper1D<char, 256>& array);

}

// GLM integration
template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
    return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
    return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
    return os << glm::to_string(quaternion);
}

// Core log macros
#define AR_CORE_TRACE(...)    ::Ardelka::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define AR_CORE_INFO(...)     ::Ardelka::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define AR_CORE_INFO_PTR(msg, ptr) ::Ardelka::Logger::GetCoreLogger()->info(msg, static_cast<void*>(ptr))
#define AR_CORE_WARN(...)     ::Ardelka::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define AR_CORE_ERROR(...)    ::Ardelka::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define AR_CORE_CRITICAL(...) ::Ardelka::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define AR_TRACE(...)         ::Ardelka::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define AR_INFO(...)          ::Ardelka::Logger::GetClientLogger()->info(__VA_ARGS__)
#define AR_INFO_PTR(msg, ptr) ::Ardelka::Logger::GetClientLogger()->info(msg, static_cast<void*>(ptr))
#define AR_WARN(...)          ::Ardelka::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define AR_ERROR(...)         ::Ardelka::Logger::GetClientLogger()->error(__VA_ARGS__)
#define AR_CRITICAL(...)      ::Ardelka::Logger::GetClientLogger()->critical(__VA_ARGS__)


