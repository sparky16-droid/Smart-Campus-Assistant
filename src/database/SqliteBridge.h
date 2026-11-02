#ifndef SQLITE_BRIDGE_H
#define SQLITE_BRIDGE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <memory>
#include <stdexcept>

enum class SqlConditionOp {
    Equal,
    NotEqual,
    GreaterThan,
    LessThan,
    Like,
    In
};

struct SqlCondition {
    std::string column;
    SqlConditionOp op;
    std::string value;

    std::string toSqlString() const;
};

/**
 * @file SqliteBridge.h
 * @brief Fluid SQL Query Builder and ORM Data Mapper in pure C++.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPTS: BUILDER DESIGN PATTERN & METAPROGRAMMING
 * - Method chaining (fluent interface) for constructing parameterized SQL statements.
 * - Prevents SQL injection through structured sanitization.
 */
class SelectQueryBuilder {
private:
    std::string m_table;
    std::vector<std::string> m_columns;
    std::vector<SqlCondition> m_conditions;
    std::string m_orderByColumn;
    bool m_ascending;
    int m_limit;
    int m_offset;

public:
    explicit SelectQueryBuilder(std::string table);

    SelectQueryBuilder& select(const std::vector<std::string>& columns);
    SelectQueryBuilder& where(const std::string& column, SqlConditionOp op, const std::string& value);
    SelectQueryBuilder& orderBy(const std::string& column, bool ascending = true);
    SelectQueryBuilder& limit(int count);
    SelectQueryBuilder& offset(int start);

    std::string build() const;
};

class InsertQueryBuilder {
private:
    std::string m_table;
    std::unordered_map<std::string, std::string> m_values;

public:
    explicit InsertQueryBuilder(std::string table);

    InsertQueryBuilder& set(const std::string& column, const std::string& value);
    InsertQueryBuilder& set(const std::string& column, int value);
    InsertQueryBuilder& set(const std::string& column, double value);
    InsertQueryBuilder& setBool(const std::string& column, bool value);

    std::string build() const;
};

class UpdateQueryBuilder {
private:
    std::string m_table;
    std::unordered_map<std::string, std::string> m_updates;
    std::vector<SqlCondition> m_conditions;

public:
    explicit UpdateQueryBuilder(std::string table);

    UpdateQueryBuilder& set(const std::string& column, const std::string& value);
    UpdateQueryBuilder& set(const std::string& column, int value);
    UpdateQueryBuilder& setBool(const std::string& column, bool value);
    UpdateQueryBuilder& where(const std::string& column, SqlConditionOp op, const std::string& value);

    std::string build() const;
};

class DeleteQueryBuilder {
private:
    std::string m_table;
    std::vector<SqlCondition> m_conditions;

public:
    explicit DeleteQueryBuilder(std::string table);

    DeleteQueryBuilder& where(const std::string& column, SqlConditionOp op, const std::string& value);
    std::string build() const;
};

#endif // SQLITE_BRIDGE_H
