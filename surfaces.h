/*
 * =====================================================
 * Siódmy projekt zaliczeniowy z
 * przedmiotu Kurs Programowania w C++ (1000-213bCPP).
 *
 * Plik nagłówkowy modułu 'surfaces'.
 *
 * Plik zawiera funkcyjną implementacje przekształceń
 * powierzchni trójwymiarowej.
 *
 * Autor:
 * Dawid Pawlik      <dp450244@students.mimuw.edu.pl>
 * =====================================================
 */


/* Początek deklaracji elementów modułu surfaces.h */

#ifndef _SURFACES_H
#define _SURFACES_H


/* Pliki nagłówkowe wykorzystywane do implementacji modułu */

#include "real.h"
#include <ostream>
#include <functional>
#include <cmath>
#include <numbers>


/* Implementowane typy modułu 'surfaces' */

/*
 * Deklaracja typu 'Point' reprezentującego punkt w przestrzeni R^2.
 */
class Point {
    public:
        const Real x;
        const Real y;

        Point(Real x, Real y) : x(x), y(y) {}

        friend std::ostream& operator<<(std::ostream& os, const Point& p) {
            os << p.x << ' ' << p.y;
            return os;
        }
};

/*
 * Deklaracja typu 'Surface' reprezentującego funkcję R^2 -> R.
 */
using Surface = std::function<Real(Point)>;


/* Przestrzeń nazw 'structs' zawierająca struktury do operacji na szablonach */

namespace structs {

    /*
     * Uogólnienie 'composition' umożliwiające częściową specjalizację.
     */
    template <typename... Ts>
    struct composition {};

    /*
     * Wariant struktury 'composition' realizujący wywołanie
     * funkcji 'compose' bez parametrów skutkujące zwróceniem identyczności.
     */
    template <>
    struct composition<> {
        static inline auto compose() {
            return [](auto a) { return a; };
        }
    };

    /*
     * Wariant struktury 'composition' realizujący
     * zakończenie rekurencyjnego wywołania funkcji 'compose'.
     */
    template <typename T>
    struct composition<T> {
        static inline auto compose(const T& f) {
            return [=](auto a) { return f(a); };
        }
    };

    /*
     * Struktura 'composition' umożliwiające rekurencyjne wywołanie
     * 'compose' skutkujące realizacją wyniku fs(...(f2(f1))...).
     */
    template <typename T, typename U, typename... Ts>
    struct composition<T, U, Ts...> {
        static inline auto compose(const T& f1, const U& f2, const Ts&... fs) {
            auto composed = [=](auto a) { return f2(f1(a)); };
            return composition<decltype(composed),
                               Ts...>::compose(composed, fs...);
        }
    };

} // Koniec przestrzeni nazw 'structs'.


/* Implementowane funkcje modułu 'surfaces' */

/* Figury przestrzenne */

/*
 * Generuje płaszczyznę f(x, y) = 0.
 */
inline Surface plain() {
    return [](Point) { return 0; };
}

/*
 * Generuje powierzchnię f(x, y) = x.
 */
inline Surface slope() {
    return [](Point p) { return p.x; };
}

/*
 * Generuje powierzchnię schodkową wzdłuż osi X o szerokości schodka s
 * (domyślnie 1) i wysokości schodka 1. Dla 0 <= x < s mamy f(x, y) = 0,
 * dla -s <= x < 0 mamy f(x, y) = -1 itd. Jeśli s <= 0, to f(x, y) = 0.
 */
inline Surface steps(Real s = 1) {
    return [=](Point p) { return s > 0 ? std::floor(p.x / s) : 0; };
}

/*
 * Generuje powierzchnię, która jest szachownicą o szerokości kratki s
 * (domyślnie 1). Dla 0 <= x < s mamy f(x, y) = 1, gdy 0 <= y < s
 * oraz f(x, y) = 0, gdy s <= y < 2 * s itd. Jeśli s <= 0, to f(x, y) = 0.
 */
inline Surface checker(Real s = 1) {
    return [=](Point p) {
        return s > 0 && std::abs(std::fmod(std::floor(p.x / s), 2)) ==
                        std::abs(std::fmod(std::floor(p.y / s), 2));
    };
}

/*
 * Generuje powierzchnię f(x, y) = x * x.
 */
inline Surface sqr() {
    return [](Point p) { return p.x * p.x; };
}

/*
 * Generuje powierzchnię sinusoidalną względem argumentu x.
 */
inline Surface sin_wave() {
    return [](Point p) { return std::sin(p.x); };
}

/*
 * Generuje powierzchnię kosinusoidalną względem argumentu x.
 */
inline Surface cos_wave() {
    return [](Point p) { return std::cos(p.x); };
}

