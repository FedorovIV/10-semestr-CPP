#!/usr/bin/env python3

import csv
import struct
import sys
import zlib


FONT = {
    " ": ["00000", "00000", "00000", "00000", "00000", "00000", "00000"],
    "-": ["00000", "00000", "00000", "11111", "00000", "00000", "00000"],
    ".": ["00000", "00000", "00000", "00000", "00000", "00110", "00110"],
    "0": ["01110", "10001", "10011", "10101", "11001", "10001", "01110"],
    "1": ["00100", "01100", "00100", "00100", "00100", "00100", "01110"],
    "2": ["01110", "10001", "00001", "00010", "00100", "01000", "11111"],
    "3": ["11110", "00001", "00001", "01110", "00001", "00001", "11110"],
    "4": ["00010", "00110", "01010", "10010", "11111", "00010", "00010"],
    "5": ["11111", "10000", "10000", "11110", "00001", "00001", "11110"],
    "6": ["01110", "10000", "10000", "11110", "10001", "10001", "01110"],
    "7": ["11111", "00001", "00010", "00100", "01000", "01000", "01000"],
    "8": ["01110", "10001", "10001", "01110", "10001", "10001", "01110"],
    "9": ["01110", "10001", "10001", "01111", "00001", "00001", "01110"],
    "A": ["01110", "10001", "10001", "11111", "10001", "10001", "10001"],
    "B": ["11110", "10001", "10001", "11110", "10001", "10001", "11110"],
    "C": ["01110", "10001", "10000", "10000", "10000", "10001", "01110"],
    "D": ["11100", "10010", "10001", "10001", "10001", "10010", "11100"],
    "E": ["11111", "10000", "10000", "11110", "10000", "10000", "11111"],
    "F": ["11111", "10000", "10000", "11110", "10000", "10000", "10000"],
    "G": ["01110", "10001", "10000", "10111", "10001", "10001", "01110"],
    "H": ["10001", "10001", "10001", "11111", "10001", "10001", "10001"],
    "I": ["11111", "00100", "00100", "00100", "00100", "00100", "11111"],
    "J": ["00001", "00001", "00001", "00001", "10001", "10001", "01110"],
    "K": ["10001", "10010", "10100", "11000", "10100", "10010", "10001"],
    "L": ["10000", "10000", "10000", "10000", "10000", "10000", "11111"],
    "M": ["10001", "11011", "10101", "10101", "10001", "10001", "10001"],
    "N": ["10001", "11001", "10101", "10011", "10001", "10001", "10001"],
    "O": ["01110", "10001", "10001", "10001", "10001", "10001", "01110"],
    "P": ["11110", "10001", "10001", "11110", "10000", "10000", "10000"],
    "R": ["11110", "10001", "10001", "11110", "10100", "10010", "10001"],
    "S": ["01111", "10000", "10000", "01110", "00001", "00001", "11110"],
    "T": ["11111", "00100", "00100", "00100", "00100", "00100", "00100"],
    "U": ["10001", "10001", "10001", "10001", "10001", "10001", "01110"],
    "W": ["10001", "10001", "10001", "10101", "10101", "10101", "01010"],
}


def load_csv(path):
    with open(path, newline="", encoding="utf-8") as handle:
        reader = csv.reader(handle)
        header = next(reader)
        counts = []
        series = {name: [] for name in header[1:]}
        for row in reader:
            counts.append(int(row[0]))
            for index, name in enumerate(header[1:], start=1):
                series[name].append(int(row[index]))
    return counts, series


def create_canvas(width, height, color):
    pixels = bytearray(width * height * 3)
    for y in range(height):
        for x in range(width):
            put_pixel(pixels, width, height, x, y, color)
    return pixels


def put_pixel(pixels, width, height, x, y, color):
    if x < 0 or x >= width or y < 0 or y >= height:
        return
    offset = (y * width + x) * 3
    pixels[offset] = color[0]
    pixels[offset + 1] = color[1]
    pixels[offset + 2] = color[2]


def draw_line(pixels, width, height, x0, y0, x1, y1, color):
    dx = abs(x1 - x0)
    sx = 1 if x0 < x1 else -1
    dy = -abs(y1 - y0)
    sy = 1 if y0 < y1 else -1
    error = dx + dy

    while True:
        put_pixel(pixels, width, height, x0, y0, color)
        if x0 == x1 and y0 == y1:
            break
        double_error = 2 * error
        if double_error >= dy:
            error += dy
            x0 += sx
        if double_error <= dx:
            error += dx
            y0 += sy


def draw_rect(pixels, width, height, x, y, rect_width, rect_height, color):
    for row in range(rect_height):
        for column in range(rect_width):
            put_pixel(pixels, width, height, x + column, y + row, color)


