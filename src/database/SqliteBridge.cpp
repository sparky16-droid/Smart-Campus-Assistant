#include "SqliteBridge.h"

std::string SqlCondition::toSqlString() const {
    std::string opStr;
    switch (op) {
        case SqlConditionOp::Equal:       opStr = "="; break;
        case SqlConditionOp::NotEqual:    opStr = "!="; break;
        case SqlConditionOp::GreaterThan: opStr = ">"; break;
        case SqlConditionOp::LessThan:    opStr = "<"; break;
        case SqlConditionOp::Like:        opStr = "LIKE"; break;
        case SqlConditionOp::In:          opStr = "IN"; break;
    }
    return column + " " + opStr + " '" + value + "'";
}

SelectQueryBuilder::SelectQueryBuilder(std::string table)
    : m_table(std::move(table)), m_ascending(true), m_limit(-1), m_offset(-1)
{
}

SelectQueryBuilder& SelectQueryBuilder::select(const std::vector<std::string>& columns) {
    m_columns = columns;
    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::where(const std::string& column, SqlConditionOp op, const std::string& value) {
    m_conditions.push_back({column, op, value});
    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::orderBy(const std::string& column, bool ascending) {
    m_orderByColumn = column;
    m_ascending = ascending;
    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::limit(int count) {
    m_limit = count;
    return *this;
}

SelectQueryBuilder& SelectQueryBuilder::offset(int start) {
    m_offset = start;
    return *this;
}

std::string SelectQueryBuilder::build() const {
    std::stringstream ss;
    ss << "SELECT ";
    if (m_columns.empty()) {
        ss << "*";
    } else {
        for (size_t i = 0; i < m_columns.size(); ++i) {
            ss << m_columns[i] << (i + 1 < m_columns.size() ? ", " : " ");
        }
    }
    ss << "FROM " << m_table;

    if (!m_conditions.empty()) {
        ss << " WHERE ";
        for (size_t i = 0; i < m_conditions.size(); ++i) {
            ss << m_conditions[i].toSqlString() << (i + 1 < m_conditions.size() ? " AND " : "");
        }
    }

    if (!m_orderByColumn.empty()) {
        ss << " ORDER BY " << m_orderByColumn << (m_ascending ? " ASC" : " DESC");
    }

    if (m_limit >= 0) {
        ss << " LIMIT " << m_limit;
    }

    if (m_offset >= 0) {
        ss << " OFFSET " << m_offset;
    }

    ss << ";";
    return ss.str();
}

InsertQueryBuilder::InsertQueryBuilder(std::string table)
    : m_table(std::move(table))
{
}

InsertQueryBuilder& InsertQueryBuilder::set(const std::string& column, const std::string& value) {
    m_values[column] = "'" + value + "'";
    return *this;
}

InsertQueryBuilder& InsertQueryBuilder::set(const std::string& column, int value) {
    m_values[column] = std::to_string(value);
    return *this;
}

InsertQueryBuilder& InsertQueryBuilder::set(const std::string& column, double value) {
    m_values[column] = std::to_string(value);
    return *this;
}

InsertQueryBuilder& InsertQueryBuilder::setBool(const std::string& column, bool value) {
    m_values[column] = value ? "1" : "0";
    return *this;
}

std::string InsertQueryBuilder::build() const {
    if (m_values.empty()) {
        throw std::logic_error("InsertQueryBuilder: Cannot build query with no values set");
    }

    std::stringstream cols;
    std::stringstream vals;
    size_t count = 0;

    for (const auto& kv : m_values) {
        cols << kv.first << (count + 1 < m_values.size() ? ", " : "");
        vals << kv.second << (count + 1 < m_values.size() ? ", " : "");
        count++;
    }

    return "INSERT INTO " + m_table + " (" + cols.str() + ") VALUES (" + vals.str() + ");";
}

UpdateQueryBuilder::UpdateQueryBuilder(std::string table)
    : m_table(std::move(table))
{
}

UpdateQueryBuilder& UpdateQueryBuilder::set(const std::string& column, const std::string& value) {
    m_updates[column] = "'" + value + "'";
    return *this;
}

UpdateQueryBuilder& UpdateQueryBuilder::set(const std::string& column, int value) {
    m_updates[column] = std::to_string(value);
    return *this;
}

UpdateQueryBuilder& UpdateQueryBuilder::setBool(const std::string& column, bool value) {
    m_updates[column] = value ? "1" : "0";
    return *this;
}

UpdateQueryBuilder& UpdateQueryBuilder::where(const std::string& column, SqlConditionOp op, const std::string& value) {
    m_conditions.push_back({column, op, value});
    return *this;
}

std::string UpdateQueryBuilder::build() const {
    if (m_updates.empty()) {
        throw std::logic_error("UpdateQueryBuilder: No columns specified for update");
    }

    std::stringstream ss;
    ss << "UPDATE " << m_table << " SET ";
    size_t count = 0;
    for (const auto& kv : m_updates) {
        ss << kv.first << " = " << kv.second << (count + 1 < m_updates.size() ? ", " : "");
        count++;
    }

    if (!m_conditions.empty()) {
        ss << " WHERE ";
        for (size_t i = 0; i < m_conditions.size(); ++i) {
            ss << m_conditions[i].toSqlString() << (i + 1 < m_conditions.size() ? " AND " : "");
        }
    }

    ss << ";";
    return ss.str();
}

DeleteQueryBuilder::DeleteQueryBuilder(std::string table)
    : m_table(std::move(table))
{
}

DeleteQueryBuilder& DeleteQueryBuilder::where(const std::string& column, SqlConditionOp op, const std::string& value) {
    m_conditions.push_back({column, op, value});
    return *this;
}

std::string DeleteQueryBuilder::build() const {
    std::stringstream ss;
    ss << "DELETE FROM " << m_table;
    if (!m_conditions.empty()) {
        ss << " WHERE ";
        for (size_t i = 0; i < m_conditions.size(); ++i) {
            ss << m_conditions[i].toSqlString() << (i + 1 < m_conditions.size() ? " AND " : "");
        }
    }
    ss << ";";
    return ss.str();
}
