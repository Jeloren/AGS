#version 330 core

uniform vec4 color1;
uniform vec4 color2;

in vec2 position;

out vec4 fragColor;

void main ()
{
    // Диагональ, проходящая из левого нижнего (-0.5, -0.5) 
    // в правый верхний (+0.5, +0.5), описывается условием x = y.
    
    // Вычисляем коэффициент смешивания на основе разности координат
    // Если y > x, мы в верхней левой части
    // Если x > y, мы в нижней правой части
    
    // Для плавного перехода (градиента) вдоль этой линии:
    float gradient = (position.y - position.x) + 0.5;
    
    // Ограничиваем значения в диапазон [0, 1] на всякий случай
    gradient = clamp(gradient, 0.0, 1.0);

    // Смешиваем цвета
    fragColor = mix(color2, color1, gradient);
}