def draw_char(pixels, width, height, x, y, character, color, scale):
    glyph = FONT[character]
    for row, pattern in enumerate(glyph):
        for column, bit in enumerate(pattern):
            if bit == "1":
                draw_rect(
                    pixels,
                    width,
                    height,
                    x + column * scale,
                    y + row * scale,
                    scale,
                    scale,
                    color,
                )


def draw_text(pixels, width, height, x, y, text, color, scale=2):
    cursor = x
    for character in text:
        glyph = FONT.get(character)
        if glyph is None:
            raise ValueError(f"Unsupported character: {character!r}")
        draw_char(pixels, width, height, cursor, y, character, color, scale)
        cursor += 6 * scale


def write_png(path, width, height, pixels):
    raw = bytearray()
    stride = width * 3
    for y in range(height):
        raw.append(0)
        start = y * stride
        raw.extend(pixels[start : start + stride])

    def chunk(tag, data):
        checksum = zlib.crc32(tag + data) & 0xFFFFFFFF
        return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", checksum)

    header = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)
    compressed = zlib.compress(bytes(raw), level=9)

    with open(path, "wb") as handle:
        handle.write(b"\x89PNG\r\n\x1a\n")
        handle.write(chunk(b"IHDR", header))
        handle.write(chunk(b"IDAT", compressed))
        handle.write(chunk(b"IEND", b""))


def project_x(value, left, right, maximum):
    if maximum == 0:
        return left
    return left + int((value / maximum) * (right - left))


def project_y(value, top, bottom, maximum):
    if maximum == 0:
        return bottom
    return bottom - int((value / maximum) * (bottom - top))


def draw_plot(counts, series, output_path):
    width = 1600
    height = 960
    left = 110
    top = 80
    right = 1180
    bottom = 820

    pixels = create_canvas(width, height, (255, 255, 255))

    grid_color = (225, 225, 225)
    axis_color = (0, 0, 0)
    text_color = (20, 20, 20)
    colors = [
        (214, 39, 40),
        (31, 119, 180),
        (44, 160, 44),
        (255, 127, 14),
        (148, 103, 189),
        (140, 86, 75),
        (227, 119, 194),
        (23, 190, 207),
        (188, 189, 34),
    ]

    max_count = max(counts)
    max_collision = max(max(values) for values in series.values())
    max_collision = max(1, max_collision)

    for step in range(0, 9):
        x = left + ((right - left) * step) // 8
        draw_line(pixels, width, height, x, top, x, bottom, grid_color)
    for step in range(0, 7):
        y = top + ((bottom - top) * step) // 6
        draw_line(pixels, width, height, left, y, right, y, grid_color)

    draw_line(pixels, width, height, left, top, left, bottom, axis_color)
    draw_line(pixels, width, height, left, bottom, right, bottom, axis_color)

    for step in range(0, 9):
        value = (max_count * step) // 8
        x = left + ((right - left) * step) // 8
        draw_line(pixels, width, height, x, bottom, x, bottom + 8, axis_color)
        draw_text(
            pixels,
            width,
            height,
            x - 15,
            bottom + 20,
            str(value),
            text_color,
            scale=2,
        )

    for step in range(0, 7):
        value = (max_collision * (6 - step)) // 6
        y = top + ((bottom - top) * step) // 6
        draw_line(pixels, width, height, left - 8, y, left, y, axis_color)
        draw_text(
            pixels,
            width,
            height,
            18,
            y - 7,
            str(value),
            text_color,
            scale=2,
        )

    for color, (name, values) in zip(colors, series.items()):
        points = []
        for count, collision in zip(counts, values):
            points.append(
                (
                    project_x(count, left, right, max_count),
                    project_y(collision, top, bottom, max_collision),
                )
            )

        for first, second in zip(points, points[1:]):
            draw_line(pixels, width, height, first[0], first[1], second[0], second[1], color)

        for point in points:
            draw_rect(pixels, width, height, point[0] - 2, point[1] - 2, 5, 5, color)

    draw_text(pixels, width, height, 1320, 90, "HASH FUNCTIONS", text_color, scale=2)
    legend_y = 150
    for color, name in zip(colors, series.keys()):
        draw_rect(pixels, width, height, 1320, legend_y, 20, 20, color)
        draw_text(pixels, width, height, 1360, legend_y - 2, name, text_color, scale=2)
        legend_y += 44

    draw_text(pixels, width, height, 430, 880, "STRINGS", text_color, scale=3)
    draw_text(pixels, width, height, 140, 30, "COLLISIONS", text_color, scale=3)
    draw_text(pixels, width, height, 380, 20, "10.05 HASH COLLISIONS", text_color, scale=3)

    write_png(output_path, width, height, pixels)


def main():
    if len(sys.argv) != 3:
        raise SystemExit("usage: 10_05_plot.py <input.csv> <output.png>")

    counts, series = load_csv(sys.argv[1])
    draw_plot(counts, series, sys.argv[2])
    print(f"10.05: wrote {sys.argv[2]}")


if __name__ == "__main__":
    main()
