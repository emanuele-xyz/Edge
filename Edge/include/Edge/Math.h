#pragma once

#include <algorithm>
#include <cstddef>
#include <utility>

template <std::size_t N>
class v
{
public:
	constexpr v() noexcept : m_elems() {}
	constexpr explicit v(const float* elems) noexcept : m_elems{} { std::copy_n(elems, N, m_elems); }
	template <typename... Args>
		requires (sizeof...(Args) == N) && (std::convertible_to<Args, float> && ...)
	constexpr v(Args... args) noexcept : m_elems{ static_cast<float>(args)... } {}
public:
	template <typename Self>
	constexpr decltype(auto) operator[](this Self&& self, std::size_t i) noexcept { return std::forward_like<Self>(self.m_elems[i]); }
public:
	template <typename Self>
	constexpr auto Elems(this Self&& self) noexcept { return self.m_elems; }
private:
	float m_elems[N];
};

template <typename Derived>
class AccessorsXYRGST
{
public:
	constexpr const float& x() const noexcept { return Self()[0]; }
	constexpr const float& y() const noexcept { return Self()[1]; }
	constexpr float& x() noexcept { return Self()[0]; }
	constexpr float& y() noexcept { return Self()[1]; }
public:
	constexpr const float& r() const noexcept { return Self()[0]; }
	constexpr const float& g() const noexcept { return Self()[1]; }
	constexpr float& r() noexcept { return Self()[0]; }
	constexpr float& g() noexcept { return Self()[1]; }
public:
	constexpr const float& s() const noexcept { return Self()[0]; }
	constexpr const float& t() const noexcept { return Self()[1]; }
	constexpr float& s() noexcept { return Self()[0]; }
	constexpr float& t() noexcept { return Self()[1]; }
protected:
	constexpr Derived& Self() noexcept { return static_cast<Derived&>(*this); }
	constexpr const Derived& Self() const noexcept { return static_cast<const Derived&>(*this); }
};

template <typename Derived>
class AccessorsXYZRGBSTP : public AccessorsXYRGST<Derived>
{
public:
	constexpr const float& z() const noexcept { return this->Self()[2]; }
	constexpr float& z() noexcept { return this->Self()[2]; }
public:
	constexpr const float& b() const noexcept { return this->Self()[2]; }
	constexpr float& b() noexcept { return this->Self()[2]; }
public:
	constexpr const float& p() const noexcept { return this->Self()[2]; }
	constexpr float& p() noexcept { return this->Self()[2]; }
};

template <typename Derived>
class AccessorsXYZWRGBASTP : public AccessorsXYZRGBSTP<Derived>
{
public:
	constexpr const float& w() const noexcept { return this->Self()[3]; }
	constexpr float& w() noexcept { return this->Self()[3]; }
public:
	constexpr const float& a() const noexcept { return this->Self()[3]; }
	constexpr float& a() noexcept { return this->Self()[3]; }
};

class v2 : public v<2>, public AccessorsXYRGST<v2>
{
public:
	using v<2>::v;
};

class v3 : public v<3>, public AccessorsXYZRGBSTP<v3>
{
public:
	using v<3>::v;
};

class v4 : public v<4>, public AccessorsXYZWRGBASTP<v4>
{
public:
	using v<4>::v;
};