/*
 * Generuje powierzchnię z koncentrycznymi naprzemiennymi pasami szerokości s
 * (domyślnie 1) o środku w punkcie (0, 0), gdzie f(x, y) = 1 w najbardziej
 * wewnętrznym okręgu (łącznie z tym okręgiem). Jeśli s <= 0, to f(x, y) = 0.
 */
inline Surface rings(Real s = 1) {
    return [=](Point p) {
        return s > 0 && ((!p.x && !p.y) ||
               std::fmod(std::ceil(std::hypot(p.x, p.y) / s), 2));
    };
}

/*
 * Generuje powierzchnię z elipsą o środku w punkcie (0, 0) o półosi długości
 * a wzdłuż osi X (domyślnie 1) i o półosi długości b wzdłuż osi Y
 * (domyślnie 1). Zachodzi f(x, y) = 1, gdy (x, y) leży wewnątrz lub na brzegu
 * elipsy, zaś f(x, y) = 0, w. p.p. Jeśli a <= 0 lub b <= 0, to f(x, y) = 0.
 */
inline Surface ellipse(Real a = 1, Real b = 1) {
    return [=](Point p) {
        return a > 0 && b > 0 &&
               (p.x * p.x) / (a * a) + (p.y * p.y) / (b * b) <= 1;
    };
}

/*
 * Generuje powierzchnię z prostokątem o środku w punkcie (0, 0) o brzegu
 * długości 2a wzdłuż X (domyślnie a = 1) i o brzegu długości 2b wzdłuż osi Y
 * (domyślnie b = 1), czyli f(x, y) = 1, gdy (x, y) leży wewnątrz lub na brzegu
 * prostokąta, zaś f(x, y) = 0, w. p.p. Jeśli a <= 0 lub b <= 0, to f(x, y) = 0.
 */
inline Surface rectangle(Real a = 1, Real b = 1) {
    return [=](Point p) {
        return a > 0 && b > 0 && std::abs(p.x) <= a && std::abs(p.y) <= b;
    };
}

/*
 * Generuje powierzchnię z paskami o szerokości s, ułożonymi wzdłuż osi X,
 * równoległymi do osi Y. Zachodzi f(x, y) = 1, gdy 0 < x <= s oraz f(x, y) = 0,
 * gdy -s < x <= 0 itd. Jeśli s <= 0, to f(x, y) = 0.
 */
inline Surface stripes(Real s) {
    return [=](Point p) {
        return s > 0 && std::abs(std::fmod(std::ceil(p.x / s), 2));
    };
}


/* Przekształcenia funkcji */

/*
 * Obraca dziedzinę powierzchni o kąt wyrażony w stopniach.
 */
inline Surface rotate(const Surface& f, Real deg) {
    return [f, rad = deg * std::numbers::pi / 180](Point p) {
        return f(Point(p.x * std::cos(-rad) - p.y * std::sin(-rad),
                       p.x * std::sin(-rad) + p.y * std::cos(-rad)));
    };
}

/*
 * Przesuwa dziedzinę powierzchni o wektor v (typu Point).
 */
inline Surface translate(const Surface& f, Point v) {
    return [=](Point p) { return f(Point(p.x - v.x, p.y - v.y)); };
}

/*
 * Skaluje dziedzinę powierzchni parą współczynników s (typu Point).
 */
inline Surface scale(const Surface& f, Point s) {
    return [=](Point p) { return f(Point(p.x / s.x, p.y / s.y)); };
}

/*
 * Odwraca dziedzinę powierzchni, tzn. zamienia (x, y) na (y, x).
 */
inline Surface invert(const Surface& f) {
    return [=](Point p) { return f(Point(p.y, p.x)); };
}

/*
 * Odbija dziedzinę powierzchni względem osi Y, tzn. zamienia x na -x.
 */
inline Surface flip(const Surface& f) {
    return [=](Point p) { return f(Point(-p.x, p.y)); };
}

/*
 * Mnoży wartości podanej powierzchni f przez podaną wartość c.
 */
inline Surface mul(const Surface& f, Real c) {
    return [=](Point p) { return f(p) * c; };
}

/*
 * Dodaje podaną wartość c do wartości podanej powierzchni f.
 */
inline Surface add(const Surface& f, Real c) {
    return [=](Point p) { return f(p) + c; };
}


/* Operacje na funkcjach */

/*
 * Oblicza wartość funkcji h(f1(p), ..., fs(p)).
 */
template <typename T, typename... Ts>
inline auto evaluate(const T& h, const Ts&... fs) {
    return [=]([[maybe_unused]] Point p) { return h(fs(p)...); };
}

/*
 * Generuje złożenie funkcji fs(...(f2(f1))...).
 */
template <typename... Ts>
inline auto compose(const Ts&... fs) {
    return structs::composition<Ts...>::compose(fs...);
}


#endif // _SURFACES_H
