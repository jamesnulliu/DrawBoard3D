#pragma once
#include "Essentials.hpp"
#include "Component.hpp"
#include "glm/glm.hpp"

#include <vector>
#include <numeric>

namespace db3d {
    class Vertex
    {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
    };

    class VertexVec : public Component
    {
    public:

        constexpr u64 size() const { return m_vertices.size(); }
        constexpr bool empty() const { return m_vertices.empty(); }

        constexpr const Vertex& operator[](u64 _index) const { return m_vertices.at(_index); }
        constexpr Vertex& operator[](u64 _index) { return m_vertices.at(_index); }

        constexpr const Vertex& at(u64 _index) const { return m_vertices.at(_index); }
        constexpr Vertex& at(u64 _index) { return m_vertices.at(_index); }

        constexpr void resize(u64 _count) { m_vertices.resize(_count); }
        constexpr void resize(u64 _count, const Vertex& _val) { m_vertices.resize(_count, _val); }

        template<typename... _ValTy>
        constexpr decltype(auto) emplace_back(_ValTy&&... _vals) { return m_vertices.emplace_back(std::forward<_ValTy>(_vals)...); }

        constexpr void push_back(const Vertex& _val) { m_vertices.push_back(_val); }
        constexpr void push_back(Vertex&& _val) { m_vertices.push_back(std::move(_val)); }

        constexpr void pop_back() { m_vertices.pop_back(); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void erase(Iter _where) { m_vertices.erase(_where); }
        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void erase(Iter _begin, Iter _end) { m_vertices.erase(_begin, _end); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, const Vertex& _val) { m_vertices.insert(_where, _val); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, Vertex&& _val) { m_vertices.insert(_where, std::move(_val)); }

        template<typename Iter>
            requires std::is_same_v<Iter, typename std::vector<Vertex>::iterator> || std::is_same_v<Iter, typename std::vector<Vertex>::const_iterator>
        constexpr void insert(Iter _where, u64 _count, const Vertex& _val) { m_vertices.insert(_where, _count, _val); }

        constexpr auto cbegin() const { return m_vertices.cbegin(); }
        constexpr auto begin() const { return m_vertices.cbegin(); }
        constexpr auto being() { return m_vertices.begin(); }
        constexpr auto cend()const { return m_vertices.cend(); }
        constexpr auto end() const { return m_vertices.cend(); }
        constexpr auto end() { return m_vertices.end(); }

        constexpr void clear() { m_vertices.clear(); }

        void UpdateCentroid()
        {
            m_centroid = std::accumulate(
                cbegin(), cend(), glm::vec3{ 0.0F },
                [](const glm::vec3& sum, const Vertex& vertex) { return sum + vertex.position; }
            );
            m_centroid /= m_vertices.size();
        }

    private:
        std::vector<Vertex> m_vertices;
        glm::vec3 m_centroid;
    };
}
