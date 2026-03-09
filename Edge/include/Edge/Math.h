#pragma once

#include <algorithm>
#include <cstddef>

template <std::size_t N>
class v
{
public:
	constexpr v() noexcept : m_elems{} {}
	constexpr explicit v(const float* elems) noexcept : m_elems{} { std::copy_n(elems, N, m_elems); }
	template <typename... Args>
		requires (sizeof...(Args) == N) && (std::convertible_to<Args, float> && ...)
	constexpr v(Args... args) noexcept : m_elems{ static_cast<float>(args)... } {}
public:
	constexpr const float& operator[](std::size_t i) const noexcept { return m_elems[i]; }
	constexpr float& operator[](std::size_t i) noexcept { return m_elems[i]; }
protected:
	float m_elems[N];
};


class v2 : public v<2>
{
public:
	constexpr const float& x() const noexcept { return m_elems[0]; }
	constexpr const float& y() const noexcept { return m_elems[1]; }
	constexpr float& x() noexcept { return m_elems[0]; }
	constexpr float& y() noexcept { return m_elems[1]; }
	constexpr const float& u() const noexcept { return m_elems[0]; }
	constexpr const float& v() const noexcept { return m_elems[1]; }
	constexpr float& u() noexcept { return m_elems[0]; }
	constexpr float& v() noexcept { return m_elems[1]; }
};

class v3 : public v<3>
{
public:
	// TODO: to be continued
};

class v4 : public v<4>
{
public:
	// TODO: to be continued
};
