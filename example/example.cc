// Przykładowy program, który generuje „trójwymiarowe„ rysunki w formacie PS.

// W katalogu z wygenerowanymi plikami PS można użyć polecenia
//     for f in *.ps; do ps2pdf $f; done
// w celu ich konwersji do formatu PDF.

#include "surfaces.h"
#include "ps_plot.h"
#include <cassert>
#include <fstream>
#include <cmath>
#include <numbers>
#include <algorithm>

using file_t = std::ofstream;
using std::cos;
using std::sqrt;
using std::numbers::pi;
using std::max;

int main() {
    assert(plain()(Point(0.0, 0.0)) == 0.0);
    assert(plain()(Point(1.0, -5.0)) == 0.0);
    assert(add(plain(), 1.0)(Point(0.5, 1.0)) == 1.0);
    assert(mul(add(plain(), 1.0), 0.25)(Point(-2.0, 25.0)) == 0.25);
    assert(evaluate([](Real const & x){return 2.0 + x;}, plain())(Point(2.5, -0.25)) == 2.0);
    assert(compose()(4.2) == 4.2);
    assert(compose([](auto x) {return x - 0.5;}, [](auto x) {return x * x;})(3.5) == 9);

    auto addition = [](Real const & x, Real const & y) {return x + y;};

    file_t out("plot00.ps");
    create_PS_plot(out, plain(), DEFAULT_MIN, DEFAULT_MAX, DEFAULT_MIN, DEFAULT_MAX, 9.0, 15.0);

    out = file_t("plot01.ps");
    create_PS_plot(out, steps(), -2.5, 2.5, -2.5, 2.5, 40, 40, 75);

    out = file_t("plot02.ps");
    create_PS_plot(out, rotate(steps(0.75), 90.0), -2.5, 2.5, -2.5, 2.5, 40, 40, 75);

    out = file_t("plot03.ps");
    create_PS_plot(out, mul(checker(), 0.5));

    out = file_t("plot04.ps");
    create_PS_plot(out, mul(checker(), -0.125), -2.5, 2.5, -2.5, 2.5, 40, 40, 75);

    out = file_t("plot05.ps");
    create_PS_plot(out, invert(sqr()), -2.0, 2.0, -2.0, 2.0, 40, 40, 75);

    out = file_t("plot06.ps");
    create_PS_plot(out, evaluate(addition, mul(sqr(), 0.5), mul(invert(sqr()), 0.25)), -2.0, 2.0, -2.0, 2.0, 40, 40, 75);

    out = file_t("plot07.ps");
    create_PS_plot(out, evaluate([](Real const & x) {return x;}, mul(sqr(), 0.5)), -2.0, 2.0, -2.0, 2.0, 40, 40, 75);

    out = file_t("plot08.ps");
    create_PS_plot(out, evaluate(addition, mul(sqr(), 0.5), mul(invert(sqr()), -0.25)), -2.0, 2.0, -2.0, 2.0, 40, 40, 75);

    out = file_t("plot09.ps");
    create_PS_plot(out, sin_wave(), -2 * pi, 2 * pi, -1.0, 1.0, 40, 10, 40);

    out = file_t("plot10.ps");
    create_PS_plot(out, scale(cos_wave(), {2.0, 1.0}), -4 * pi, 4 * pi, -1.0, 1.0, 40, 5, 20);

    out = file_t("plot11.ps");
    create_PS_plot(out, evaluate([](Real const & x, Real const & y) {return x * y;}, mul(sin_wave(), 0.75), mul(invert(sin_wave()), 0.75)), -6.0, 6.0, -6.0, 6.0, 40, 40, 30);

    out = file_t("plot12.ps");
    create_PS_plot(out, compose(evaluate(addition, mul(sqr(), 3.0), mul(invert(sqr()), 3.0)), [](Real const & x) {return sqrt(x);}, [](Real const & x) {return cos(x);}), -6.5, 6.5, -6.5, 6.5, 50, 50, 30);

    out = file_t("plot13.ps");
    create_PS_plot(out, evaluate([](Real const & x, Real const & y, Real const & z) {return x + y + z;}, mul(sqr(), 0.5), mul(invert(sqr()), -0.25), mul(checker(), 0.25)), -2.5, 2.5, -2.5, 2.5, 40, 40, 75);

    out = file_t("plot14.ps");
    create_PS_plot(out, translate(mul(ellipse(1.0, 0.5), -1.5), {0.0, -1.0}), -2.0, 2.0, -2.0, 2.0, 50, 50, 100);

    out = file_t("plot15.ps");
    create_PS_plot(out, rotate(mul(rectangle(1.5, 1.0), 0.25), -45.0), -2.0, 2.0, -2.0, 2.0, 50, 50, 100);

    out = file_t("plot16.ps");
    create_PS_plot(out, mul(stripes(0.25), 0.25));

    out = file_t("plot17.ps");
    create_PS_plot(out, mul(flip(stripes(0.25)), 0.25));

    out = file_t("plot18.ps");
    create_PS_plot(out, evaluate([](Real const & x, Real const & y) {return max(x, y);}, mul(slope(), 0.25), mul(flip(slope()), 0.25)));

    out = file_t("plot19.ps");
    create_PS_plot(out, mul(rings(0.5), 0.125), -2.0, 2.0, -2.0, 2.0, 50, 50, 100);
}
