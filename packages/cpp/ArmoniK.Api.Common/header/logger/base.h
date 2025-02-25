#pragma once
/**
 * @file base.h
 * @brief Logger interface
 */

#include <string_view>

#include "context.h"
#include "level.h"

namespace API_COMMON_NAMESPACE::logger {
/**
 * @interface ILogger
 * @brief Logger interface.
 */
class ILogger {
protected:
  Level level_ = Level::Info;

protected:
  /**
   * @brief Default constructor.
   */
  ILogger() noexcept = default;
  /**
   * @brief Constructs a logger with a logging level.
   * @param level Logging level to use for the logger.
   */
  explicit ILogger(Level level) noexcept : level_(level) {}

public:
  /**
   * @brief Destructor.
   */
  virtual ~ILogger();

  /**
   * @brief Write a new message to the log.
   * @param level Logging level to use for this message.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  virtual void log(Level level, std::string_view message, const Context &message_context = {}) = 0;

public:
  /**
   * @brief Set the logging level.
   * @param level Logging level.
   * @attention Not thread safe.
   */
  void set_level(Level level) noexcept { level_ = level; }
  /**
   * @brief Get the current logging level.
   * @return The current logging level.
   * @attention Not thread safe.
   */
  Level get_level() const noexcept { return level_; }

public:
  /**
   * @brief Write a new message to the log with verbose log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void verbose(std::string_view message, const Context &message_context = {}) {
    log(Level::Verbose, message, message_context);
  }
  /**
   * @brief Write a new message to the log with debug log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void debug(std::string_view message, const Context &message_context = {}) {
    log(Level::Debug, message, message_context);
  }
  /**
   * @brief Write a new message to the log with info log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void info(std::string_view message, const Context &message_context = {}) {
    log(Level::Info, message, message_context);
  }
  /**
   * @brief Write a new message to the log with warning log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void warning(std::string_view message, const Context &message_context = {}) {
    log(Level::Warning, message, message_context);
  }
  /**
   * @brief Write a new message to the log with error log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void error(std::string_view message, const Context &message_context = {}) {
    log(Level::Error, message, message_context);
  }
  /**
   * @brief Write a new message to the log with fatal log level.
   * @param message Message to log.
   * @param message_context Context specific for this message.
   */
  void fatal(std::string_view message, const Context &message_context = {}) {
    log(Level::Fatal, message, message_context);
  }
};
} // namespace API_COMMON_NAMESPACE::logger
