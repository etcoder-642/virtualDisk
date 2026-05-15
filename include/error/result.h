#ifndef RESULT_H
#define RESULT_H

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>

#include "error.h"


namespace vfs {

    template<typename T>
    class Result {
        private:
            std::variant<T, FSError> data;

            Result() = default;
        public:
            // Factory methods
            static Result<T> Ok(T value) {
                Result r;
                r.data = std::move(value);
                return r;
            }

            static Result<T> Err(std::string message, std::string suggestion = "") {
                Result r;
                r.data = FSError(message, suggestion);
                return r;
            }

            // State Checks
            bool isOk() const {
                return std::holds_alternative<T>(data);
            }

            bool isErr() const {
                return std::holds_alternative<FSError>(data);
            }

            // Extraction
            T unwrap() {
                return std::get<T>(data);
            }

            FSError unwrapErr() {
                return std::get<FSError>(data);
            }
    };

    // Void Specialization
    template<>
    class Result<void> {
        private:
            std::optional<FSError> err_;

            Result() = default;
        public:
            // Factory methods
            static Result<void> Ok() {
                return Result{};
            }

            static Result<void> Err(std::string message, std::string suggestion = "") {
                Result r;
                r.err_ = FSError(message, suggestion);
                return r;
            }

            // State Checks
            bool isOk() const {
                return !err_.has_value();
            }

            bool isErr() const {
                return err_.has_value();
            }

            // Extraction
            FSError& unwrapErr() {
                return err_.value();
            }
    };
}

#endif