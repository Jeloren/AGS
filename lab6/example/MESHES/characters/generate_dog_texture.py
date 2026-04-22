#!/usr/bin/env python3
"""
Генератор текстуры для OBJ модели собаки
Создает текстуру в формате PPM (или PNG если доступна PIL)
"""

import random
import argparse
import struct
import sys

try:
    from PIL import Image, ImageDraw
    HAS_PIL = True
except ImportError:
    HAS_PIL = False


def generate_dog_texture(width=512, height=512, dog_pattern="brown"):
    """
    Генерирует текстуру для собаки с различными паттернами
    Используя встроенные возможности Python (не требует numpy)

    Args:
        width: ширина текстуры в пиксельях
        height: высота текстуры в пиксельях
        dog_pattern: тип паттерна ('brown', 'spotted', 'white', 'black', 'dalmatian')
    
    Returns:
        Список списков RGB кортежей или PIL Image
    """

    # Выбираем цвета
    if dog_pattern == "brown":
        # Коричневый оттенок (основной цвет собаки)
        base_color = (139, 69, 19)  # Седло коричневого цвета
        dark_color = (101, 50, 15)  # Темный коричневый
        light_color = (184, 134, 84)  # Светлый коричневый
    elif dog_pattern == "spotted":
        # Пятнистый рыжий
        base_color = (210, 105, 30)  # Шоколадный/рыжий
        dark_color = (139, 69, 19)  # Темный коричневый
        light_color = (255, 140, 0)  # Оранжевый
    elif dog_pattern == "white":
        # Белый с пятнами
        base_color = (240, 240, 240)  # Белый
        dark_color = (200, 200, 200)  # Серый
        light_color = (255, 255, 255)  # Светлый белый
    elif dog_pattern == "black":
        # Черный
        base_color = (30, 30, 30)  # Черный
        dark_color = (10, 10, 10)  # Очень темный
        light_color = (80, 80, 80)  # Темный серый
    elif dog_pattern == "dalmatian":
        # Далматинец (белый с черными пятнами)
        base_color = (255, 255, 255)  # Белый
        dark_color = (0, 0, 0)  # Черный
        light_color = (200, 200, 200)  # Светлый серый
    else:
        base_color = (165, 42, 42)  # Brown

    # Создаем пиксели
    pixels = []
    for y in range(height):
        row = []
        for x in range(width):
            # Добавляем шум
            noise = tuple(random.randint(-30, 30) for _ in range(3))
            color = tuple(max(0, min(255, base_color[i] + noise[i])) for i in range(3))
            row.append(color)
        pixels.append(row)

    # Добавляем пятна
    if dog_pattern == "spotted":
        add_spots_simple(pixels, dark_color, num_spots=15, min_radius=5, max_radius=15)
    elif dog_pattern == "white":
        add_spots_simple(pixels, dark_color, num_spots=8, min_radius=3, max_radius=10)
    elif dog_pattern == "dalmatian":
        add_spots_simple(pixels, dark_color, num_spots=25, min_radius=3, max_radius=8)

    # Добавляем градиент
    add_gradient_simple(pixels, height)

    # Если есть PIL, преобразуем в изображение
    if HAS_PIL:
        img = Image.new('RGB', (width, height))
        img_pixels = img.load()
        for y in range(height):
            for x in range(width):
                img_pixels[x, y] = pixels[y][x]
        return img
    else:
        return pixels


def add_spots_simple(pixels, color, num_spots=20, min_radius=5, max_radius=20):
    """Добавляет пятна к текстуре (без numpy)"""
    height = len(pixels)
    width = len(pixels[0]) if height > 0 else 0

    for _ in range(num_spots):
        # Случайное место для пятна
        x = random.randint(0, width - 1)
        y = random.randint(0, height - 1)
        radius = random.randint(min_radius, max_radius)

        # Рисуем пятно
        for dy in range(-radius, radius + 1):
            for dx in range(-radius, radius + 1):
                if dx * dx + dy * dy <= radius * radius:
                    px, py = x + dx, y + dy
                    if 0 <= px < width and 0 <= py < height:
                        # Смешиваем цвет пятна с оригинальным
                        orig = pixels[py][px]
                        pixels[py][px] = tuple(
                            int(orig[i] * 0.5 + color[i] * 0.5) for i in range(3)
                        )


