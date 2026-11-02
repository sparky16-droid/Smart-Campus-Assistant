#ifndef CAMPUS_REPOSITORY_H
#define CAMPUS_REPOSITORY_H

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <mutex>

/**
 * @file CampusRepository.h
 * @brief Generic Template Repository for storing, searching, and filtering campus entities.
 * 
 * OBJECT-ORIENTED PROGRAMMING CONCEPT: TEMPLATES & GENERIC PROGRAMMING
 * - Allows any entity class (BuildingEntity, FacilityEntity, DepartmentEntity, etc.)
 *   to be managed in a type-safe, generic repository without code duplication.
 * - Supports functional predicates (std::function<bool(const T&)>) and custom comparators.
 * - Thread-safe access via std::mutex.
 */
template<typename T>
class CampusRepository {
private:
    std::vector<std::shared_ptr<T>> m_items;
    mutable std::mutex m_mutex;

public:
    CampusRepository() = default;
    ~CampusRepository() = default;

    // Prevent copying to maintain single repository integrity
    CampusRepository(const CampusRepository&) = delete;
    CampusRepository& operator=(const CampusRepository&) = delete;

    // Move operations
    CampusRepository(CampusRepository&& other) noexcept {
        std::lock_guard<std::mutex> lock(other.m_mutex);
        m_items = std::move(other.m_items);
    }

    void add(std::shared_ptr<T> item) {
        if (!item) {
            throw std::invalid_argument("CampusRepository::add: Cannot add null item");
        }
        std::lock_guard<std::mutex> lock(m_mutex);
        // Replace if exists with same ID
        for (auto& existing : m_items) {
            if (existing->getId() == item->getId()) {
                existing = item;
                return;
            }
        }
        m_items.push_back(item);
    }

    std::shared_ptr<T> getById(const std::string& id) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& item : m_items) {
            if (item->getId() == id) {
                return item;
            }
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<T>> getAll() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_items;
    }

    size_t count() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_items.size();
    }

    bool remove(const std::string& id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = std::remove_if(m_items.begin(), m_items.end(), [&id](const std::shared_ptr<T>& item) {
            return item->getId() == id;
        });
        if (it != m_items.end()) {
            m_items.erase(it, m_items.end());
            return true;
        }
        return false;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_items.clear();
    }

    // Generic filtering by predicate lambda
    std::vector<std::shared_ptr<T>> filter(std::function<bool(const T&)> predicate) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::shared_ptr<T>> results;
        for (const auto& item : m_items) {
            if (predicate(*item)) {
                results.push_back(item);
            }
        }
        return results;
    }

    // Find first matching element
    std::shared_ptr<T> findFirst(std::function<bool(const T&)> predicate) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (const auto& item : m_items) {
            if (predicate(*item)) {
                return item;
            }
        }
        return nullptr;
    }

    // Generic sorting with custom comparator lambda
    std::vector<std::shared_ptr<T>> getSorted(std::function<bool(const T&, const T&)> comparator) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::vector<std::shared_ptr<T>> copy = m_items;
        std::sort(copy.begin(), copy.end(), [&comparator](const std::shared_ptr<T>& a, const std::shared_ptr<T>& b) {
            return comparator(*a, *b);
        });
        return copy;
    }

    // Generic pagination
    std::vector<std::shared_ptr<T>> paginate(size_t page, size_t pageSize) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (pageSize == 0) return {};
        size_t start = page * pageSize;
        if (start >= m_items.size()) return {};
        size_t end = std::min(start + pageSize, m_items.size());
        return std::vector<std::shared_ptr<T>>(m_items.begin() + start, m_items.begin() + end);
    }
};

#endif // CAMPUS_REPOSITORY_H
