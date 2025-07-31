#include "pch.h"

#include <sqlite3.h>

#include "manager.h"

namespace ymsummorizer::storage::db::sqlite {
/*
SQLITE_API int sqlite3_bind_blob(sqlite3_stmt*, int, const void*, int n, void(*)(void*));
SQLITE_API int sqlite3_bind_blob64(sqlite3_stmt*, int, const void*, sqlite3_uint64,
                        void(*)(void*));
SQLITE_API int sqlite3_bind_zeroblob(sqlite3_stmt*, int, int n);
SQLITE_API int sqlite3_bind_zeroblob64(sqlite3_stmt*, int, sqlite3_uint64);

SQLITE_API int sqlite3_bind_value(sqlite3_stmt*, int, const sqlite3_value*);
SQLITE_API int sqlite3_bind_pointer(sqlite3_stmt*, int, void*, const char*,void(*)(void*));
*/
template <typename TArg>
void manager::prapare_statement_arg(sqlite3_stmt* stmt, TArg argument, int index) {
  AUTOTRACE
  /* For encoding issues:

  SQLITE_API int sqlite3_bind_text(sqlite3_stmt*,int,const char*,int,void(*)(void*));
  SQLITE_API int sqlite3_bind_text16(sqlite3_stmt*, int, const void*, int, void(*)(void*));
  SQLITE_API int sqlite3_bind_text64(sqlite3_stmt*, int, const char*, sqlite3_uint64,
                           void(*)(void*), unsigned char encoding);
  */
  if constexpr (std::is_same_v<TArg, std::string>) {
    sqlite3_bind_text(stmt, index, argument.c_str(), argument.size(), SQLITE_TRANSIENT);
  } else if constexpr (std::is_same_v<TArg, std::string_view>) {
    sqlite3_bind_text(stmt, index, argument.c_str(), argument.size(), SQLITE_TRANSIENT);
  } else if constexpr (std::is_same_v<TArg, bool>) {
  } else if constexpr (std::is_same_v<TArg, unsigned char>) {
  } else if constexpr (std::is_same_v<TArg, float>) {
    sqlite3_bind_double(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, double>) {
    sqlite3_bind_double(stmt, index, argument);

  } else if constexpr (std::is_same_v<TArg, int>) {
    sqlite3_bind_int(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, unsigned int>) {
    sqlite3_bind_int(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, short>) {
    sqlite3_bind_int(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, unsigned short>) {
    sqlite3_bind_int(stmt, index, argument);

  } else if constexpr (std::is_same_v<TArg, size_t>) {
    sqlite3_bind_int64(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, long>) {
    sqlite3_bind_int64(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, long long>) {
    sqlite3_bind_int64(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, unsigned long>) {
    sqlite3_bind_int64(stmt, index, argument);
  } else if constexpr (std::is_same_v<TArg, unsigned long long>) {
    sqlite3_bind_int64(stmt, index, argument);

  } else if constexpr (std::is_null_pointer_v<TArg>) {
    sqlite3_bind_null(stmt, index);
  } else {
    log()->error("Error: Unknown argument {0} type, value: {1}", index, std::to_string(argument));
    stmt = nullptr;
  }
}

template <typename... TArgs>
sqlite3_stmt* manager::prapare_statement(const char* sql_query, TArgs&&... arguments) {
  AUTOTRACE
  const char* tail = nullptr;
  sqlite3_stmt* stmt = nullptr;
  sqlite3_prepare_v2(db_handler_, sql_query, strlen(sql_query) + 1, &stmt, &tail);

  auto lambda = [&stmt]<size_t N>(auto& arguments_tuple) constexpr {
    constexpr size_t argument_count = sizeof...(TArgs);
    if constexpr (argument_count > N) {
      prapare_statement_arg(stmt, std::get<N>(arguments_tuple), N + 1);
    }
  };

  const auto arguments_tuple = std::forward_as_tuple(arguments...);
  lambda.template operator()<0>(arguments_tuple);
  lambda.template operator()<1>(arguments_tuple);
  lambda.template operator()<2>(arguments_tuple);
  lambda.template operator()<3>(arguments_tuple);
  lambda.template operator()<4>(arguments_tuple);
  lambda.template operator()<5>(arguments_tuple);
  lambda.template operator()<6>(arguments_tuple);
  lambda.template operator()<7>(arguments_tuple);
  lambda.template operator()<8>(arguments_tuple);
  lambda.template operator()<9>(arguments_tuple);
  lambda.template operator()<10>(arguments_tuple);
  lambda.template operator()<11>(arguments_tuple);
  lambda.template operator()<12>(arguments_tuple);
  lambda.template operator()<13>(arguments_tuple);
  lambda.template operator()<14>(arguments_tuple);
  lambda.template operator()<15>(arguments_tuple);
  lambda.template operator()<16>(arguments_tuple);

  const int result_code = sqlite3_step(stmt);
  constexpr std::array ok_codes = {SQLITE_ROW, SQLITE_DONE};

  if (std::find(ok_codes.cbegin(), ok_codes.cend(), result_code) == ok_codes.cend()) {
    std::string keys_format;
    for (size_t i = 0; i < sizeof...(TArgs); ++i) {
      keys_format += "'{" + std::to_string(i + 2) + "}' ";
    }

    log()->error(
        "Error: Failed to prepare statement. Error message: \"{0}\". sql_query:\"{1}\". "
        "Arguments: " +
            (keys_format.empty() ? "none" : keys_format),
        sqlite3_errmsg(db_handler_),
        sql_query,
        std::forward<decltype(arguments)>(arguments)...);
    sqlite3_finalize(stmt);
    stmt = nullptr;
  }

  return stmt;
}
}  // namespace ymsummorizer::storage::db::sqlite
