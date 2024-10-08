// To jest funkcyjna implementacja modułu ps_plot.

#include <cassert>
#include <string>
#include <cmath>
#include <numbers>
#include <ranges>
#include <algorithm>
#include "ps_plot.h"

namespace {
    using std::ostream;
    using std::string;
    using std::numbers::pi;
    using std::views::iota;
    using std::views::transform;
    using std::for_each;

    // Komendy PS
    constexpr string PROLOGUE = "%!PS\n";
    constexpr string MOVE_TO = " moveto ";
    constexpr string LINE_TO = " lineto\n";
    constexpr string EPILOGUE = "stroke\n";

    // Ustawienia strony PS
    constexpr size_t WIDTH = 595;
    constexpr size_t HEIGHT = 842;
    constexpr size_t CENTER_H = WIDTH / 2;
    constexpr size_t CENTER_V = HEIGHT / 2;

    // Konwersja na współrzędne wydruku w formacie PS
    Point const screen(Point const p, Real const x_shift,
                       Real const y_shift, Real const scale) {
        return Point((p.x - x_shift) * scale + CENTER_H,
                     (p.y - y_shift) * scale + CENTER_V);
    }

    // Przygotowanie widoku, ujawniające trzeci wymiar (wartości powierzchni)
    Point const view(Surface const f, Point const p) {
        Real const pi4 = pi / 4;
        Real const pi3 = pi / 3;
        Point const r1_p(p.x * cos(pi4) - p.y * sin(pi4),
                         p.x * sin(pi4) + p.y * cos(pi4));
        Point const r2_p(r1_p.x, f(p) * sin(pi3) + r1_p.y * cos(pi3));
        return r2_p;
    }

} // anonimowa przestrzeń nazw

void create_PS_plot(ostream & out, Surface const f,
                    Real const x_min, Real const x_max,
                    Real const y_min, Real const y_max,
                    size_t const x_count, size_t const y_count,
                    size_t const scale) {
    assert(x_count > 0 && y_count > 0);
    assert(x_min < x_max);
    assert(y_min < y_max);

    Real const x_step = (x_max - x_min) / (x_count - 1);
    Real const y_step = (y_max - y_min) / (y_count - 1);
    Real const x_shift = (x_min + x_max) / 2;
    Real const y_shift = (y_min + y_max) / 2;

    auto x_trans = [=](auto const x) {
        return x_min + x * x_step;
    };
    auto y_trans = [=](auto const y) {
        return y_min + y * y_step;
    };
    auto const xs = iota(static_cast<size_t>(0), x_count) | transform(x_trans);
    auto const ys = iota(static_cast<size_t>(0), y_count) | transform(y_trans);

    out << PROLOGUE;
    for_each(xs.begin(), xs.end(), [&](auto const x) {
        for_each(ys.begin(), ys.end(), [&](auto const y) {
            y < y_trans(y_count - 1) ?
                out << screen(view(f, Point(x, y)), x_shift, y_shift, scale)
                    << MOVE_TO
                    << screen(view(f, Point(x, y + y_step)), x_shift, y_shift, scale)
                    << LINE_TO
                : out;
            x < x_trans(x_count - 1) ?
                out << screen(view(f, Point(x, y)), x_shift, y_shift, scale)
                    << MOVE_TO
                    << screen(view(f, Point(x + x_step, y)), x_shift, y_shift, scale)
                    << LINE_TO
                : out;
        });
    });
    out << EPILOGUE;
}