def add_gradient_simple(pixels, height):
    """Добавляет градиент для большей реалистичности"""
    width = len(pixels[0]) if height > 0 else 0

    for y in range(height):
        # Постепенно темнеет к низу
        factor = 1.0 - (y / height) * 0.3
        for x in range(width):
            color = pixels[y][x]
            pixels[y][x] = tuple(int(color[i] * factor) for i in range(3))


def save_texture(img, filename):
    """Сохраняет текстуру в файл (PNG если доступна PIL, иначе PPM)"""
    if HAS_PIL:
        img.save(filename)
        print(f"Текстура сохраена: {filename}")
    else:
        # Сохраняем как PPM файл (простой формат, не требует внешних библиотек)
        height = len(img)
        width = len(img[0]) if height > 0 else 0
        
        # Конвертируем имя файла
        if filename.endswith('.png'):
            filename = filename.replace('.png', '.ppm')
        
        with open(filename, 'wb') as f:
            # PPM header
            f.write(b'P6\n')
            f.write(f'{width} {height}\n'.encode())
            f.write(b'255\n')
            
            # Пиксели
            for y in range(height):
                for x in range(width):
                    color = img[y][x]
                    f.write(bytes(color))
        
        print(f"Текстура сохраена (PPM формат): {filename}")
        print(f"Вы можете конвертировать PPM в PNG используя команду: convert {filename} {filename.replace('.ppm', '.png')}")


def create_material_file(texture_name, mtl_filename="dog.mtl"):
    """Создает MTL файл для использования текстуры"""
    mtl_content = f"""# Material for Dog
newmtl DogMaterial
Ka 1.0 1.0 1.0
Kd 0.8 0.8 0.8
Ks 0.2 0.2 0.2
Ns 32.0
map_Kd {texture_name}
"""
    with open(mtl_filename, "w") as f:
        f.write(mtl_content)
    print(f"MTL файл создан: {mtl_filename}")


def main():
    parser = argparse.ArgumentParser(
        description="Генератор текстуры для OBJ модели собаки"
    )
    parser.add_argument(
        "--pattern",
        choices=["brown", "spotted", "white", "black", "dalmatian"],
        default="brown",
        help="Тип окраски собаки (по умолчанию: brown)",
    )
    parser.add_argument(
        "--width", type=int, default=512, help="Ширина текстуры (по умолчанию: 512)"
    )
    parser.add_argument(
        "--height",
        type=int,
        default=512,
        help="Высота текстуры (по умолчанию: 512)",
    )
    parser.add_argument(
        "--output",
        default="dog_texture.png",
        help="Файл выходной текстуры (по умолчанию: dog_texture.png)",
    )
    parser.add_argument(
        "--mtl",
        default="dog.mtl",
        help="Файл материала MTL (по умолчанию: dog.mtl)",
    )

    args = parser.parse_args()

    print(f"Создание текстуры собаки ({args.pattern})...")
    print(f"Размер: {args.width}x{args.height} пиксельей")
    if not HAS_PIL:
        print("(PIL не установлена - будет использован формат PPM)")

    # Генерируем текстуру
    texture = generate_dog_texture(
        width=args.width, height=args.height, dog_pattern=args.pattern
    )

    # Сохраняем текстуру
    save_texture(texture, args.output)

    # Создаем MTL файл
    create_material_file(args.output, args.mtl)

    print("\nДоступные окраски:")
    print("  brown      - Коричневая собака")
    print("  spotted    - Пятнистая рыжая собака")
    print("  white      - Белая собака")
    print("  black      - Черная собака")
    print("  dalmatian  - Далматинец (черно-белая)")
    print("\nПримеры использования:")
    print(f"  python3 {parser.prog} --pattern spotted")
    print(f"  python3 {parser.prog} --pattern dalmatian --width 1024 --height 1024")
    print(f"  python3 {parser.prog} --pattern white --output white_dog.png")


if __name__ == "__main__":
    main()
